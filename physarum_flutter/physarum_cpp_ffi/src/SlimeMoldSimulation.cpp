#include "SlimeMoldClass.h"

SlimeMoldSimulation::SlimeMoldSimulation() {}

SlimeMoldSimulation::SlimeMoldSimulation(it xSize, it ySize) {
	srand(it(time(NULL)));

	location = Location(xSize, ySize);
	factory = SlimeAgentFactory();
	analyser = AgentGraphAnalyser();
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

void SlimeMoldSimulation::setUp(AgentSettings agentSettings, LocationSettings locationSettings, AnalyserSettings analyserSettings, it startPopulation) {

	ft rotationAngle = it(agentSettings.rotationAngle) % 360;
	rotationAngle = rotationAngle * PI / 180;

	//матрицы нужные для поворота мув вектора и сенсоров
	SlimeAgent::rightRotationMatrix = { {cos(rotationAngle), -sin(rotationAngle)}, { sin(rotationAngle), cos(rotationAngle)} };
	SlimeAgent::leftRotationMatrix = { {cos(-rotationAngle), -sin(-rotationAngle)}, { sin(-rotationAngle), cos(-rotationAngle)} };
	SlimeAgent::depositPerStep = agentSettings.depositPerStep;
	
	// параметры агентов
	agentSettings.sensorsAngle = it(agentSettings.sensorsAngle) % 360;
	agentSettings.sensorsAngle = agentSettings.sensorsAngle * PI / 180;
	this->factory.settings = agentSettings;
	this->factory.location = &(this->location);

	// параметры локации
	this->location.settings = locationSettings;

	// параметры анализатора
	this->analyser.settings = analyserSettings;

	//стартовая популяция
	particles = this->factory.generatePopulationRandomPositions(startPopulation, this->location.getSizes());
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
	double sumTime = 0;
	double count = 0;
	
	while (stepCount--) {
		cout << particles.size() << endl;
		//double time1 = omp_get_wtime();
		makeStep();
		shuffle(particles.begin(), particles.end(), g);
		if (stepCount % 100 == 0 && location.generators.size() >= 2) {
			analyser.makeGraph(particles, location.generators);
			analyser.minimizeGraph();
			analyser.calculateMetrics();
		}
		//double time2 = omp_get_wtime();
		//count++;
		//sumTime += time2 - time1;
		//cout << sumTime/ count << endl;
	}
}
