// coverage:ignore-file
// GENERATED CODE - DO NOT MODIFY BY HAND
// ignore_for_file: type=lint
// ignore_for_file: unused_element, deprecated_member_use, deprecated_member_use_from_same_package, use_function_type_syntax_for_parameters, unnecessary_const, avoid_init_to_null, invalid_override_different_default_values_named, prefer_expression_function_bodies, annotate_overrides, invalid_annotation_target, unnecessary_question_mark

part of 'main_screen_state.dart';

// **************************************************************************
// FreezedGenerator
// **************************************************************************

T _$identity<T>(T value) => value;

final _privateConstructorUsedError = UnsupportedError(
    'It seems like you constructed your class using `MyClass._()`. This constructor is only meant to be used by freezed and you are not supposed to need it nor use it.\nPlease check the documentation here for more information: https://github.com/rrousselGit/freezed#custom-getters-and-methods');

/// @nodoc
mixin _$MainScreenState {
  bool get isAlgoWorking => throw _privateConstructorUsedError;
  bool get isNeedRestart => throw _privateConstructorUsedError;
  TextEditingController get stepCountTextEditingController =>
      throw _privateConstructorUsedError;

  @JsonKey(ignore: true)
  $MainScreenStateCopyWith<MainScreenState> get copyWith =>
      throw _privateConstructorUsedError;
}

/// @nodoc
abstract class $MainScreenStateCopyWith<$Res> {
  factory $MainScreenStateCopyWith(
          MainScreenState value, $Res Function(MainScreenState) then) =
      _$MainScreenStateCopyWithImpl<$Res, MainScreenState>;
  @useResult
  $Res call(
      {bool isAlgoWorking,
      bool isNeedRestart,
      TextEditingController stepCountTextEditingController});
}

/// @nodoc
class _$MainScreenStateCopyWithImpl<$Res, $Val extends MainScreenState>
    implements $MainScreenStateCopyWith<$Res> {
  _$MainScreenStateCopyWithImpl(this._value, this._then);

  // ignore: unused_field
  final $Val _value;
  // ignore: unused_field
  final $Res Function($Val) _then;

  @pragma('vm:prefer-inline')
  @override
  $Res call({
    Object? isAlgoWorking = null,
    Object? isNeedRestart = null,
    Object? stepCountTextEditingController = null,
  }) {
    return _then(_value.copyWith(
      isAlgoWorking: null == isAlgoWorking
          ? _value.isAlgoWorking
          : isAlgoWorking // ignore: cast_nullable_to_non_nullable
              as bool,
      isNeedRestart: null == isNeedRestart
          ? _value.isNeedRestart
          : isNeedRestart // ignore: cast_nullable_to_non_nullable
              as bool,
      stepCountTextEditingController: null == stepCountTextEditingController
          ? _value.stepCountTextEditingController
          : stepCountTextEditingController // ignore: cast_nullable_to_non_nullable
              as TextEditingController,
    ) as $Val);
  }
}

/// @nodoc
abstract class _$$_MainScreenStateCopyWith<$Res>
    implements $MainScreenStateCopyWith<$Res> {
  factory _$$_MainScreenStateCopyWith(
          _$_MainScreenState value, $Res Function(_$_MainScreenState) then) =
      __$$_MainScreenStateCopyWithImpl<$Res>;
  @override
  @useResult
  $Res call(
      {bool isAlgoWorking,
      bool isNeedRestart,
      TextEditingController stepCountTextEditingController});
}

/// @nodoc
class __$$_MainScreenStateCopyWithImpl<$Res>
    extends _$MainScreenStateCopyWithImpl<$Res, _$_MainScreenState>
    implements _$$_MainScreenStateCopyWith<$Res> {
  __$$_MainScreenStateCopyWithImpl(
      _$_MainScreenState _value, $Res Function(_$_MainScreenState) _then)
      : super(_value, _then);

  @pragma('vm:prefer-inline')
  @override
  $Res call({
    Object? isAlgoWorking = null,
    Object? isNeedRestart = null,
    Object? stepCountTextEditingController = null,
  }) {
    return _then(_$_MainScreenState(
      isAlgoWorking: null == isAlgoWorking
          ? _value.isAlgoWorking
          : isAlgoWorking // ignore: cast_nullable_to_non_nullable
              as bool,
      isNeedRestart: null == isNeedRestart
          ? _value.isNeedRestart
          : isNeedRestart // ignore: cast_nullable_to_non_nullable
              as bool,
      stepCountTextEditingController: null == stepCountTextEditingController
          ? _value.stepCountTextEditingController
          : stepCountTextEditingController // ignore: cast_nullable_to_non_nullable
              as TextEditingController,
    ));
  }
}

/// @nodoc

class _$_MainScreenState extends _MainScreenState {
  _$_MainScreenState(
      {required this.isAlgoWorking,
      required this.isNeedRestart,
      required this.stepCountTextEditingController})
      : super._();

  @override
  final bool isAlgoWorking;
  @override
  final bool isNeedRestart;
  @override
  final TextEditingController stepCountTextEditingController;

  @override
  String toString() {
    return 'MainScreenState(isAlgoWorking: $isAlgoWorking, isNeedRestart: $isNeedRestart, stepCountTextEditingController: $stepCountTextEditingController)';
  }

  @override
  bool operator ==(dynamic other) {
    return identical(this, other) ||
        (other.runtimeType == runtimeType &&
            other is _$_MainScreenState &&
            (identical(other.isAlgoWorking, isAlgoWorking) ||
                other.isAlgoWorking == isAlgoWorking) &&
            (identical(other.isNeedRestart, isNeedRestart) ||
                other.isNeedRestart == isNeedRestart) &&
            (identical(other.stepCountTextEditingController,
                    stepCountTextEditingController) ||
                other.stepCountTextEditingController ==
                    stepCountTextEditingController));
  }

  @override
  int get hashCode => Object.hash(runtimeType, isAlgoWorking, isNeedRestart,
      stepCountTextEditingController);

  @JsonKey(ignore: true)
  @override
  @pragma('vm:prefer-inline')
  _$$_MainScreenStateCopyWith<_$_MainScreenState> get copyWith =>
      __$$_MainScreenStateCopyWithImpl<_$_MainScreenState>(this, _$identity);
}

abstract class _MainScreenState extends MainScreenState {
  factory _MainScreenState(
      {required final bool isAlgoWorking,
      required final bool isNeedRestart,
      required final TextEditingController
          stepCountTextEditingController}) = _$_MainScreenState;
  _MainScreenState._() : super._();

  @override
  bool get isAlgoWorking;
  @override
  bool get isNeedRestart;
  @override
  TextEditingController get stepCountTextEditingController;
  @override
  @JsonKey(ignore: true)
  _$$_MainScreenStateCopyWith<_$_MainScreenState> get copyWith =>
      throw _privateConstructorUsedError;
}
