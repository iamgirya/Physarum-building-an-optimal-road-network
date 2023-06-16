import 'package:flutter_riverpod/flutter_riverpod.dart';

import 'models/graph_model.dart';

final nowGraphsFieldGraphStateHolder =
    NotifierProvider<GraphNotifier, Graph>(() => GraphNotifier());

final bestGraphsFieldGraphStateHolder =
    NotifierProvider<GraphNotifier, Graph>(() => GraphNotifier());

class GraphNotifier extends Notifier<Graph> {
  @override
  Graph build() => Graph.empty();

// ignore: avoid_public_notifier_properties
  @override
  Graph get state => super.state;

  void update(Graph Function(Graph) func) {
    super.state = func(super.state);
  }
}
