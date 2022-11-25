#include <math.h>
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
class SlimeAgentsSettings;
class Location;

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

	void setUp(vector<ft> pv, vector<ft> mv, vector<ft> lsv, vector<ft> csv, vector<ft> rsv);
	//движение
	void moveTurn(SlimeAgentsSettings& set, Location& loc);
	//сканирование
	void skanTurn(SlimeAgentsSettings& set, Location& loc);
	// функция хода
	void makeFullTurn(SlimeAgentsSettings& set, Location& loc);
private:

	template<typename T>
	vector<T> vSum(vector<T>& x, vector<T>& y);
	template<typename T>
	vector<T> vMult(vector<T>& x, vector<vector<T>>& r);
	// определяет направление поворота
	it activateSensors(Location& loc);
	// поворот векторов скорости и сенсоров
	void rotate(SlimeAgentsSettings& set, bool isRigth);
	//сдвиг и установка пикселя, если это возможно
	bool move(Location& loc);
	// нанесение следа на карту
	void makeDeposit(SlimeAgentsSettings& set, Location& loc);
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

	SlimeAgentsSettings();

	SlimeAgentsSettings(it sp, ft sod, it sw, ft sa, ft ra, ft ss, ft dps, ft corcd = 0);

	void setUp(it, ft, it, ft, ft, ft, ft, ft );

	SlimeAgent* generateAgent(vector<ft> startPosition, ft startAngle);

	vector<SlimeAgent*> generatePopulationInPixel(vector<ft>& startPosition);

	vector<SlimeAgent*> generatePopulationRandomPositions(vector<it> sizes);
};

class Location {
public:
	vector<vector<ft>> trailMap;

	Location();

	Location(it x, it y, it dif, ft dec, bool ipb, bool icma);

	vector<it> getSizes();

	void castDecay();

	void castDiffusion();

	bool canMakeMove(vector <it>& xy, vector <it>& oldxy);

	vector<it> getPixelOnCoord(vector <ft>& xy);

private:
	it xSize;
	it ySize;
	it diffusionSize; // пока не робит
	ft decayFactor; //mult

	bool isPeriodicBoundary;
	bool isCanMultiAgent;
	vector<vector<bool>> agentMap;

	bool checkMatrix(it i, it j);
};

class SlimeMoldSimulation {
public:
	Location location;
	SlimeAgentsSettings settings;
	vector<SlimeAgent*> particles;

	SlimeMoldSimulation(Location& l, SlimeAgentsSettings& s);

	void startSimulation(vector<ft> startPosition);
private:

	void makeStep();

	void outputInConsoleTrayMap();

	void outputInConsoleAgentMap();

	void outputInFileAgentMap();
};