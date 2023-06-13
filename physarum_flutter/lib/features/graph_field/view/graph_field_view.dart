import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import '../../setting_panel/state/simulation_setting_state_holder.dart';
import '../graph_fields_manager.dart';
import '../graph_field_state_holders.dart';

import 'graph_painter.dart';

class GraphDrawField extends ConsumerStatefulWidget {
  final double sizeOfField;
  final bool isBest;

  const GraphDrawField({
    super.key,
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

    final widthScale = widget.sizeOfField /
        ref.watch(
          settingsStateHolder.select(
            (value) =>
                value.settingsControllers['locationX'] ?? widget.sizeOfField,
          ),
        );
    final heightScale = widget.sizeOfField /
        ref.watch(
          settingsStateHolder.select(
            (value) =>
                value.settingsControllers['locationY'] ?? widget.sizeOfField,
          ),
        );

    final sizeScale = heightScale / widthScale;

    final painter = GraphPainter(
      graph,
      widthScale,
      heightScale,
    );
    return Column(
      children: [
        Text(widget.isBest ? 'Лучший результат' : 'Нынешний граф'),
        SizedBox(
          width: sizeScale >= 1
              ? widget.sizeOfField
              : widget.sizeOfField * sizeScale,
          height: sizeScale > 1
              ? widget.sizeOfField / sizeScale
              : widget.sizeOfField,
          child: Stack(
            children: [
              CustomPaint(
                painter: painter,
                size: Size(
                  sizeScale >= 1
                      ? widget.sizeOfField
                      : widget.sizeOfField * sizeScale,
                  sizeScale > 1
                      ? widget.sizeOfField / sizeScale
                      : widget.sizeOfField,
                ),
              ),
              if (!widget.isBest)
                GestureDetector(
                  onTapDown: (touchData) {
                    manager.onTap(
                      touchData.localPosition,
                      widthScale,
                      heightScale,
                    );
                  },
                  onSecondaryTapDown: (touchData) {
                    manager.onSecondTap(
                      touchData.localPosition,
                      widthScale,
                      heightScale,
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
