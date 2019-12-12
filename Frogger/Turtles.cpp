#include "Turtles.h"
#include <iostream>

Turtles::Turtles(const sf::Vector2f &bodySize, const sf::Vector2f &initialPosition, float speed, float distOffScreen ,int numTurtles, 
	const sf::Color &color) : Entity(bodySize, initialPosition, speed, distOffScreen, color){

	for (int i = 0; i < numTurtles; i++) {
		turtles.push_back(sf::RectangleShape(bodySize));
		
		//Afterwards, we apply a funny looking formula to calculate the x position of each turtle. What we want is for each turtle
		//to be printed right next to each other, in a row, like so -> [][][] where a [] is a turtle. Multipling i (number of the "ith" 
		//turtle) by how wide each turtle it is, and adding it to the initial x position will shift each turtle down by its own width.
		turtles.back().setPosition(initialPosition.x + (i * bodySize.x), initialPosition.y);
		turtles.back().setFillColor(color);
		//turtles.back().setOutlineColor(sf::Color::Green);
		//turtles.back().setOutlineThickness(5);
	}

}

void Turtles::moveEntity(const sf::Vector2f &windowSize, const sf::Event &e){
	float actualSpeed = HORIZONTAL_SPEED;//Save the initial speed that the turtles are moving...

	//Depending on whether or not the game is paused, multiply the speed by 0 or 1. If the game is paused, multiplying the speed by zero
	//will make it so that the turtles moves by 0 pixels each frame (effectively standing still!).
	actualSpeed *= (isEntityPaused) ? 0 : 1;

	for (auto &turtle : turtles) {
		turtle.move(actualSpeed, 0);

		respawnEntity(static_cast<sf::Vector2u>(windowSize));
	}
}

void Turtles::respawnEntity(const sf::Vector2u &windowSize){
	//This is the disance the object has to move off screen in order for it to be redrawn on the other side. It's Mostly arbrirary lol.
	const float TOTAL_DISTANCE_OFF_SCREEN = DISTANCE_OFFSCREEN + turtles.front().getSize().x;

	//when the LAST turtle has moved the of the left side of the window, respawn all the turtles on the right side.
	if (turtles.back().getPosition().x + turtles.back().getSize().x < -TOTAL_DISTANCE_OFF_SCREEN) {

		//Set only the front turtle to right side of the screen, and then in a loop, have the other turtles follow suit.
		turtles.front().setPosition(static_cast<float>(windowSize.x), turtles.front().getPosition().y);	
		for (int i = 1; i < turtles.size(); i++) {
			turtles[i].setPosition(turtles[i - 1].getPosition().x + turtles.front().getSize().x, turtles.front().getPosition().y);	
		}
	}
}

void Turtles::drawEntity(sf::RenderWindow &window){
	for (auto &turtle : turtles)
		window.draw(turtle);
}

bool Turtles::isTouchingEntity(const Entity &other) const{
	for(auto &turtle : turtles)
		if(turtle.getGlobalBounds().intersects(sf::FloatRect(other.getBody().getPosition(), other.getBody().getSize())))
			return true;
	return false;
}

//Return the position of the turtle in the back, which is the only position that is relevant. I need to know the position of the turtle in
//the back as to space out each "Turtles" object relative to the one in front of it. 
const sf::Vector2f &Turtles::getPosition() const{
	return turtles.back().getPosition();
}
