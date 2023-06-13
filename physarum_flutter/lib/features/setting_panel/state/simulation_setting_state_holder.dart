import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'settings_state.dart';

final settingsStateHolder =
    StateProvider<SettingsState>((ref) => SettingsState.empty());