#pragma once
#include "Frog.h"

class Log : public Entity{
	public:
		Log(const sf::Vector2f &bodySize, const sf::Vector2f &initialPosition, float speed, float offset, 
			const sf::Color &color = sf::Color());
		void moveEntity(const sf::Vector2f &windowSize, const sf::Event &e = sf::Event()) override;
	private:
		float offset;
};

