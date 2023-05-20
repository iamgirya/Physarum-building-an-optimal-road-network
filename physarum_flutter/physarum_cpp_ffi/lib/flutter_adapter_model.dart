import 'dart:ffi' as ffi;

import 'package:ffi/ffi.dart';

// TODO на генерики переписать массивы

final class IntArray extends ffi.Struct {
  external ffi.Pointer<ffi.Int32> data;

  @ffi.Int32()
  external int length;

  static create(Arena arena, int length) {
    int sizeIntArray = ffi.sizeOf<IntArray>();
    int sizeInt = ffi.sizeOf<ffi.Int>();
    ffi.Pointer<IntArray> array = arena.allocate(1 * sizeIntArray);
    array.ref.length = length;
    array.ref.data = arena.allocate(length * sizeInt);
    return array;
  }
}

final class DoubleArray extends ffi.Struct {
  external ffi.Pointer<ffi.Double> data;

  @ffi.Int32()
  external int length;
}

final class IntArrayArray extends ffi.Struct {
  external ffi.Pointer<IntArray> data;

  @ffi.Int32()
  external int length;
}

final class SlimeMoldNetworkFFI extends ffi.Struct {
  external ffi.Pointer<IntArrayArray> graph;
  external ffi.Pointer<IntArray> exitPointsX;
  external ffi.Pointer<IntArray> exitPointsY;
  external ffi.Pointer<IntArray> towns;

  @ffi.Int32()
  external int length;
}

class SlimeMoldNetwork {
  List<int> towns = [];
  List<List<int>> exitPoints = [];
  List<List<int>> graph = [];
}
