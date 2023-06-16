import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import '../features/setting_panel/state/simulation_setting_state_holder.dart';

class DrawField extends ConsumerWidget {
  final double sizeOfField;
  final String title;
  final CustomPainter Function(double widthScale, double heightScale)
      painterConstructor;
  final Widget? Function(double widthScale, double heightScale, WidgetRef ref)
      builder;

  const DrawField({
    super.key,
    required this.sizeOfField,
    required this.title,
    required this.painterConstructor,
    required this.builder,
  });

  @override
  Widget build(BuildContext context, WidgetRef ref) {
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

    final painter = painterConstructor(widthScale, heightScale);
    final child = builder(widthScale, heightScale, ref);
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
              if (child != null) child,
            ],
          ),
        ),
      ],
    );
  }
}
