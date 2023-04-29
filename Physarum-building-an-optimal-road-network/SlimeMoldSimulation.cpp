#include "SlimeMoldClass.h"

SlimeMoldSimulation::SlimeMoldSimulation() {
	srand(it(time(NULL)));

	population = 0;

	//матрицы нужные для поворота мув вектора и сенсоров
	rightRotationMatrix = { {cos(0), -sin(0)}, { sin(0), cos(0)} };
	leftRotationMatrix = { {cos(-0), -sin(-0)}, { sin(-0), cos(-0)} };

	// параметры агентов
	sensorOffsetDistance = 0;
	sensorWidth = 1;
	sensorAngle = 0;
	stepSize = 0;
	depositPerStep = 0;
	chanceOfRandomChangeDirection = 0;

	// параметры локации
	diffusionSize = 0;
	decayFactor = 0;
	isPeriodicBoundary = 0;
	isCanMultiAgent = 0;
	location = Location(0, 0, this);
}

void SlimeMoldSimulation::setLocation(it xSize, it ySize) {
	location = Location(xSize, ySize, this);
}

void SlimeMoldSimulation::makeStep() {
	int i;

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
			particles[i]->timeToLife--;
			it generatorIndex = particles[i]->checkGeneratorsTurn();
			if (generatorIndex != -1 || particles[i]->timeToLife <=0) {
				particles[i]->deadTurn(generatorIndex);
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

#pragma omp parallel
	{
		vector<SlimeAgent*> newPart;
#pragma omp for nowait
		for (i = 0; i < generators.size(); i++) {
			newPart = generatePopulationInPixel(generators[i].second, generators[i].first, i);
			location.trailMap[it(generators[i].first[0])][it(generators[i].first[1])]+=generators[i].second*depositPerStep;
#pragma omp critical
			particles.insert(particles.end(), newPart.begin(), newPart.end());
		}
	}
	

	location.castDiffusion();
	location.castDecay();
}

SlimeAgent* SlimeMoldSimulation::generateAgent(vector<ft> startPosition, ft startAngle, it teamIndex = -1) {
	ft piStartAngle = startAngle / 180 * PI;
	vector<ft> moveVector = { stepSize * cos(startAngle), stepSize * sin(startAngle) };
	vector<ft> lsVector = { sensorOffsetDistance * cos(startAngle + sensorAngle), sensorOffsetDistance * sin(startAngle + sensorAngle) };
	vector<ft> csVector = { sensorOffsetDistance * cos(startAngle), sensorOffsetDistance * sin(startAngle) };
	vector<ft> rsVector = { sensorOffsetDistance * cos(startAngle - sensorAngle), sensorOffsetDistance * sin(startAngle - sensorAngle) };
	SlimeAgent* tmp = new SlimeAgent();
	tmp->setUp(startAngle, teamIndex, startTimeToLife ,startPosition, moveVector, lsVector, csVector, rsVector, this);
	return tmp;
}

vector<SlimeAgent*> SlimeMoldSimulation::generatePopulationInPixel(it count, vector<ft>& startPosition, it teamIndex = -1) {
	vector<SlimeAgent*> rezult;

	for (int i = 0; i < count; i++) {
		if (teamIndex != -1 && !generatorsQueue[teamIndex].empty()) {
			ft tmp = generatorsQueue[teamIndex].front();
			generatorsQueue[teamIndex].pop();
			rezult.push_back(generateAgent(startPosition, tmp, teamIndex));
		}
		else {
			rezult.push_back(generateAgent(startPosition, (ft(rand() % 360)), teamIndex));
		}
	}
	return rezult;
}

vector<SlimeAgent*> SlimeMoldSimulation::generatePopulationRandomPositions(it count, vector<it> sizes) {
	vector<SlimeAgent*> rezult;

	for (int i = 0; i < count; i++) {
		rezult.push_back(generateAgent({ ft(rand() % sizes[0]) , ft(rand() % sizes[1]) }, (ft(rand() % 360))));
	}
	return rezult;
}

void SlimeMoldSimulation::setUp(it ttl, it sp, ft sod, it sw, ft sa, ft ra, ft ss, ft dps, ft corcd, it dif, ft dec, bool ipb, bool icma) {
	population = sp;
	startTimeToLife = ttl;

	ra = it(ra) % 360;
	ra = ra * PI / 180;

	sa = it(sa) % 360;
	sa = sa * PI / 180;
	//матрицы нужные для поворота мув вектора и сенсоров
	rightRotationMatrix = { {cos(ra), -sin(ra)}, { sin(ra), cos(ra)} };
	leftRotationMatrix = { {cos(-ra), -sin(-ra)}, { sin(-ra), cos(-ra)} };
	
	// параметры агентов
	sensorOffsetDistance = sod;
	sensorWidth = 1;
	sensorAngle = sa;
	stepSize = ss;
	depositPerStep = dps;
	chanceOfRandomChangeDirection = corcd;

	// параметры локации
	diffusionSize = dif;
	decayFactor = dec;
	isPeriodicBoundary = ipb;
	isCanMultiAgent = icma;
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
