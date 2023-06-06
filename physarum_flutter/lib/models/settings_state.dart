import 'package:flutter/widgets.dart';
import 'package:freezed_annotation/freezed_annotation.dart';
import '../support/settings_data.dart';

part 'settings_state.freezed.dart';

@Freezed(makeCollectionsUnmodifiable: false)
class SettingsState with _$SettingsState {
  const SettingsState._();

  factory SettingsState({
    required Map<String, TextEditingController> settingsControllers,
  }) = _SettingsState;

  static SettingsState empty() => SettingsState(
        settingsControllers: settingsDefault.map(
          (key, value) =>
              MapEntry(key, TextEditingController()..text = value.toString()),
        ),
      );
}
