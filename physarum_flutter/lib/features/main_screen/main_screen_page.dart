import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';

import 'package:physarum_flutter/features/graph_field/ui/graph_field_view.dart';
import 'package:physarum_flutter/features/main_screen/main_screen_manager.dart';
import 'package:physarum_flutter/features/main_screen/main_screen_state_holder.dart';

class MainPage extends ConsumerStatefulWidget {
  const MainPage({super.key});

  @override
  ConsumerState<MainPage> createState() => _MainPageState();
}

class _MainPageState extends ConsumerState<MainPage> {
  @override
  Widget build(BuildContext context) {
    final manager = ref.watch(physarumManager);
    final state = ref.watch(mainScreenStateHolder);
    return Scaffold(
      appBar: AppBar(
        backgroundColor: Theme.of(context).colorScheme.inversePrimary,
        title: const Text('Physarum Network'),
      ),
      body: Center(
        child: Row(
          mainAxisAlignment: MainAxisAlignment.start,
          children: [
            const GraphDrawField(
              sizeOfField: 200,
              sizeOfPixel: 3,
            ),
            const SizedBox(
              width: 15,
            ),
            Column(
              children: [
                SizedBox(
                  width: 200,
                  child: TextField(
                    controller: state.stepCountTextEditingController,
                  ),
                ),
                const SizedBox(
                  height: 15,
                ),
                TextButton(
                  onPressed: () {
                    manager.onExecuteTap();
                  },
                  child: const Text('Выполнить'),
                ),
                const SizedBox(
                  height: 15,
                ),
                TextButton(
                  onPressed: () {
                    manager.onStopTap();
                  },
                  child: const Text('Остановить'),
                ),
                const SizedBox(
                  height: 15,
                ),
                TextButton(
                  onPressed: () {
                    manager.onRestartTap();
                  },
                  child: const Text('Сбросить'),
                ),
              ],
            ),
          ],
        ),
      ),
    );
  }
}
