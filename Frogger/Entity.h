#pragma once

#include <SFML/Graphics.hpp>
#include <functional>

/**
* This abstract class will provide a base for Frogger entities to inherit from. Entities in Frogger include the frog, the cars/buses, the
* logs, and the turtles. Entities will be able to move, check to see if they are intersecting another entity, and respawn on the opposite 
* side of the window,
*/
class Entity{
	public:
		Entity(const sf::Vector2f &bodySize, const sf::Vector2f &initialPosition, float speed, float distOffScreen, 
			const sf::Color &color = sf::Color());
		virtual ~Entity();

		virtual void moveEntity(const sf::Vector2f &windowSize, const sf::Event &e = sf::Event()) = 0;
		virtual void pause();
		virtual void unpause();
		virtual void drawEntity(sf::RenderWindow &window);//draws the entity to the screen
		virtual bool isTouchingEntity(const Entity &other) const;//checks to see if this entity is touching another one.

		const sf::RectangleShape &getBody() const;
		float getHorizontalSpeed() const;
	protected:

		/**
		*\brief Function to let an entity respawn on the opposite side of the screem.
		*
		*@param windowSize - The size of the window, which will allow me to know if the entity has cross over any of the four edges of the
		*window.
		*/
		virtual void respawnEntity(const sf::Vector2u &windowSize);
	protected:
		sf::RectangleShape body;
		const float HORIZONTAL_SPEED;
		const float DISTANCE_OFFSCREEN;
		bool isEntityPaused;
};

