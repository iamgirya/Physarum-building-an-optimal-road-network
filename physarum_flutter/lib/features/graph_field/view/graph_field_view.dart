import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import '../../setting_panel/state/simulation_setting_state_holder.dart';
import '../graph_fields_manager.dart';

import '../models/graph_model.dart';
import 'graph_painter.dart';

class GraphDrawField extends ConsumerWidget {
  final double sizeOfField;
  final String title;
  final Graph graph;
  final bool isEditor;

  const GraphDrawField({
    super.key,
    required this.sizeOfField,
    required this.title,
    required this.graph,
    this.isEditor = false,
  });

  @override
  Widget build(BuildContext context, WidgetRef ref) {
    final manager = ref.watch(graphFieldsManager);

    final widthScale = sizeOfField /
        ref.watch(
          settingsStateHolder.select(
            (value) => value.settingsControllers['locationX'] ?? sizeOfField,
          ),
        );
    final heightScale = sizeOfField /
        ref.watch(
          settingsStateHolder.select(
            (value) => value.settingsControllers['locationY'] ?? sizeOfField,
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
        Text(title),
        SizedBox(
          width: sizeScale >= 1 ? sizeOfField : sizeOfField * sizeScale,
          height: sizeScale > 1 ? sizeOfField / sizeScale : sizeOfField,
          child: Stack(
            children: [
              CustomPaint(
                painter: painter,
                size: Size(
                  sizeScale >= 1 ? sizeOfField : sizeOfField * sizeScale,
                  sizeScale > 1 ? sizeOfField / sizeScale : sizeOfField,
                ),
              ),
              if (isEditor)
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
