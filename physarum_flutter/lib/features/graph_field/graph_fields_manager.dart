import 'dart:ui';

import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'graph_field_state_holders.dart';
import '../../models/pair.dart';

final graphFieldsManager = Provider<GraphFieldManager>((ref) {
  return GraphFieldManager(
    bestGraphHolder: ref.read(bestGraphsFieldGraphStateHolder.notifier),
    nowGraphHolder: ref.read(nowGraphsFieldGraphStateHolder.notifier),
  );
});

class GraphFieldManager {
  final GraphNotifier bestGraphHolder;
  final GraphNotifier nowGraphHolder;
  GraphFieldManager({
    required this.bestGraphHolder,
    required this.nowGraphHolder,
  });

  void onTap(Offset coords, double sizeOfPixel) {
    if (!nowGraphHolder.state.isGraphBuilded) {
      final newPoint = Pair(
        (coords.dx ~/ sizeOfPixel),
        (coords.dy ~/ sizeOfPixel),
      );

      final graph = nowGraphHolder.state;
      final searchResult = nowGraphHolder.state.exitPoints.indexOf(newPoint);
      if (searchResult != -1) {
        nowGraphHolder.update((state) => state..towns[searchResult] += 1);
      } else {
        nowGraphHolder.update(
          (state) => graph
            ..exitPoints.add(newPoint)
            ..towns.add(1),
        );
      }
    }
  }

  void onSecondTap(Offset coords, double sizeOfPixel) {
    if (!nowGraphHolder.state.isGraphBuilded) {
      final oldPoint = Pair(
        (coords.dx ~/ sizeOfPixel),
        (coords.dy ~/ sizeOfPixel),
      );

      final searchResult = nowGraphHolder.state.exitPoints.indexOf(oldPoint);
      if (searchResult != -1) {
        if (nowGraphHolder.state.towns[searchResult] == 1) {
          nowGraphHolder.update(
            (state) => state
              ..exitPoints.removeAt(searchResult)
              ..towns.removeAt(searchResult),
          );
        } else {
          nowGraphHolder.update((state) => state..towns[searchResult] -= 1);
        }
      }
    }
  }
}
