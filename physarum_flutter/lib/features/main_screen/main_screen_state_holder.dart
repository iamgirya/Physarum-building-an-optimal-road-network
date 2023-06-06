import 'package:flutter_riverpod/flutter_riverpod.dart';
import '../../models/main_screen_state.dart';

final mainScreenStateHolder =
    StateProvider<MainScreenState>((ref) => MainScreenState.empty());
