import 'package:flutter/material.dart';

import 'package:physarum_flutter/models/graph_model.dart';

class GraphPainter extends CustomPainter {
  GraphPainter(this.graph, this.sizeOfPixel);
  final Graph graph;
  final double sizeOfPixel;

  @override
  void paint(Canvas canvas, Size size) {
    Size pointSize = Size(sizeOfPixel, sizeOfPixel);
    final Rect backgroundRect = Offset.zero & size;
    canvas.drawRect(
      backgroundRect,
      Paint()..color = Colors.blue,
    );

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
