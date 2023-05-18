import 'dart:ui';

import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:physarum_flutter/features/graph_field/graph_field_state.dart';
import 'package:physarum_flutter/models/graph_model.dart';
import 'package:physarum_flutter/models/pair.dart';

final graphFieldManager = Provider<GraphFieldManager>((ref) {
  return GraphFieldManager(
    graphHolder: ref.read(graphsFieldGraphStateHolder.notifier),
  );
});

class GraphFieldManager {
  final GraphNotifier graphHolder;
  GraphFieldManager({
    required this.graphHolder,
  });

  void onTap(Offset coords, double sizeOfPixel) {
    final newPoint = Pair(
      (coords.dx ~/ sizeOfPixel),
      (coords.dy ~/ sizeOfPixel),
    );

    final graph = graphHolder.state;
    final searchResult = graphHolder.state.exitPoints.indexOf(newPoint);
    if (searchResult != -1) {
      graphHolder.update((state) => state..towns[searchResult] += 1);
    } else {
      graphHolder.update(
        (state) => graph
          ..exitPoints.add(newPoint)
          ..towns.add(1),
      );
    }
  }

  void onSecondTap(Offset coords, double sizeOfPixel) {
    final oldPoint = Pair(
      (coords.dx ~/ sizeOfPixel),
      (coords.dy ~/ sizeOfPixel),
    );

    final searchResult = graphHolder.state.exitPoints.indexOf(oldPoint);
    if (searchResult != -1) {
      if (graphHolder.state.towns[searchResult] == 1) {
        graphHolder.update(
          (state) => state
            ..exitPoints.removeAt(searchResult)
            ..towns.removeAt(searchResult),
        );
      } else {
        graphHolder.update((state) => state..towns[searchResult] -= 1);
      }
    }
  }

  void setNewGraph(Graph newGraph) {
    graphHolder.update((state) => newGraph);
  }
}
