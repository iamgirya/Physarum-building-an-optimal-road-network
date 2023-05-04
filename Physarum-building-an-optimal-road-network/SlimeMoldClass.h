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
	//����� ���������
	SlimeMoldSimulation* settings;
	//������ �������� �������
	vector<ft> positionVector;

	// ������� ���������� �������
	vector<it> pixelVector;

	//������, ���� ������ ���� ������� ��������� ��������
	vector<ft> moveVector;

	//������, ��� ��������� �������
	vector<ft> leftSensorVector;
	vector<ft> centerSensorVector;
	vector<ft> rigthSensorVector;

	//����� �����
	it timeToLife;
	//�� ������ ���������� �����
	it teamIndex;
	//������ ����������� ��� ���������
	pair<ft ,vector<ft>> startAngle;
	// ������ � ����������� ��������
	vector<vector<it>> pathVector;

	SlimeAgent();

	void setUp(ft, it, it, vector<ft> , vector<ft>, vector<ft>, vector<ft>, vector<ft>, SlimeMoldSimulation*);
	//��������
	void moveTurn();
	//������������
	void skanTurn();
	//�������� � ������
	short int isTimeToDeath();
	// ������� ����
	void makeFullTurn();
private:
	const ft addOnGenerator = 5;
	const ft addOnDeath = -1.8;

	template<typename T>
	vector<T> vSum(vector<T>& x, vector<T>& y);
	template<typename T>
	vector<T> vMult(vector<T>& x, vector<vector<T>>& r);
	// ���������� ����������� ��������
	it activateSensors();
	// ������� �������� �������� � ��������
	void rotate(bool isRigth);
	//����� � ��������� �������, ���� ��� ��������
	bool move();
	// ��������� ����� �� �����
	void makeDeposit();
};

class SlimeAgentFactory {
public:
	it startTimeToLife;
	ft sensorOffsetDistance;
	ft sensorAngle;
	ft stepSize;

	//��������
	SlimeMoldSimulation* settings;

	SlimeAgentFactory();

	SlimeAgent* generateAgent(vector<ft> startPosition, ft startAngle, it teamIndex);

	vector<SlimeAgent*> generatePopulationInPixel(it count, vector<ft>& startPosition, it teamIndex);

	vector<SlimeAgent*> generatePopulationRandomPositions(it count, vector<it> sizes);
};

class Generator {
public:
	vector<ft> position;
	queue<ft> generatorsQueue;
	it agentsPerStep;
	it teamIndex;
	SlimeAgentFactory* factory;

	Generator();

	Generator(vector<ft> position, it agentsPerStep, it teamIndex, SlimeAgentFactory* factory);

	vector<SlimeAgent*> generate();

	bool isNear(vector<it>& position, it team);

	void push(ft angle);
private:
	const it rangeOfGenerator = 2;
	const ft someAngle = 10;
};

class Location {
public:
	ft decayFactor; //mult
	bool isPeriodicBoundary;
	bool isCanMultiAgent;

	vector<vector<ft>> trailMap;
	vector<vector<short int>> agentMap;
	vector<vector<short int>> blockMap;

	vector<Generator*> generators;
	vector<pair<pair<it, it>, pair<it, it>>> blockRectangles;

	Location();

	Location(it x, it y);

	vector<it> getSizes();

	void castDecay();

	void castDiffusion();

	void onAgentsDeath(vector<vector<it>> points, ft diff, it indexOfGenerator, ft startAngle);

	bool canMakeMove(vector <it>& xy, vector <it>& oldxy);

	vector<it> getPixelOnCoord(vector <ft>& xy);

	bool checkMatrix(it i, it j);

	int checkNearGenerator(vector<it>& position, it teamIndex);

private:
	it xSize;
	it ySize;
};

class SlimeMoldSimulation {
public:
	Location location;
	vector<SlimeAgent*> particles;
	SlimeAgentFactory factory;

	it population;
	vector<vector<ft>> leftRotationMatrix;
	vector<vector<ft>> rightRotationMatrix;
	ft depositPerStep;

	SlimeMoldSimulation();

	void setLocation(it xSize, it ySize);

	void setUp(it ttl, it sp, ft sod, ft sa, ft ra, ft ss, ft dps, ft dec, bool ipb, bool icma);

	void startSimulation(vector<ft> startPosition);

private:

	void makeStep();

	void outputInConsoleTrayMap();

	void outputInConsoleAgentMap();

	void outputInFileAgentMap();

	bool updateSettingsFromFile();

	void outputInBmp(bool);
};