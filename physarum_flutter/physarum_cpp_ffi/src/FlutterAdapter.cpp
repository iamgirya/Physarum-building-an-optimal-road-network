#include "SlimeMoldClass.h"

SlimeMoldSimulation sim;

void restartSimulation() {
	sim = SlimeMoldSimulation(200, 200);
	sim.setUp(80, 0, 6, 45, 45, 1, 3, 1.5, 0, 1);
	sim.placeGenerators({ 
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
	);
}

SlimeMoldNetwork *parseSimulationToNetwork(vector<vector<it>>& graph, vector<it>& towns, vector<pair<it,it>>& exitPoints) {
	// parsing time!
	int sizeOfNetwork = graph.size();
	auto *result = (SlimeMoldNetwork *) malloc(sizeof(SlimeMoldNetwork));
	result->length = sizeOfNetwork;

	// exitPoints
    auto *pointsX = (IntArray *) malloc(sizeof(IntArray));
	pointsX->length = sizeOfNetwork;
	pointsX->data = (int32_t *) malloc(pointsX->length * sizeof(int32_t));
	for (int j = 0; j < pointsX->length; j++) {
		pointsX->data[j] = exitPoints[j].first;
	}
	result->exitPointsX = pointsX;

	auto *pointsY = (IntArray *) malloc(sizeof(IntArray));
	pointsY->length = sizeOfNetwork;
	pointsY->data = (int32_t *) malloc(pointsY->length * sizeof(int32_t));
	for (int j = 0; j < pointsY->length; j++) {
		pointsY->data[j] = exitPoints[j].second;
	}
	result->exitPointsY = pointsY;

	// towns
	auto *exitTowns = (IntArray *) malloc(sizeof(IntArray));
	exitTowns->length = sizeOfNetwork;
	exitTowns->data = (int32_t *) malloc(exitTowns->length * sizeof(int32_t));
	for (int j = 0; j < exitTowns->length; j++) {
		exitTowns->data[j] = towns[j];
	}
	result->towns = exitTowns;

	// graph
	auto *exitGraph = (IntArrayArray *) malloc(sizeof(IntArrayArray));
	exitGraph->length = sizeOfNetwork;
	exitGraph->data = (IntArray *) malloc(exitGraph->length * sizeof(IntArray));
	for (int i = 0; i < exitGraph->length; i++) {
		auto *exitGraphPart = (IntArray *) malloc(sizeof(IntArray));
		exitGraphPart->length = graph[i].size();
		exitGraphPart->data = (int32_t *) malloc(exitGraphPart->length * sizeof(int32_t));
		for (int j = 0; j < exitGraphPart->length; j++) {
			exitGraphPart->data[j] = graph[i][j];
		}
		exitGraph->data[i] = *exitGraphPart;
	}
	result->graph = exitGraph;

	return result;
}

FFI_PLUGIN_EXPORT void execute(int stepCount, int isNeedRestart) {
	if (isNeedRestart) {
		restartSimulation();
	}

	sim.startSimulation(stepCount);
}

FFI_PLUGIN_EXPORT SlimeMoldNetwork *getGraph(bool isNeedBest) {
	if (isNeedBest) {
		return parseSimulationToNetwork(sim.analyser.bestGraph, sim.analyser.bestTowns, sim.analyser.bestExitPoints);
	} else {
		return parseSimulationToNetwork(sim.analyser.graph, sim.analyser.towns, sim.analyser.exitPoints);
	}
}

FFI_PLUGIN_EXPORT DoubleArray *getBestMetrics() {
	auto *metrics = (DoubleArray *) malloc(sizeof(DoubleArray));
	if (sim.analyser.bestGraph.size()) {
		metrics->length = 4;
		metrics->data = (double *) malloc(metrics->length * sizeof(double));
		metrics->data[0] = sim.analyser.metricWeigth;
		metrics->data[1] = sim.analyser.metricOverDistance;
		metrics->data[2] = sim.analyser.metricResistance;
		metrics->data[3] = sim.analyser.metricDeltaFlow;
	} else {
		metrics->length = 0;
	}
	return metrics;
}