// GENERATED CODE - DO NOT MODIFY BY HAND

part of 'graph_model.dart';

// **************************************************************************
// JsonSerializableGenerator
// **************************************************************************

_$_Graph _$$_GraphFromJson(Map<String, dynamic> json) => _$_Graph(
      towns: (json['towns'] as List<dynamic>).map((e) => e as int).toList(),
      exitPoints: (json['exitPoints'] as List<dynamic>)
          .map((e) => (e as List<dynamic>).map((e) => e as int).toList())
          .toList(),
      graph: (json['graph'] as List<dynamic>)
          .map((e) => (e as List<dynamic>).map((e) => e as int).toList())
          .toList(),
    );

Map<String, dynamic> _$$_GraphToJson(_$_Graph instance) => <String, dynamic>{
      'towns': instance.towns,
      'exitPoints': instance.exitPoints,
      'graph': instance.graph,
    };
