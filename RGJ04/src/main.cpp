#include <iostream>
#include <SFML/Graphics.hpp>

#include "PlayerActor.hpp"

int main() {    

	sf::RenderWindow app;
	app.Create(sf::VideoMode(800,600,32), "Le areamorph.", sf::Style::Close);

	PlayerActor actor(1);
	actor.SetColor(sf::Color(0,255,0));
	actor.InitializeAreas();


	while(app.IsOpened()) {
		sf::Event event;
		while(app.GetEvent(event)) {
			if(event.Type == sf::Event::Closed) {
				app.Close();
			} else if(event.Type == sf::Event::KeyPressed) {
				if(event.Key.Code == sf::Key::Escape) {
					app.Close();
				}
			}
		}

		actor.Update(0);

		app.Clear(sf::Color(0,0,0));

		actor.Draw(app);

		app.Display();
	}
}
