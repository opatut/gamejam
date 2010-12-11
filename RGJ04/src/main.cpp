#include <iostream>
#include <SFML/Graphics.hpp>

#include "Area.hpp"

int main() {    

	sf::RenderWindow app;
	app.Create(sf::VideoMode(800,600,32), "Le areamorph.", sf::Style::Close);

	Area area1(Point(10,10));


	while(app.IsOpened()) {
		sf::Event event;
		while(app.GetEvent(event)) {
			if(event.Type == sf::Event::Closed) {
				app.Close();
			}
		}

		app.Clear(sf::Color(0,0,0));

		area1.Draw(app);

		app.Display();
	}
}
