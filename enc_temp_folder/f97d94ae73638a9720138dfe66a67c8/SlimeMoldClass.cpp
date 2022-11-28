#include <SlimeMoldClass.h>

Location::Location() {}

Location::Location(it x, it y, SlimeMoldSimulation* set) {
	xSize = x;
	ySize = y;
	settings = set;

	trailMap = vector<vector<ft>>(xSize, vector<ft>(ySize, 0.0));
	agentMap = vector<vector<bool>>(xSize, vector<bool>(ySize, false));
	
}

void Location::castDecay() {
	for (int i = 0; i < xSize; i++) {
		for (int j = 0; j < ySize; j++) {
			trailMap[i][j] *= settings->decayFactor;
		}
	}
}

void Location::castDiffusion() {
	vector<vector<ft>> newMap = vector<vector<ft>>(xSize, vector<ft>(ySize, 0.0));
	for (it i = 0; i < xSize; i++) {
		for (it j = 0; j < ySize; j++) {
			if (trailMap[i][j]) {
				for (it k = -1; k <= 1; k++) {
					for (it l = -1; l <= 1; l++) {
						if (checkMatrix(i + k, j + l)) {
							newMap[i + k][j + l] += 0.1 * trailMap[i][j];
						}
					}
				}
				newMap[i][j] += 0.1 * trailMap[i][j];
			}
		}
	}
	trailMap = newMap;
}

bool Location::canMakeMove(vector <it>& xy, vector <it>& oldxy) {
	if (!(settings->isCanMultiAgent)) {
		if (!agentMap[xy[0]][xy[1]]) {
			agentMap[oldxy[0]][oldxy[1]] = false;
			agentMap[xy[0]][xy[1]] = true;
			return true;
		}
		return false;
	}

	return true;
}

vector<it> Location::getPixelOnCoord(vector <ft>& xy) {
	vector<it> rezult = { it(xy[0]), it(xy[1]) };
	if (settings->isPeriodicBoundary) {
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
	return (i >= 0 && j >= 0 && i < xSize&& j < ySize);
}

vector<it> Location::getSizes() {
	return { xSize, ySize };
}


SlimeAgent::SlimeAgent() {}

void SlimeAgent::setUp(vector<ft> pv, vector<ft> mv, vector<ft> lsv, vector<ft> csv, vector<ft> rsv, SlimeMoldSimulation* set) {
	positionVector = pv;
	moveVector = mv;
	leftSensorVector = lsv;
	centerSensorVector = csv;
	rigthSensorVector = rsv;
	pixelVector = { it(pv[0]), it(pv[1]) };
	settings = set;
}
//��������
void SlimeAgent::moveTurn() {
	// ���� �������� �������
	if (move()) {
		makeDeposit();
	}
	else {
		rotate(rand() % 2);
	}
}
//������������
void SlimeAgent::skanTurn() {
	//���� ���������� ��������
	if (settings->chanceOfRandomChangeDirection > (rand() % 100 + 1)) {
		rotate(rand() % 2);
	}
	else {
		// -1 - ������� ������, 0 - ���������� �����������, 1 - ������� �������
		it rotateSide = activateSensors();
		if (rotateSide != 0) {
			rotate(rotateSide == 1);
		}
	}
}
// ������� ����
void SlimeAgent::makeFullTurn() {
	moveTurn();
	skanTurn();
}

template<typename T>
vector<T> SlimeAgent::vSum(vector<T>& x, vector<T>& y) {
	vector<T> rezult(x.size());
	for (int i = 0; i < x.size(); i++) {
		rezult[i] = x[i] + y[i];
	}
	return rezult;
}

template<typename T>
vector<T> SlimeAgent::vMult(vector<T>& x, vector<vector<T>>& r) {
	vector<T> rezult(x.size(), 0);
	for (int i = 0; i < x.size(); i++) {
		for (int j = 0; j < x.size(); j++) {
			rezult[i] += x[j] * r[j][i];
		}
	}
	return rezult;
}
// ���������� ����������� ��������
it SlimeAgent::activateSensors() {
	it lw, cw, rw;
	vector<it> pixelSensorPosition;
	vector<ft> tmp;

	tmp = vSum(positionVector, leftSensorVector);
	pixelSensorPosition = settings->location.getPixelOnCoord(tmp);
	if (!pixelSensorPosition.empty()) {
		lw = settings->location.trailMap[pixelSensorPosition[0]][pixelSensorPosition[1]];
	}
	else {
		lw = 0;
	}

	tmp = vSum(positionVector, centerSensorVector);
	pixelSensorPosition = settings->location.getPixelOnCoord(tmp);
	if (!pixelSensorPosition.empty()) {
		cw = settings->location.trailMap[pixelSensorPosition[0]][pixelSensorPosition[1]];
	}
	else {
		cw = 0;
	}

	tmp = vSum(positionVector, rigthSensorVector);
	pixelSensorPosition = settings->location.getPixelOnCoord(tmp);
	if (!pixelSensorPosition.empty()) {
		rw = settings->location.trailMap[pixelSensorPosition[0]][pixelSensorPosition[1]];
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
	}
	else if (cw == lw && cw == rw) {
		return (rand() % 3) - 1;
	}
}
// ������� �������� �������� � ��������
void SlimeAgent::rotate(bool isRigth) {
	if (isRigth) {
		moveVector = vMult(moveVector, settings->rightRotationMatrix);
		leftSensorVector = vMult(leftSensorVector, settings->rightRotationMatrix);
		centerSensorVector = vMult(centerSensorVector, settings->rightRotationMatrix);
		rigthSensorVector = vMult(rigthSensorVector, settings->rightRotationMatrix);
	}
	else {
		moveVector = vMult(moveVector, settings->leftRotationMatrix);
		leftSensorVector = vMult(leftSensorVector, settings->leftRotationMatrix);
		centerSensorVector = vMult(centerSensorVector, settings->leftRotationMatrix);
		rigthSensorVector = vMult(rigthSensorVector, settings->leftRotationMatrix);
	}
}
//����� � ��������� �������, ���� ��� ��������
bool SlimeAgent::move() {
	vector<ft> newPosition = vSum(positionVector, moveVector);
	vector<it> newPixel = settings->location.getPixelOnCoord(newPosition);
	if (newPixel.empty() || !settings->location.canMakeMove(newPixel, pixelVector)) {
		return false;
	}
	else {
		positionVector = newPosition;
		pixelVector = newPixel;
		return true;
	}
}
// ��������� ����� �� �����
void SlimeAgent::makeDeposit() {
	settings->location.trailMap[pixelVector[0]][pixelVector[1]] += settings->depositPerStep;
}



SlimeMoldSimulation::SlimeMoldSimulation() {
	srand(it(time(NULL)));

	population = 0;

	//������� ������ ��� �������� ��� ������� � ��������
	rightRotationMatrix = { {cos(0), -sin(0)}, { sin(0), cos(0)} };
	leftRotationMatrix = { {cos(-0), -sin(-0)}, { sin(-0), cos(-0)} };

	// ��������� �������
	sensorOffsetDistance = 0;
	sensorWidth = 1;
	sensorAngle = 0;
	stepSize = 0;
	depositPerStep = 0;
	chanceOfRandomChangeDirection = 0;

	// ��������� �������
	diffusionSize = 0;
	decayFactor = 0;
	isPeriodicBoundary = 0;
	isCanMultiAgent = 0;
	location = Location(0, 0, this);
}

void SlimeMoldSimulation::setLocation(it xSize, it ySize) {
	location = Location(xSize, ySize, this);
}

void SlimeMoldSimulation::startSimulation(vector<ft> startPosition) {
	if (updateSettingsFromFile()) {
		it count = 0;
		std::random_device rd;
		std::mt19937 g(rd());

		while (true) {
			// ���-�� � rotate �����������
			makeStep();
			shuffle(particles.begin(), particles.end(), g);
			if (count % 30 == 0) {
				outputInFileAgentMap();
				updateSettingsFromFile();
			}

			count++;
		}
	}
}

void SlimeMoldSimulation::makeStep() {
	for (int i = 0; i < particles.size(); i++) {
		particles[i]->moveTurn();
	}
	for (int i = 0; i < particles.size(); i++) {
		particles[i]->skanTurn();
	}

	location.castDiffusion();
	location.castDecay();
}

void SlimeMoldSimulation::outputInConsoleTrayMap() {
	for (int i = 0; i < location.trailMap.size(); i++) {
		for (int j = 0; j < location.trailMap[0].size(); j++) {
			cout << " "[location.trailMap[i][j] < 10] << it(location.trailMap[i][j]) << ' ';
		}
		cout << endl;
	}
}

void SlimeMoldSimulation::outputInConsoleAgentMap() {
	vector<vector<bool>> tmp = vector<vector<bool>>(location.trailMap.size(), vector<bool>(location.trailMap[0].size(), false));
	for (int i = 0; i < particles.size(); i++) {
		tmp[particles[i]->pixelVector[0]][particles[i]->pixelVector[1]] = true;
	}

	for (int i = 0; i < location.trailMap.size(); i++) {
		for (int j = 0; j < location.trailMap[0].size(); j++) {
			if (tmp[i][j]) {
				cout << "1" << " ";
			}
			else {
				cout << "." << " ";
			}

		}
		cout << endl;
	}

	cout << endl;
}

void SlimeMoldSimulation::outputInFileAgentMap() {
	static int ii = 0;
	ii++;
	ii %= 1000;
	ofstream output("out" + to_string(ii)+ ".txt");

	vector<vector<bool>> tmp = vector<vector<bool>>(location.trailMap.size(), vector<bool>(location.trailMap[0].size(), false));
	for (int i = 0; i < particles.size(); i++) {
		tmp[particles[i]->pixelVector[0]][particles[i]->pixelVector[1]] = true;
	}

	for (int i = 0; i < location.trailMap.size(); i++) {
		for (int j = 0; j < location.trailMap[0].size(); j++) {
			if (tmp[i][j]) {
				output << "1" << " ";
			}
			else {
				output << "." << " ";
			}

		}
		output << endl;
	}

	output.close();
}

SlimeAgent* SlimeMoldSimulation::generateAgent(vector<ft> startPosition, ft startAngle) {
	startAngle = startAngle / 180 * PI;
	vector<ft> moveVector = { stepSize * cos(startAngle), stepSize * sin(startAngle) };
	vector<ft> lsVector = { sensorOffsetDistance * cos(startAngle + sensorAngle), sensorOffsetDistance * sin(startAngle + sensorAngle) };
	vector<ft> csVector = { sensorOffsetDistance * cos(startAngle), sensorOffsetDistance * sin(startAngle) };
	vector<ft> rsVector = { sensorOffsetDistance * cos(startAngle - sensorAngle), sensorOffsetDistance * sin(startAngle - sensorAngle) };
	SlimeAgent* tmp = new SlimeAgent();
	tmp->setUp(startPosition, moveVector, lsVector, csVector, rsVector, this);
	return tmp;
}

vector<SlimeAgent*> SlimeMoldSimulation::generatePopulationInPixel(it count, vector<ft>& startPosition) {
	vector<SlimeAgent*> rezult;

	for (int i = 0; i < population; i++) {
		rezult.push_back(generateAgent(startPosition, (ft(rand() % 360))));
	}
	return rezult;
}

vector<SlimeAgent*> SlimeMoldSimulation::generatePopulationRandomPositions(it count, vector<it> sizes) {
	vector<SlimeAgent*> rezult;

	for (int i = 0; i < count; i++) {
		rezult.push_back(generateAgent({ ft(rand() % sizes[0]) , ft(rand() % sizes[1]) }, (ft(rand() % 360))));
	}
	return rezult;
}

void SlimeMoldSimulation::setUp(it sp, ft sod, it sw, ft sa, ft ra, ft ss, ft dps, ft corcd, it dif, ft dec, bool ipb, bool icma) {
	population = sp;
	
	ra = it(ra) % 360;
	ra = ra * PI / 180;

	sa = it(sa) % 360;
	sa = sa * PI / 180;
	//������� ������ ��� �������� ��� ������� � ��������
	rightRotationMatrix = { {cos(ra), -sin(ra)}, { sin(ra), cos(ra)} };
	leftRotationMatrix = { {cos(-ra), -sin(-ra)}, { sin(-ra), cos(-ra)} };
	
	// ��������� �������
	sensorOffsetDistance = sod;
	sensorWidth = 1;
	sensorAngle = sa;
	stepSize = ss;
	depositPerStep = dps;
	chanceOfRandomChangeDirection = corcd;

	// ��������� �������
	diffusionSize = dif;
	decayFactor = dec;
	isPeriodicBoundary = ipb;
	isCanMultiAgent = icma;
}

bool SlimeMoldSimulation::updateSettingsFromFile() {
	ifstream input("settings.txt");

	string isNeedToUpdate;
	ft sod; it sw; ft sa; ft ra; ft ss; ft dps; ft corcd; it dif; ft dec; bool ipb; bool icma;
	it newPopulation;
	try {
		input >> isNeedToUpdate;
		if (isNeedToUpdate == "yes") {
			input >> sod >> sw >> sa >> ra >> ss >> dps >> corcd >> dif >> dec >> ipb >> icma;
			input >> newPopulation;
			bool isNeedToUpdateAgents = population != 0 && (sensorOffsetDistance != sod || sensorAngle != sa || stepSize != ss);
			bool isNeedToResizePopulation = population != newPopulation;

			setUp(newPopulation, sod , 1 , sa , ra , ss , dps , corcd , dif , dec , ipb , icma);

			if (isNeedToResizePopulation) {
				if (particles.size() < newPopulation) {
					auto newPart = generatePopulationRandomPositions(newPopulation - particles.size(), location.getSizes());
					for (auto u : newPart) {
						particles.push_back(u);
					}
				}
				else {
					particles.resize(newPopulation);
				}
			}

			if (isNeedToUpdateAgents) {
				for (it i = 0; i < population; i++) {
					ft partAngle = atan2(particles[i]->positionVector[0], particles[i]->positionVector[1]) / PI * 180;
					particles[i] = generateAgent(particles[i]->positionVector, partAngle);
				}
			}

			input.close();

			ofstream output("settings.txt");
			output << "no" << endl;
			output << sod << ' ' << sw << ' ' << sa << ' ' << ra << ' ' << ss << ' ' << dps << ' ' << corcd << ' ' << dif << ' ' << dec << ' ' << ipb << ' ' << icma << endl;
			output << newPopulation;
			output.close();
			return true;
		}
		else {
			input.close();
			return false;
		}
	}
	catch (exception) {
		input.close();
		return false;
	}
}
