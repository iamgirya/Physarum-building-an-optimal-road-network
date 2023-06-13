class SettingsData {
  final String name;
  final num defaultValue;
  const SettingsData(this.name, this.defaultValue);
}

const Map<String, SettingsData> settingsMappedData = {
  //location
  'locationX': SettingsData('Ширина локации', 200),
  'locationY': SettingsData('Высота локации', 200),
  'startPopulation': SettingsData('Стартовая популяция', 0),
  'decayFactor': SettingsData('Множитель разложения', 1.5),
  'isPeriodicBoundary': SettingsData('Бесконечное поле', 0),
  'isCanMultiAgent': SettingsData('Множество агентов в одной точке', 1),
  //agent
  'timeToLive': SettingsData('Время жизни агента', 80),
  'sensorOffsetDistance': SettingsData('Длина сенсоров', 6),
  'sensorsAngle': SettingsData('Угол сенсоров', 45),
  'rotationAngle': SettingsData('Угол поворота', 45),
  'stepSize': SettingsData('Длина шага', 1),
  'depositPerStep': SettingsData('След за шаг', 5),
  //analyser
  'weigthCoef': SettingsData('Коэффициент весовой метрики', 1),
  'overDistanceCoef': SettingsData('Коэффициент весовой метрики', 1),
  'deltaFlowCoef': SettingsData('Коэффициент весовой метрики', 0.5),
  'resistanceCoef': SettingsData('Коэффициент весовой метрики', 1),
  'edgesRange': SettingsData('Максимальная длина ребёр', 16),
  'vertexRange': SettingsData('Радиус зоны вершины', 8),
  'minVertexMass': SettingsData('Минимальная масса вершины', 4),
  'minEdgeAngle': SettingsData('Минимальный угол рёбер', 15)
};
