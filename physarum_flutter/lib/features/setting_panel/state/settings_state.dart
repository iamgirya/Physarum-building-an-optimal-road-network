import 'package:freezed_annotation/freezed_annotation.dart';
import '../../../support/settings_data.dart';

part 'settings_state.freezed.dart';

@Freezed(makeCollectionsUnmodifiable: false)
class SettingsState with _$SettingsState {
  const SettingsState._();

  factory SettingsState({
    required Map<String, num> settingsControllers,
  }) = _SettingsState;

  static SettingsState empty() => SettingsState(
        settingsControllers: Map.unmodifiable(
          settingsValues.map(
            (key, value) => MapEntry(
              key,
              value.defaultValue,
            ),
          ),
        ),
      );
}
