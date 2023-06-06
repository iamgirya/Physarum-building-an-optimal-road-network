import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:physarum_cpp_ffi/get_graph_func.dart';
import 'package:physarum_cpp_ffi/get_best_metrics_func.dart';
import 'package:physarum_cpp_ffi/ffi_core.dart' as ffi;
import 'package:physarum_cpp_ffi/execute_func.dart';
import 'package:physarum_cpp_ffi/flutter_adapter_model.dart';
import 'package:physarum_cpp_ffi/set_up_settings.dart';
import 'package:physarum_cpp_ffi/set_up_towns.dart';
import '../graph_field/graph_field_state_holders.dart';
import 'main_screen_state_holder.dart';
import '../setting_panel/simulation_setting_state_holder.dart';
import '../../models/graph_model.dart';
import '../../models/main_screen_state.dart';
import '../../models/pair.dart';
import '../../models/settings_state.dart';

final physarumManager = Provider<PhysarumManager>((ref) {
  return PhysarumManager(
    mainScreenState: ref.watch(mainScreenStateHolder.notifier),
    bestGraphHolder: ref.watch(bestGraphsFieldGraphStateHolder.notifier),
    nowGraphHolder: ref.watch(nowGraphsFieldGraphStateHolder.notifier),
    settingsState: ref.watch(settingsStateHolder.notifier),
  );
});

class PhysarumManager {
  static const int iterationPerStep = 10;
  final StateController<MainScreenState> mainScreenState;
  final GraphNotifier bestGraphHolder;
  final GraphNotifier nowGraphHolder;
  final StateController<SettingsState> settingsState;
  PhysarumManager({
    required this.mainScreenState,
    required this.bestGraphHolder,
    required this.nowGraphHolder,
    required this.settingsState,
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
    _setGraphFromNetwork(Graph.empty(), isBest: false);
    _setGraphFromNetwork(Graph.empty(), isBest: true);
  }

  void onStopTap() async {
    // защита на случай, если пользователь нажмёт сброс, стоп и старп.
    if (!mainScreenState.state.isNeedRestart) {
      mainScreenState.update((state) => state.copyWith(isAlgoWorking: false));
    }
  }

  void onExecuteTap() {
    if (nowGraphHolder.state.towns.isEmpty) {
      return;
    }

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
    if (isLaunch) {
      ffi.bindings.setUpSimulation(
        settingsState.state.settingsControllers
            .map((key, value) => MapEntry(key, num.parse(value.text))),
      );
      ffi.bindings.setUpTowns(
        nowGraphHolder.state.exitPoints
            .map((e) => [e.first, e.second])
            .toList(),
        nowGraphHolder.state.towns,
      );
    }

    await ffi.bindings.executeAsync(iterationPerStep);
    if (mainScreenState.state.isAlgoWorking &&
        !mainScreenState.state.isNeedRestart) {
      final bestNetwork = ffi.bindings.getGraph(true);
      _setGraphFromNetwork(bestNetwork, isBest: true);

      final nowNetwork = ffi.bindings.getGraph(false);
      _setGraphFromNetwork(nowNetwork, isBest: false);

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

  // принимает первым аргументов SlimeMoldNetwork или Graph
  // TODO переделай на силед
  void _setGraphFromNetwork(Object networkOrGraph, {required bool isBest}) {
    Graph graph;
    if (networkOrGraph is SlimeMoldNetwork) {
      graph = Graph.empty();
      for (int i = 0; i < networkOrGraph.exitPoints.length; i++) {
        graph.towns.add(networkOrGraph.towns[i]);
        graph.exitPoints.add(
          Pair(
            networkOrGraph.exitPoints[i][0],
            networkOrGraph.exitPoints[i][1],
          ),
        );
        graph.graph.add(networkOrGraph.graph[i]);
      }
    } else if (networkOrGraph is Graph) {
      graph = networkOrGraph;
    } else {
      throw TypeError();
    }

    final stateHolder = isBest ? bestGraphHolder : nowGraphHolder;
    stateHolder.update((state) => graph);
  }
}
