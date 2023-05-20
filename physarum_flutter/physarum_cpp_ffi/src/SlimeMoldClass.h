#if _WIN32
#define FFI_PLUGIN_EXPORT __declspec(dllexport)
#else
#define FFI_PLUGIN_EXPORT
#endif

#define _CRT_SECURE_NO_WARNINGS
#include <math.h>
#include "omp.h"
#include <tuple>
#include <vector>
#include <stdio.h>
#include <algorithm>
#include <set>
#include <map>
#include <queue>
#include <string>
#include <random>
#include <iterator>
#include <iostream>
#include <fstream>
#include <chrono>
#include<functional>
#pragma once
using namespace std;
typedef long long it;
typedef double ft;
const double PI = acos(-1.0);

extern "C" {

    class SlimeAgentFactory;
    class Generator;
    class SlimeAgent;
    class SlimeMoldSimulation;
    class Location;
    class AgentGraphAnalyser;

    class SlimeAgent {
    public:
        Location* location;
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
        //из какого генератора рождён
        it teamIndex;
        //Вектор направления при появлении
        pair<ft ,vector<ft>> startAngle;
        // вектор с пройденными клетками
        vector<vector<it>> pathVector;

        //единые для всех параметры
        static vector<vector<ft>> leftRotationMatrix;
        static vector<vector<ft>> rightRotationMatrix;
        static ft depositPerStep;

        void setUp(ft, it, it, vector<ft> , vector<ft>, vector<ft>, vector<ft>, vector<ft>, Location*);
        //движение
        void moveTurn();
        //сканирование
        void skanTurn();
        //старение и смерть
        short int isTimeToDeath();
        // функция хода
        void makeFullTurn();
    private:
        const ft addOnGenerator = 5;
        const ft addOnDeath = -1.8;

        vector<ft> vSum(vector<ft>& x, vector<ft>& y);

        vector<ft> vMult(vector<ft>& x, vector<vector<ft>>& r);
        // определяет направление поворота
        it activateSensors();
        // поворот векторов скорости и сенсоров
        void rotate(bool isRigth);
        //сдвиг и установка пикселя, если это возможно
        bool move();
        // нанесение следа на карту
        void makeDeposit();
    };

    class SlimeAgentFactory {
    public:
        it startTimeToLife;
        ft sensorOffsetDistance;
        ft sensorAngle;
        ft stepSize;

        Location* location;

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

    //вторая часть
    // TODO потом все методы и приколы нужно нормально расположить и связять друг за другом
    class AgentGraphAnalyser {
    public:
        ft edgesRange;
        ft vertexRange;
        it minVertexMass;
        ft minEdgeAngle;
        // списки смежности
        vector<vector<it>> graph;
        // множество точек
        vector<pair<it, it>> exitPoints;
        // приоритетность точек
        vector<it> towns;
        // индексы точек с не нулевым приоритетом
        vector<it> townIndexes;

        // лучший, согласно метрикам, граф
        vector<vector<it>> bestGraph;
        vector<pair<it, it>> bestExitPoints;
        vector<it> bestTowns;
        ft metricsSum;
        ft metricWeigth;
        ft metricOverDistance;
        ft metricResistance;
        ft metricDeltaFlow;

        // перезапустить
        void clear();

        //преобразует множество точек в нормальный граф
        void makeGraph(vector<SlimeAgent*> particles, vector<Generator*> generators);

        // Решает 4 проблемы артефактов:
        // одинокие - вершины с 1 ребром или без них;
        // ломаные - линия, разбитая на несколько ребёр, что нужно соединить в одно;
        // треугольники - вершина, что соединена только с двумя вершинами, что уже соединены;
        // ромбы - четыре вершины, которые вместе образуют четырёх угольник.
        void minimizeGraph();

        // Вычисляет значение во сколько раз длина дорог превышает минимально возможную
        ft calculateWeigth();

        // вычисляет сумму произведений веса пути и отношения кратчайшего пути к дистанции между вершинами. Вес пути - это то, сколько приоритета по нему передаётся 
        ft calculateOverDistance();

        // находит частное от деления суммы приоритета неусточивых городов на весь приоритет. Город неустойчий, если существует мост, при удалении которого город отделится от большего по приоритету компоненты связности
        // TODO здесь закоменчено нахождение наибольшего цикла. Надо бы как-то использовать в метрике, ибо он тоже роляет
        ft calculateResistance();

        // т.к. нам интересует только то, чобы сеть _могла_ работать ещё долгое время без дополнительных путей, то среди потока нас интересует лишь
        // одно число - максимальный поток на одном из рёбер. Ибо в случае роста потоков, скорее всего, это ребро будет главной проблемой
        // Так что мы вычисляем максимум потока для ребра полного графа и этим числом делим максимум потока нынешнего графа.
        ft calculateDeltaFlow();

        // вычисляет все метрики и сохраняет лучший граф
        void calculateMetrics();

        // проверить связность
        bool checkConnected();
    private:
        // минимальная величина вектора разницы, чтобы считать, что два ребрра можно сложить в одно
        ft minRezultVectorLength = -1;
        // геттер суммы приоритетов каждого города
        ft sumOfPriority(); ft _sumOfPriority = -1;
        // матрица весов
        vector<vector<ft>> weigthGraph;
        // матрица величины потока
        vector<vector<ft>> flowGraph;
        // матрица кратчайших путей между городами
        vector<vector<vector<it>>> waysGraph;
        

        function<double(pair<it, it>, pair<it, it>)> distance = [](pair<it, it> i, pair<it, it> j) { return sqrt(pow(i.first - j.first, 2) + pow(i.second - j.second, 2)); };
        function<double(pair<ft, ft>)> length = [](pair<ft, ft> p) { return sqrt(pow(p.first, 2) + pow(p.second, 2)); };
        function<pair<it, it>(pair<it, it>, pair<it, it>)> average = [](pair<it, it> i, pair<it, it> j) {return make_pair((i.first + j.first) / 2, (i.second + j.second) / 2); };

        // удалить ребро
        void eraseEdge(int index, int vertex);

        // проверить ребро
        bool findEdge(int index, int vertex);

        // проверяет вершину с двумя рёбрами, можно ли её убрать и соединить две смежные вершины одним ребром
        bool canConnectEdges(int i);

        // проверка на четырёхугольник. То есть есть ли у данной вершины две смежные вершины, которые имеют общую смежную вершину, не равную данной.
        vector<it> checkRect(int index);

        // строит граф кратчайших путей для каждого из townIndexes. Для каждого i и j хранится кратчайший между ними путь. 
        // Для самой себя или до недостижимой вершины он хранит пустой путь
        void buildWays();

        // Для каждой пары городов находим кратчайший маршрут между ними
        // Для каждого маршрута проходимся по нему и добавляем поток между данными городами каждому из рёбер в вес
        void buildFlow();

        // Создаём матрицу с весом рёбер, где вес - расстояние между вершинами
        void buildWeigth();

    };

    class SlimeMoldSimulation {
    public:
        it population;
        vector<SlimeAgent*> particles;
        Location location;
        SlimeAgentFactory factory;
        AgentGraphAnalyser analyser;

        SlimeMoldSimulation();

        SlimeMoldSimulation(it xSize, it ySize);

        void setUp(it timeToLive, it startPopulation, ft sensorOffsetDistance, ft sensorAngle, ft rotateAngle, ft stepSize, ft depositPerStep, ft decayFactor, bool isPeriodicBoundary, bool isCanMultiAgent, ft edgesRange, ft vertexRange, it minVertexMass, ft minEdgeAngle);

        void placeGenerators(vector<pair<it, it>>, vector<it>);

        void startSimulation(it);

        void outputInBmpGraph(vector<pair<it, it>>& points, vector<vector<it>>& graph, bool isChangedSettings, int number);

    private:

        void makeStep();

        void outputInConsoleTrayMap();

        void outputInConsoleAgentMap();

        void outputInFileAgentMap();

        bool updateSettingsFromFile(bool);

        void outputInBmp(bool);

    };

    //Flutter Adapter part

    // массив
    typedef struct {
        int *data;
        int length;
    } IntArray;

    typedef struct {
        double *data;
        int length;
    } DoubleArray;

    //массив массивов
    typedef struct {
        IntArray *data;
        int length;
    } IntArrayArray;

    // структура выходного графа
    typedef struct {
        IntArrayArray *graph;
        IntArray *exitPointsX;
        IntArray *exitPointsY;
        IntArray *towns;
        int length;
    } SlimeMoldNetwork;

    void restartSimulation();

    SlimeMoldNetwork *parseSimulationToNetwork(vector<vector<it>>& graph, vector<it>& towns, vector<pair<it,it>>& exitPoints);

    FFI_PLUGIN_EXPORT void execute(int stepCount);

    FFI_PLUGIN_EXPORT SlimeMoldNetwork *getGraph(bool isNeedBest);

    FFI_PLUGIN_EXPORT DoubleArray *getBestMetrics();

    FFI_PLUGIN_EXPORT void setUpSimulation(it xSize, it ySize, it timeToLive, it startPopulation, ft sensorOffsetDistance, ft sensorAngle, ft rotateAngle, ft stepSize, ft depositPerStep, ft decayFactor, bool isPeriodicBoundary, bool isCanMultiAgent, ft edgesRange, ft vertexRange, it minVertexMass, ft minEdgeAngle);

    FFI_PLUGIN_EXPORT void setUpTowns(IntArray* x, IntArray* y, IntArray* towns);

}