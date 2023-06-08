import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import '../../graph_field/graph_field_state_holders.dart';

class VertexList extends ConsumerStatefulWidget {
  const VertexList({super.key});

  @override
  ConsumerState<VertexList> createState() => _VertexListState();
}

class _VertexListState extends ConsumerState<VertexList> {
  @override
  Widget build(BuildContext context) {
    final graph = ref.watch(nowGraphsFieldGraphStateHolder);
    return graph.isGraphBuilded
        ? const SizedBox()
        : Column(
            children: [
              const Row(
                mainAxisAlignment: MainAxisAlignment.spaceAround,
                children: [
                  Text('Индекс'),
                  Text(
                    'Координаты',
                  ),
                  Text('Вес'),
                ],
              ),
              Expanded(
                child: ListView.separated(
                  itemBuilder: (context, i) {
                    return Row(
                      mainAxisAlignment: MainAxisAlignment.spaceAround,
                      children: [
                        Text(i.toString()),
                        Text(
                          '${graph.exitPoints[i].first}.${graph.exitPoints[i].second}',
                        ),
                        Text(graph.towns[i].toString()),
                      ],
                    );
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
