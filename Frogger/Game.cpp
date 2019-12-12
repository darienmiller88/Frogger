#include "Game.h"
#include "SFML Game engine/Math stuff/Math.h"
#include "SFML Game engine/Asset Manager/AssetManager.h"
#include <future>

//I am using the expression "windowWidth / 14.f" to initialize the frogs size because in the actual game, the game window is 14 times
//wider than the actual frog. Also, the height of the window is large enough to contain 17 rectangular rows, making the height of each
//row windowHeight / 17. Using this information, we can properly scale the frog and the rows to the window perfectly.
Game::Game(int windowWidth, int windowHeight) :

windowSize(windowWidth, windowHeight),

//Number of lanes that will be in the game. The number here is VERY important, due to an unexplained. It is best to keep it under 20, 
//as well as even. If it were to be 17 for example, the collision detection would happen one lane over.
NUM_LANES(16),

TIME_TO_PLAY(30.f),

TIME_LEFT_LENGTH(windowWidth / 2.f),

//initialize the lane height to the height of the window / numberOfLanes. This will allow NUM_LANES - 1 lanes to fit in the 
//window perfectly.
LANE_HEIGHT(windowHeight / static_cast<float>(NUM_LANES)), 

//Initialize the frog's width to be 14 times smaller than the width of the window (true to the actual game), and to be the same height 
//as the lane height.
FROG_SIZE(windowWidth / 14.f, LANE_HEIGHT),

//Set isPaused to false. This variable will assist in pausing the game, and letting me know when the game is paused or unpaused.
isPaused(false), 

//initialize the elements of the frog. The frog will start at the first checkpoint, in the middle of othe screen
frog(FROG_SIZE, sf::Vector2f(windowWidth / 2.f, windowHeight - LANE_HEIGHT * 2), sf::Color::Green),

//Initialize the first checkpoint, which is the purple strip at the bottom of the screen.
firstCheckPoint(sf::Vector2f((float)windowWidth, LANE_HEIGHT)), 

//Initialize the second checkpoint using the size of the first checkpoint
secondCheckPoint(firstCheckPoint),

timeText("TIME", AssetManager::get().getFont(FontNames::Fonts::ARCADE_N), 30),

oneUpText("1-UP", AssetManager::get().getFont(FontNames::Fonts::ARCADE_N), 30),

highScoreText("HI-SCORE", AssetManager::get().getFont(FontNames::Fonts::ARCADE_N), 30),

timeLeft( {windowWidth / 2.f, LANE_HEIGHT / 2.f} ),

secondsText("", AssetManager::get().getFont(FontNames::Fonts::CALIBRI_B)),

score("0", AssetManager::get().getFont(FontNames::Fonts::ARCADE_N), 30)
{
	timer.startTimer();
	initializeCheckPoints();
	initializeTexts();
	initializeTimeObject();
	initializeLogs();
	initializeCarLanes();
	initializeTurtles();
	initializeHomeTiles();
}

void Game::drawState(sf::RenderWindow &window){
	window.draw(water);//This MUST be drawn first! If not, the logs and turtles, and the frog will be drawn over it.

	for (auto &life : livesShapes)
		window.draw(life);
	
	for (auto &tile : homeTiles)
		window.draw(tile);

	window.draw(topOfHome);

	drawLanes(window);
	drawTurtles(window);
	drawLogs(window);
	window.draw(timeText); 
	window.draw(oneUpText);
	window.draw(highScoreText);
	window.draw(timeLeft);
	window.draw(score);
	window.draw(firstCheckPoint);
	window.draw(secondCheckPoint);
	frog.drawEntity(window);
}

void Game::update(const sf::Time &deltaTime){
	timer.runTimer();

	//If the frog intersects the first checkpoint, set its respawn point to that checkpoint
	if (firstCheckPoint.getGlobalBounds().intersects(sf::FloatRect(frog.getBody().getPosition(), frog.getBody().getSize())))
		frog.setRespawnPoint({ windowSize.x / 2.f, firstCheckPoint.getPosition().y });
	//Do the same for the second checkpoint!
	else if (secondCheckPoint.getGlobalBounds().intersects(sf::FloatRect(frog.getBody().getPosition(), frog.getBody().getSize())))
		frog.setRespawnPoint({ windowSize.x / 2.f, secondCheckPoint.getPosition().y });

	float newWidth = 0.f;
	
	//Only map to the newWidth variable when the clock hasn't reached the desire time. This will prevent the time bar (timeLeft) from 
	//decreasing in reverse.
	if (timer.getElaspedTime().asSeconds() <= TIME_TO_PLAY )
		newWidth = mapper(timer.getElaspedTime().asSeconds(), 0.f, TIME_TO_PLAY, TIME_LEFT_LENGTH, 0.f);

	//Resize the width of the time bar proportionally to the increasing clock time. This will create the illusion that the bar is shrinking
	//over time!
	timeLeft.setSize({ newWidth, timeLeft.getSize().y });
	

	for (auto &lane : laneOfCars)
		lane.updateEntites();

	for (auto &lane : laneOfTurtles)
		lane.updateEntites();

	for (auto &lane : laneOfLogs)
		lane.updateEntites();
	
	attachFrogToEntity(); 
	handleFrogCarCollision();
}

void Game::pause(){
	frog.pause();
	timer.stopTimer();
	for (auto &lane : laneOfCars) 
		lane.pauseEntities();

	for (auto &lane : laneOfTurtles)
		lane.pauseEntities();

	for (auto &lane : laneOfLogs)
		lane.pauseEntities();
	std::cout << "paused!\n";
}

void Game::unpause(){
	frog.unpause();
	timer.startTimer();
	for (auto &lane : laneOfCars)
		lane.unpauseEntities();

	for (auto &lane : laneOfTurtles)
		lane.unpauseEntities();

	for (auto &lane : laneOfLogs)
		lane.unpauseEntities();
	std::cout << "unpaused!\n";
}

void Game::handleInput(StateManager &manager, const sf::Event &e, const sf::RenderWindow &window){
	
	if (e.type == sf::Event::KeyPressed and e.key.code == sf::Keyboard::P and !isPaused) {
		pause();
		isPaused = true;
	}
	else if (e.type == sf::Event::KeyPressed and e.key.code == sf::Keyboard::P and isPaused) {
		unpause();
		isPaused = false;
	}
	else if (e.type == sf::Event::KeyPressed and e.key.code == sf::Keyboard::Space) {
		frog.setFrogToRespawn();
	}

	//From the frogs perspective, the size of the window
	frog.moveEntity( { static_cast<float>(window.getSize().x), window.getSize().y - LANE_HEIGHT }, e);
	
}

void Game::initializeCheckPoints() {
	//This will position the first lane perfectly one lane length above the window
	firstCheckPoint.setPosition(0.f, windowSize.y - LANE_HEIGHT * 2);
	firstCheckPoint.setFillColor(sf::Color(200, 0, 250));

	//In the actual game, there is enough room for EXACTLY five lanes of cars between the first check point and the second check point,
	//so in order to position the second check point perfectly, we must position at EXACTLY six lane lengths above the first check point.
	//This way, there will be room for 5 lanes of cars, and the second check point will reside above the fifth lane.
	secondCheckPoint.setPosition(0.f, firstCheckPoint.getPosition().y - (6 * LANE_HEIGHT));
	secondCheckPoint.setFillColor(sf::Color(200, 0, 250));

	//This rectangle will will colored blue to represent the water. It will be as wide as the window, and just tall enough to cover the
	//portion of the window just above the second checkpoint.
	water.setSize({ static_cast<float>(windowSize.x), secondCheckPoint.getPosition().y});
	water.setFillColor(sf::Color::Blue);
}

void Game::initializeCarLanes(){
	//initialize each lane to have the position of each row between the first and second checkpoint. Between these check points, there
	//is room for exactly five lanes, so we have to add exactly five lanes.
	for (size_t i = 1; i <= 5; i++) {
		//The y position of each lane will start just above the first check point, and will be LANE_HEIGHT * i for each consecutive one.
		//Multiplying the lane height by i will allow you to go over to the next row!
		laneOfCars.push_back(Lane<Car>(sf::Vector2f(0, firstCheckPoint.getPosition().y - (LANE_HEIGHT * i)), firstCheckPoint.getSize()));

		//Also allow each car in the lane to move at a random speed.
		float randSpeed = rand.getFloatInRange(0.2f, 0.3f);
		sf::Color carColor(rand.getIntInRange(0, 256), rand.getIntInRange(0, 256), rand.getIntInRange(0, 256));

		//for each lane, we must add three cars!
		for (size_t j = 0; j < 3; j++) {

			//In frogger, there are both buses and cars, and the buses are twice as long the cars. Not only that, the buses are in the 
			//very last lane just before the second checkpoint before the logs. When i == 5, set the car width to be twice the car length.
			const float CAR_WIDTH = (i != 5) ? LANE_HEIGHT : LANE_HEIGHT * 2;

			//Have the cars in each lane be a fixed from each other. It's arbritary for the most part :P. The number we are multiplying
			//CAR_WIDTH by is the amount of car lengths - 1 that will be in between two cars. So in lanes 1 - 4, each car will start 5 car
			//lengths away from the one in front of it, whereas in the fifth lane, the buses will be 3 bus lengths away.
			const float DISTANCE_BETWEEN_CARS = (i != 5) ? CAR_WIDTH * 5 : CAR_WIDTH * 3;

			//depending on the lane, the cars will move to the left or right. Cars in even number lanes will move to the right, and cars
			//in odd number lanes will move to the left.
			const int SPEED_DIRECTION = (i % 2 != 0) ? -1 : 1;

			//Create a car object using the above information. The car will take in order, its size, its initial position, a color, and
			//how fast it is going.
			Car c(sf::Vector2f(CAR_WIDTH, LANE_HEIGHT), sf::Vector2f(windowSize.x / 2.f - (DISTANCE_BETWEEN_CARS * j),
				laneOfCars.back().getPosition().y), carColor, SPEED_DIRECTION * randSpeed, DISTANCE_BETWEEN_CARS);

			//Add the car to its respective lane.
			laneOfCars.back().addEntityToLane(c);
		}
	}
}

void Game::initializeTimeObject(){
	//The gap between each life rectangle.
	const int GAP = 10;
	int offset = 0;
	for (int i = 0; i < frog.getRemainingLives(); i++) {
		//We well make each life about 1/2 as wide as the frog, and half as tall as each lane
		livesShapes.push_back(sf::RectangleShape({ frog.getBody().getSize().x * .5f, LANE_HEIGHT / 2.f }));

		//The x position of each frog will be offset by each of their widths, causing them be printed next to each other. 
		livesShapes.back().setPosition(0 + (i * livesShapes.back().getSize().x) + offset, windowSize.y - livesShapes.back().getSize().y * 2);
		offset += GAP;
	}

	timeLeft.setFillColor(sf::Color::Green);

	//we will decrease the size of the rectangle representing the amount of time you have left, so in order to do this from left to right,
	//we have to rotate it 180.
	timeLeft.setRotation(180);

	//Since we are rotating the rectangle, its origin is now the top right corner instead of the top left corner, so we have set its origin
	//back to the top left. We set its origin to be the top right if you ignore the rotation, but once rotated, it now becomes the top left.
	timeLeft.setOrigin(timeLeft.getSize().x, 0);

	//Set the green rectangle representing the amount of time you have left to be to the right of the "TIME" text.
	timeLeft.setPosition(timeText.getPosition().x - timeLeft.getSize().x, static_cast<float>(windowSize.y));
}

void Game::initializeTexts(){
	//Set the time in the Lower right corner.
	timeText.setPosition(windowSize.x - timeText.getGlobalBounds().width, windowSize.y - timeText.getGlobalBounds().height - 5);
	timeText.setFillColor(sf::Color::Yellow);

	//Set the 1-UP text in the upper left corner.
	oneUpText.setPosition(oneUpText.getGlobalBounds().width, 0);

	//Set the high score text in the middle of the screen.
	highScoreText.setPosition(windowSize.x / 2.f - highScoreText.getGlobalBounds().width / 2.f, 0);

	//Set the position of the score to be directly under the "High Score" text. 
	score.setPosition(windowSize.x / 2.f, highScoreText.getGlobalBounds().height * 1.5f + highScoreText.getPosition().y);
	score.setFillColor(sf::Color::Red);
	score.setOrigin(score.getGlobalBounds().width / 2.f, score.getGlobalBounds().height / 2.f);
}

void Game::initializeLogs(){
	//The logs will be in lanes 2, 3, and 5
	laneOfLogs.push_back(Lane<Log>({ 0, secondCheckPoint.getPosition().y - LANE_HEIGHT * 2 }, { (float)windowSize.x, LANE_HEIGHT }));
	laneOfLogs.push_back(Lane<Log>({ 0, secondCheckPoint.getPosition().y - LANE_HEIGHT * 3 }, { (float)windowSize.x, LANE_HEIGHT }));
	laneOfLogs.push_back(Lane<Log>({ 0, secondCheckPoint.getPosition().y - LANE_HEIGHT * 5 }, { (float)windowSize.x, LANE_HEIGHT }));

	for (int lane = 1; lane <= laneOfLogs.size(); lane++) {
		const float MOVEMENT_SPEED = rand.getFloatInRange(0.1f, 0.3f);
		const float LOG_LENGTH = 150;//(lane == 1) ? LANE_HEIGHT * 2 : (lane == 2) ? LANE_HEIGHT * 3.5 : LANE_HEIGHT * 2;
		const float DISTANCE_BETWEEN_LOGS = LOG_LENGTH * 2;
		const int NUM_LOGS = 3;//(lane == 1 or lane == 2) ? 3 : 4;

		for (int j = 0; j < NUM_LOGS; j++) {
			sf::Color c(160, 82, 45);

			if (j == NUM_LOGS - 1) {
				c = sf::Color(78, 255, 120);
			}
				
			Log log({ LOG_LENGTH, LANE_HEIGHT }, { windowSize.x / 2.f - (DISTANCE_BETWEEN_LOGS * j), laneOfLogs[lane - 1].getPosition().y },
				MOVEMENT_SPEED, DISTANCE_BETWEEN_LOGS, c);

			std::cout << "log #" << j + 1 << " x position: " << log.getBody().getPosition().x << "\n";
 
			laneOfLogs[lane - 1].addEntityToLane(log);
		}
		std::cout << "lane size: " << laneOfLogs[lane - 1].getNumEnities() << " for lane: " << lane << "\n\n";
	}

}

//In the actual game, the turtles are in lanes 1 and 4, so only these lanes will be added to the lane of logs
void Game::initializeTurtles(){
	//The first lane of turtles will be just above the second checkpoint.
	laneOfTurtles.push_back(Lane<Turtles>({ 0, secondCheckPoint.getPosition().y - LANE_HEIGHT }, { (float)windowSize.x, LANE_HEIGHT }));

	//The second lane of turtles will be the fourth lane above the second checkpoint
	laneOfTurtles.push_back(Lane<Turtles>({ 0, secondCheckPoint.getPosition().y - LANE_HEIGHT * 4 }, { (float)windowSize.x, LANE_HEIGHT }));

	//We will iterate through all of two turtles lol
	for (int lane = 0; lane < laneOfTurtles.size(); lane++){
		const float MOVEMENT_SPEED = -.5f;

		//The groups of turtle objects will have a gap of 2 turtles in the first lane, and a gap of 3 turtles in the second lane.
		const float DISTANCE_BETWEEN_TURTLE_GROUPS = (lane == 0) ? LANE_HEIGHT * 2 : LANE_HEIGHT * 3;
		
		const int NUM_TURTLE_GROUPS = (lane == 0) ?  3 : 4;
		
		// The first lane has 5 groups of 3 turtles, whereas the fourth lane has 4 groups of 2 turtles
		const int NUM_TURTLES = (lane == 0) ? 3 : 2 ;

		sf::Vector2f firstTurtlePosition(windowSize.x / 2.f, laneOfTurtles[lane].getPosition().y);

		for (int j = 0; j < NUM_TURTLE_GROUPS; j++){
			Turtles turtles({ LANE_HEIGHT, LANE_HEIGHT }, firstTurtlePosition, MOVEMENT_SPEED, DISTANCE_BETWEEN_TURTLE_GROUPS, 
				NUM_TURTLES, sf::Color::Red);

			//After the first group of turtles is created, adjust the x position so that the next group of frogs will be a certain distance
			//away from the "Turtles" object in front of it.
			firstTurtlePosition.x = turtles.getPosition().x + DISTANCE_BETWEEN_TURTLE_GROUPS;

			laneOfTurtles[lane].addEntityToLane(turtles);
		}
	}
}

void Game::initializeHomeTiles(){
	const int NUM_TILES = 9.f;
	const float TILE_WIDTH = windowSize.x / (float)NUM_TILES;

	for (size_t i = 0; i < NUM_TILES; i++){
		homeTiles.push_back(sf::RectangleShape({ TILE_WIDTH, LANE_HEIGHT * 2 }));
		homeTiles.back().setPosition(i * (TILE_WIDTH * 2), laneOfLogs.back().getPosition().y - (LANE_HEIGHT * 2));
		homeTiles.back().setFillColor(sf::Color::White);
		//std::cout << "x: " << homeTiles.back().getPosition().x << " and y: " << homeTiles.back().getPosition().y << "\n";
	}

	topOfHome.setSize({ (float)windowSize.x, homeTiles.front().getSize().y / 3.f });
	topOfHome.setPosition(0, homeTiles.front().getPosition().y);
	topOfHome.setFillColor(sf::Color::White);
}

void Game::drawLanes(sf::RenderWindow &window){
	for (auto &lane : laneOfCars) {
		lane.draw(window);
	}
}

void Game::drawTurtles(sf::RenderWindow &window){
	for (auto &lane : laneOfTurtles)
		lane.draw(window);
}

void Game::drawLogs(sf::RenderWindow &window){
	for (auto &log : laneOfLogs)
		log.draw(window);
}

void Game::handleFrogCarCollision(){
	//In order to know which lane the frog is in, we will make use of the mapper function to map the frogs y position to a range of
	//0 to numlanes - 2. We will count the frogs starting lane (the first check point) as the 0th lane, and the first lane of cars as
	//the first lane. Since we ignore the first two lanes (the one below the first check point, and the first checkpoint itself), we have
	//to take away two from the upper limit of the second range to compensate.
	int whichLane = (int)mapper(frog.getBody().getPosition().y, firstCheckPoint.getPosition().y, 0.f, 0.f, (float)NUM_LANES - 2);

	//Only check for the car lanes when the frog is in between the first and second checkpoints (Lanes 1 - 5!), otherwise ignore the check.
	//Failure to check for this will result in the "whichLane" variable going out of bounds of the vector.
	if(whichLane > 0 and whichLane < 6)
		for (auto &car : laneOfCars[whichLane - 1].getEntities()) {
			if (car.isTouchingEntity(frog)) {
				frog.setFrogToRespawn();
				//std::cout << "car hit frog, lane: " << whichLane << "\n";

				if (frog.getRemainingLives() != 0){
					frog.decreaseLives();
					livesShapes.pop_back();
				}
			}

		}
}

void Game::attachFrogToEntity(){
	//Start checking for the frogs collision with the logs and turtles only when it has cross the second checkpoint.
	if (frog.getBody().getPosition().y < secondCheckPoint.getPosition().y) {
		for (auto &lane : laneOfTurtles) {
			for (auto& turtles : lane.getEntities())
				if (frog.attachTo(turtles, windowSize))
					std::cout << "Frog in water!\n";
		}

		for (auto &lane : laneOfLogs) {
			for (auto &log : lane.getEntities())
				if (!frog.attachTo(log, windowSize))
					std::cout << "Frog in water!\n";
		}	
	}
}