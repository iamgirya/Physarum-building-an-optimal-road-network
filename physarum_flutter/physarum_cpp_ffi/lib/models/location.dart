import 'dart:ffi';

import 'arrays.dart';

final class LocationFFI extends Struct {
  external Pointer<IntArrayArray> trailMap;
  external Pointer<DoubleArray> agentsX;
  external Pointer<DoubleArray> agentsY;
}

class Location {
  List<List<int>> trailMap = [];
  List<List<double>> exitPoints = [];
}
