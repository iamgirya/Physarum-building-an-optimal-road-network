const Map<String, String> settingsName = {
  'locationX': 'Ширина локации',
  'locationY': 'Высота локации',
  'timeToLive': 'Время жизни агента',
  'startPopulation': 'Стартовая популяция',
  'sensorOffsetDistance': 'Длина сенсоров',
  'sensorsAngle': 'Угол сенсоров',
  'rotationAngle': 'Угол поворота',
  'stepSize': 'Длина шага',
  'depositPerStep': 'След за шаг',
  'decayFactor': 'Множитель разложения',
  'isPeriodicBoundary': 'Бесконечное поле',
  'isCanMultiAgent': 'Множество агентов в одной точке',
  'edgesRange': 'Максимальная длина ребёр',
  'vertexRange': 'Радиус зоны вершины',
  'minVertexMass': 'Минимальная масса вершины',
  'minEdgeAngle': 'Минимальный угол рёбер',
};

const Map<String, num> settingsDefault = {
  'locationX': 200,
  'locationY': 200,
  'timeToLive': 80,
  'startPopulation': 0,
  'sensorOffsetDistance': 6,
  'sensorsAngle': 45,
  'rotationAngle': 45,
  'stepSize': 1,
  'depositPerStep': 3,
  'decayFactor': 1.5,
  'isPeriodicBoundary': 0,
  'isCanMultiAgent': 1,
  'edgesRange': 16,
  'vertexRange': 8,
  'minVertexMass': 4,
  'minEdgeAngle': 15,
};
