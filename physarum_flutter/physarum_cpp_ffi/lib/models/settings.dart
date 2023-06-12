import 'dart:ffi';

// важно сохранять порядок как в C++ коде

final class LocationSettings extends Struct {
  @Int64()
  external int xSize;
  @Int64()
  external int ySize;
  @Double()
  external double decayFactor;
  @Bool()
  external bool isPeriodicBoundary;
  @Bool()
  external bool isCanMultiAgent;
}

final class AgentSettings extends Struct {
  @Int64()
  external int timeToLive;
  @Double()
  external double sensorOffsetDistance;
  @Double()
  external double sensorsAngle;
  @Double()
  external double stepSize;
  @Double()
  external double rotationAngle;
  @Double()
  external double depositPerStep;
}

final class AnalyserSettings extends Struct {
  @Double()
  external double weigthCoef;
  @Double()
  external double overDistanceCoef;
  @Double()
  external double deltaFlowCoef;
  @Double()
  external double resistanceCoef;
  @Double()
  external double edgesRange;
  @Double()
  external double vertexRange;
  @Double()
  external double minVertexMass;
  @Double()
  external double minEdgeAngle;
}
