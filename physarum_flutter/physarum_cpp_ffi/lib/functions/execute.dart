import 'dart:async';
import 'dart:ffi';

import '../ffi_core.dart';

Future<void> makeStep(int stepCount) async {
  return await executeInIsolate<void>(
      _makeStepForIsolate, {'stepCount': stepCount});
}

// Проблема с верхнеуровневыми функциями.
// В связи с тем, что через SendPort нужно передавать только верхнеуровневые функции
// и для того, чтобы сделать общую реализацию для всех возможных функций,
// приходится писать вот такой "адаптер", чтобы передать как параметр в executeInIsolate
void _makeStepForIsolate(Map<String, dynamic> args) {
  final execute = lookup<NativeFunction<Void Function(Int)>>('execute')
      .asFunction<void Function(int)>();
  return execute(args['stepCount']);
}
