#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Car.h"

/*
* Lane class to keep track of the cars, logs and turtles in the game. Each lane object will have its own list of entities.
*/
template<class type>
class Lane{
	public:
		Lane(const sf::Vector2f &initialPosition, const sf::Vector2f &size) : position(initialPosition), size(size){

		}

		const sf::Vector2f &getPosition() const {
			return position;
		}

		void addEntityToLane(const type &entity) {
			entities.push_back(entity);
		}

		void draw(sf::RenderWindow &window) {
			for (auto &entity : entities)
				entity.drawEntity(window);
		}

		void updateEntites() {
			for (auto &entity : entities)
				entity.moveEntity(size);
		}

		void pauseEntities() {
			for (auto &entity : entities)
				entity.pause();
		}

		void unpauseEntities() {
			for (auto &entity : entities)
				entity.unpause();
		}

		int getNumEnities() const {
			return entities.size();
		}

		const sf::Vector2f &getLaneSize() const {
			return size;
		}

		const std::vector<type> &getEntities() const {
			return entities;
		}
	private:
		sf::Vector2f position, size;
		std::vector<type> entities;
};


