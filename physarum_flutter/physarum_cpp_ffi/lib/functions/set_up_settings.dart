import 'dart:ffi';

import 'package:ffi/ffi.dart';
import 'package:physarum_cpp_ffi/models/settings.dart';

import '../ffi_core.dart';

void setUpSimulation(Map<String, num> settings) {
  final execute = lookup<
          NativeFunction<
              Void Function(
                Pointer<AgentSettings>,
                Pointer<LocationSettings>,
                Pointer<AnalyserSettings>,
                Int32,
              )>>('setUpSimulation')
      .asFunction<
          void Function(
            Pointer<AgentSettings>,
            Pointer<LocationSettings>,
            Pointer<AnalyserSettings>,
            int,
          )>();

  Arena arena = Arena();

  try {
    Pointer<AgentSettings> agentSettings =
        arena.allocate(sizeOf<AgentSettings>());
    agentSettings.ref.timeToLive = settings['timeToLive']!.toInt();
    agentSettings.ref.sensorOffsetDistance =
        settings['sensorOffsetDistance']!.toDouble();
    agentSettings.ref.stepSize = settings['stepSize']!.toDouble();
    agentSettings.ref.sensorsAngle = settings['sensorsAngle']!.toDouble();
    agentSettings.ref.rotationAngle = settings['rotationAngle']!.toDouble();
    agentSettings.ref.depositPerStep = settings['depositPerStep']!.toDouble();

    Pointer<LocationSettings> locationSettings =
        arena.allocate(sizeOf<LocationSettings>());
    locationSettings.ref.xSize = settings['locationX']!.toInt();
    locationSettings.ref.ySize = settings['locationY']!.toInt();
    locationSettings.ref.decayFactor = settings['decayFactor']!.toDouble();
    locationSettings.ref.isPeriodicBoundary =
        (settings['isPeriodicBoundary'] != null &&
            settings['isPeriodicBoundary'] != 0);
    locationSettings.ref.isCanMultiAgent = (settings['isCanMultiAgent'] != 0);

    Pointer<AnalyserSettings> analyserSettings =
        arena.allocate(sizeOf<AnalyserSettings>());
    analyserSettings.ref.weigthCoef = settings['weigthCoef']!.toDouble();
    analyserSettings.ref.overDistanceCoef =
        settings['overDistanceCoef']!.toDouble();
    analyserSettings.ref.deltaFlowCoef = settings['deltaFlowCoef']!.toDouble();
    analyserSettings.ref.resistanceCoef =
        settings['resistanceCoef']!.toDouble();
    analyserSettings.ref.edgesRange = settings['edgesRange']!.toDouble();
    analyserSettings.ref.vertexRange = settings['vertexRange']!.toDouble();
    analyserSettings.ref.minVertexMass = settings['minVertexMass']!.toDouble();
    analyserSettings.ref.minEdgeAngle = settings['minEdgeAngle']!.toDouble();

    final startPopulation = settings['startPopulation']!.toInt();

    execute(agentSettings, locationSettings, analyserSettings, startPopulation);

    arena.releaseAll();
  } catch (error) {
    print(error);
    return;
  }
}
