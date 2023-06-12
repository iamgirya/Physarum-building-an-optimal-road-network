import 'dart:ffi';

import 'arrays.dart';

final class SlimeMoldNetworkFFI extends Struct {
  external Pointer<IntArrayArray> graph;
  external Pointer<IntArray> exitPointsX;
  external Pointer<IntArray> exitPointsY;
  external Pointer<IntArray> towns;

  @Int32()
  external int length;
}

class SlimeMoldNetwork {
  List<int> towns = [];
  List<List<int>> exitPoints = [];
  List<List<int>> graph = [];
}
