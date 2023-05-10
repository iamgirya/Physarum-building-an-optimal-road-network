#include "SlimeMoldClass.h"

SlimeMoldSimulation::SlimeMoldSimulation(it xSize, it ySize) {
	srand(it(time(NULL)));

	population = 0;
	location = Location(xSize, ySize);
	factory = SlimeAgentFactory();
}

void SlimeMoldSimulation::makeStep() {
	int i; ft timeForOneIteration1, timeForOneIteration2, timeForOneIteration3;
	timeForOneIteration1 = omp_get_wtime();
#pragma omp parallel for
	for (i = 0; i < particles.size(); i++) {
		particles[i]->moveTurn();
	}
	timeForOneIteration2 = omp_get_wtime();
	timeForOneIteration1 = timeForOneIteration2 - timeForOneIteration1;
	timeForOneIteration1 = omp_get_wtime();
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
	timeForOneIteration2 = omp_get_wtime();
	timeForOneIteration1 = timeForOneIteration2 - timeForOneIteration1;
	timeForOneIteration1 = omp_get_wtime();
#pragma omp parallel for
	for (i = 0; i < particles.size(); i++) {
		particles[i]->skanTurn();
	}
	// 30 ms
	timeForOneIteration2 = omp_get_wtime();
	timeForOneIteration1 = timeForOneIteration2 - timeForOneIteration1;
	timeForOneIteration1 = omp_get_wtime();
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
	timeForOneIteration2 = omp_get_wtime();
	timeForOneIteration1 = timeForOneIteration2 - timeForOneIteration1;
	timeForOneIteration1 = omp_get_wtime();
	location.castDiffusion(); // 15 ms
	timeForOneIteration2 = omp_get_wtime();
	timeForOneIteration1 = timeForOneIteration2 - timeForOneIteration1;
	timeForOneIteration1 = omp_get_wtime();
	location.castDecay();
	timeForOneIteration2 = omp_get_wtime();
	timeForOneIteration1 = timeForOneIteration2 - timeForOneIteration1;
	timeForOneIteration1 = omp_get_wtime();
}

void SlimeMoldSimulation::setUp(it timeToLive, it startPopulation, ft sensorOffsetDistance, ft sensorsAngle, ft rotationAngle, ft stepSize, ft depositPerStep, ft decayFactor, bool isPeriodicBoundary, bool isCanMultiAgent) {

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
}

void SlimeMoldSimulation::startSimulation(vector<ft> startPosition) {
	if (updateSettingsFromFile(true)) {
		it count = 0;
		std::random_device rd;
		std::mt19937 g(rd());
		ft timeForOneIteration1, timeForOneIteration2;
		ft sumTime = 0;
		while (true) {
			timeForOneIteration1 = omp_get_wtime();

			makeStep(); // 450 // 340 - если параллельно
			
			timeForOneIteration2 = omp_get_wtime();

			count++;
			timeForOneIteration1 = timeForOneIteration2 - timeForOneIteration1;
			cout << count << endl << timeForOneIteration1 << endl;
			sumTime += timeForOneIteration1;
			cout << sumTime/count << endl << endl; // 12

			shuffle(particles.begin(), particles.end(), g); // 3
			if (count % 4 == 0) {
				bool isUpdated = updateSettingsFromFile(false);
				outputInBmp(isUpdated); // 130 - txt // 5 - bmp
			}
			if (count % 100 == 0) {
				//дебаг код
				auto tmp = AgentGraphAnalyser();
				tmp.edgesRange = 14;
				tmp.vertexRange = 8;
				tmp.minVertexMass = 8;
				tmp.minEdgeAngle = 15;
				auto tmptmp = tmp.makeGraph(particles, location.generators);
				outputInBmpGraph(tmptmp, tmp.graph, false);
				tmp.minimizeGraph();
				auto fds1 = tmp.calculateWeigth();
				auto fds2 = tmp.calculateDeltaFlow();
				auto fds3 = tmp.calculateOmega();
				cout << 1;
				outputInBmpGraph(tmp.exitPoints, tmp.graph, true);
			}
		}
	}
}
