import 'package:freezed_annotation/freezed_annotation.dart';

import '../../../support/pair.dart';

part 'location.freezed.dart';

@freezed
class Location with _$Location {
  const Location._();

  const factory Location({
    required List<List<int>> trailMap,
    required List<Pair<double>> exitPoints,
  }) = _Location;

  static Location empty() => const Location(trailMap: [], exitPoints: []);
}
