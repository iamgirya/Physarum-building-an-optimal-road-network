import 'dart:math';

import 'package:flutter/material.dart';

import '../models/location.dart';

class LocationPainter extends CustomPainter {
  final Location location;
  final double widthScale;
  final double heightScale;
  LocationPainter(this.location, this.widthScale, this.heightScale);

  @override
  void paint(Canvas canvas, Size size) {
    final sideScale = min(widthScale, heightScale);
    Size pointSize = Size(sideScale, sideScale);
    final Rect backgroundRect = Offset.zero & size;
    canvas.drawRect(
      backgroundRect,
      Paint()..color = Colors.black,
    );
    final Color pointColor = Colors.blue.withOpacity(0.5);

    // points
    for (int i = 0; i < location.exitPoints.length; i++) {
      final double x = location.exitPoints[i].first;
      final double y = location.exitPoints[i].second;
      final Rect point = Offset(x * sideScale, y * sideScale) & pointSize;
      canvas.drawRect(
        point,
        Paint()..color = pointColor,
      );
    }
  }

  @override
  bool shouldRepaint(LocationPainter oldDelegate) => true;
}
