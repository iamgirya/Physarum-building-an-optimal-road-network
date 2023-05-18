import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:physarum_flutter/features/graph_field/graph_field_state_holder.dart';
import 'package:physarum_flutter/features/graph_field/graph_field_manager.dart';

import 'package:physarum_flutter/features/graph_field/ui/graph_painter.dart';

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
    final graph = ref.watch(graphsFieldGraphStateHolder);
    final painter = GraphPainter(
      graph,
      widget.sizeOfPixel,
    );
    return SizedBox(
      height: widget.sizeOfField * widget.sizeOfPixel,
      width: widget.sizeOfField * widget.sizeOfPixel,
      child: Stack(
        children: [
          CustomPaint(
            painter: painter,
            size: Size(
              widget.sizeOfField * widget.sizeOfPixel,
              widget.sizeOfField * widget.sizeOfPixel,
            ),
          ),
          GestureDetector(
            onTapDown: (touchData) {
              manager.onTap(touchData.localPosition, widget.sizeOfPixel);
            },
            onSecondaryTapDown: (touchData) {
              manager.onSecondTap(touchData.localPosition, widget.sizeOfPixel);
            },
          ),
        ],
      ),
    );
  }
}
