import 'dart:ffi';

import '../ffi_core.dart';
import '../models/graph.dart';

Graph getGraph(bool isNeedBest) {
  final execute =
      lookup<NativeFunction<Pointer<GraphFFI> Function(Bool)>>('getGraph')
          .asFunction<Pointer<GraphFFI> Function(bool)>();
  final struct = execute(isNeedBest).ref;

  Graph result = Graph();
  for (int i = 0; i < struct.length; i++) {
    // точки
    result.exitPoints
        .add([struct.exitPointsX.ref.data[i], struct.exitPointsY.ref.data[i]]);
    // города
    result.towns.add(struct.towns.ref.data[i]);
    // граф
    result.graph.add([]);
    final subArray = struct.graph.ref.data[i];
    for (int j = 0; j < subArray.length; j++) {
      result.graph[i].add(subArray.data[j]);
    }
  }

  return result;
}
