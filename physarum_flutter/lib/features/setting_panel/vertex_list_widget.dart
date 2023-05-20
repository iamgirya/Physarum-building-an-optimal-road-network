import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:physarum_flutter/features/graph_field/graph_field_state_holders.dart';

class VertexList extends ConsumerStatefulWidget {
  const VertexList({super.key});

  @override
  ConsumerState<VertexList> createState() => _VertexListState();
}

class _VertexListState extends ConsumerState<VertexList> {
  @override
  Widget build(BuildContext context) {
    // TODO сделать настройку вершин через менюшку
    final graph = ref.watch(nowGraphsFieldGraphStateHolder);
    return graph.isGraphBuilded
        ? const _IsAlgoWorkingMessage()
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

class _IsAlgoWorkingMessage extends StatelessWidget {
  const _IsAlgoWorkingMessage();

  @override
  Widget build(BuildContext context) {
    return const Center(
      child: Text(
        'Для установки новых точек нужно сбросить нынешнюю симуляцию',
      ),
    );
  }
}
