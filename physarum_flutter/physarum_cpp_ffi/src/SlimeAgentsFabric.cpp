#include "SlimeMoldClass.h"

SlimeAgentFactory::SlimeAgentFactory() {}

SlimeAgent* SlimeAgentFactory::generateAgent(vector<ft> startPosition, ft startAngle, it teamIndex = -1) {
	ft stepSize = settings.stepSize;
	ft sensorOffsetDistance = settings.sensorOffsetDistance;
	ft startTimeToLife = settings.startTimeToLife;
	ft sensorsAngle = settings.sensorsAngle;

	ft piStartAngle = startAngle / 180 * PI;
	vector<ft> moveVector = { stepSize * cos(startAngle), stepSize * sin(startAngle) };
	vector<ft> lsVector = { sensorOffsetDistance * cos(startAngle + sensorsAngle), sensorOffsetDistance * sin(startAngle + sensorsAngle) };
	vector<ft> csVector = { sensorOffsetDistance * cos(startAngle), sensorOffsetDistance * sin(startAngle) };
	vector<ft> rsVector = { sensorOffsetDistance * cos(startAngle - sensorsAngle), sensorOffsetDistance * sin(startAngle - sensorsAngle) };
	SlimeAgent* tmp = new SlimeAgent();
	tmp->setUp(startAngle, teamIndex, startTimeToLife, startPosition, moveVector, lsVector, csVector, rsVector, location);
	return tmp;
}

vector<SlimeAgent*> SlimeAgentFactory::generatePopulationInPixel(it count, vector<ft>& startPosition, it teamIndex = -1) {
	vector<SlimeAgent*> rezult;

	for (int i = 0; i < count; i++) {
		rezult.push_back(generateAgent(startPosition, (ft(rand() % 360)), teamIndex));
	}
	return rezult;
}

vector<SlimeAgent*> SlimeAgentFactory::generatePopulationRandomPositions(it count, vector<it> sizes) {
	vector<SlimeAgent*> rezult;

	for (int i = 0; i < count; i++) {
		rezult.push_back(generateAgent({ ft(rand() % sizes[0]) , ft(rand() % sizes[1]) }, (ft(rand() % 360))));
	}
	return rezult;
}