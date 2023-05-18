import 'package:flutter/material.dart';

import '../../../models/graph_model.dart';

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
      Paint()..color = Colors.black,
    );

    // points
    for (int i = 0; i < graph.exitPoints.length; i++) {
      final int x = graph.exitPoints[i][0];
      final int y = graph.exitPoints[i][1];
      final Rect point = Offset(x - 1, y - 1) & pointSize;
      final Color pointColor =
          graph.towns[i] != 0 ? Colors.white : Colors.lightGreen;

      canvas.drawRect(
        point,
        Paint()..color = pointColor,
      );
    }
  }

  @override
  bool shouldRepaint(GraphPainter oldDelegate) => oldDelegate.graph == graph;
}
