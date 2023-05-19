import 'dart:async';
import 'dart:ffi';
import 'dart:isolate';

import 'physarum_core.dart';
import 'physarum_flutter_adapter_model.dart';

// TODO Какой же это говнокод. Обещаю исправить

/// Counter to identify [_ExecuteRequest]s and [_ExecuteResponse]s.
int _nextSumRequestId = 0;

/// A request to compute `sum`.
///
/// Typically sent from one isolate to another.
class _ExecuteRequest {
  final int id;
  final int stepCount;
  final int isNeedRestart;

  const _ExecuteRequest(this.id, this.stepCount, this.isNeedRestart);
}

/// A response with the result of `sum`.
///
/// Typically sent from one isolate to another.
class _ExecuteResponse {
  final int id;

  const _ExecuteResponse(this.id);
}

/// Mapping from [_ExecuteRequest] `id`s to the completers corresponding to the correct future of the pending request.
final Map<int, Completer<void>> _executeRequests = <int, Completer<void>>{};

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
        final Completer<void> completer = _executeRequests[data.id]!;
        _executeRequests.remove(data.id);
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
        if (data is _ExecuteRequest) {
          bindings._execute(data.stepCount, data.isNeedRestart);
          final _ExecuteResponse response = _ExecuteResponse(data.id);
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

extension ExecuteFunc on PhysarumCppFfiBindings {
  Future<void> executeAsync(int a, int b) async {
    final SendPort helperIsolateSendPort = await _helperIsolateSendPort;
    final int requestId = _nextSumRequestId++;
    final _ExecuteRequest request = _ExecuteRequest(requestId, a, b);
    final Completer<void> completer = Completer<void>();
    _executeRequests[requestId] = completer;
    helperIsolateSendPort.send(request);
    return completer.future;
  }

  void _execute(int stepCount, int b) {
    final execute = lookup<NativeFunction<NativeExecute>>('execute')
        .asFunction<FFIExecute>();
    execute(stepCount, b);
  }
}
