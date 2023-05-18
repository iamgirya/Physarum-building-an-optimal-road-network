import 'package:flutter_riverpod/flutter_riverpod.dart';

import 'package:physarum_flutter/models/graph_model.dart';

final graphsFieldGraphStateHolder =
    NotifierProvider<GraphNotifier, Graph>(() => GraphNotifier());

class GraphNotifier extends Notifier<Graph> {
  @override
  Graph build() => Graph.empty();

// не пиздеть
// ignore: avoid_public_notifier_properties
  @override
  Graph get state => super.state;

  void update(Graph Function(Graph) func) {
    super.state = func(super.state);
  }

  @override
  bool updateShouldNotify(Graph previous, Graph next) => true;
}
