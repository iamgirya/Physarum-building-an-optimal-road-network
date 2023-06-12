import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'main_screen_state.dart';

final mainScreenStateHolder =
    StateProvider<MainScreenState>((ref) => MainScreenState.empty());
