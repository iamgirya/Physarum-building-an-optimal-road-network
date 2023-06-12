import 'dart:ffi';

import 'package:ffi/ffi.dart';

// На генерики переписать массивы нельзя из-за свойств Struct

final class IntArray extends Struct {
  external Pointer<Int32> data;

  @Int32()
  external int length;

  static create(Arena arena, int length) {
    int sizeIntArray = sizeOf<IntArray>();
    int sizeInt = sizeOf<Int>();
    Pointer<IntArray> array = arena.allocate(1 * sizeIntArray);
    array.ref.length = length;
    array.ref.data = arena.allocate(length * sizeInt);
    return array;
  }
}

final class DoubleArray extends Struct {
  external Pointer<Double> data;

  @Int32()
  external int length;
}

final class IntArrayArray extends Struct {
  external Pointer<IntArray> data;

  @Int32()
  external int length;
}
