#include "Frog.h"
#include "Log.h"
#include "SFML Game engine/Math stuff/Math.h"
#include <iostream>

Frog::Frog(const sf::Vector2f &bodySize, const sf::Vector2f &initialPosition, const sf::Color &color) : lives(3),
Entity(bodySize, initialPosition, 0.f, 0.f, color){
	respawnPoint = initialPosition;
}

void Frog::moveEntity(const sf::Vector2f &windowSize, const sf::Event &e){
	//move the frog by its own width/height in all four directions
	if (e.type == sf::Event::KeyPressed and e.key.code == sf::Keyboard::Up and !isEntityPaused) 
		body.move(0, -body.getSize().y);
	else if (e.type == sf::Event::KeyPressed and e.key.code == sf::Keyboard::Down and !isEntityPaused) 
		body.move(0, body.getSize().y);
	else if (e.type == sf::Event::KeyPressed and e.key.code == sf::Keyboard::Left and !isEntityPaused)
		body.move(-body.getSize().x, 0);
	else if (e.type == sf::Event::KeyPressed and e.key.code == sf::Keyboard::Right and !isEntityPaused)
		body.move(body.getSize().x, 0);

	//constrain the x and y positions of the frog between the right side, left, top and bottom of the window.
	float x = constrain(body.getPosition().x, 0.f, windowSize.x - body.getSize().x);
	float y = constrain(body.getPosition().y, 0.f, windowSize.y - body.getSize().y);

	body.setPosition(x, y);
}

int Frog::getRemainingLives() const{
	return lives;
}

void Frog::setRespawnPoint(const sf::Vector2f &respawn){
	respawnPoint = respawn;
}

void Frog::setFrogToRespawn(){
	//std::cout << "respawn point: " << respawnPoint.x << " and " << respawnPoint.y << "\n";
	body.setPosition(respawnPoint);
}

void Frog::decreaseLives(){
	lives--;
}

bool Frog::attachTo(const Entity &entityToAttachTo, const sf::Vector2u &windowSize){
	if (entityToAttachTo.isTouchingEntity(*this)) {
		body.move(entityToAttachTo.getHorizontalSpeed(), 0);
		static int count = 0;
		count++;
		if (dynamic_cast<Log*>(const_cast<Entity*>(&entityToAttachTo)) != nullptr and count % 50000 == 0) {
			std::cout << "attached to a log!\n";
		}
		
		float x = constrain(body.getPosition().x, 0.f, windowSize.x - body.getSize().x);
		body.setPosition(x, body.getPosition().y);
		
		return true;
	}
	
	return false;
}