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

	SlimeAgent();

	void setUp(vector<ft> , vector<ft>, vector<ft>, vector<ft>, vector<ft>, SlimeMoldSimulation*);
	//��������
	void moveTurn();
	//������������
	void skanTurn();
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

private:
	it xSize;
	it ySize;

	bool checkMatrix(it i, it j);
};

class SlimeMoldSimulation {
public:
	Location location;
	vector<SlimeAgent*> particles;

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

	it diffusionSize; // ���� �� �����
	ft decayFactor; //mult
	bool isPeriodicBoundary;
	bool isCanMultiAgent;

	SlimeMoldSimulation();

	void setLocation(it xSize, it ySize);

	void setUp(it sp, ft sod, it sw, ft sa, ft ra, ft ss, ft dps, ft corcd, it dif, ft dec, bool ipb, bool icma);

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
};