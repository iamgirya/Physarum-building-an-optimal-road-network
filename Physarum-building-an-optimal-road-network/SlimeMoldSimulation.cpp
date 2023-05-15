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

vector<vector<ft>> saveTmp;
vector<vector<ft>> savicc;
bool isFirst = true;

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
			//cout << count << endl << timeForOneIteration1 << endl;
			sumTime += timeForOneIteration1;
			//cout << sumTime/count << endl; // 12

			shuffle(particles.begin(), particles.end(), g); // 3
			if (count % 4 == 0) {
				bool isUpdated = updateSettingsFromFile(false);
				outputInBmp(isUpdated); // 130 - txt // 5 - bmp
			}
			if (count % 100 == 0) {
				//дебаг код
				auto tmp = AgentGraphAnalyser();
				tmp.edgesRange = 16;
				tmp.vertexRange = 8;
				tmp.minVertexMass = 4;
				tmp.minEdgeAngle = 15;
				tmp.makeGraph(particles, location.generators);
				outputInBmpGraph(tmp.exitPoints, tmp.graph, false);
				tmp.minimizeGraph();
				outputInBmpGraph(tmp.exitPoints, tmp.graph, true);
				if (isFirst) {
					savicc.push_back({ 9999, 9999, 9999, 9999, 9999 });
					savicc.push_back({ 9999, 9999, 9999, 9999, 9999 });
					savicc.push_back({ 9999, 9999, 9999, 9999, 9999 });
					savicc.push_back({ 9999, 9999, 9999, 9999, 9999 });
					savicc.push_back({ 9999, 9999, 9999, 9999, 9999 });
					savicc.push_back({ 9999, 9999, 9999, 9999, 9999 });
					isFirst = false;
				}
				if (tmp.checkConnected()) {
					auto fds1 = tmp.calculateWeigth();
					auto fds2 = tmp.calculateDeltaFlow();
					auto fds3 = tmp.calculateOverDistance();
					auto fds4 = tmp.calculateResistance();
					vector <ft> fds = {fds1, fds2, fds3, ft(fds4.first), ft(fds4.second), };
					
					for (int yui = 0; yui < fds.size(); yui++) {
						if (fds[yui] < savicc[yui][yui]) {
							savicc[yui][0] = fds[0];
							savicc[yui][1] = fds[1];
							savicc[yui][2] = fds[2];
							savicc[yui][3] = fds[3];
							savicc[yui][4] = fds[4];
						
							cout << yui << endl;
							cout << fds1 << endl;
							cout << fds2 << endl;
							cout << fds3 << endl;
							cout << fds4.first << endl;
							cout << fds4.second << endl;
							cout << endl;
						}
					}

					if (fds[0] * fds[1] * fds[2] < savicc[5][0] * savicc[5][1] * savicc[5][2]) {
						int yui = 5;
						savicc[yui][0] = fds[0];
						savicc[yui][1] = fds[1];
						savicc[yui][2] = fds[2];
						savicc[yui][3] = fds[3];
						savicc[yui][4] = fds[4];

						cout << yui << endl;
						cout << fds1 << endl;
						cout << fds2 << endl;
						cout << fds3 << endl;
						cout << fds4.first << endl;
						cout << fds4.second << endl;
						cout << endl;
					}
					
					
					saveTmp.push_back({ fds1 , fds2 , fds3, ft(fds4.first), ft(fds4.second) });
					
				}
			}
		}
	}
}
