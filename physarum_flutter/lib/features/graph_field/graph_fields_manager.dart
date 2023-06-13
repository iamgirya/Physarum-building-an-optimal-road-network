import 'dart:math';
import 'dart:ui';

import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'graph_field_state_holders.dart';
import '../../support/pair.dart';

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

  bool _validateCoords(String textCoords) {
    if (RegExp(r'(\d+)\s(\d+)').hasMatch(textCoords)) {
      final coords = textCoords.split(' ');
      if (int.tryParse(coords[0]) != null && int.tryParse(coords[1]) != null) {
        final x = int.parse(coords[0]);
        final y = int.parse(coords[1]);
        if (x < 200 && y < 200 && x >= 0 && y >= 0) {
          return true;
        }
      }
    }
    return false;
  }

  void changeVectex(int index, String textCoords, String priority) {
    if (_validateCoords(textCoords) && int.tryParse(priority) != null) {
      final coords = textCoords.split(' ');
      nowGraphHolder.update(
        (state) => state
          ..towns[index] = int.parse(priority)
          ..exitPoints[index] =
              Pair(int.parse(coords[0]), int.parse(coords[1])),
      );
    }
  }

  void onTap(Offset coords, double widthScale, double heightScale) {
    if (!nowGraphHolder.state.isGraphBuilded) {
      final scale = min(widthScale, heightScale);
      final newPoint = Pair(
        ((coords.dx / scale).round()),
        ((coords.dy / scale).round()),
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

  void onSecondTap(Offset coords, double widthScale, double heightScale) {
    if (!nowGraphHolder.state.isGraphBuilded) {
      final scale = min(widthScale, heightScale);
      final oldPoint = Pair(
        ((coords.dx / scale).round()),
        ((coords.dy / scale).round()),
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

  void validateVertex(num width, num height) {
    final validPoints = <Pair>[];
    final validTowns = <int>[];
    for (int i = 0; i < nowGraphHolder.state.exitPoints.length; i++) {
      final point = nowGraphHolder.state.exitPoints[i];
      if (point.first < width && point.second < height) {
        validPoints.add(point);
        validTowns.add(nowGraphHolder.state.towns[i]);
      }
    }
    nowGraphHolder.update(
      (state) => state.copyWith(exitPoints: validPoints, towns: validTowns),
    );
  }
}
