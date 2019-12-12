#include "Car.h"
#include <iostream>

Car::Car(const sf::Vector2f &bodySize, const sf::Vector2f &initialPosition, const sf::Color &color, float speed, float distOffScreen) : 
Entity(bodySize, initialPosition, speed, distOffScreen, color){
}

void Car::moveEntity(const sf::Vector2f &windowSize, const sf::Event &e){
	float actualSpeed = HORIZONTAL_SPEED;//Save the initial speed that the car is moving...
	
	//Depending on whether or not the game is paused, multiply the speed by 0 or 1. If the game is paused, multiplying the speed by zero
	//will make it so that the car moves by 0 pixels each frame (effectively standing still!).
	actualSpeed *= (isEntityPaused) ? 0 : 1 ;

	body.move(actualSpeed, 0);
	respawnEntity(static_cast<sf::Vector2u>(windowSize));
}