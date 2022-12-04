#define _CRT_SECURE_NO_WARNINGS
#include <math.h>
#include "omp.h"
#include <tuple>
#include <vector>
#include <stdio.h>
#include <algorithm>
#include <queue>
#include <string>
#include <random>
#include <iterator>
#include <iostream>
#include <fstream>
#include <chrono>
#pragma once
using namespace std;
typedef long long it;
typedef double ft;

const double PI = acos(-1.0);

class SlimeAgent;
class SlimeMoldSimulation;
class Location;

class SlimeAgent {
public:
	//здесь настройки
	SlimeMoldSimulation* settings;
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

	//время жизни
	it timeToLife;

	SlimeAgent();

	void setUp(it, vector<ft> , vector<ft>, vector<ft>, vector<ft>, vector<ft>, SlimeMoldSimulation*);
	//движение
	void moveTurn();
	//сканирование
	void skanTurn();
	//старение
	bool timeTurn();
	//смерть
	void deadTurn();
	// функция хода
	void makeFullTurn();
private:

	template<typename T>
	vector<T> vSum(vector<T>& x, vector<T>& y);
	template<typename T>
	vector<T> vMult(vector<T>& x, vector<vector<T>>& r);
	// определяет направление поворота
	it activateSensors();
	// поворот векторов скорости и сенсоров
	void rotate(bool isRigth);
	//сдвиг и установка пикселя, если это возможно
	bool move();
	// нанесение следа на карту
	void makeDeposit();
};

class Location {
public:
	//здесь настройки
	SlimeMoldSimulation* settings;

	vector<vector<ft>> trailMap;
	vector<vector<bool>> agentMap;

	Location();

	Location(it x, it y, SlimeMoldSimulation* set);

	vector<it> getSizes();

	void castDecay();

	void castDiffusion();

	bool canMakeMove(vector <it>& xy, vector <it>& oldxy);

	vector<it> getPixelOnCoord(vector <ft>& xy);

	bool checkMatrix(it i, it j);

private:
	it xSize;
	it ySize;
};

class SlimeMoldSimulation {
public:
	Location location;
	vector<SlimeAgent*> particles;

	vector<pair<vector<ft>, it>> generators;

	vector<vector<ft>> leftRotationMatrix;
	vector<vector<ft>> rightRotationMatrix;
	ft depositPerStep;
	ft chanceOfRandomChangeDirection;

	//используемые для создания
	ft sensorAngle;
	ft sensorOffsetDistance;
	it sensorWidth; //пока автоединица, не используется
	ft stepSize;
	it population;
	it startTimeToLife;

	it diffusionSize; // пока не робит
	ft decayFactor; //mult
	bool isPeriodicBoundary;
	bool isCanMultiAgent;

	SlimeMoldSimulation();

	void setLocation(it xSize, it ySize);

	void setUp(it ttl, it sp, ft sod, it sw, ft sa, ft ra, ft ss, ft dps, ft corcd, it dif, ft dec, bool ipb, bool icma);

	void startSimulation(vector<ft> startPosition);

	SlimeAgent* generateAgent(vector<ft> startPosition, ft startAngle);
private:

	vector<SlimeAgent*> generatePopulationInPixel(it count, vector<ft>& startPosition);

	vector<SlimeAgent*> generatePopulationRandomPositions(it count, vector<it> sizes);

	void makeStep();

	void outputInConsoleTrayMap();

	void outputInConsoleAgentMap();

	void outputInFileAgentMap();

	bool updateSettingsFromFile();

	void outputInBmp(bool);
};