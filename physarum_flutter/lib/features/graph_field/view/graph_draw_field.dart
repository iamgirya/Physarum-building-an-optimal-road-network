import 'package:flutter/material.dart';

import '../graph_fields_manager.dart';
import '../models/graph_model.dart';
import '../../../support/draw_field.dart';
import 'graph_painter.dart';

class GraphDrawField extends DrawField {
  GraphDrawField({
    super.key,
    required super.sizeOfField,
    required super.title,
    required bool isEditor,
    required Graph graph,
  }) : super(
          builder: (widthScale, heightScale, ref) {
            final manager = ref.watch(graphFieldsManager);
            return isEditor
                ? GestureDetector(
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
                  )
                : null;
          },
          painterConstructor: (widthScale, heightScale) => GraphPainter(
            graph,
            widthScale,
            heightScale,
          ),
        );
}
