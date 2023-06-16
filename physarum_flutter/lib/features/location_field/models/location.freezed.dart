// coverage:ignore-file
// GENERATED CODE - DO NOT MODIFY BY HAND
// ignore_for_file: type=lint
// ignore_for_file: unused_element, deprecated_member_use, deprecated_member_use_from_same_package, use_function_type_syntax_for_parameters, unnecessary_const, avoid_init_to_null, invalid_override_different_default_values_named, prefer_expression_function_bodies, annotate_overrides, invalid_annotation_target, unnecessary_question_mark

part of 'location.dart';

// **************************************************************************
// FreezedGenerator
// **************************************************************************

T _$identity<T>(T value) => value;

final _privateConstructorUsedError = UnsupportedError(
    'It seems like you constructed your class using `MyClass._()`. This constructor is only meant to be used by freezed and you are not supposed to need it nor use it.\nPlease check the documentation here for more information: https://github.com/rrousselGit/freezed#custom-getters-and-methods');

/// @nodoc
mixin _$Location {
  List<List<int>> get trailMap => throw _privateConstructorUsedError;
  List<Pair<double>> get exitPoints => throw _privateConstructorUsedError;

  @JsonKey(ignore: true)
  $LocationCopyWith<Location> get copyWith =>
      throw _privateConstructorUsedError;
}

/// @nodoc
abstract class $LocationCopyWith<$Res> {
  factory $LocationCopyWith(Location value, $Res Function(Location) then) =
      _$LocationCopyWithImpl<$Res, Location>;
  @useResult
  $Res call({List<List<int>> trailMap, List<Pair<double>> exitPoints});
}

/// @nodoc
class _$LocationCopyWithImpl<$Res, $Val extends Location>
    implements $LocationCopyWith<$Res> {
  _$LocationCopyWithImpl(this._value, this._then);

  // ignore: unused_field
  final $Val _value;
  // ignore: unused_field
  final $Res Function($Val) _then;

  @pragma('vm:prefer-inline')
  @override
  $Res call({
    Object? trailMap = null,
    Object? exitPoints = null,
  }) {
    return _then(_value.copyWith(
      trailMap: null == trailMap
          ? _value.trailMap
          : trailMap // ignore: cast_nullable_to_non_nullable
              as List<List<int>>,
      exitPoints: null == exitPoints
          ? _value.exitPoints
          : exitPoints // ignore: cast_nullable_to_non_nullable
              as List<Pair<double>>,
    ) as $Val);
  }
}

/// @nodoc
abstract class _$$_LocationCopyWith<$Res> implements $LocationCopyWith<$Res> {
  factory _$$_LocationCopyWith(
          _$_Location value, $Res Function(_$_Location) then) =
      __$$_LocationCopyWithImpl<$Res>;
  @override
  @useResult
  $Res call({List<List<int>> trailMap, List<Pair<double>> exitPoints});
}

/// @nodoc
class __$$_LocationCopyWithImpl<$Res>
    extends _$LocationCopyWithImpl<$Res, _$_Location>
    implements _$$_LocationCopyWith<$Res> {
  __$$_LocationCopyWithImpl(
      _$_Location _value, $Res Function(_$_Location) _then)
      : super(_value, _then);

  @pragma('vm:prefer-inline')
  @override
  $Res call({
    Object? trailMap = null,
    Object? exitPoints = null,
  }) {
    return _then(_$_Location(
      trailMap: null == trailMap
          ? _value._trailMap
          : trailMap // ignore: cast_nullable_to_non_nullable
              as List<List<int>>,
      exitPoints: null == exitPoints
          ? _value._exitPoints
          : exitPoints // ignore: cast_nullable_to_non_nullable
              as List<Pair<double>>,
    ));
  }
}

/// @nodoc

class _$_Location extends _Location {
  const _$_Location(
      {required final List<List<int>> trailMap,
      required final List<Pair<double>> exitPoints})
      : _trailMap = trailMap,
        _exitPoints = exitPoints,
        super._();

  final List<List<int>> _trailMap;
  @override
  List<List<int>> get trailMap {
    if (_trailMap is EqualUnmodifiableListView) return _trailMap;
    // ignore: implicit_dynamic_type
    return EqualUnmodifiableListView(_trailMap);
  }

  final List<Pair<double>> _exitPoints;
  @override
  List<Pair<double>> get exitPoints {
    if (_exitPoints is EqualUnmodifiableListView) return _exitPoints;
    // ignore: implicit_dynamic_type
    return EqualUnmodifiableListView(_exitPoints);
  }

  @override
  String toString() {
    return 'Location(trailMap: $trailMap, exitPoints: $exitPoints)';
  }

  @override
  bool operator ==(dynamic other) {
    return identical(this, other) ||
        (other.runtimeType == runtimeType &&
            other is _$_Location &&
            const DeepCollectionEquality().equals(other._trailMap, _trailMap) &&
            const DeepCollectionEquality()
                .equals(other._exitPoints, _exitPoints));
  }

  @override
  int get hashCode => Object.hash(
      runtimeType,
      const DeepCollectionEquality().hash(_trailMap),
      const DeepCollectionEquality().hash(_exitPoints));

  @JsonKey(ignore: true)
  @override
  @pragma('vm:prefer-inline')
  _$$_LocationCopyWith<_$_Location> get copyWith =>
      __$$_LocationCopyWithImpl<_$_Location>(this, _$identity);
}

abstract class _Location extends Location {
  const factory _Location(
      {required final List<List<int>> trailMap,
      required final List<Pair<double>> exitPoints}) = _$_Location;
  const _Location._() : super._();

  @override
  List<List<int>> get trailMap;
  @override
  List<Pair<double>> get exitPoints;
  @override
  @JsonKey(ignore: true)
  _$$_LocationCopyWith<_$_Location> get copyWith =>
      throw _privateConstructorUsedError;
}
