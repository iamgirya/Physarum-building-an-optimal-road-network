import 'package:flutter/widgets.dart';
import 'package:freezed_annotation/freezed_annotation.dart';
part 'main_screen_state.freezed.dart';

@Freezed(makeCollectionsUnmodifiable: false)
class MainScreenState with _$MainScreenState {
  const MainScreenState._();

  factory MainScreenState({
    required bool isAlgoWorking,
    required bool isNeedRestart,
    required TextEditingController stepCountTextEditingController,
  }) = _MainScreenState;

  static MainScreenState empty() => MainScreenState(
        isAlgoWorking: false,
        isNeedRestart: true,
        stepCountTextEditingController: TextEditingController(),
      );
}
