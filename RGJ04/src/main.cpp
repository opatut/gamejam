#include <iostream>
#include <SFML/Graphics.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/lexical_cast.hpp>

#include "World.hpp"
#include "PlayerActor.hpp"
#include "AiActor.hpp"
#include "Definitions.hpp"

std::string names[] = { "You", "Amy Ameba", "Phagocyte", "Euglena", "P-Dub", "Steve", "Amoebia Pond", "Your gf"};
sf::Color colors[] = { sf::Color(0,255,0), sf::Color::Red, sf::Color::Blue, sf::Color::Yellow, sf::Color(0,100,0), sf::Color(255,0,255), sf::Color(0,255,255),
					   sf::Color::White };
sf::RenderWindow app;
sf::Font monofont;
int game_mode = 0;


bool game() {
	// WORLD ETC.

	World& world = World::get_mutable_instance();
	world.SetWindow(&app);
	world.Reset();

	float time_to_action = 0;
	int next_actor = 0;
	int num_actors = 8;
	if(game_mode == 0) num_actors = 2;
	if(game_mode == 1) num_actors = 4;
	if(game_mode == 2) num_actors = 8;
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
		if(i==0)
			actor_label = new sf::Text("> " + world.GetActorById(i).GetName(), monofont, 14);
		else
			actor_label = new sf::Text("  " + world.GetActorById(i).GetName(), monofont, 14);
		actor_label->SetPosition(15,10+20*i);
		actor_label->SetColor(world.GetActorById(i).GetColor());
		actor_labels.push_back(actor_label);
	}

	sf::Text debug("Debug label", monofont, 14);
	debug.SetPosition(10, WINDOW_HEIGHT-24);




	sf::Clock clock;
	clock.Reset();

	while(true) { // will be exited by <return>
		float time_diff = clock.GetElapsedTime();
		clock.Reset();

		if(actor_done) {
			actor_labels[next_actor].SetString("  " + world.GetActorById(next_actor).GetName());

			next_actor++;
			if(next_actor >= num_actors)
				next_actor = 0;
			time_to_action = TIME_PER_ROUND / num_actors;
			actor_done = false;

			actor_labels[next_actor].SetString("> " + world.GetActorById(next_actor).GetName());
		} else if(time_to_action <= 0 && next_actor != 0) {
			// let ai actor perform action
			actor_done = world.GetActorById(next_actor).Evolve();
		}
		time_to_action -= time_diff;

		sf::Event event;
		while(app.GetEvent(event)) {
			if(event.Type == sf::Event::Closed) {
				return false;
			} else if(event.Type == sf::Event::KeyPressed) {
				if(event.Key.Code == sf::Key::Escape) {
					return true;
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

		// UPDATE
		world.Update(0);

		debug.SetString("FPS: " + boost::lexical_cast<std::string>(round(1.f/time_diff)));

		// DRAW
		app.Clear(sf::Color(0,0,0));
		world.Draw(app);
		for(unsigned int i = 0; i < actor_labels.size(); ++i) {
			app.Draw(actor_labels[i]);
		}
		app.Draw(debug);
		app.Display();
	}
}

bool menu() {

	boost::ptr_vector<sf::Text> menu;
	int active_entry = game_mode;
	std::string entries[] = { "2 Player match", "4 Player match", "8 Player match", "Quit game" };
	int num_entries = 4;

	for(int i = 0; i < num_entries; i++) {
		sf::Text* t = new sf::Text(entries[i], monofont, 20);
		menu.push_back(t);
	}

	sf::Text title("AMEBOID", monofont, 120);
	title.SetPosition(WINDOW_WIDTH/2 - title.GetRect().Width/2, WINDOW_HEIGHT/2 - title.GetRect().Height/2);

	sf::Text surtitle("opatut presents", monofont, 30);
	surtitle.SetPosition(WINDOW_WIDTH/2 - surtitle.GetRect().Width/2, WINDOW_HEIGHT/4);

	sf::Shape black_rect = sf::Shape::Rectangle(0,0,WINDOW_WIDTH, WINDOW_HEIGHT, sf::Color::Black);

	sf::Clock clock;
	clock.Reset();

	float rect_alpha = 1;

	while(true) { // will be exited by <return>
		float time_diff = clock.GetElapsedTime();
		clock.Reset();

		sf::Event event;
		while(app.GetEvent(event)) {
			if(event.Type == sf::Event::Closed) {
				return false; // stop program
			} else if(event.Type == sf::Event::KeyPressed) {
				if(event.Key.Code == sf::Key::Escape) {
					return false;
				} else if(event.Key.Code == sf::Key::Return) {
					game_mode = active_entry;
					if(active_entry == 3)
						return false;
					else
						return true;
				} else if(event.Key.Code == sf::Key::Down) {
					active_entry++;
					if(active_entry >= menu.size())
						active_entry = 0;
				} else if(event.Key.Code == sf::Key::Up) {
					active_entry--;
					if(active_entry < 0)
						active_entry = menu.size() - 1;
				}
			}
		}

		// UPDATE
		for(int i = 0; i < menu.size(); ++i) {
			if(i==active_entry) {
				menu[i].SetStyle(sf::Text::Bold);
				menu[i].SetColor(sf::Color::White);
			} else {
				menu[i].SetStyle(sf::Text::Regular);
				menu[i].SetColor(sf::Color(180,180,180));
			}
			menu[i].SetPosition(WINDOW_WIDTH / 2 - menu[i].GetRect().Width / 2, WINDOW_HEIGHT - menu.size()*40 - 20 + i*40);
		}

		rect_alpha -= time_diff*0.5;
		if(rect_alpha < 0)
			rect_alpha = 0;
		black_rect.SetColor(sf::Color(0,0,0,255*rect_alpha));


		// DRAW
		app.Clear();
		for(int i = 0; i < menu.size(); ++i) {
			app.Draw(menu[i]);
		}
		app.Draw(title);
		app.Draw(surtitle);
		app.Draw(black_rect);
		app.Display();
	}
	return false;
}

int main() {    
	// CREATE WINDOW
	app.Create(sf::VideoMode(WINDOW_WIDTH,WINDOW_HEIGHT,32), "AMEBOID - RedditGameJam 04 - by opatut", sf::Style::Close);
	app.SetFramerateLimit(100);

	// LOAD DATA
	if(!monofont.LoadFromFile("fonts/TT-KP-RE.TTF")) {
		std::cerr << "Could not load font file. Exiting." << std::endl;
		exit(1);
	}

	bool running = true;
	while(running) {
		if(!menu()) running = false;
		else if(!game()) running = false;
	}


}
