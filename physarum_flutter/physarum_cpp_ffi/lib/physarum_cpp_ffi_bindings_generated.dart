// ignore_for_file: always_specify_types
// ignore_for_file: camel_case_types
// ignore_for_file: non_constant_identifier_names
// авто ген не работает для с++ заголовков, так что ручками-ручками
import 'dart:ffi' as ffi;

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

typedef Native_getGraph = ffi.Pointer<SlimeMoldNetworkFFI> Function(
    ffi.Int, ffi.Int);
typedef FFI_getGraph = ffi.Pointer<SlimeMoldNetworkFFI> Function(int, int);

class PhysarumCppFfiBindings {
  final ffi.Pointer<T> Function<T extends ffi.NativeType>(String symbolName)
      _lookup;
  PhysarumCppFfiBindings(ffi.DynamicLibrary dynamicLibrary)
      : _lookup = dynamicLibrary.lookup;
  PhysarumCppFfiBindings.fromLookup(
      ffi.Pointer<T> Function<T extends ffi.NativeType>(String symbolName)
          lookup)
      : _lookup = lookup;

  int sum(int a, int b) {
    return _sum(a, b);
  }

  late final _sumPtr =
      _lookup<ffi.NativeFunction<ffi.Int Function(ffi.Int, ffi.Int)>>('sum');
  late final _sum = _sumPtr.asFunction<int Function(int, int)>();

  SlimeMoldNetwork execute(int stepCount, int b) {
    final struct = _execute(stepCount, b).ref;

    SlimeMoldNetwork result = SlimeMoldNetwork();
    for (int i = 0; i < struct.length; i++) {
      // точки
      result.exitPoints.add(
          [struct.exitPointsX.ref.data[i], struct.exitPointsY.ref.data[i]]);
      // города
      result.towns.add(struct.towns.ref.data[i]);
      // граф
      result.graph.add([]);
      final subArray = struct.graph.ref.data[i];
      for (int j = 0; j < subArray.length; j++) {
        result.graph[i].add(subArray.data[j]);
      }
    }

    return result;
  }

  late final _executePtr =
      _lookup<ffi.NativeFunction<Native_getGraph>>('execute');
  late final _execute = _executePtr.asFunction<FFI_getGraph>();

  int sum_long_running(
    int a,
    int b,
  ) {
    return _sum_long_running(
      a,
      b,
    );
  }

  late final _sum_long_runningPtr =
      _lookup<ffi.NativeFunction<ffi.Int Function(ffi.Int, ffi.Int)>>(
          'sum_long_running');
  late final _sum_long_running =
      _sum_long_runningPtr.asFunction<int Function(int, int)>();
}
