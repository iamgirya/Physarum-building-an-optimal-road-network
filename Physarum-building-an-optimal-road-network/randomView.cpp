#include <math.h>
#include <tuple>
#include <vector>
#include <stdio.h>
#include <algorithm>
#include <queue>
#include <string>
#include <iostream>
#include <fstream>
#include <chrono>
using namespace std;
typedef long long ll;
typedef long long it;
typedef double ft;

const double PI = acos(-1.0);

class SlimeAgent {
public:
	//вектор нынешней позиции
	vector<ft> positionVector;

	// нынеший занимаемый пиксель
	vector<it> pixelVector;

	//вектор, куда должно быть сделано следующее движение
	vector<ft> moveVector;

	//вектор, где находится сенсоры
	vector<ft> leftSensorVector;
	vector<ft> centerSensorVector;
	vector<ft> rigthSensorVector;

	SlimeAgent(vector<ft> pv, vector<ft> mv, vector<ft> lsv, vector<ft> csv, vector<ft> rsv) {
		positionVector = pv;
		moveVector = mv;
		leftSensorVector = lsv;
		centerSensorVector = csv;
		rigthSensorVector = rsv;
		pixelVector = { it(pv[0]), it(pv[1]) };
	}
	//движение
	void moveTurn(SlimeAgentsSettings& set, Location& loc) {
		// если движение успешно
		if (move(loc)) {
			makeDeposit(set, loc);
		}
		else {
			rotate(set, rand() % 2);
		}
	}
	//сканирование
	void skanTurn(SlimeAgentsSettings& set, Location& loc) {
		//шанс рандомного поворота
		if (set.chanceOfRandomChangeDirection > (rand() % 100 + 1)) {
			rotate(set, rand() % 2);
		}
		else {
			// -1 - поворот налево, 0 - сохранение направления, 1 - поворот направо
			it rotateSide = activateSensors(loc);
			if (rotateSide != 0) {
				rotate(set, rotateSide == 1);
			}
		}
	}
	// функция хода
	void makeFullTurn(SlimeAgentsSettings& set, Location& loc) {
		moveTurn(set, loc);
		skanTurn(set, loc);
	}
private:

	template<typename T>
	vector<T> vSum(vector<T> x, vector<T> y) {
		vector<T> rezult(x.size());
		for (int i = 0; i < x.size(); i++) {
			rezult[i] = x[i] + y[i];
		}
		return rezult;
	}
	
	template<typename T>
	vector<T> vMult(vector<T> x, vector<vector<T>> r) {
		vector<T> rezult(x.size(), 0);
		for (int i = 0; i < x.size(); i++) {
			for (int j = 0; j < x.size(); j++) {
				rezult += x[i] * r[j][i];
			}
		}
		return rezult;
	}
	// определяет направление поворота
	it activateSensors(Location& loc) {
		it lw, cw, rw;
		vector<it> pixelSensorPosition;
		
		pixelSensorPosition = loc.getPixelOnCoord(vSum(positionVector, leftSensorVector));
		if (!pixelSensorPosition.empty()) {
			lw = loc.trailMap[pixelSensorPosition[0]][pixelSensorPosition[1]];
		}
		else {
			lw = 0;
		}

		pixelSensorPosition = loc.getPixelOnCoord(vSum(positionVector, centerSensorVector));
		if (!pixelSensorPosition.empty()) {
			cw = loc.trailMap[pixelSensorPosition[0]][pixelSensorPosition[1]];
		}
		else {
			cw = 0;
		}

		pixelSensorPosition = loc.getPixelOnCoord(vSum(positionVector, rigthSensorVector));
		if (!pixelSensorPosition.empty()) {
			rw = loc.trailMap[pixelSensorPosition[0]][pixelSensorPosition[1]];
		}
		else {
			rw = 0;
		}

		if ((cw >= rw && cw > lw) || (cw > rw && cw >= lw)) {
			return 0;
		}
		else if (rw > cw && rw > lw) {
			return 1;
		}
		else if (lw > cw && lw > rw) {
			return -1;
		} else if (cw == lw && cw == rw) {
			return (rand() % 3) -1;
		}
	}
	// поворот векторов скорости и сенсоров
	void rotate(SlimeAgentsSettings& set, bool isRigth) {
		if (isRigth) {
			moveVector = vMult(moveVector, set.rightRotationMatrix);
			leftSensorVector = vMult(moveVector, set.rightRotationMatrix);
			centerSensorVector = vMult(moveVector, set.rightRotationMatrix);
			rigthSensorVector = vMult(moveVector, set.rightRotationMatrix);
		}
		else {
			moveVector = vMult(moveVector, set.leftRotationMatrix);
			leftSensorVector = vMult(moveVector, set.leftRotationMatrix);
			centerSensorVector = vMult(moveVector, set.leftRotationMatrix);
			rigthSensorVector = vMult(moveVector, set.leftRotationMatrix);
		}
	}
	//сдвиг и установка пикселя, если это возможно
	bool move(Location& loc) {
		vector<ft> newPosition = vSum(positionVector, moveVector);
		vector<it> newPixel = loc.getPixelOnCoord(newPosition);
		if (newPixel.empty() || !loc.canMakeMove(newPixel)) {
			return false;
		}
		else {
			positionVector = newPosition;

			if (!loc.isCanMultiAgent) {
				loc.agentMap[pixelVector[0]][pixelVector[1]] = false;
				loc.agentMap[newPixel[0]][newPixel[1]] = true;
			}
			pixelVector = newPixel;
			
			return true;
		}
	}
	// нанесение следа на карту
	void makeDeposit(SlimeAgentsSettings& set, Location& loc) {
		loc.trailMap[pixelVector[0]][pixelVector[1]] += set.depositPerStep;
	}
};

class SlimeAgentsSettings {
public:
	//активно используемые;
	vector<vector<ft>> leftRotationMatrix;
	vector<vector<ft>> rightRotationMatrix;
	ft depositPerStep;
	ft chanceOfRandomChangeDirection;
	
	//используемые для создания
	ft sensorAngle;
	ft sensorOffsetDistance;
	it sensorWidth; //пока автоединица, не используется
	ft stepSize;
	it startPopulation;

	void setUp(it sp, ft sod, it sw, ft sa, ft ra, ft ss, ft dps, ft corcd = 0) {
		ra = it(ra)%360;
		//матрицы нужные для поворота мув вектора и сенсоров
		ra = ra * PI / 180;
		rightRotationMatrix = { {cos(ra), -sin(ra)}, { sin(ra), cos(ra)} };
		leftRotationMatrix = { {cos(-ra), -sin(-ra)}, { sin(-ra), cos(-ra)} };

		sa = it(sa) % 360;
		sensorAngle = sa * PI / 180;
		startPopulation = sp;
		sensorOffsetDistance = sod;
		sensorWidth = 1;
		stepSize = ss;
		depositPerStep = dps;
		chanceOfRandomChangeDirection = corcd;

		srand(it(time(NULL)));
	}

	SlimeAgent generateAgent(vector<ft>& startPosition, ft startAngle) {
		startAngle = startAngle / 180 * PI;
		vector<ft> moveVector = {stepSize*cos(startAngle), stepSize * sin(startAngle)};
		vector<ft> lsVector = { sensorOffsetDistance * cos(startAngle + sensorAngle), sensorOffsetDistance * sin(startAngle+ sensorAngle) };
		vector<ft> csVector = { sensorOffsetDistance * cos(startAngle), sensorOffsetDistance * sin(startAngle) };
		vector<ft> rsVector = { sensorOffsetDistance * cos(startAngle- sensorAngle), sensorOffsetDistance * sin(startAngle- sensorAngle) };

		return SlimeAgent(startPosition, moveVector, lsVector, csVector, rsVector);
	}

	vector<SlimeAgent> generatePopulationInPixel(vector<ft>& startPosition) {
		vector<SlimeAgent> rezult(startPopulation);

		ft angle = 0;
		for (int i = 0; i < startPopulation; i++) {
			rezult[i] = generateAgent(startPosition, (ft(rand()%360))/180.0*PI);
			angle += sensorAngle;
		}
	}
};

class Location {
public:
	it xSize;
	it ySize;
	vector<vector<ft>> trailMap;

	it diffusionSize;
	ft decayFactor; //mult

	bool isPeriodicBoundary;
	bool isCanMultiAgent;
	vector<vector<bool>> agentMap;

	Location(it x, it y, it dif, ft dec, bool ipb, bool icma) {
		xSize = x;
		ySize - y;
		diffusionSize = dif;
		decayFactor = dec;
		isPeriodicBoundary = ipb;
		isCanMultiAgent = icma;

		trailMap = vector<vector<ft>>(xSize, vector<ft>(ySize, 0.0));
		if (isCanMultiAgent) {
			agentMap = vector<vector<bool>>(xSize, vector<bool>(ySize, false));
		}
	}

	void castDecay() {
		for (int i = 0; i < xSize; i++) {
			for (int j = 0; j < ySize; j++) {
				trailMap[i][j] *= decayFactor;
			}
		}
	}

	void castDiffusion() {
		vector<vector<ft>> newMap = vector<vector<ft>>(xSize, vector<ft>(ySize, 0.0));
		for (int i = 0; i < xSize; i++) {
			for (int j = 0; j < ySize; j++) {
				if (trailMap[i][j]) {
					for (int k = -1; k <= 1; k++) {
						for (int l = -1; l <= 1; l++) {
							if (checkMatrix(i+k, j + l)) {
								newMap[i+k][j+l] += 0.1 * trailMap[i][j];
							}
						}
					}
					newMap[i][j] += 0.1 * trailMap[i][j];
				}
			}
		}
		trailMap = newMap;
	}

	bool canMakeMove(vector <it> xy) {
		if (!isCanMultiAgent) {
			return !agentMap[xy[0]][xy[1]];
		}
		return true;
	}

	vector<it> getPixelOnCoord(vector <ft> xy) {
		vector<it> rezult = { it(xy[0]), it(xy[1]) };
		if (isPeriodicBoundary) {
			if (rezult[0] < 0) {
				rezult[0] += xSize;
			}
			else if (rezult[0] >= xSize) {
				rezult[0] -= xSize;
			}
			if (rezult[1] < 0) {
				rezult[1] += ySize;
			} else if (rezult[1] >= ySize) {
				rezult[1] -= ySize;
			}
			return rezult;
		}
		else {
			if (rezult[0] < 0 || rezult[0] >= xSize || rezult[1] < 0 || rezult[1] >= ySize) {
				return vector<it>();
			}
		}
	}

private:
	bool checkMatrix(it i, it j) {
		return (i >= 0 && j >= 0 && i < xSize&& j < ySize);
	}
};

class SlimeMoldSimulation {
public:
	Location location;
	SlimeAgentsSettings settings;
	vector<SlimeAgent> particles;

};

int main()
{

}


/*
At each execution step of the scheduler, every agent attempts to move forward one step in the cur-rent direction.
After every agent has attempted to move, the entire population performs its sensory behavior. If the movement is 
successful (i.e., if the next site is not occupied) the agent moves to the new site and deposits a constant
chemoattractant value. If the movement is not successful, the agent remains in its current position, no
chemoattractant is deposited, and a new orientation is randomly se-lected. Agents are selected from the population
randomly in the motor and sensory stages to avoid the possibility of long term bias by sequential ordering. The
agent both deposits to and senses from the trail map, resulting in an autocrine mode of stimulus/response. Note
that patterning is also possible with a simple passive agent response—where the agent responds to chemoattractant
concentration without affecting the chemoattractant levels. The passive approach to particle RD computing is
described in [25], and a comparison of the active and passive approaches can be found in [26].
*/