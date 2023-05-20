#include "SlimeMoldClass.h"

SlimeMoldSimulation::SlimeMoldSimulation() {}

SlimeMoldSimulation::SlimeMoldSimulation(it xSize, it ySize) {
	srand(it(time(NULL)));

	population = 0;
	location = Location(xSize, ySize);
	factory = SlimeAgentFactory();
	analyser = AgentGraphAnalyser();
}

void SlimeMoldSimulation::makeStep() {
	int i; ft timeForOneIteration1, timeForOneIteration2, timeForOneIteration3;
#pragma omp parallel for
	for (i = 0; i < particles.size(); i++) {
		particles[i]->moveTurn();
	}
	vector<SlimeAgent*> stillAlive;
#pragma omp parallel
	{
		vector<SlimeAgent*> stillAlivePrivate;
#pragma omp for nowait // в конце цикла потоки не ждут друг друга, а идут дальше
		for (i = 0; i < particles.size(); i++) {
			if (particles[i]->isTimeToDeath()) {
				delete particles[i];
			}
			else {
				stillAlivePrivate.push_back(particles[i]);
			}	
		}
#pragma omp critical // данная строка выполняется по очереди
		stillAlive.insert(stillAlive.end(), stillAlivePrivate.begin(), stillAlivePrivate.end());
	}
	particles = stillAlive;
#pragma omp parallel for
	for (i = 0; i < particles.size(); i++) {
		particles[i]->skanTurn();
	}
	// 30 ms
#pragma omp parallel
	{
		vector<SlimeAgent*> newPart;
#pragma omp for nowait
		for (int i = 0; i < location.generators.size(); i++) {
			newPart = location.generators[i]->generate();
#pragma omp critical
			particles.insert(particles.end(), newPart.begin(), newPart.end());
		}
	}
	location.castDiffusion(); // 15 ms
	location.castDecay();
}

void SlimeMoldSimulation::setUp(it timeToLive, it startPopulation, ft sensorOffsetDistance, ft sensorsAngle, ft rotationAngle, ft stepSize, ft depositPerStep, ft decayFactor, bool isPeriodicBoundary, bool isCanMultiAgent, ft edgesRange, ft vertexRange, ft minVertexMass, ft minEdgeAngle) {

	rotationAngle = it(rotationAngle) % 360;
	rotationAngle = rotationAngle * PI / 180;

	sensorsAngle = it(sensorsAngle) % 360;
	sensorsAngle = sensorsAngle * PI / 180;
	//матрицы нужные для поворота мув вектора и сенсоров
	SlimeAgent::rightRotationMatrix = { {cos(rotationAngle), -sin(rotationAngle)}, { sin(rotationAngle), cos(rotationAngle)} };
	SlimeAgent::leftRotationMatrix = { {cos(-rotationAngle), -sin(-rotationAngle)}, { sin(-rotationAngle), cos(-rotationAngle)} };
	SlimeAgent::depositPerStep = depositPerStep;
	
	this->population = startPopulation;
	// параметры агентов
	this->factory.startTimeToLife = timeToLive;
	this->factory.sensorOffsetDistance = sensorOffsetDistance;
	this->factory.sensorAngle = sensorsAngle;
	this->factory.stepSize = stepSize;
	this->factory.location = &(this->location);

	// параметры локации
	this->location.decayFactor = decayFactor;
	this->location.isPeriodicBoundary = isPeriodicBoundary;
	this->location.isCanMultiAgent = isCanMultiAgent;

	// параметры анализатора
	this->analyser.edgesRange = edgesRange;
	this->analyser.vertexRange = vertexRange;
	this->analyser.minVertexMass = minVertexMass;
	this->analyser.minEdgeAngle = minEdgeAngle;
}

void SlimeMoldSimulation::placeGenerators(vector<pair<it, it>> positions, vector<it> agentsPerSec) {
	for (int i = 0; i < positions.size(); i++) {
		Generator* tmpGenerator = new Generator({ ft(positions[i].first), ft(positions[i].second)}, ft(agentsPerSec[i]), i, &factory);
		location.generators.push_back(tmpGenerator);
	}
}

void SlimeMoldSimulation::startSimulation(it stepCount) {
	std::random_device rd;
	std::mt19937 g(rd());
	while (stepCount--) {
		makeStep();
		shuffle(particles.begin(), particles.end(), g);
		if (stepCount % 100 == 0) {
			analyser.makeGraph(particles, location.generators);
			analyser.minimizeGraph();
			analyser.calculateMetrics();
		}
	}
}
