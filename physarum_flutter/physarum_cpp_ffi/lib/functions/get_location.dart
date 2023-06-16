import 'dart:ffi';

import '../ffi_core.dart';
import '../models/location.dart';

Location getLocation() {
  final execute =
      lookup<NativeFunction<Pointer<LocationFFI> Function()>>('getLocation')
          .asFunction<Pointer<LocationFFI> Function()>();
  final struct = execute().ref;

  Location result = Location();
  for (int i = 0; i < struct.agentsX.ref.length; i++) {
    // точки
    result.exitPoints
        .add([struct.agentsX.ref.data[i], struct.agentsY.ref.data[i]]);
  }
  for (int i = 0; i < struct.trailMap.ref.length; i++) {
    result.trailMap.add([]);
    final subArray = struct.trailMap.ref.data[i];
    for (int j = 0; j < subArray.length; j++) {
      result.trailMap[i].add(subArray.data[j]);
    }
  }

  return result;
}
