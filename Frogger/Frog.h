#pragma once
#include "Entity.h"

class Frog : public Entity{
	public:
		Frog(const sf::Vector2f &bodySize, const sf::Vector2f &initialPosition, const sf::Color &color);
		
		void moveEntity(const sf::Vector2f &windowSize, const sf::Event &e) override;
		int getRemainingLives() const;

		//set the position of the point from where the frog will respawn when it dies or reaches the top of the screen.
		void setRespawnPoint(const sf::Vector2f &respawn);

		//change the position of the frog to that of the respawn point.
		void setFrogToRespawn();
		void decreaseLives();
		bool attachTo(const Entity &entityToAttachTo, const sf::Vector2u &windowSize);
	private:
		int lives;
		sf::Vector2f respawnPoint;
};