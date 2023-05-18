import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';

import 'package:physarum_flutter/features/graph_field/ui/graph_field_view.dart';
import 'package:physarum_flutter/features/main_screen/physarum_call_manager.dart';

class MainPage extends ConsumerStatefulWidget {
  const MainPage({super.key, required this.title});
  final String title;

  @override
  ConsumerState<MainPage> createState() => _MainPageState();
}

class _MainPageState extends ConsumerState<MainPage> {
  @override
  Widget build(BuildContext context) {
    final manager = ref.watch(physarumManager);
    return Scaffold(
      appBar: AppBar(
        backgroundColor: Theme.of(context).colorScheme.inversePrimary,
        title: Text(widget.title),
      ),
      body: Center(
        child: Column(
          children: [
            const GraphDrawField(
              sizeOfField: 200,
              sizeOfPixel: 3,
            ),
            TextButton(
              onPressed: () {
                manager.onExecuteButtonTap();
              },
              child: const Text('Выполнить'),
            ),
          ],
        ),
      ),
    );
  }
}
