import 'dart:async';
import 'dart:ffi';
import 'dart:io';
import 'dart:isolate';

// init lookup
const String _libName = 'physarum_cpp_ffi';
final DynamicLibrary _dylib = () {
  if (Platform.isMacOS || Platform.isIOS) {
    return DynamicLibrary.open('$_libName.framework/$_libName');
  }
  if (Platform.isAndroid || Platform.isLinux) {
    return DynamicLibrary.open('lib$_libName.so');
  }
  if (Platform.isWindows) {
    return DynamicLibrary.open('$_libName.dll');
  }
  throw UnsupportedError('Unknown platform: ${Platform.operatingSystem}');
}();
final Pointer<T> Function<T extends NativeType>(String symbolName) lookup =
    _dylib.lookup;

//Реализация работы с изолятами.

// Так как программа запущена в главном изоляте, а тяжеловесные вызовы функций запускаются в дополнительном,
// необходимо наладить связь между ними.
// В главном изоляте создаётся ReceivePort, что отвечает за две функции:
// 1) Он получает в себя события из изолятов о том, что функция с индексом Х посчитана с результатом Y.
// После чего он вызывает нужные Completer-ы.
// 2) При инициализации нового изолята получает его SendPort (предварительно передав ему свой SendPort)
// и сохраняет в главном изоляте, для последующего доступа.
// В дополнительном изолятя имеется свой ReceivePort, который занимается только выполнением внутри себя функции

// Событие запроса выполнения, формируемое при вызове функции в главном изоляте
// Индекс нужен для запоминания, какой Completer должен получить результат выполнения этой функции
// Сама функция и аргументы, чтобы её можно было запустить в изоляте вне зависимости от реализации оной
class ExecuteRequest {
  final int id;
  final Function function;
  final Map<String, dynamic> args;

  const ExecuteRequest(this.id, this.function, this.args);
}

// Результат, что формируется в доп. изоляте.
// Аналогично индекс определяет то, какой Completer должен получить result
class ExecuteResponse {
  final int id;
  final dynamic result;

  const ExecuteResponse(this.id, this.result);
}

// Сохраняем Completer значения по индексам, чтобы изоляты работали параллельно
final Map<int, Completer<dynamic>> executeRequests =
    <int, Completer<dynamic>>{};

// номер посылки
int nextSumRequestId = 0;
Future<T> executeInIsolate<T>(executeFunc, Map<String, dynamic> args) async {
  // получаем SendPort дополнительного изолята
  final SendPort helperIsolateSendPort1 = await helperIsolateSendPort;

  // сохраняем посылку
  final int requestId = nextSumRequestId++;
  final ExecuteRequest request = ExecuteRequest(requestId, executeFunc, args);
  final Completer<T> completer = Completer<T>();
  executeRequests[requestId] = completer;

  // передаём событие в очередь новому изоляту, чтобы он начал работу
  helperIsolateSendPort1.send(request);

  // возвращаем вычисленное значение, когда основной изолят обработает событие получения результата
  return completer.future;
}

// SendPort дополнительного изолята, в который пишутся события на выполнение функции
Future<SendPort> helperIsolateSendPort = () async {
  // Так как создание изолята асинхронно, то создадим Completer, который в конечном итоге вернёт нам его SendPort
  final Completer<SendPort> completer = Completer<SendPort>();

  // ReceivePort главного изолята
  final ReceivePort receivePort = ReceivePort()
    ..listen((dynamic data) {
      if (data is SendPort) {
        // Возвращаем в главный изолят SendPort дополнительного
        completer.complete(data);
        return;
      }
      if (data is ExecuteResponse) {
        // Получили результат, записываем его в нужный Completer
        final Completer<void> completer = executeRequests[data.id]!;
        executeRequests.remove(data.id);
        completer.complete();
        return;
      }
      throw UnsupportedError('Unsupported message type: ${data.runtimeType}');
    });

  // Создаём изолят
  await Isolate.spawn((SendPort sendPort) async {
    // В изоляте создаём ReceivePort, который будет обрабатывать запросы на выполнение функций
    final ReceivePort helperReceivePort = ReceivePort()
      ..listen((dynamic data) {
        if (data is ExecuteRequest) {
          // выполняем функцию
          final result = data.function(data.args);
          final ExecuteResponse response = ExecuteResponse(data.id, result);
          // возвращаем в SendPort главного изолята событие с результатом
          sendPort.send(response);
          return;
        }
        throw UnsupportedError('Unsupported message type: ${data.runtimeType}');
      });

    // после создания внутри изолята ReceivePort передаём его SendPort для сохранения оного в основном изоляте
    // Для этого нам и нужно было передавать в изолят SendPort главного изолята
    sendPort.send(helperReceivePort.sendPort);
  }, receivePort.sendPort);

  return completer.future;
}();
