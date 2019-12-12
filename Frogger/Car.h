#pragma once
#include "Frog.h"

class Car : public Entity{
	public:
		Car(const sf::Vector2f &bodySize, const sf::Vector2f &initialPosition, const sf::Color &color, float speed, float distOffscreen);
		void moveEntity(const sf::Vector2f &windowSize, const sf::Event &e = sf::Event()) override;
};