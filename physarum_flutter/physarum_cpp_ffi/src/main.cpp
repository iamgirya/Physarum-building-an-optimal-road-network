﻿#include "SlimeMoldClass.h"

int main()
{
	SlimeMoldSimulation sim = SlimeMoldSimulation(200, 200);
	auto loc = LocationSettings();
	loc.decayFactor = 1.5;
	loc.isCanMultiAgent = 1;
	loc.isPeriodicBoundary = 0;
	loc.xSize = 200;
	loc.ySize = 200;
	auto age = AgentSettings();
	age.depositPerStep = 3;
	age.rotationAngle = 45;
	age.sensorsAngle = 45;
	age.sensorOffsetDistance = 6;
	age.timeToLive = 80;
	age.stepSize = 1;
	auto ana = AnalyserSettings();
	ana.weigthCoef = 1;
	ana.overDistanceCoef = 1;
	ana.deltaFlowCoef = 0.5;
	ana.resistanceCoef = 1;
	ana.edgesRange = 16;
	ana.vertexRange = 8;
	ana.minVertexMass = 4;
	ana.minEdgeAngle = 15;

	sim.setUp(age, loc, ana, 0);
	sim.placeGenerators({make_pair(117,96), make_pair(122,71), make_pair(144,58) , make_pair(150,84) }, {1,1,1,1});
	/*sim.placeGenerators({
		make_pair(8,121),
		make_pair(35, 126),
		make_pair(35, 94),
		make_pair(59, 65),
		make_pair(20, 49),
		make_pair(45, 6),
		make_pair(82, 19),
		make_pair(98, 31),
		make_pair(82, 67),
		make_pair(72, 101),
		make_pair(95, 77),
		make_pair(123, 49),
		make_pair(146, 36),
		make_pair(182, 44),
		make_pair(153, 69),
		make_pair(191, 78),
		make_pair(135, 83),
		make_pair(158, 123),
		make_pair(105, 114),
		make_pair(122, 111),
		make_pair(96, 153),
		make_pair(83, 126),
		make_pair(127, 183),
		make_pair(153, 190) }, {
		1,
		3,
		3,
		3,
		1,
		3,
		3,
		3,
		3,
		6,
		3,
		3,
		3,
		1,
		3,
		1,
		3,
		1,
		3,
		1,
		3,
		3,
		1,
		1
		}
	);*/
	sim.startSimulation(2000);
}



/*
auto tmp = AgentGraphAnalyser();
	tmp.exitPoints = {
		make_pair(8,121),
		make_pair(35, 126),
		make_pair(35, 94),
		make_pair(59, 65),
		make_pair(20, 49),
		make_pair(45, 6),
		make_pair(82, 19),
		make_pair(98, 31),
		make_pair(82, 67),
		make_pair(72, 101),
		make_pair(95, 77),
		make_pair(123, 49),
		make_pair(146, 36),
		make_pair(182, 44),
		make_pair(153, 69),
		make_pair(191, 78),
		make_pair(135, 83),
		make_pair(158, 123),
		make_pair(105, 114),
		make_pair(122, 111),
		make_pair(96, 153),
		make_pair(83, 126),
		make_pair(127, 183),
		make_pair(153, 190)
	};

	tmp.graph = vector<vector<it>>(tmp.exitPoints.size());
	tmp.graph[0].push_back(1);
	tmp.graph[1].push_back(2);
	tmp.graph[2].push_back(3);
	tmp.graph[3].push_back(4);
	tmp.graph[3].push_back(5);
	tmp.graph[5].push_back(6);
	tmp.graph[6].push_back(7);
	tmp.graph[7].push_back(8);
	tmp.graph[8].push_back(9);
	tmp.graph[1].push_back(9);
	tmp.graph[3].push_back(9);
	tmp.graph[9].push_back(10);
	tmp.graph[10].push_back(11);
	tmp.graph[7].push_back(11);
	tmp.graph[11].push_back(12);
	tmp.graph[12].push_back(13);
	tmp.graph[11].push_back(14);
	tmp.graph[14].push_back(15);
	tmp.graph[14].push_back(16);
	tmp.graph[16].push_back(17);
	tmp.graph[16].push_back(18);
	tmp.graph[18].push_back(19);
	tmp.graph[18].push_back(20);
	tmp.graph[20].push_back(21);
	tmp.graph[9].push_back(21);
	tmp.graph[20].push_back(22);
	tmp.graph[22].push_back(23);
	for (int i = 0; i < tmp.graph.size(); i++) {
		for (it j : tmp.graph[i]) {
			if (i < j) {
				tmp.graph[j].push_back(i);
			}
		}
		tmp.towns.push_back(3);
		tmp.townIndexes.push_back(i);
	}
	tmp.towns[9] = 6;
	tmp.towns[23] = 1;
	tmp.towns[22] = 1;
	tmp.towns[19] = 1;
	tmp.towns[17] = 1;
	tmp.towns[15] = 1;
	tmp.towns[13] = 1;
	tmp.towns[4] = 1;
	tmp.towns[0] = 1;

	cout << tmp.calculateWeigth() << endl;
	cout << tmp.calculateDeltaFlow() << endl;
	cout << tmp.calculateOverDistance() << endl;
	cout << tmp.calculateResistance() << endl;
	cout << endl;
*/