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

	SlimeAgent();

	void setUp(it, vector<ft> , vector<ft>, vector<ft>, vector<ft>, vector<ft>, SlimeMoldSimulation*);
	//��������
	void moveTurn();
	//������������
	void skanTurn();
	//��������
	bool timeTurn();
	//������
	void deadTurn();
	// ������� ����
	void makeFullTurn();
private:

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

class Location {
public:
	//����� ���������
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

	//������������ ��� ��������
	ft sensorAngle;
	ft sensorOffsetDistance;
	it sensorWidth; //���� �����������, �� ������������
	ft stepSize;
	it population;
	it startTimeToLife;

	it diffusionSize; // ���� �� �����
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