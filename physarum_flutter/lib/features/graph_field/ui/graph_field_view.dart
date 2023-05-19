import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:physarum_flutter/features/graph_field/graph_fields_manager.dart';
import 'package:physarum_flutter/features/graph_field/graph_field_state_holders.dart';

import 'package:physarum_flutter/features/graph_field/ui/graph_painter.dart';

class GraphDrawField extends ConsumerStatefulWidget {
  final double sizeOfPixel;
  final double sizeOfField;
  final bool isBest;

  const GraphDrawField({
    super.key,
    required this.sizeOfPixel,
    required this.sizeOfField,
    required this.isBest,
  });

  @override
  ConsumerState<GraphDrawField> createState() => _GraphDrawFieldState();
}

class _GraphDrawFieldState extends ConsumerState<GraphDrawField> {
  @override
  Widget build(BuildContext context) {
    final manager = ref.watch(graphFieldsManager);
    final graph = widget.isBest
        ? ref.watch(bestGraphsFieldGraphStateHolder)
        : ref.watch(nowGraphsFieldGraphStateHolder);
    final painter = GraphPainter(
      graph,
      widget.sizeOfPixel,
    );
    return Column(
      children: [
        Text(widget.isBest ? 'Лучший результат' : 'Нынешний граф'),
        SizedBox(
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
              if (!widget.isBest)
                GestureDetector(
                  onTapDown: (touchData) {
                    manager.onTap(touchData.localPosition, widget.sizeOfPixel);
                  },
                  onSecondaryTapDown: (touchData) {
                    manager.onSecondTap(
                      touchData.localPosition,
                      widget.sizeOfPixel,
                    );
                  },
                ),
            ],
          ),
        ),
      ],
    );
  }
}
