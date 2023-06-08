import 'package:flutter/material.dart';
import 'package:flutter_hooks/flutter_hooks.dart';
import 'package:hooks_riverpod/hooks_riverpod.dart';
import '../../graph_field/graph_field_state_holders.dart';
import '../../graph_field/graph_fields_manager.dart';

class VertexList extends HookConsumerWidget {
  const VertexList({super.key});

  @override
  Widget build(BuildContext context, WidgetRef ref) {
    final graph = ref.watch(nowGraphsFieldGraphStateHolder);
    return graph.isGraphBuilded
        ? const SizedBox()
        : Column(
            children: [
              const Row(
                mainAxisAlignment: MainAxisAlignment.spaceAround,
                children: [
                  Text('Номер'),
                  Text(
                    'Координаты',
                  ),
                  Text('Вес'),
                ],
              ),
              Expanded(
                child: ListView.separated(
                  itemBuilder: (context, i) {
                    return VertexLine(i);
                  },
                  separatorBuilder: (context, i) {
                    return const SizedBox(
                      height: 5,
                    );
                  },
                  itemCount: graph.towns.length,
                ),
              ),
            ],
          );
  }
}

class VertexLine extends HookConsumerWidget {
  const VertexLine(this.i, {super.key});

  final int i;

  @override
  Widget build(BuildContext context, WidgetRef ref) {
    final manager = ref.watch(graphFieldsManager);
    final graph = ref.watch(nowGraphsFieldGraphStateHolder);

    final coordsTextController = useTextEditingController(
      text: '${graph.exitPoints[i].first} ${graph.exitPoints[i].second}',
    );
    final priorityTextController = useTextEditingController(
      text: graph.towns[i].toString(),
    );

    coordsTextController.addListener(
      () => manager.changeVectex(
        i,
        coordsTextController.text,
        priorityTextController.text,
      ),
    );
    priorityTextController.addListener(
      () => manager.changeVectex(
        i,
        coordsTextController.text,
        priorityTextController.text,
      ),
    );

    return Row(
      mainAxisAlignment: MainAxisAlignment.spaceAround,
      children: [
        Expanded(child: Center(child: Text(i.toString()))),
        Expanded(
          child: TextField(
            controller: coordsTextController,
            textAlign: TextAlign.center,
          ),
        ),
        Expanded(
          child: TextField(
            controller: priorityTextController,
            textAlign: TextAlign.center,
          ),
        ),
      ],
    );
  }
}
