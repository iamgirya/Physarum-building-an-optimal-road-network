import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:physarum_flutter/models/settings_state.dart';

final settingsStateHolder =
    StateProvider<SettingsState>((ref) => SettingsState.empty());
