import 'dart:async';
import 'dart:ffi';
import 'dart:isolate';

import '../ffi_core.dart';

Future<void> executeAsync(int stepCount) async {
  final SendPort helperIsolateSendPort1 = await helperIsolateSendPort;
  final int requestId = nextSumRequestId++;
  final ExecuteRequest request = ExecuteRequest(requestId, stepCount);
  final Completer<void> completer = Completer<void>();
  executeRequests[requestId] = completer;
  helperIsolateSendPort1.send(request);
  return completer.future;
}

void execute(int stepCount) {
  final execute =
      lookup<NativeFunction<NativeExecute>>('execute').asFunction<FFIExecute>();
  execute(stepCount);
}
