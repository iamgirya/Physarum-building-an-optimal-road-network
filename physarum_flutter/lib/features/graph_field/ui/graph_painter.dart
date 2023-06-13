import 'package:flutter/material.dart';

import '../models/graph_model.dart';
import '../../../support/pair.dart';

class GraphPainter extends CustomPainter {
  final Graph graph;
  final double widthScale;
  final double heightScale;
  GraphPainter(this.graph, this.widthScale, this.heightScale);

  @override
  void paint(Canvas canvas, Size size) {
    Size pointSize = Size(widthScale, heightScale);
    final Rect backgroundRect = Offset.zero & size;
    canvas.drawRect(
      backgroundRect,
      Paint()..color = Colors.black,
    );

    // graph
    const Color edgeColor = Colors.red;
    for (int h = 0; h < graph.graph.length; h++) {
      for (int k = 0; k < graph.graph[h].length; k++) {
        Pair first = graph.exitPoints[h];
        Pair second = graph.exitPoints[graph.graph[h][k]];

        canvas.drawLine(
          Offset(
            first.first * pointSize.width + widthScale / 2,
            first.second * pointSize.height + heightScale / 2,
          ),
          Offset(
            second.first * pointSize.width + widthScale / 2,
            second.second * pointSize.height + heightScale / 2,
          ),
          Paint()..color = edgeColor,
        );
      }
    }

    // points
    for (int i = 0; i < graph.exitPoints.length; i++) {
      final int x = graph.exitPoints[i].first;
      final int y = graph.exitPoints[i].second;
      final Rect point =
          Offset(x * pointSize.width, y * pointSize.height) & pointSize;
      final Color pointColor =
          graph.towns[i] != 0 ? Colors.white : Colors.lightGreen;

      canvas.drawRect(
        point,
        Paint()..color = pointColor,
      );
    }
  }

  @override
  bool shouldRepaint(GraphPainter oldDelegate) => true;
}
