#pragma once
#include "SFML Game engine/States/GameState.h"
#include "SFML Game engine/Utilities/Random.h"
#include "SFML Game engine/Utilities/Timer.h"
#include "Frog.h"
#include "Car.h"
#include "Log.h"
#include "Turtles.h"
#include "Lane.h"

class Game : public GameState{
	public:
		Game(int windowWidth, int windowHeight);

		//display the state to the screen
		void drawState(sf::RenderWindow &window) override;

		void update(const sf::Time &deltaTime) override;

		//function to allow a gamestate to pause while another gamestate is running
		void pause() override;

		//since we can pause game states, we also need to be able to unpause/resume them!
		void unpause() override;

		//Purely virtual function to allow subclasses of the State class to handle mouse and keyboard input, and change states from
		//within their own state (for example, a title screen stating containing a menu with settings, play game, quit, etc.)
		void handleInput(StateManager &manager, const sf::Event &e, const sf::RenderWindow &window) override;

	private:
		void initializeCheckPoints();
		void initializeCarLanes();
		void initializeTimeObject();
		void initializeTexts();
		void initializeLogs();
		void initializeTurtles();
		void initializeHomeTiles();
		void drawLanes(sf::RenderWindow &window);
		void drawTurtles(sf::RenderWindow &window);
		void drawLogs(sf::RenderWindow &window);
		void handleFrogCarCollision();
		void attachFrogToEntity();
	private:	
		const int NUM_LANES;
		//How much time the frog has to go from the bottom of the screen to the top
		const float TIME_TO_PLAY;

		//The constant will determine the length of the rectangular bar that represents the amount of time you have.
		const float TIME_LEFT_LENGTH;

		//Determines how tall each lane (rectangular row) in the game is.
		const float LANE_HEIGHT;
		const sf::Vector2f FROG_SIZE;
		bool isPaused;
		Frog frog;
		Random rand;
		std::vector<Lane<Car>> laneOfCars;
		std::vector <Lane<Log>> laneOfLogs;
		std::vector<Lane<Turtles>> laneOfTurtles;
		sf::Vector2u windowSize;
		std::vector<sf::RectangleShape> homeTiles;
		sf::RectangleShape topOfHome;

		//rectangular shape representing each life you have. What else am I supposed to call it?
		std::vector<sf::RectangleShape> livesShapes;
		

		//These rectangles represent the two purple checkpoints in frogger, where the frog wil respawn when it dies.
		sf::RectangleShape firstCheckPoint, secondCheckPoint;
		
		//This rectangle will be used to represent the green bar at the bottom of the screen next "TIME". Its length for the most
		//part is arbitrary, as it decreases proportionally to the clock.
		sf::RectangleShape timeLeft;

		sf::RectangleShape water;
		sf::Text timeText, oneUpText, highScoreText, score, secondsText, rectXWidth;
		Timer timer;
};