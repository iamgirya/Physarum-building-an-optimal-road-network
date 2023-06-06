import 'dart:async';
import 'dart:ffi';
import 'dart:io';
import 'dart:isolate';

import 'functions/execute.dart';

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

// TODO Какой же это говнокод. Обещаю исправить
typedef NativeExecute = Void Function(Int);
typedef FFIExecute = void Function(int);

/// Counter to identify [ExecuteRequest]s and [ExecuteResponse]s.
int nextSumRequestId = 0;

/// A request to compute `sum`.
///
/// Typically sent from one isolate to another.
class ExecuteRequest {
  final int id;
  final int stepCount;

  const ExecuteRequest(this.id, this.stepCount);
}

/// A response with the result of `sum`.
///
/// Typically sent from one isolate to another.
class ExecuteResponse {
  final int id;

  const ExecuteResponse(this.id);
}

/// Mapping from [ExecuteRequest] `id`s to the completers corresponding to the correct future of the pending request.
final Map<int, Completer<void>> executeRequests = <int, Completer<void>>{};

/// The SendPort belonging to the helper isolate.
Future<SendPort> helperIsolateSendPort = () async {
  // The helper isolate is going to send us back a SendPort, which we want to
  // wait for.
  final Completer<SendPort> completer = Completer<SendPort>();

  // Receive port on the main isolate to receive messages from the helper.
  // We receive two types of messages:
  // 1. A port to send messages on.
  // 2. Responses to requests we sent.
  final ReceivePort receivePort = ReceivePort()
    ..listen((dynamic data) {
      if (data is SendPort) {
        // The helper isolate sent us the port on which we can sent it requests.
        completer.complete(data);
        return;
      }
      if (data is ExecuteResponse) {
        // The helper isolate sent us a response to a request we sent.
        final Completer<void> completer = executeRequests[data.id]!;
        executeRequests.remove(data.id);
        completer.complete();
        return;
      }
      throw UnsupportedError('Unsupported message type: ${data.runtimeType}');
    });

  // Start the helper isolate.
  await Isolate.spawn((SendPort sendPort) async {
    final ReceivePort helperReceivePort = ReceivePort()
      ..listen((dynamic data) {
        // On the helper isolate listen to requests and respond to them.
        if (data is ExecuteRequest) {
          execute(data.stepCount);
          final ExecuteResponse response = ExecuteResponse(data.id);
          sendPort.send(response);
          return;
        }
        throw UnsupportedError('Unsupported message type: ${data.runtimeType}');
      });

    // Send the port to the main isolate on which we can receive requests.
    sendPort.send(helperReceivePort.sendPort);
  }, receivePort.sendPort);

  // Wait until the helper isolate has sent us back the SendPort on which we
  // can start sending requests.
  return completer.future;
}();
