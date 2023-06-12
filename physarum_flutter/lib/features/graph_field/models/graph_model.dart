import 'package:freezed_annotation/freezed_annotation.dart';
import 'package:flutter/foundation.dart';
import '../../../support/pair.dart';

part 'graph_model.freezed.dart';

@Freezed(makeCollectionsUnmodifiable: false)
class Graph with _$Graph {
  const Graph._();

  factory Graph({
    required List<int> towns,
    required List<Pair> exitPoints,
    required List<List<int>> graph,
  }) = _Graph;

  bool get isGraphBuilded => graph.isNotEmpty;

  static Graph empty() => Graph(towns: [], exitPoints: [], graph: []);
}
