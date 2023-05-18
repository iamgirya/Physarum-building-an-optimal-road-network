import 'dart:ffi' as ffi;

// TODO на генерики переписать массивы

final class IntArray extends ffi.Struct {
  external ffi.Pointer<ffi.Int32> data;

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

typedef NativeGetGraph = ffi.Pointer<SlimeMoldNetworkFFI> Function(
    ffi.Int, ffi.Int);
typedef FFIGetGraph = ffi.Pointer<SlimeMoldNetworkFFI> Function(int, int);
