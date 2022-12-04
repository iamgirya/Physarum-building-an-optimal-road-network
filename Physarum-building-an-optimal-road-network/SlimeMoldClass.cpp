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
	it i, j;
#pragma omp parallel for private(j)
	for (i = 0; i < xSize; i++) {
		for (j = 0; j < ySize; j++) {
			trailMap[i][j] *= settings->decayFactor;
		}
	}
}

void Location::castDiffusion() {
	vector<vector<ft>> newMap = vector<vector<ft>>(xSize, vector<ft>(ySize, 0.0));
	it i, j, k, l;
#pragma omp parallel for private(j, k, l)
	for (i = 0; i < xSize; i++) {
		for (j = 0; j < ySize; j++) {
			if (trailMap[i][j]) {
				for (k = -1; k <= 1; k++) {
					for (l = -1; l <= 1; l++) {
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

void SlimeAgent::setUp(it ti, it ttl, vector<ft> pv, vector<ft> mv, vector<ft> lsv, vector<ft> csv, vector<ft> rsv, SlimeMoldSimulation* set) {
	positionVector = pv;
	moveVector = mv;
	leftSensorVector = lsv;
	centerSensorVector = csv;
	rigthSensorVector = rsv;
	pixelVector = { it(pv[0]), it(pv[1]) };
	settings = set;
	timeToLife = ttl;
	teamIndex = ti;
}
//движение и старение
void SlimeAgent::moveTurn() {
	// если движение успешно
	if (move()) {
		makeDeposit();
	}
	else {
		rotate(rand() % 2);
	}
}
//старение
bool SlimeAgent::timeTurn() {
	
	for (it i = 0; i < settings->generators.size(); i++) {
		if (abs(pixelVector[0] - it(settings->generators[i].first[0])) <= 1 && abs(pixelVector[1] - it(settings->generators[i].first[1])) <= 1 && i != teamIndex) {
			return true;
		}
	}

	timeToLife--;
	return timeToLife <= 0;
}
void SlimeAgent::deadTurn() {
	if (timeToLife >= 0) {
		return;
	}
	const it delta = 2;
	it i, j;
	for (i = -delta; i <= delta; i++) {
		for (j = -delta; j <= delta; j++) {
			// кринжово
			if (settings->location.checkMatrix(pixelVector[0] + i, pixelVector[1] + j)) {
				settings->location.trailMap[pixelVector[0] + i][pixelVector[1] + j] -= settings->depositPerStep*2;
			}
		}
	}
}
//сканирование
void SlimeAgent::skanTurn() {
	//шанс рандомного поворота
	if (settings->chanceOfRandomChangeDirection != 0 && settings->chanceOfRandomChangeDirection > (rand() % 100 + 1)) {
		rotate(rand() % 2);
	} else {
		// -1 - поворот налево, 0 - сохранение направления, 1 - поворот направо
		it rotateSide = activateSensors();
		if (rotateSide != 0) {
			rotate(rotateSide == 1);
		}
	}
}
// функция хода
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
// определяет направление поворота
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
// поворот векторов скорости и сенсоров
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
//сдвиг и установка пикселя, если это возможно
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
// нанесение следа на карту
void SlimeAgent::makeDeposit() {
	settings->location.trailMap[pixelVector[0]][pixelVector[1]] += settings->depositPerStep;
}



SlimeMoldSimulation::SlimeMoldSimulation() {
	srand(it(time(NULL)));

	population = 0;

	//матрицы нужные для поворота мув вектора и сенсоров
	rightRotationMatrix = { {cos(0), -sin(0)}, { sin(0), cos(0)} };
	leftRotationMatrix = { {cos(-0), -sin(-0)}, { sin(-0), cos(-0)} };

	// параметры агентов
	sensorOffsetDistance = 0;
	sensorWidth = 1;
	sensorAngle = 0;
	stepSize = 0;
	depositPerStep = 0;
	chanceOfRandomChangeDirection = 0;

	// параметры локации
	diffusionSize = 0;
	decayFactor = 0;
	isPeriodicBoundary = 0;
	isCanMultiAgent = 0;
	location = Location(0, 0, this);
}

void SlimeMoldSimulation::setLocation(it xSize, it ySize) {
	location = Location(xSize, ySize, this);
}

void SlimeMoldSimulation::makeStep() {
	int i;
#pragma omp parallel for
	for (i = 0; i < particles.size(); i++) {
		particles[i]->moveTurn();
	}

	vector<SlimeAgent*> stillAlive;
#pragma omp parallel
	{
		vector<SlimeAgent*> stillAlivePrivate;
#pragma omp for nowait // в конце цикла потоки не ждут друг друга, а идут дальше
		for (i = 0; i < particles.size(); i++) {
			if (particles[i]->timeTurn()) {
				particles[i]->deadTurn();
				delete particles[i];
			}
			else {
				stillAlivePrivate.push_back(particles[i]);
			}
		}
#pragma omp critical // данная строка выполняется по очереди
		stillAlive.insert(stillAlive.end(), stillAlivePrivate.begin(), stillAlivePrivate.end());
	}
	particles = stillAlive;

#pragma omp parallel for
	for (i = 0; i < particles.size(); i++) {
		particles[i]->skanTurn();
	}

#pragma omp parallel
	{
		vector<SlimeAgent*> newPart;
#pragma omp for nowait
		for (i = 0; i < generators.size(); i++) {
			newPart = generatePopulationInPixel(generators[i].second, generators[i].first, i);
#pragma omp critical
			particles.insert(particles.end(), newPart.begin(), newPart.end());
		}
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

void SlimeMoldSimulation::outputInBmp(bool isChangedSettings = false) {
	static int bmpi = -1;
	static vector<ft> colorVector = { (rand() % 100 / 100.0), (rand() % 100 / 100.0) , (rand() % 100 / 100.0) };
	bmpi++;
	bmpi %= 1000;
	int w = location.getSizes()[0], h = location.getSizes()[1];

	FILE* f;
	unsigned char* img = NULL;
	int filesize = 54 + 3 * w * h;

	img = (unsigned char*)malloc(3 * w * h);
	memset(img, 0, 3 * w * h);

	int r, g, b;
	/*
	for (int i = 0; i < w; i++)
	{
		for (int j = 0; j < h; j++)
		{
			
			int x = i; int y = (h - 1) - j;
			r = location.trailMap[i][j];
			g = 0;
			b = 0;
			if (r > 255) r = 255;
			if (g > 255) g = 255;
			if (b > 255) b = 255;
			img[(x + y * w) * 3 + 2] = (unsigned char)(r);
			img[(x + y * w) * 3 + 1] = (unsigned char)(g);
			img[(x + y * w) * 3 + 0] = (unsigned char)(b);
		}
	}
	*/
	
	if (!isChangedSettings) {
		for (int colorOffset = 0; colorOffset < 3; colorOffset++) {
			for (int i = 0; i < particles.size(); i++) {
				if (img[(particles[i]->pixelVector[0] + particles[i]->pixelVector[1] * w) * 3 + colorOffset] > (255 - 8) * colorVector[colorOffset]) {
					img[(particles[i]->pixelVector[0] + particles[i]->pixelVector[1] * w) * 3 + colorOffset] += 8 * colorVector[colorOffset];
				}
				else {
					img[(particles[i]->pixelVector[0] + particles[i]->pixelVector[1] * w) * 3 + colorOffset] = 255* colorVector[colorOffset];
				}
			}
		}
	}
	else {
		for (int i = 0; i < particles.size(); i++) {
			if (img[(particles[i]->pixelVector[0] + particles[i]->pixelVector[1] * w) * 3 + 2] > 255 - 8) {
				img[(particles[i]->pixelVector[0] + particles[i]->pixelVector[1] * w) * 3 + 2] += 8;
			}
			else {
				img[(particles[i]->pixelVector[0] + particles[i]->pixelVector[1] * w) * 3 + 2] = 255;
			}
		}
	}
	
	for (int i = 0; i < generators.size(); i++) {
		img[(it(generators[i].first[0]) + it(generators[i].first[1]) * w) * 3 + 0] = 255;
		img[(it(generators[i].first[0]) + it(generators[i].first[1]) * w) * 3 + 1] = 255;
		img[(it(generators[i].first[0]) + it(generators[i].first[1]) * w) * 3 + 2] = 255;
		img[(it(generators[i].first[0] +1) + it(generators[i].first[1]) * w) * 3 + 0] = 255;
		img[(it(generators[i].first[0] +1) + it(generators[i].first[1]) * w) * 3 + 1] = 255;
		img[(it(generators[i].first[0] +1) + it(generators[i].first[1]) * w) * 3 + 2] = 255;
		img[(it(generators[i].first[0]) + it(generators[i].first[1]+1) * w) * 3 + 0] = 255;
		img[(it(generators[i].first[0]) + it(generators[i].first[1]+1) * w) * 3 + 1] = 255;
		img[(it(generators[i].first[0]) + it(generators[i].first[1]+1) * w) * 3 + 2] = 255;
		img[(it(generators[i].first[0] - 1) + it(generators[i].first[1]) * w) * 3 + 0] = 255;
		img[(it(generators[i].first[0] - 1) + it(generators[i].first[1]) * w) * 3 + 1] = 255;
		img[(it(generators[i].first[0] - 1) + it(generators[i].first[1]) * w) * 3 + 2] = 255;
		img[(it(generators[i].first[0]) + it(generators[i].first[1] - 1) * w) * 3 + 0] = 255;
		img[(it(generators[i].first[0]) + it(generators[i].first[1] - 1) * w) * 3 + 1] = 255;
		img[(it(generators[i].first[0]) + it(generators[i].first[1] - 1) * w) * 3 + 2] = 255;
	}

	unsigned char bmpfileheader[14] = { 'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0 };
	unsigned char bmpinfoheader[40] = { 40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0 };
	unsigned char bmppad[3] = { 0,0,0 };

	bmpfileheader[2] = (unsigned char)(filesize);
	bmpfileheader[3] = (unsigned char)(filesize >> 8);
	bmpfileheader[4] = (unsigned char)(filesize >> 16);
	bmpfileheader[5] = (unsigned char)(filesize >> 24);

	bmpinfoheader[4] = (unsigned char)(w);
	bmpinfoheader[5] = (unsigned char)(w >> 8);
	bmpinfoheader[6] = (unsigned char)(w >> 16);
	bmpinfoheader[7] = (unsigned char)(w >> 24);
	bmpinfoheader[8] = (unsigned char)(h);
	bmpinfoheader[9] = (unsigned char)(h >> 8);
	bmpinfoheader[10] = (unsigned char)(h >> 16);
	bmpinfoheader[11] = (unsigned char)(h >> 24);
	

	string name = "img" + to_string(bmpi) + ".bmp";
	f = fopen(name.data(), "wb");
	fwrite(bmpfileheader, 1, 14, f);
	fwrite(bmpinfoheader, 1, 40, f);
	for (int i = 0; i < h; i++)
	{
		fwrite(img + (w * (h - i - 1) * 3), 3, w, f);
		fwrite(bmppad, 1, (4 - (w * 3) % 4) % 4, f);
	}

	free(img);
	fclose(f);
}

SlimeAgent* SlimeMoldSimulation::generateAgent(vector<ft> startPosition, ft startAngle, it teamIndex = -1) {
	startAngle = startAngle / 180 * PI;
	vector<ft> moveVector = { stepSize * cos(startAngle), stepSize * sin(startAngle) };
	vector<ft> lsVector = { sensorOffsetDistance * cos(startAngle + sensorAngle), sensorOffsetDistance * sin(startAngle + sensorAngle) };
	vector<ft> csVector = { sensorOffsetDistance * cos(startAngle), sensorOffsetDistance * sin(startAngle) };
	vector<ft> rsVector = { sensorOffsetDistance * cos(startAngle - sensorAngle), sensorOffsetDistance * sin(startAngle - sensorAngle) };
	SlimeAgent* tmp = new SlimeAgent();
	tmp->setUp(teamIndex, startTimeToLife ,startPosition, moveVector, lsVector, csVector, rsVector, this);
	return tmp;
}

vector<SlimeAgent*> SlimeMoldSimulation::generatePopulationInPixel(it count, vector<ft>& startPosition, it teamIndex = -1) {
	vector<SlimeAgent*> rezult;

	for (int i = 0; i < count; i++) {
		rezult.push_back(generateAgent(startPosition, (ft(rand() % 360)), teamIndex));
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

void SlimeMoldSimulation::setUp(it ttl, it sp, ft sod, it sw, ft sa, ft ra, ft ss, ft dps, ft corcd, it dif, ft dec, bool ipb, bool icma) {
	population = sp;
	startTimeToLife = ttl;

	ra = it(ra) % 360;
	ra = ra * PI / 180;

	sa = it(sa) % 360;
	sa = sa * PI / 180;
	//матрицы нужные для поворота мув вектора и сенсоров
	rightRotationMatrix = { {cos(ra), -sin(ra)}, { sin(ra), cos(ra)} };
	leftRotationMatrix = { {cos(-ra), -sin(-ra)}, { sin(-ra), cos(-ra)} };
	
	// параметры агентов
	sensorOffsetDistance = sod;
	sensorWidth = 1;
	sensorAngle = sa;
	stepSize = ss;
	depositPerStep = dps;
	chanceOfRandomChangeDirection = corcd;

	// параметры локации
	diffusionSize = dif;
	decayFactor = dec;
	isPeriodicBoundary = ipb;
	isCanMultiAgent = icma;
}

bool SlimeMoldSimulation::updateSettingsFromFile() {
	ifstream input("settings.txt");

	string isNeedToUpdate;
	it ttl; ft sod; it sw; ft sa; ft ra; ft ss; ft dps; ft corcd; it dif; ft dec; bool ipb; bool icma;
	it newPopulation; it countOfGenerators;
	try {
		input >> isNeedToUpdate;
		if (isNeedToUpdate == "yes" || stepSize == 0) {
			input >> ttl >> sod >> sw >> sa >> ra >> ss >> dps >> corcd >> dif >> dec >> ipb >> icma;
			input >> newPopulation >> countOfGenerators;

			generators.clear();
			for (int i = 0; i < countOfGenerators; i++) {
				vector<ft> pos(2);
				it rate;
				input >> pos[0] >> pos[1] >> rate;
				
				generators.push_back(make_pair(pos, rate));
			}

			bool isNeedToUpdateAgents = population != 0 && (sensorOffsetDistance != sod || sensorAngle != sa || stepSize != ss);
			bool isNeedToResizePopulation = population != newPopulation;

			setUp(ttl, newPopulation, sod , 1 , sa , ra , ss , dps , corcd , dif , dec , ipb , icma);

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
				for (it i = 0; i < particles.size(); i++) {
					ft partAngle = atan2(particles[i]->positionVector[0], particles[i]->positionVector[1]) / PI * 180;
					particles[i] = generateAgent(particles[i]->positionVector, partAngle);
				}
			}

			input.close();

			ofstream output("settings.txt");
			output << "no" << endl;
			output << ttl << ' ' << sod << ' ' << sw << ' ' << sa << ' ' << ra << ' ' << ss << ' ' << dps << ' ' << corcd << ' ' << dif << ' ' << dec << ' ' << ipb << ' ' << icma << endl;
			output << newPopulation << ' ' << countOfGenerators;
			for (int i = 0; i < countOfGenerators; i++) {
				output << endl << generators[i].first[0] << ' ' << generators[i].first[1] << ' ' << generators[i].second;
			}
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

void SlimeMoldSimulation::startSimulation(vector<ft> startPosition) {
	if (updateSettingsFromFile()) {
		it count = 0;
		std::random_device rd;
		std::mt19937 g(rd());
		ft timeForOneIteration1, timeForOneIteration2;
		ft sumTime = 0;
		while (true) {
			timeForOneIteration1 = omp_get_wtime();

			makeStep(); // 450 // 340 - если параллельно
			
			timeForOneIteration2 = omp_get_wtime();

			count++;
			timeForOneIteration1 = timeForOneIteration2 - timeForOneIteration1;
			cout << count << endl << timeForOneIteration1 << endl;
			sumTime += timeForOneIteration1;
			cout << sumTime/count << endl << endl; // 12

			shuffle(particles.begin(), particles.end(), g); // 3
			if (count % 30 == 0) {
				bool isUpdated = updateSettingsFromFile();
				outputInBmp(isUpdated); // 130 - txt // 5 - bmp
			}
		}
	}
}
