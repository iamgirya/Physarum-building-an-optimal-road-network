import 'package:flutter/material.dart';
import 'package:hooks_riverpod/hooks_riverpod.dart';
import '../../graph_field/graph_field_state_holders.dart';
import 'vertex_line.dart';

class VertexList extends HookConsumerWidget {
  const VertexList({super.key});

  @override
  Widget build(BuildContext context, WidgetRef ref) {
    final isGraphBuilded = ref.watch(
      nowGraphsFieldGraphStateHolder.select((value) => value.isGraphBuilded),
    );
    final townsCount = ref.watch(
      nowGraphsFieldGraphStateHolder.select((value) => value.towns.length),
    );
    return isGraphBuilded
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
                  itemCount: townsCount,
                ),
              ),
            ],
          );
  }
}
