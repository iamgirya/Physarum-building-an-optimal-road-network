#include "SlimeMoldClass.h"

SlimeMoldSimulation sim;

FFI_PLUGIN_EXPORT void setUpTowns(IntArray* x, IntArray* y, IntArray* towns) {
	vector <it> generatorsPriotiry;
	vector <pair<it,it>> generatorsPoint;
	for (int i = 0; i < x->length; i++) {
		generatorsPoint.push_back(make_pair(x->data[i], y->data[i]));
		generatorsPriotiry.push_back(towns->data[i]);
	}

	sim.placeGenerators(generatorsPoint, generatorsPriotiry);
}

FFI_PLUGIN_EXPORT void setUpSimulation(AgentSettings* agentSettings, LocationSettings* locationSettings, AnalyserSettings* analyserSettings, it startPopulation) {
	// пересоздаЄм симул€цию
	sim = SlimeMoldSimulation(locationSettings->xSize, locationSettings->ySize);
	
	sim.setUp(*agentSettings, *locationSettings, *analyserSettings, startPopulation);
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

FFI_PLUGIN_EXPORT void execute(int stepCount) {
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