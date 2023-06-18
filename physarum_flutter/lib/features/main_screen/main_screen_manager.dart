import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import '../graph_field/graph_field_state_holders.dart';
import '../graph_field/graph_fields_manager.dart';
import '../location_field/location_state_holders.dart';
import 'physarum_repository.dart';
import 'state/main_screen_state_holder.dart';
import '../setting_panel/state/simulation_setting_state_holder.dart';
import '../graph_field/models/graph_model.dart';
import 'state/main_screen_state.dart';
import '../setting_panel/state/settings_state.dart';

final mainScreenManager = Provider<MainScreenManager>((ref) {
  return MainScreenManager(
    graphManager: ref.watch(graphFieldsManager),
    mainScreenHolder: ref.watch(mainScreenStateHolder.notifier),
    bestGraphHolder: ref.watch(bestGraphsFieldGraphStateHolder.notifier),
    nowGraphHolder: ref.watch(nowGraphsFieldGraphStateHolder.notifier),
    locationHolder: ref.watch(locationStateHolder.notifier),
    settingsHolder: ref.watch(settingsStateHolder.notifier),
    physarumRepository: ref.watch(physarumRepositoryProvider),
  );
});

class MainScreenManager {
  static const int iterationPerStep = 10;
  final GraphFieldManager graphManager;
  final StateController<MainScreenState> mainScreenHolder;
  final GraphNotifier bestGraphHolder;
  final GraphNotifier nowGraphHolder;
  final StateController<SettingsState> settingsHolder;
  final PhysarumRepository physarumRepository;
  final LocationNotifier locationHolder;
  MainScreenManager({
    required this.mainScreenHolder,
    required this.bestGraphHolder,
    required this.nowGraphHolder,
    required this.settingsHolder,
    required this.graphManager,
    required this.physarumRepository,
    required this.locationHolder,
  });

  void changeDrawerMode() => mainScreenHolder
      .update((state) => state.copyWith(isBestOnDrawer: !state.isBestOnDrawer));

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
    _setGraph(isBest: false);
    _setGraph(isBest: true);
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

    if (nowGraphHolder.state.towns.isEmpty &&
        settingsHolder.state.settingsControllers['startPopulation']! == 0) {
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
      physarumRepository.setUpSimulation(
        settingsHolder.state.settingsControllers,
      );
      physarumRepository.setUpTowns(
        nowGraphHolder.state.exitPoints,
        nowGraphHolder.state.towns,
      );
    }

    await physarumRepository.makeStep(iterationPerStep);
    if (mainScreenHolder.state.isAlgoWorking &&
        !mainScreenHolder.state.isNeedRestart) {
      final bestNetwork = physarumRepository.getGraph(true);
      _setGraph(graph: bestNetwork, isBest: true);

      final nowNetwork = physarumRepository.getGraph(false);
      _setGraph(graph: nowNetwork, isBest: false);

      final newLocation = physarumRepository.getLocation();
      locationHolder.update((state) => newLocation);

      final metrics = physarumRepository.getBestMetrics();
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

  void showInfoDialog(context) {
    showDialog(
      context: context,
      builder: (context) => AlertDialog(
        title: const Text('Как пользоваться программой'),
        content: const Text(
            '''С помощью ЛКМ на левом графе можно задать вершины маршрутной сети. При повторном клике на вершину ЛКМ будет увеличен её приоритет.
При ПКМ приоритет будет уменьшен. При достижении нулевого приоритета, вершина убирается.
Координаты вершин и их приоритет можно задать самостоятельно на вкладке "Вершины".
        
На вкладке "Расширенные настройки" находятся настройки симуляции. Подробное описание каждой из настроек можно найти в тексте зимней курсовой работы.
        
После того, как задано количество итераций алгоритма, можно запустить программу нажатием на "Выполнить". Итерации можно остановить с помощью "Остановить" или же обнулить всю программу с помощью "Сбросить".'''),
        actions: <Widget>[
          TextButton(
            child: const Text('Понятно'),
            onPressed: () {
              Navigator.of(context).pop();
            },
          ),
        ],
      ),
    );
  }

  void _setGraph({
    required bool isBest,
    Graph? graph,
  }) {
    graph = graph ?? Graph.empty();
    final stateHolder = isBest ? bestGraphHolder : nowGraphHolder;
    stateHolder.update((state) => graph!);
  }
}
