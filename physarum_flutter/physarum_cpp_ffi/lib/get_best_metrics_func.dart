import 'dart:ffi';

import 'physarum_core.dart';
import 'physarum_flutter_adapter_model.dart';

extension GetBestMetricsFunc on PhysarumCppFfiBindings {
  /// 0 - вес
  /// 1 - дистанция
  /// 2 - устойчивость
  /// 3 - избыток потока
  List<double> getBestMetrics() {
    return _getBestMetrics();
  }

  List<double> _getBestMetrics() {
    final execute = lookup<NativeFunction<Pointer<DoubleArray> Function()>>(
            'getBestMetrics')
        .asFunction<Pointer<DoubleArray> Function()>();
    final struct = execute().ref;
    List<double> array = [];
    for (int i = 0; i < struct.length; i++) {
      array.add(struct.data[i]);
    }

    return array;
  }
}
