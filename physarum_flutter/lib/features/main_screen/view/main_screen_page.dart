import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';

import '../../graph_field/graph_field_state_holders.dart';
import '../../graph_field/view/graph_field_view.dart';
import 'graph_edit_drawer.dart';
import 'main_interface_column.dart';

import '../../setting_panel/view/setting_panel_view.dart';

class MainPage extends StatelessWidget {
  const MainPage({super.key});

  @override
  Widget build(BuildContext context) {
    const sizeOfFied = 600.0;
    return Scaffold(
      appBar: AppBar(
        backgroundColor: Theme.of(context).colorScheme.inversePrimary,
        title: const Text('Physarum Network'),
      ),
      body: Stack(
        children: [
          Row(
            crossAxisAlignment: CrossAxisAlignment.start,
            mainAxisAlignment: MainAxisAlignment.spaceBetween,
            children: [
              Consumer(
                builder: (context, ref, child) {
                  final graph = ref.watch(nowGraphsFieldGraphStateHolder);
                  return GraphDrawField(
                    sizeOfField: sizeOfFied,
                    title: 'Текущий граф',
                    graph: graph,
                    isEditor: true,
                  );
                },
              ),
              const SizedBox(
                width: 15,
              ),
              const MainInterfaceColumn(),
              const SizedBox(
                width: 15,
              ),
              const SizedBox(
                width: sizeOfFied,
                height: sizeOfFied,
                child: SettingsPanel(),
              ),
            ],
          ),
          const GraphEditDrawer(sizeOfFied: sizeOfFied),
        ],
      ),
    );
  }
}
