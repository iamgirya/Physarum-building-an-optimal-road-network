import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:physarum_flutter/features/graph_field/graph_field_graph.dart';
import 'package:physarum_flutter/features/graph_field/graph_field_manager.dart';

import 'graph_painter.dart';

class GraphDrawField extends ConsumerStatefulWidget {
  final double sizeOfPixel;
  final double sizeOfField;

  const GraphDrawField({
    super.key,
    required this.sizeOfPixel,
    required this.sizeOfField,
  });

  @override
  ConsumerState<GraphDrawField> createState() => _GraphDrawFieldState();
}

class _GraphDrawFieldState extends ConsumerState<GraphDrawField> {
  @override
  Widget build(BuildContext context) {
    final manager = ref.watch(graphFieldManager);
    final painter = GraphPainter(
        ref.watch(graphsFieldGraphStateHolder), widget.sizeOfPixel);
    return SizedBox(
      height: widget.sizeOfField * widget.sizeOfPixel,
      width: widget.sizeOfField * widget.sizeOfPixel,
      child: Stack(
        children: [
          GestureDetector(
            onTapDown: (touchData) {
              manager.onTap(touchData.localPosition);
            },
            onSecondaryTapDown: (touchData) {
              manager.onSecondTap(touchData.localPosition);
            },
          ),
          CustomPaint(
            painter: painter,
            size: Size(widget.sizeOfField * widget.sizeOfPixel,
                widget.sizeOfField * widget.sizeOfPixel),
          ),
        ],
      ),
    );
  }
}
