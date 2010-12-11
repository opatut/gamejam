#include <iostream>
#include <SFML/Graphics.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

#include "World.hpp"
#include "PlayerActor.hpp"
#include "AiActor.hpp"
#include "Definitions.hpp"

std::string names[] = { "You", "Amy the ameba", "Phagocyte", "Euglena"};
sf::Color colors[] = { sf::Color(0,255,0), sf::Color::Red, sf::Color::Blue, sf::Color::Yellow };

int main() {    
	sf::RenderWindow app;
	app.Create(sf::VideoMode(800,600,32), "AMEBOID - RedditGameJam 04 - by opatut", sf::Style::Close);


	// WORLD ETC.

	World world(&app);

	float time_to_action = 0;
	int next_actor = 0;
	int num_actors = 4;
	bool actor_done = false;

	PlayerActor* actor = new PlayerActor(0);
	actor->SetName(names[0]);
	actor->SetColor(colors[0]);
	actor->InitializeAreas();
	world.AddActor(actor, true);

	for(int i = 1; i < num_actors; ++i) {
		AiActor* aiactor = new AiActor(i);
		aiactor->SetName(names[i]);
		aiactor->SetColor(colors[i]);
		aiactor->InitializeAreas();
		world.AddActor(aiactor);
	}


	// GUI ETC.
	boost::ptr_vector<sf::Text> actor_labels;
	sf::Text* actor_label;
	for(int i = 0; i < num_actors; i++) {
		actor_label = new sf::Text(world.GetActorById(i).GetName());
		actor_label->SetCharacterSize(12);
		actor_label->SetPosition(15,10+15*i);
		actor_label->SetColor(world.GetActorById(i).GetColor());
		actor_labels.push_back(actor_label);
	}






	sf::Clock clock;
	clock.Reset();

	while(app.IsOpened()) {
		float time_diff = clock.GetElapsedTime();
		clock.Reset();

		if(actor_done) {
			actor_labels[next_actor].SetStyle(sf::Text::Regular);
			actor_labels[next_actor].SetString(world.GetActorById(next_actor).GetName());

			next_actor++;
			if(next_actor >= num_actors)
				next_actor = 0;
			time_to_action = .3f;
			actor_done = false;

			//actor_labels[next_actor].SetStyle(sf::Text::Bold);
			actor_labels[next_actor].SetString("> " + world.GetActorById(next_actor).GetName());
		} else if(time_to_action <= 0 && next_actor != 0) {
			// let ai actor perform action
			actor_done = world.GetActorById(next_actor).Evolve();
		}
		time_to_action -= time_diff;

		sf::Event event;
		while(app.GetEvent(event)) {
			if(event.Type == sf::Event::Closed) {
				app.Close();
			} else if(event.Type == sf::Event::KeyPressed) {
				if(event.Key.Code == sf::Key::Escape) {
					app.Close();
				}
			} else if(event.Type == sf::Event::MouseButtonPressed) {
				sf::Vector2i m(event.MouseButton.X, event.MouseButton.Y);

				if(m.x >= (WINDOW_WIDTH - FIELD_SIZE) / 2 &&
				   m.x <= (WINDOW_WIDTH - FIELD_SIZE) / 2 + FIELD_SIZE &&
				   m.y >= (WINDOW_HEIGHT - FIELD_SIZE) / 2 &&
				   m.y <= (WINDOW_HEIGHT- FIELD_SIZE) / 2 + FIELD_SIZE &&
				   next_actor == 0)	{
					actor_done = world.Clicked(m);
				}

			}
		}

		world.Update(0);

		app.Clear(sf::Color(0,0,0));
		world.Draw(app);
		for(unsigned int i = 0; i < actor_labels.size(); ++i) {
			app.Draw(actor_labels[i]);
		}
		app.Display();
	}
}
