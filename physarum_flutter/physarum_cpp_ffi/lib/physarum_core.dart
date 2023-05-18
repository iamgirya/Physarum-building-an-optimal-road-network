import 'dart:ffi';
import 'dart:io';

const String _libName = 'physarum_cpp_ffi';

final DynamicLibrary _dylib = () {
  if (Platform.isMacOS || Platform.isIOS) {
    return DynamicLibrary.open('$_libName.framework/$_libName');
  }
  if (Platform.isAndroid || Platform.isLinux) {
    return DynamicLibrary.open('lib$_libName.so');
  }
  if (Platform.isWindows) {
    return DynamicLibrary.open('$_libName.dll');
  }
  throw UnsupportedError('Unknown platform: ${Platform.operatingSystem}');
}();

class PhysarumCppFfiBindings {
  final Pointer<T> Function<T extends NativeType>(String symbolName) lookup;
  PhysarumCppFfiBindings(DynamicLibrary dynamicLibrary)
      : lookup = dynamicLibrary.lookup;
  PhysarumCppFfiBindings.fromLookup(this.lookup);
}

final PhysarumCppFfiBindings bindings = PhysarumCppFfiBindings(_dylib);
