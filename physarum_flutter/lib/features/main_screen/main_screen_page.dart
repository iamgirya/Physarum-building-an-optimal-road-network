import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';

import 'package:physarum_flutter/features/graph_field/ui/graph_field_view.dart';
import 'package:physarum_flutter/features/main_screen/graph_edit_drawer_view.dart';
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
      body: Stack(
        children: [
          Row(
            crossAxisAlignment: CrossAxisAlignment.start,
            mainAxisAlignment: MainAxisAlignment.spaceBetween,
            children: [
              const GraphDrawField(
                sizeOfField: 200,
                sizeOfPixel: 3,
                isBest: false,
              ),
              const SizedBox(
                width: 15,
              ),
              Container(
                margin: const EdgeInsets.only(top: 15),
                decoration: BoxDecoration(
                  color: Colors.black.withOpacity(0.1),
                  borderRadius: const BorderRadius.all(
                    Radius.circular(10),
                  ),
                ),
                child: Column(
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
                    MainScreenButton(
                      onPressed: (context) => manager.onExecuteTap(),
                      title: 'Выполнить',
                    ),
                    const SizedBox(
                      height: 15,
                    ),
                    MainScreenButton(
                      onPressed: (context) => manager.onStopTap(),
                      title: 'Остановить',
                    ),
                    const SizedBox(
                      height: 15,
                    ),
                    MainScreenButton(
                      onPressed: (context) => manager.onRestartTap(),
                      title: 'Сбросить',
                    ),
                    const Spacer(),
                    MainScreenButton(
                      onPressed: (context) {
                        manager.onStopTap();
                        Scaffold.of(context).openEndDrawer();
                      },
                      title: 'Настроить',
                    ),
                    const SizedBox(
                      height: 15,
                    ),
                  ],
                ),
              ),
              const SizedBox(
                width: 15,
              ),
              Container(
                width: 600,
              )
            ],
          ),
          const GraphEditDrawer(),
        ],
      ),
    );
  }
}

class MainScreenButton extends StatelessWidget {
  final Function(BuildContext context) onPressed;
  final String title;

  const MainScreenButton({
    super.key,
    required this.onPressed,
    required this.title,
  });

  @override
  Widget build(BuildContext context) {
    return TextButton(
      onPressed: () => onPressed(context),
      child: Text(title),
    );
  }
}
