#include <iostream>
#include <SFML/Graphics.hpp>

int main() {    

	sf::RenderWindow app;
	app.Create(sf::VideoMode(800,600,32), "Le areamorph.", sf::Style::Close);


	while(app.IsOpened()) {
		sf::Event event;
		while(app.GetEvent(event)) {
			if(event.Type == sf::Event::Closed) {
				app.Close();
			}
		}

		app.Clear(sf::Color(0,0,0));

		app.Draw(shape);

		app.Display();
	}
}
