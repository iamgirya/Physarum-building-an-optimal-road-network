import 'dart:ffi';

import '../ffi_core.dart';
import '../models/arrays.dart';

/// 0 - вес
/// 1 - дистанция
/// 2 - устойчивость
/// 3 - избыток потока
List<double> getBestMetrics() {
  final execute =
      lookup<NativeFunction<Pointer<DoubleArray> Function()>>('getBestMetrics')
          .asFunction<Pointer<DoubleArray> Function()>();
  final struct = execute().ref;
  List<double> array = [];
  for (int i = 0; i < struct.length; i++) {
    array.add(struct.data[i]);
  }

  return array;
}
