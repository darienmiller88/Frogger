#include "Entity.h"

Entity::Entity(const sf::Vector2f &bodySize, const sf::Vector2f &initialPosition, float speed, float distOffScreen, 
	const sf::Color &color) : isEntityPaused(false), HORIZONTAL_SPEED(speed), DISTANCE_OFFSCREEN(distOffScreen) {
	body.setPosition(initialPosition);
	body.setSize(bodySize);
	body.setFillColor(color);
}

Entity::~Entity(){
}

void Entity::pause(){
	isEntityPaused = true;
}

void Entity::unpause(){
	isEntityPaused = false;
}

void Entity::drawEntity(sf::RenderWindow &window){
	window.draw(body);
}

bool Entity::isTouchingEntity(const Entity &other) const{
	return body.getGlobalBounds().intersects(sf::FloatRect(other.body.getPosition(), other.body.getSize()));
}

const sf::RectangleShape &Entity::getBody() const{
	return body;
}

float Entity::getHorizontalSpeed() const{
	return HORIZONTAL_SPEED;
}

void Entity::respawnEntity(const sf::Vector2u &windowSize){
	//This is the disance the object has to move off screen in order for it to be redrawn on the other side.
	const float TOTAL_DISTANCE_OFF_SCREEN = body.getSize().x + DISTANCE_OFFSCREEN;

	//when the entity has moved its the total distance off of the right side of the window, respawn on the 
	//the left side.
	if (body.getPosition().x > windowSize.x + TOTAL_DISTANCE_OFF_SCREEN)
		body.setPosition(0 - body.getSize().x, body.getPosition().y);

	//Vice versa for the left side!
	else if (body.getPosition().x <= -TOTAL_DISTANCE_OFF_SCREEN)
		body.setPosition((float)windowSize.x, body.getPosition().y);
}