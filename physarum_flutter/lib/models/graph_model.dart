import 'package:freezed_annotation/freezed_annotation.dart';
import 'package:flutter/foundation.dart';

part 'graph_model.freezed.dart';
part 'graph_model.g.dart';

@freezed
class Graph with _$Graph {
  const factory Graph({
    required List<int> towns,
    required List<List<int>> exitPoints,
    required List<List<int>> graph,
  }) = _Graph;

  factory Graph.fromJson(Map<String, Object?> json) => _$GraphFromJson(json);
}
