#include "SlimeMoldClass.h"

Generator::Generator() {}

Generator::Generator(vector<ft> position, it agentsPerStep, it teamIndex, SlimeAgentFactory* factory) {
	this->position = { position[0], position[1] };
	this->agentsPerStep = agentsPerStep;
	this->teamIndex = teamIndex;
	this->generatorsQueue = queue<ft>();
	this->factory = factory;
}

bool Generator::isNear(vector<it>& position, it team) {
	return team != teamIndex && abs(this->position[0] - it(position[0])) <= rangeOfGenerator && abs(this->position[1] - it(position[1])) <= rangeOfGenerator;
}

void Generator::push(ft angle) {
	ft newAngle = angle + (rand() % 2 == 0 ? -1 : 1) * someAngle;
	if (generatorsQueue.size() < 5000) {
		generatorsQueue.push(newAngle);
	}
}

vector<SlimeAgent*> Generator::generate() {
	vector<SlimeAgent*> rezult;

	for (int i = 0; i < agentsPerStep; i++) {
		if (!generatorsQueue.empty()) {
			ft startAngle = generatorsQueue.front();
			generatorsQueue.pop();
			rezult.push_back(factory->generateAgent(position, startAngle, teamIndex));
		}
		else {
			rezult.push_back(factory->generateAgent(position, (ft(rand() % 360)), teamIndex));
		}
	}
	return rezult;
}