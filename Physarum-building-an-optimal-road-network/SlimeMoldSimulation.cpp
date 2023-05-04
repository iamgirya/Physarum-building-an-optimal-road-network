#include "SlimeMoldClass.h"

SlimeMoldSimulation::SlimeMoldSimulation() {
	srand(it(time(NULL)));

	population = 0;

	//матрицы нужные для поворота мув вектора и сенсоров
	rightRotationMatrix = { {cos(0), -sin(0)}, { sin(0), cos(0)} };
	leftRotationMatrix = { {cos(-0), -sin(-0)}, { sin(-0), cos(-0)} };

	// параметры локации
	location = Location(0, 0);
	factory = SlimeAgentFactory();
}

void SlimeMoldSimulation::setLocation(it xSize, it ySize) {
	location = Location(xSize, ySize);
}

void SlimeMoldSimulation::makeStep() {
	int i;
	
#pragma omp parallel for
	for (i = 0; i < particles.size(); i++) {
		particles[i]->moveTurn();
	}
	// 12 ms для 100 сек
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

void SlimeMoldSimulation::setUp(it timeToLive, it startPopulation, ft sensorOffsetDistance, ft sensorsAngle, ft rotationAngle, ft stepSize, ft depositPerStep, ft decayFactor, bool isPeriodicBoundary, bool isCanMultiAgent) {
	this->population = startPopulation;

	rotationAngle = it(rotationAngle) % 360;
	rotationAngle = rotationAngle * PI / 180;

	sensorsAngle = it(sensorsAngle) % 360;
	sensorsAngle = sensorsAngle * PI / 180;
	//матрицы нужные для поворота мув вектора и сенсоров
	this->rightRotationMatrix = { {cos(rotationAngle), -sin(rotationAngle)}, { sin(rotationAngle), cos(rotationAngle)} };
	this->leftRotationMatrix = { {cos(-rotationAngle), -sin(-rotationAngle)}, { sin(-rotationAngle), cos(-rotationAngle)} };
	
	// параметры агентов
	this->factory.startTimeToLife = timeToLive;
	this->factory.sensorOffsetDistance = sensorOffsetDistance;
	this->factory.sensorAngle = sensorsAngle;
	this->factory.stepSize = stepSize;
	this->factory.settings = this;
	this->depositPerStep = depositPerStep;


	// параметры локации
	this->location.decayFactor = decayFactor;
	this->location.isPeriodicBoundary = isPeriodicBoundary;
	this->location.isCanMultiAgent = isCanMultiAgent;
}

void SlimeMoldSimulation::startSimulation(vector<ft> startPosition) {
	if (updateSettingsFromFile()) {
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
				bool isUpdated = updateSettingsFromFile();
				outputInBmp(isUpdated); // 130 - txt // 5 - bmp
			}
		}
	}
}
