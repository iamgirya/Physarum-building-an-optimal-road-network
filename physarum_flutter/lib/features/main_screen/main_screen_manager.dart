import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:physarum_cpp_ffi/get_graph_func.dart';
import 'package:physarum_cpp_ffi/get_best_metrics_func.dart';
import 'package:physarum_cpp_ffi/physarum_core.dart' as ffi;
import 'package:physarum_cpp_ffi/physarum_cpp_execute_func.dart';
import 'package:physarum_cpp_ffi/physarum_flutter_adapter_model.dart';
import 'package:physarum_flutter/features/graph_field/graph_fields_manager.dart';
import 'package:physarum_flutter/features/main_screen/main_screen_state_holder.dart';
import 'package:physarum_flutter/models/graph_model.dart';
import 'package:physarum_flutter/models/main_screen_state.dart';
import 'package:physarum_flutter/models/pair.dart';

final physarumManager = Provider<PhysarumManager>((ref) {
  return PhysarumManager(
    graphFieldManager: ref.watch(graphFieldsManager),
    mainScreenState: ref.watch(mainScreenStateHolder.notifier),
  );
});

class PhysarumManager {
  static const int iterationPerStep = 10;
  final GraphFieldManager graphFieldManager;
  final StateController<MainScreenState> mainScreenState;
  PhysarumManager({
    required this.graphFieldManager,
    required this.mainScreenState,
  });

  void onRestartTap() async {
    mainScreenState.update(
      (state) => state.copyWith(
        isNeedRestart: true,
        metricWeigth: -1,
        metricDistance: -1,
        metricResistance: -1,
        metricFlow: -1,
      ),
    );
    graphFieldManager.setNowGraph(Graph.empty());
    graphFieldManager.setBestGraph(Graph.empty());
  }

  void onStopTap() async {
    // защита на случай, если пользователь нажмёт сброс, стоп и старп.
    if (!mainScreenState.state.isNeedRestart) {
      mainScreenState.update((state) => state.copyWith(isAlgoWorking: false));
    }
  }

  void onExecuteTap() {
    if (!mainScreenState.state.isAlgoWorking) {
      final count = int.tryParse(
        mainScreenState.state.stepCountTextEditingController.text,
      );
      if (count != null && count >= 1) {
        mainScreenState.update((state) => state.copyWith(isAlgoWorking: true));

        // если нужно перезапустить, то перезапускаем и плюсовую часть
        if (mainScreenState.state.isNeedRestart) {
          mainScreenState
              .update((state) => state.copyWith(isNeedRestart: false));
          _callNextStep(count, true);
        } else {
          _callNextStep(count, false);
        }
      }
    } else if (mainScreenState.state.isAlgoWorking &&
        mainScreenState.state.isNeedRestart) {
      // В случае, если выполнение последнего шага не закончилось, пользователь нажал на рестарт
      // и сразу нажал на выполнить, то мы заносим это нажатие кнопки в очередь Future
      // Оно гарантированно выполнится после того, как отработает последний _callNextStep
      // А значит оный сделает isAlgoWorking = false и в рекурсию функции не уйдут
      Future(() => onExecuteTap());
    }
  }

  Future<void> _callNextStep(int stepCount, bool isLaunch) async {
    // вычисляем новую сеть
    await ffi.bindings.executeAsync(iterationPerStep, isLaunch ? 1 : 0);
    if (mainScreenState.state.isAlgoWorking &&
        !mainScreenState.state.isNeedRestart) {
      final bestNetwork = ffi.bindings.getGraph(true);
      graphFieldManager.setBestGraph(_parseNetworkToGraph(bestNetwork));

      final nowNetwork = ffi.bindings.getGraph(false);
      graphFieldManager.setNowGraph(_parseNetworkToGraph(nowNetwork));

      final metrics = ffi.bindings.getBestMetrics();
      if (metrics.isNotEmpty) {
        mainScreenState.update(
          (state) => state.copyWith(
            metricWeigth: metrics[0],
            metricDistance: metrics[1],
            metricResistance: metrics[2],
            metricFlow: metrics[3],
          ),
        );
      }
      // вычисляем сколько шагов осталось
      stepCount--;
      mainScreenState.state.stepCountTextEditingController.text =
          (stepCount).toString();
      if (stepCount != 0) {
        _callNextStep(stepCount, false);
      } else {
        mainScreenState.update((state) => state.copyWith(isAlgoWorking: false));
      }
    } else {
      mainScreenState.update((state) => state.copyWith(isAlgoWorking: false));
    }
  }

  Graph _parseNetworkToGraph(SlimeMoldNetwork network) {
    final graph = Graph.empty();
    for (int i = 0; i < network.exitPoints.length; i++) {
      graph.towns.add(network.towns[i]);
      graph.exitPoints
          .add(Pair(network.exitPoints[i][0], network.exitPoints[i][1]));
      graph.graph.add(network.graph[i]);
    }
    return graph;
  }
}
