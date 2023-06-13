class SettingsValue {
  final String name;
  final num defaultValue;
  const SettingsValue(this.name, this.defaultValue);
}

const Map<String, SettingsValue> settingsValues = {
  //location
  'locationX': SettingsValue('Ширина локации', 200),
  'locationY': SettingsValue('Высота локации', 200),
  'startPopulation': SettingsValue('Стартовая популяция', 0),
  'decayFactor': SettingsValue('Множитель разложения', 1.5),
  'isPeriodicBoundary': SettingsValue('Бесконечное поле', 0),
  'isCanMultiAgent': SettingsValue('Множество агентов в одной точке', 1),
  //agent
  'timeToLive': SettingsValue('Время жизни агента', 80),
  'sensorOffsetDistance': SettingsValue('Длина сенсоров', 6),
  'sensorsAngle': SettingsValue('Угол сенсоров', 45),
  'rotationAngle': SettingsValue('Угол поворота', 45),
  'stepSize': SettingsValue('Длина шага', 1),
  'depositPerStep': SettingsValue('След за шаг', 5),
  //analyser
  'weigthCoef': SettingsValue('Коэффициент весовой метрики', 1),
  'overDistanceCoef': SettingsValue('Коэффициент весовой метрики', 1),
  'deltaFlowCoef': SettingsValue('Коэффициент весовой метрики', 0.5),
  'resistanceCoef': SettingsValue('Коэффициент весовой метрики', 1),
  'edgesRange': SettingsValue('Максимальная длина ребёр', 16),
  'vertexRange': SettingsValue('Радиус зоны вершины', 8),
  'minVertexMass': SettingsValue('Минимальная масса вершины', 4),
  'minEdgeAngle': SettingsValue('Минимальный угол рёбер', 15)
};
