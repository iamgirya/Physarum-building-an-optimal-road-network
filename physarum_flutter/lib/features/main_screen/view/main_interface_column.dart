import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:physarum_flutter/features/main_screen/view/main_screen_button.dart';
import 'package:physarum_flutter/features/main_screen/main_screen_manager.dart';

import 'package:physarum_flutter/features/main_screen/main_screen_state_holder.dart';

class MainInterfaceColumn extends ConsumerStatefulWidget {
  const MainInterfaceColumn({super.key});

  @override
  ConsumerState<MainInterfaceColumn> createState() =>
      _MainInterfaceColumnState();
}

class _MainInterfaceColumnState extends ConsumerState<MainInterfaceColumn> {
  @override
  Widget build(BuildContext context) {
    final manager = ref.watch(physarumManager);
    final state = ref.watch(mainScreenStateHolder);
    return Container(
      margin: const EdgeInsets.only(top: 15),
      decoration: BoxDecoration(
        color: Colors.black.withOpacity(0.1),
        borderRadius: const BorderRadius.all(
          Radius.circular(10),
        ),
      ),
      child: Column(
        children: [
          Container(
            padding: const EdgeInsets.symmetric(horizontal: 10),
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
          const Text('Вес:'),
          Text(
            state.metricWeigth == -1
                ? '-'
                : state.metricWeigth.toStringAsFixed(3),
          ),
          const SizedBox(
            height: 5,
          ),
          const Text('Дистанция'),
          Text(
            state.metricDistance == -1
                ? '-'
                : state.metricDistance.toStringAsFixed(3),
          ),
          const SizedBox(
            height: 5,
          ),
          const Text('Устойчивость'),
          Text(
            state.metricResistance == -1
                ? '-'
                : state.metricResistance.toStringAsFixed(3),
          ),
          const SizedBox(
            height: 5,
          ),
          const Text('Поток'),
          Text(
            state.metricFlow == -1 ? '-' : state.metricFlow.toStringAsFixed(3),
          ),
          const SizedBox(
            height: 15,
          ),
        ],
      ),
    );
  }
}
