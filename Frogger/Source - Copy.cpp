#include <iostream>
//#include "SFML Game engine/Utilities/Font string names.h"
#include "SFML Game engine/Asset Manager/AssetManager.h"
#include "SFML Game engine/Math stuff/Math.h"
#include <SFML/Graphics.hpp>

int main() {
	using namespace std;
	using namespace sf;
	//StateManager m(800, 900, "Frogger");	
	RenderWindow window(VideoMode(600, 600), "Snake!", sf::Style::Close | sf::Style::Titlebar);
	Clock clock;

	float rectWidth = 200.f, newRectWidth = 0.f;
	const float MAX_TIME = 5.f;
	RectangleShape rect(Vector2f(rectWidth, 20.f));
	Text time("", AssetManager::get().getFont(FontNames::Fonts::CALIBRI_B)), rectWText("", AssetManager::get().getFont(FontNames::Fonts::CALIBRI_B));
	
	time.setPosition(100, 200);
	rectWText.setPosition(100, 300);
	rect.setPosition(300, 300);
	rect.setRotation(180);
	rect.setFillColor(Color::Green);

	while (window.isOpen()) {
		Event e;

		if (clock.getElapsedTime().asSeconds() <= MAX_TIME)
			newRectWidth = mapper(clock.getElapsedTime().asSeconds(), 0.f, MAX_TIME, rectWidth, 0.f);

		rect.setSize({ newRectWidth, 20.f });
		
		while (window.pollEvent(e)) {
			if (e.type == Event::Closed)
				window.close();
		}

		time.setString("Time: " + to_string(clock.getElapsedTime().asSeconds()) + " seconds");
		//rectWText.setString("Rect width: " + to_string(newRectWidth) + " pixels");
		
		window.clear();
		window.draw(time);
		window.draw(rect);
		window.display();
	}
	//m.runGame();
}