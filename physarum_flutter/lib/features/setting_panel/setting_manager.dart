import 'package:flutter_riverpod/flutter_riverpod.dart';
import '../graph_field/graph_field_state_holders.dart';
import 'state/simulation_setting_state_holder.dart';
import 'state/settings_state.dart';

final settingsManager = Provider<SettingsManager>((ref) {
  return SettingsManager(
    nowGraphHolder: ref.watch(nowGraphsFieldGraphStateHolder.notifier),
    settingsState: ref.watch(settingsStateHolder.notifier),
  );
});

class SettingsManager {
  final GraphNotifier nowGraphHolder;
  final StateController<SettingsState> settingsState;
  SettingsManager({
    required this.nowGraphHolder,
    required this.settingsState,
  });

  void setNewValue(String settingsKey, String text) {
    if (num.tryParse(text) != null) {
      settingsState.update(
        (state) => state.copyWith(
          settingsControllers: Map.unmodifiable(
            Map.from(state.settingsControllers)
              ..[settingsKey] = num.parse(text),
          ),
        ),
      );
    }
  }
}
