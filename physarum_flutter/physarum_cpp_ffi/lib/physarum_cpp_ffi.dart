import 'dart:async';
import 'dart:ffi';
import 'dart:io';
import 'dart:isolate';

import 'physarum_cpp_ffi_bindings_generated.dart';

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

final PhysarumCppFfiBindings _bindings = PhysarumCppFfiBindings(_dylib);

//functions

Future<SlimeMoldNetwork> executeAsync(int a, int b) async {
  final SendPort helperIsolateSendPort = await _helperIsolateSendPort;
  final int requestId = _nextSumRequestId++;
  final _ExecuteRequest request = _ExecuteRequest(requestId, a, b);
  final Completer<SlimeMoldNetwork> completer = Completer<SlimeMoldNetwork>();
  _sumRequests[requestId] = completer;
  helperIsolateSendPort.send(request);
  return completer.future;
}

/// Counter to identify [_ExecuteRequest]s and [_ExecuteResponse]s.
int _nextSumRequestId = 0;

/// A request to compute `sum`.
///
/// Typically sent from one isolate to another.
class _ExecuteRequest {
  final int id;
  final int a;
  final int b;

  const _ExecuteRequest(this.id, this.a, this.b);
}

/// A response with the result of `sum`.
///
/// Typically sent from one isolate to another.
class _ExecuteResponse {
  final int id;
  final SlimeMoldNetwork result;

  const _ExecuteResponse(this.id, this.result);
}

/// Mapping from [_ExecuteRequest] `id`s to the completers corresponding to the correct future of the pending request.
final Map<int, Completer<SlimeMoldNetwork>> _sumRequests =
    <int, Completer<SlimeMoldNetwork>>{};

/// The SendPort belonging to the helper isolate.
Future<SendPort> _helperIsolateSendPort = () async {
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
      if (data is _ExecuteResponse) {
        // The helper isolate sent us a response to a request we sent.
        final Completer<SlimeMoldNetwork> completer = _sumRequests[data.id]!;
        _sumRequests.remove(data.id);
        completer.complete(data.result);
        return;
      }
      throw UnsupportedError('Unsupported message type: ${data.runtimeType}');
    });

  // Start the helper isolate.
  await Isolate.spawn((SendPort sendPort) async {
    final ReceivePort helperReceivePort = ReceivePort()
      ..listen((dynamic data) {
        // On the helper isolate listen to requests and respond to them.
        if (data is _ExecuteRequest) {
          final SlimeMoldNetwork result = _bindings.execute(data.a, data.b);
          final _ExecuteResponse response = _ExecuteResponse(data.id, result);
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
