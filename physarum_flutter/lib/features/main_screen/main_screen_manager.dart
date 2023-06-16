import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:physarum_cpp_ffi/physarum_bindings.dart' as bindings;
import '../graph_field/graph_field_state_holders.dart';
import '../graph_field/graph_fields_manager.dart';
import 'state/main_screen_state_holder.dart';
import '../setting_panel/state/simulation_setting_state_holder.dart';
import '../graph_field/models/graph_model.dart';
import 'state/main_screen_state.dart';
import '../../support/pair.dart';
import '../setting_panel/state/settings_state.dart';

final mainScreenManager = Provider<MainScreenManager>((ref) {
  return MainScreenManager(
    graphManager: ref.watch(graphFieldsManager),
    mainScreenHolder: ref.watch(mainScreenStateHolder.notifier),
    bestGraphHolder: ref.watch(bestGraphsFieldGraphStateHolder.notifier),
    nowGraphHolder: ref.watch(nowGraphsFieldGraphStateHolder.notifier),
    settingsHolder: ref.watch(settingsStateHolder.notifier),
  );
});

class MainScreenManager {
  static const int iterationPerStep = 10;
  final GraphFieldManager graphManager;
  final StateController<MainScreenState> mainScreenHolder;
  final GraphNotifier bestGraphHolder;
  final GraphNotifier nowGraphHolder;
  final StateController<SettingsState> settingsHolder;
  MainScreenManager({
    required this.mainScreenHolder,
    required this.bestGraphHolder,
    required this.nowGraphHolder,
    required this.settingsHolder,
    required this.graphManager,
  });

  void onRestartTap() async {
    mainScreenHolder.update(
      (state) => state.copyWith(
        isNeedRestart: true,
        metricWeigth: -1,
        metricDistance: -1,
        metricResistance: -1,
        metricFlow: -1,
      ),
    );
    _setGraphFromNetwork(isBest: false);
    _setGraphFromNetwork(isBest: true);
  }

  void onStopTap() async {
    // защита на случай, если пользователь нажмёт сброс, стоп и старп.
    if (!mainScreenHolder.state.isNeedRestart) {
      mainScreenHolder.update((state) => state.copyWith(isAlgoWorking: false));
    }
  }

  void onExecuteTap() {
    graphManager.validateVertex(
      settingsHolder.state.settingsControllers['locationX']!,
      settingsHolder.state.settingsControllers['locationY']!,
    );

    if (nowGraphHolder.state.towns.isEmpty) {
      return;
    }

    if (!mainScreenHolder.state.isAlgoWorking) {
      final count = int.tryParse(
        mainScreenHolder.state.stepCountTextEditingController.text,
      );
      if (count != null && count >= 1) {
        mainScreenHolder.update((state) => state.copyWith(isAlgoWorking: true));

        // если нужно перезапустить, то перезапускаем и плюсовую часть
        if (mainScreenHolder.state.isNeedRestart) {
          mainScreenHolder
              .update((state) => state.copyWith(isNeedRestart: false));
          _callNextStep(count, true);
        } else {
          _callNextStep(count, false);
        }
      }
    } else if (mainScreenHolder.state.isAlgoWorking &&
        mainScreenHolder.state.isNeedRestart) {
      // В случае, если выполнение последнего шага не закончилось, пользователь нажал на рестарт
      // и сразу нажал на выполнить, то мы заносим это нажатие кнопки в очередь Future
      // Оно гарантированно выполнится после того, как отработает последний _callNextStep
      // А значит оный сделает isAlgoWorking = false и в рекурсию функции не уйдут
      Future(() => onExecuteTap());
    }
  }

  Future<void> _callNextStep(int stepCount, bool isLaunch) async {
    if (isLaunch) {
      bindings.setUpSimulation(
        settingsHolder.state.settingsControllers,
      );
      bindings.setUpTowns(
        nowGraphHolder.state.exitPoints
            .map((e) => [e.first, e.second])
            .toList(),
        nowGraphHolder.state.towns,
      );
    }

    await bindings.execute(iterationPerStep);
    if (mainScreenHolder.state.isAlgoWorking &&
        !mainScreenHolder.state.isNeedRestart) {
      final bestNetwork = bindings.getGraph(true);
      _setGraphFromNetwork(networkOrGraph: bestNetwork, isBest: true);

      final nowNetwork = bindings.getGraph(false);
      _setGraphFromNetwork(networkOrGraph: nowNetwork, isBest: false);

      final test = bindings.getLocation();

      final metrics = bindings.getBestMetrics();
      if (metrics.isNotEmpty) {
        mainScreenHolder.update(
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
      mainScreenHolder.state.stepCountTextEditingController.text =
          (stepCount).toString();
      if (stepCount != 0) {
        _callNextStep(stepCount, false);
      } else {
        mainScreenHolder
            .update((state) => state.copyWith(isAlgoWorking: false));
      }
    } else {
      mainScreenHolder.update((state) => state.copyWith(isAlgoWorking: false));
    }
  }

  void _setGraphFromNetwork({
    required bool isBest,
    bindings.SlimeMoldNetwork? networkOrGraph,
  }) {
    Graph graph;
    if (networkOrGraph != null) {
      graph = Graph(
        towns: List.from(networkOrGraph.towns),
        exitPoints:
            networkOrGraph.exitPoints.map((e) => Pair(e[0], e[1])).toList(),
        graph: List.from(networkOrGraph.graph),
      );
    } else {
      graph = Graph.empty();
    }

    final stateHolder = isBest ? bestGraphHolder : nowGraphHolder;
    stateHolder.update((state) => graph);
  }
}
