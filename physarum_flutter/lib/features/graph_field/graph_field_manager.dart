import 'dart:ui';

import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:physarum_flutter/features/graph_field/graph_field_graph.dart';
import 'package:physarum_flutter/models/graph_model.dart';

final graphFieldManager = Provider<LandingPageManager>((ref) {
  return LandingPageManager(
    graph: ref.read(graphsFieldGraphStateHolder.notifier),
  );
});

class LandingPageManager {
  final StateController<Graph> graph;
  LandingPageManager({
    required this.graph,
  });

  void onTap(Offset coords) {}

  void onSecondTap(Offset coords) {}
}
