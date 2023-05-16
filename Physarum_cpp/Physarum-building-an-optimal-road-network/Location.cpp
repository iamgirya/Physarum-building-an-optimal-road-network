#include "SlimeMoldClass.h"

Location::Location() {}

Location::Location(it x, it y) {
	xSize = x;
	ySize = y;

	decayFactor = 0;
	isPeriodicBoundary = 0;
	isCanMultiAgent = 0;

	generators = vector<Generator*>();
	trailMap = vector<vector<ft>>(xSize, vector<ft>(ySize, 0.0));
	agentMap = vector<vector<short int>>(xSize, vector<short int>(ySize, 0));
	blockMap = vector<vector<short int>>(xSize, vector<short int>(ySize, 1));
	blockRectangles = vector<pair<pair<it, it>, pair<it, it>>>();
}

void Location::castDecay() {
	it i, j;
#pragma omp parallel for private(j)
	for (i = 0; i < xSize; i++) {
		for (j = 0; j < ySize; j++) {
			if (trailMap[i][j] > 0) {
				trailMap[i][j] -= decayFactor;
			}
		}
	}
}

void Location::castDiffusion() {
	//TODO пу-пу-пу, почему с диффузией не работает)
	return;
	vector<vector<ft>> newMap = vector<vector<ft>>(xSize, vector<ft>(ySize, 0.0));
	it i, j, k, l;
#pragma omp parallel for private(j, k, l)
	for (i = 0; i < xSize; i++) {
		for (j = 0; j < ySize; j++) {
			if (trailMap[i][j] > 0) {
				for (k = -1; k <= 1; k++) { 
					for (l = -1; l <= 1; l++) {
						if ((l == 0 || k == 0) && checkMatrix(i + k, j + l)) {
							newMap[i + k][j + l] += 0.04 * trailMap[i][j];
						}
					}
				}
				newMap[i][j] += 0.84 * trailMap[i][j];
			}
			else {
				newMap[i][j] += trailMap[i][j];
			}
		}
	}
	trailMap = newMap;
}

bool Location::canMakeMove(vector <it>& xy, vector <it>& oldxy) {
	if (!blockMap[xy[0]][xy[1]]) {
		return false;
	}

	if (!(isCanMultiAgent)) {
		if (!agentMap[xy[0]][xy[1]]) {
			agentMap[oldxy[0]][oldxy[1]] = 0;
			agentMap[xy[0]][xy[1]] = 1;
			return true;
		}
		return false;
	}

	return true;
}

vector<it> Location::getPixelOnCoord(vector <ft>& xy) {
	vector<it> rezult = { it(xy[0]), it(xy[1]) };
	if (isPeriodicBoundary) {
		if (rezult[0] < 0) {
			rezult[0] += xSize;
			xy[0] += xSize;
		}
		else if (rezult[0] >= xSize) {
			rezult[0] -= xSize;
			xy[0] -= xSize;
		}
		if (rezult[1] < 0) {
			rezult[1] += ySize;
			xy[1] += ySize;
		}
		else if (rezult[1] >= ySize) {
			rezult[1] -= ySize;
			xy[1] -= ySize;
		}
		return rezult;
	}
	else {
		if (rezult[0] < 0 || rezult[0] >= xSize || rezult[1] < 0 || rezult[1] >= ySize) {
			return vector<it>();
		}
		else {
			return rezult;
		}
	}
}

bool Location::checkMatrix(it i, it j) {
	return (i >= 0 && j >= 0 && i < xSize&& j < ySize && blockMap[i][j]);
}

vector<it> Location::getSizes() {
	return { xSize, ySize };
}

void Location::onAgentsDeath(vector<vector<it>> points, ft diff, it indexOfGenerator, ft startAngle) {
	if (indexOfGenerator != -1) {
		generators[indexOfGenerator]->push(startAngle);
	}

	for (int i = 0; i < points.size(); i++) {
		trailMap[points[i][0]][points[i][1]] += diff;
	}
}

int Location::checkNearGenerator(vector<it>& position, it teamIndex) {
	for (it i = 0; i < generators.size(); i++) {
		if (generators[i]->isNear(position, teamIndex)) {
			return i;
		}
	}
	return -1;
}