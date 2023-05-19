import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:physarum_flutter/features/graph_field/graph_field_state_holders.dart';
import 'package:physarum_cpp_ffi/physarum_core.dart' as ffi;
import 'package:physarum_flutter/features/setting_panel/simulation_setting_state_holder.dart';
import 'package:physarum_flutter/models/settings_state.dart';

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

  void onSaveSetting() {
    // TODO валидация и прочие приколы
  }
}
