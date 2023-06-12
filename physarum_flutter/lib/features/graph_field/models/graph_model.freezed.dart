// coverage:ignore-file
// GENERATED CODE - DO NOT MODIFY BY HAND
// ignore_for_file: type=lint
// ignore_for_file: unused_element, deprecated_member_use, deprecated_member_use_from_same_package, use_function_type_syntax_for_parameters, unnecessary_const, avoid_init_to_null, invalid_override_different_default_values_named, prefer_expression_function_bodies, annotate_overrides, invalid_annotation_target, unnecessary_question_mark

part of 'graph_model.dart';

// **************************************************************************
// FreezedGenerator
// **************************************************************************

T _$identity<T>(T value) => value;

final _privateConstructorUsedError = UnsupportedError(
    'It seems like you constructed your class using `MyClass._()`. This constructor is only meant to be used by freezed and you are not supposed to need it nor use it.\nPlease check the documentation here for more information: https://github.com/rrousselGit/freezed#custom-getters-and-methods');

/// @nodoc
mixin _$Graph {
  List<int> get towns => throw _privateConstructorUsedError;
  List<Pair> get exitPoints => throw _privateConstructorUsedError;
  List<List<int>> get graph => throw _privateConstructorUsedError;

  @JsonKey(ignore: true)
  $GraphCopyWith<Graph> get copyWith => throw _privateConstructorUsedError;
}

/// @nodoc
abstract class $GraphCopyWith<$Res> {
  factory $GraphCopyWith(Graph value, $Res Function(Graph) then) =
      _$GraphCopyWithImpl<$Res, Graph>;
  @useResult
  $Res call({List<int> towns, List<Pair> exitPoints, List<List<int>> graph});
}

/// @nodoc
class _$GraphCopyWithImpl<$Res, $Val extends Graph>
    implements $GraphCopyWith<$Res> {
  _$GraphCopyWithImpl(this._value, this._then);

  // ignore: unused_field
  final $Val _value;
  // ignore: unused_field
  final $Res Function($Val) _then;

  @pragma('vm:prefer-inline')
  @override
  $Res call({
    Object? towns = null,
    Object? exitPoints = null,
    Object? graph = null,
  }) {
    return _then(_value.copyWith(
      towns: null == towns
          ? _value.towns
          : towns // ignore: cast_nullable_to_non_nullable
              as List<int>,
      exitPoints: null == exitPoints
          ? _value.exitPoints
          : exitPoints // ignore: cast_nullable_to_non_nullable
              as List<Pair>,
      graph: null == graph
          ? _value.graph
          : graph // ignore: cast_nullable_to_non_nullable
              as List<List<int>>,
    ) as $Val);
  }
}

/// @nodoc
abstract class _$$_GraphCopyWith<$Res> implements $GraphCopyWith<$Res> {
  factory _$$_GraphCopyWith(_$_Graph value, $Res Function(_$_Graph) then) =
      __$$_GraphCopyWithImpl<$Res>;
  @override
  @useResult
  $Res call({List<int> towns, List<Pair> exitPoints, List<List<int>> graph});
}

/// @nodoc
class __$$_GraphCopyWithImpl<$Res> extends _$GraphCopyWithImpl<$Res, _$_Graph>
    implements _$$_GraphCopyWith<$Res> {
  __$$_GraphCopyWithImpl(_$_Graph _value, $Res Function(_$_Graph) _then)
      : super(_value, _then);

  @pragma('vm:prefer-inline')
  @override
  $Res call({
    Object? towns = null,
    Object? exitPoints = null,
    Object? graph = null,
  }) {
    return _then(_$_Graph(
      towns: null == towns
          ? _value.towns
          : towns // ignore: cast_nullable_to_non_nullable
              as List<int>,
      exitPoints: null == exitPoints
          ? _value.exitPoints
          : exitPoints // ignore: cast_nullable_to_non_nullable
              as List<Pair>,
      graph: null == graph
          ? _value.graph
          : graph // ignore: cast_nullable_to_non_nullable
              as List<List<int>>,
    ));
  }
}

/// @nodoc

class _$_Graph extends _Graph with DiagnosticableTreeMixin {
  _$_Graph({required this.towns, required this.exitPoints, required this.graph})
      : super._();

  @override
  final List<int> towns;
  @override
  final List<Pair> exitPoints;
  @override
  final List<List<int>> graph;

  @override
  String toString({DiagnosticLevel minLevel = DiagnosticLevel.info}) {
    return 'Graph(towns: $towns, exitPoints: $exitPoints, graph: $graph)';
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties
      ..add(DiagnosticsProperty('type', 'Graph'))
      ..add(DiagnosticsProperty('towns', towns))
      ..add(DiagnosticsProperty('exitPoints', exitPoints))
      ..add(DiagnosticsProperty('graph', graph));
  }

  @override
  bool operator ==(dynamic other) {
    return identical(this, other) ||
        (other.runtimeType == runtimeType &&
            other is _$_Graph &&
            const DeepCollectionEquality().equals(other.towns, towns) &&
            const DeepCollectionEquality()
                .equals(other.exitPoints, exitPoints) &&
            const DeepCollectionEquality().equals(other.graph, graph));
  }

  @override
  int get hashCode => Object.hash(
      runtimeType,
      const DeepCollectionEquality().hash(towns),
      const DeepCollectionEquality().hash(exitPoints),
      const DeepCollectionEquality().hash(graph));

  @JsonKey(ignore: true)
  @override
  @pragma('vm:prefer-inline')
  _$$_GraphCopyWith<_$_Graph> get copyWith =>
      __$$_GraphCopyWithImpl<_$_Graph>(this, _$identity);
}

abstract class _Graph extends Graph {
  factory _Graph(
      {required final List<int> towns,
      required final List<Pair> exitPoints,
      required final List<List<int>> graph}) = _$_Graph;
  _Graph._() : super._();

  @override
  List<int> get towns;
  @override
  List<Pair> get exitPoints;
  @override
  List<List<int>> get graph;
  @override
  @JsonKey(ignore: true)
  _$$_GraphCopyWith<_$_Graph> get copyWith =>
      throw _privateConstructorUsedError;
}
