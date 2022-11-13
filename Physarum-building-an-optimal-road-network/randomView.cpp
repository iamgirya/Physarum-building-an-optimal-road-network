#include <math.h>
#include <tuple>
#include <vector>
#include <stdio.h>
#include <algorithm>
#include <queue>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;
typedef long long ll;
typedef long long intType;
typedef double floatType;

const double PI = acos(-1.0);

class SlimeAgent {
public:
	//вектор нынешней позиции
	vector<floatType> positionVector;

	//вектор, куда должно быть сделано следующее движение
	vector<floatType> moveVector;

	//вектор, где находится сенсоры
	vector<floatType> leftSensorVector;
	vector<floatType> centerSensorVector;
	vector<floatType> rigthSensorVector;

	// нынеший занимаемый пиксель
	vector<intType> pixelVector;

	// функция хода
	void makeTurn(SlimeAgentsSettings& set, Location& loc) {
		intType weightSe
	}
private:
	// определяет направление поворота
	intType activateSensors(SlimeAgentsSettings& set, Location& loc) {

	}
	//сдвиг и установка пикселя
	void move(SlimeAgentsSettings& set, Location& loc) {

	}
	// поворот векторов скорости и сенсоров
	void rotate(SlimeAgentsSettings& set, bool isRigth) {

	}
	// нанесение следа на карту
	void makeDeposit(SlimeAgentsSettings& set, Location& loc) {

	}
};

class SlimeAgentsSettings {
public:
	//floatType rotateAngle;
	vector<vector<floatType>> leftRotationMatrix;
	vector<vector<floatType>> rightRotationMatrix;

	//floatType sensorAngle;
	vector<vector<floatType>> leftSensorPositionMatrix;
	vector<vector<floatType>> rigthSensorPositionMatrix;

	floatType sensorOffsetDistance;
	intType sensorWidth; //пока автоединица
	floatType stepSize;
	floatType depositPerStep;
	floatType chanceOfRandomChangeDirection;

	void setUp(floatType sod, intType sw, floatType sa, floatType ra, floatType ss, floatType dps, floatType corcd = 0) {
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
	}
};

class Location {
public:
	intType xSize;
	intType ySize;
	vector<vector<intType>> trailMap;

	floatType startPopulationPerArea;
	intType diffusionSize;
	floatType decayFactor; //mult

	bool isPeriodicBoundary;
	bool isCanMultiAgent;

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
