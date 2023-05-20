import 'dart:ffi';
import 'package:ffi/ffi.dart';
import 'package:physarum_cpp_ffi/flutter_adapter_model.dart';

import 'ffi_core.dart';

extension SetUpTownsFunc on PhysarumCppFfiBindings {
  void setUpTowns(List<List<int>> points, List<int> towns) {
    final execute = lookup<
            NativeFunction<
                Void Function(Pointer<IntArray>, Pointer<IntArray>,
                    Pointer<IntArray>)>>('setUpTowns')
        .asFunction<
            void Function(
                Pointer<IntArray>, Pointer<IntArray>, Pointer<IntArray>)>();

    Arena arena = Arena();

    Pointer<IntArray> positionX = IntArray.create(arena, points.length);
    Pointer<IntArray> positionY = IntArray.create(arena, points.length);
    Pointer<IntArray> townsPriority = IntArray.create(arena, points.length);
    for (int i = 0; i < points.length; i++) {
      positionX.ref.data[i] = points[i][0];
      positionY.ref.data[i] = points[i][1];
      townsPriority.ref.data[i] = towns[i];
    }

    execute(positionX, positionY, townsPriority);
    arena.releaseAll();
  }
}
