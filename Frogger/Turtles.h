#pragma once
#include "Entity.h"

//Instead of having a "Turtle" class, with each turtle object only having one turtle, I decided to make it plural and have this class hold
//several turtles instead! In frogger, there are a select number of groups of turtles in a lane, with each group having their own number
//of turtles, so in order to replicate this behavior, each "Turtles" object will an array or vector of turtles! :D
class Turtles : public Entity{
	public:
		Turtles(const sf::Vector2f &bodySize, const sf::Vector2f &initialPosition, float speed, float distOffScreen, int numTurtles, 
			const sf::Color& color = sf::Color());
		void moveEntity(const sf::Vector2f &windowSize, const sf::Event &e = sf::Event()) override;
		void respawnEntity(const sf::Vector2u &windowSize) override;
		void drawEntity(sf::RenderWindow &window) override;
		bool isTouchingEntity(const Entity& other) const override;

		//NOTE: Return the position of the turtle in the back, which is the only position that is relevant. I need to know the position 
		//of the turtle in the back as to space out each "Turtles" object relative to the one in front of it. 
		const sf::Vector2f &getPosition() const;
	private:
		std::vector<sf::RectangleShape> turtles;
};