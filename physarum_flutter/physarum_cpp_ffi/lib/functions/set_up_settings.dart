import 'dart:ffi';

import '../ffi_core.dart';

void setUpSimulation(Map<String, num> settings) {
  final execute = lookup<
          NativeFunction<
              Void Function(
                Double,
                Double,
                Double,
                Double,
                Int,
                Int,
                Int,
                Int,
                Double,
                Double,
                Double,
                Double,
                Double,
                Double,
                Bool,
                Bool,
                Double,
                Double,
                Double,
                Double,
              )>>('setUpSimulation')
      .asFunction<
          void Function(
            double,
            double,
            double,
            double,
            int,
            int,
            int,
            int,
            double,
            double,
            double,
            double,
            double,
            double,
            bool,
            bool,
            double,
            double,
            double,
            double,
          )>();

  execute(
    settings['weigthCoef']?.toDouble() ?? 1,
    settings['overDistanceCoef']?.toDouble() ?? 1,
    settings['deltaFlowCoef']?.toDouble() ?? 0.5,
    settings['resistanceCoef']?.toDouble() ?? 1,
    settings['locationX']?.toInt() ?? 200,
    settings['locationY']?.toInt() ?? 200,
    settings['timeToLive']?.toInt() ?? 80,
    settings['startPopulation']?.toInt() ?? 0,
    settings['sensorOffsetDistance']?.toDouble() ?? 6,
    settings['sensorsAngle']?.toDouble() ?? 45,
    settings['rotationAngle']?.toDouble() ?? 45,
    settings['stepSize']?.toDouble() ?? 1,
    settings['depositPerStep']?.toDouble() ?? 3,
    settings['decayFactor']?.toDouble() ?? 1.5,
    (settings['isPeriodicBoundary'] != null &&
        settings['isPeriodicBoundary'] != 0),
    (settings['isCanMultiAgent'] != 0),
    settings['edgesRange']?.toDouble() ?? 16,
    settings['vertexRange']?.toDouble() ?? 8,
    settings['minVertexMass']?.toDouble() ?? 4,
    settings['minEdgeAngle']?.toDouble() ?? 15,
  );
}
