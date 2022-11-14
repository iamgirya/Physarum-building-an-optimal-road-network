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

	//вектор, куда должно быть сделано следующее движение
	vector<ft> moveVector;

	//вектор, где находится сенсоры
	vector<ft> leftSensorVector;
	vector<ft> centerSensorVector;
	vector<ft> rigthSensorVector;

	// нынеший занимаемый пиксель
	vector<it> pixelVector;

	// функция хода
	void makeTurn(SlimeAgentsSettings& set, Location& loc) {
		if (set.chanceOfRandomChangeDirection > (rand() % 100 + 1)) {
			rotate(set, rand()%2);
		}
		else {
			// -1 - поворот налево, 0 - сохранение направления, 1 - поворот направо
			it rotateSide = activateSensors(loc);
			if (rotateSide != 0) {
				rotate(set, rotateSide == 1);
			}
		}

		// если движение успешно
		if (move(loc)) {
			makeDeposit(set, loc);
		}
		else {
			rotate(set, rand() % 2);
		}
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
	//floatType rotateAngle;
	vector<vector<ft>> leftRotationMatrix;
	vector<vector<ft>> rightRotationMatrix;

	//floatType sensorAngle;
	vector<vector<ft>> leftSensorPositionMatrix;
	vector<vector<ft>> rigthSensorPositionMatrix;

	ft sensorOffsetDistance;
	it sensorWidth; //пока автоединица
	ft stepSize;

	ft depositPerStep;
	ft chanceOfRandomChangeDirection;

	void setUp(ft sod, it sw, ft sa, ft ra, ft ss, ft dps, ft corcd = 0) {
		//матрицы нужные для поворота мув вектора
		ra = ra * PI/180;
		rightRotationMatrix = { {cos(ra), -sin(ra)}, { sin(ra), cos(ra)} };
		leftRotationMatrix = { {cos(-ra), -sin(-ra)}, { sin(-ra), cos(-ra)} };

		//матрицы для получения сенсоров из вуз вектора
		sa = sa * PI / 180;
		rigthSensorPositionMatrix = { {cos(sa), -sin(sa)}, { sin(sa), cos(sa)} };
		leftSensorPositionMatrix = { {cos(-sa), -sin(-sa)}, { sin(-sa), cos(-sa)} };

		//характеристики
		sensorOffsetDistance = sod;
		sensorWidth = 1;
		stepSize = ss;
		depositPerStep = dps;
		chanceOfRandomChangeDirection = corcd;

		srand(it(time(NULL)));
	}
};

class Location {
public:
	it xSize;
	it ySize;
	vector<vector<it>> trailMap;

	ft startPopulationPerArea;
	it diffusionSize;
	ft decayFactor; //mult

	bool isPeriodicBoundary;
	bool isCanMultiAgent;
	vector<vector<bool>> agentMap;

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
