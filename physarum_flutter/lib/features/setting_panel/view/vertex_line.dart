import 'package:flutter/material.dart';
import 'package:flutter_hooks/flutter_hooks.dart';
import 'package:hooks_riverpod/hooks_riverpod.dart';
import '../../graph_field/graph_field_state_holders.dart';
import '../../graph_field/graph_fields_manager.dart';

class VertexLine extends HookConsumerWidget {
  const VertexLine(this.i, {super.key});
  final int i;

  @override
  Widget build(BuildContext context, WidgetRef ref) {
    final manager = ref.read(graphFieldsManager);

    final coordsTextController = useTextEditingController();
    final priorityTextController = useTextEditingController();
    // такой select нужен для того, чтобы при кликах на граф менялись текст филды, причём только нужные
    ref.watch(
      nowGraphsFieldGraphStateHolder.select((value) {
        String newCoord =
            '${value.exitPoints[i].first} ${value.exitPoints[i].second}';
        String newPriority = value.towns[i].toString();
        if (coordsTextController.text != newCoord ||
            priorityTextController.text != newPriority) {
          coordsTextController.text = newCoord;
          priorityTextController.text = newPriority;
        }
        return (value.exitPoints[i], value.towns[i]);
      }),
    );

    return Row(
      mainAxisAlignment: MainAxisAlignment.spaceAround,
      children: [
        Expanded(child: Center(child: Text(i.toString()))),
        Expanded(
          child: TextField(
            onChanged: (value) => manager.changeVectex(
              i,
              coordsTextController.text,
              priorityTextController.text,
            ),
            controller: coordsTextController,
            textAlign: TextAlign.center,
          ),
        ),
        Expanded(
          child: TextField(
            onChanged: (value) => manager.changeVectex(
              i,
              coordsTextController.text,
              priorityTextController.text,
            ),
            controller: priorityTextController,
            textAlign: TextAlign.center,
          ),
        ),
      ],
    );
  }
}
