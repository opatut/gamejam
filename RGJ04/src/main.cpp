#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/lexical_cast.hpp>

#include "World.hpp"
#include "PlayerActor.hpp"
#include "AiActor.hpp"
#include "Definitions.hpp"

std::string names[] = { "You", "Amy", "Bill", "Charlie", "Detlef", "Emma", "Fred", "GladOS" };
sf::Color colors[] = { sf::Color(0,255,0), sf::Color::Red, sf::Color::Blue, sf::Color::Yellow,
					   sf::Color(0,100,0), sf::Color(255,0,255), sf::Color(0,255,255), sf::Color::White };
std::string credits[] = {"Programmed by opatut", "Music: Where Am I? by hamsteralliance.com", "Font: TT-KP by Christophe Caignaert"};
//std::string help[] = {"You are an ameba.", "There are other amebae.", "They will eat you.", "Try to eat them."};

sf::RenderWindow app;
sf::Font monofont;
sf::Image thisisyou_image;
sf::Music background_music;
float music_volume = 1.f;
int game_mode = 0;
bool music_running = true;


void ToggleMusic() {
	music_running = !music_running;
	if(music_running) background_music.Play();
	else background_music.Pause();
}

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
//	if(game_mode == 2) num_actors = 8;
	bool actor_done = false;

	PlayerActor* actor = new PlayerActor(0);
	actor->SetName(names[0]);
	actor->SetColor(colors[0]);
	actor->InitializeArea();
	world.AddActor(actor, true);

	for(int i = 1; i < num_actors; ++i) {
		AiActor* aiactor = new AiActor(i);
		aiactor->SetName(names[i]);
		aiactor->SetColor(colors[i]);
		aiactor->InitializeArea();
		world.AddActor(aiactor);
	}


	// GUI ETC.
	boost::ptr_vector<sf::Text> actor_labels;
	sf::Text* actor_label;
	for(int i = 0; i < num_actors; i++) {
		actor_label = new sf::Text(world.GetActorById(i).ToString(i==0), monofont, 14);
		actor_label->SetPosition(15,WINDOW_HEIGHT/2-FIELD_SIZE/2+20*i);
		actor_label->SetColor(world.GetActorById(i).GetColor());
		actor_labels.push_back(actor_label);
	}

	sf::Text debug("Debug label", monofont, 14);
	debug.SetPosition(10, WINDOW_HEIGHT-24);

	sf::Text myscore("0", monofont, 100);
	myscore.SetPosition(WINDOW_WIDTH - (WINDOW_WIDTH/2-FIELD_SIZE/2) + 40, WINDOW_HEIGHT/2-FIELD_SIZE/2 - 30);
	myscore.SetColor(sf::Color::White);


	sf::Sprite thisisyou(thisisyou_image);
	thisisyou.SetPosition(WINDOW_WIDTH/2 - FIELD_SIZE/2 - 33, WINDOW_HEIGHT/2 - FIELD_SIZE / 2 - 25);
	//thisisyou.SetPosition(0,0);
	float thisisyou_alpha = 1;

	sf::Clock clock;
	clock.Reset();

	sf::Shape black_rect = sf::Shape::Rectangle(0,0,WINDOW_WIDTH, WINDOW_HEIGHT, sf::Color::Black);
	float rect_alpha = 1;
	bool leaving = false;
	bool leaving_value = false;

	while(true) { // will be exited by <return>
		float time_diff = clock.GetElapsedTime();
		clock.Reset();

		if(actor_done) {
			actor_labels[next_actor].SetString(world.GetActorById(next_actor).ToString(false));

			next_actor++;
			if(next_actor >= num_actors)
				next_actor = 0;
			time_to_action = TIME_PER_ROUND / num_actors;
			actor_done = false;

			actor_labels[next_actor].SetString(world.GetActorById(next_actor).ToString(true));
		} else if(time_to_action <= 0 && next_actor != 0) {
			// let ai actor perform action
			actor_done = world.GetActorById(next_actor).Evolve();
		}
		time_to_action -= time_diff;

		sf::Event event;
		while(app.GetEvent(event)) {
			if(event.Type == sf::Event::Closed) {
				leaving = true;
				leaving_value = false;
			} else if(event.Type == sf::Event::KeyPressed) {
				if(event.Key.Code == sf::Key::Escape) {
					leaving = true;
					leaving_value = true;
				} else if(event.Key.Code == sf::Key::M) {
					ToggleMusic();
				} else if(event.Key.Code == sf::Key::S) {
					if(next_actor == 0)
						actor_done = true;
				}
			} else if(event.Type == sf::Event::MouseButtonPressed) {
				sf::Vector2i m(event.MouseButton.X, event.MouseButton.Y);

				if(next_actor == 0)	{
					actor_done = world.Clicked(m);
				}

			}
		}

		// UPDATE
		world.Update(0);

		debug.SetString("FPS: " + boost::lexical_cast<std::string>(round(1.f/time_diff)));
		myscore.SetString(boost::lexical_cast<std::string>(world.GetActorById(0).GetScore()));

		thisisyou_alpha -= time_diff * 0.1;
		if(thisisyou_alpha < 0) thisisyou_alpha = 0;
		thisisyou.SetColor(sf::Color(255,255,255,255*thisisyou_alpha));

		if(!leaving) rect_alpha -= time_diff*0.5;
		else rect_alpha += time_diff*1.5;
		if(rect_alpha < 0)
			rect_alpha = 0;
		if(rect_alpha > 1)
			rect_alpha = 1;
		black_rect.SetColor(sf::Color(0,0,0,255*rect_alpha));

		// MUSIC FADING
		if(leaving && !leaving_value && music_volume > 0) {
			music_volume -= 2*time_diff;
			if(music_volume < 0)
				music_volume = 0;
			background_music.SetVolume(music_volume*100);
		}

		if(rect_alpha >= 1 && leaving && (leaving_value || music_volume == 0)) {
			return leaving_value;
		}

		// DRAW
		app.Clear(sf::Color(0,0,0));
		world.Draw(app);
		for(unsigned int i = 0; i < actor_labels.size(); ++i) {
			app.Draw(actor_labels[i]);
		}
		if(thisisyou_alpha > 0)
			app.Draw(thisisyou);
		app.Draw(debug);
		app.Draw(myscore);
		app.Draw(black_rect);
		app.Display();
	}
}

bool menu() {

	boost::ptr_vector<sf::Text> menu;
	int active_entry = game_mode;
	std::string entries[] = { "2 Player match", "4 Player match", "8 Player match", "Music on", "Credits", "Quit game" };
	int num_entries = 6;

	for(int i = 0; i < num_entries; i++) {
		sf::Text* t = new sf::Text(entries[i], monofont, 20);
		menu.push_back(t);
	}

	sf::Text title("AMEBOID", monofont, 120);
	title.SetPosition(WINDOW_WIDTH/2 - title.GetRect().Width/2, WINDOW_HEIGHT/2 - title.GetRect().Height/2 - 100);

	sf::Text surtitle("opatut presents", monofont, 30);
	surtitle.SetPosition(WINDOW_WIDTH/2 - surtitle.GetRect().Width/2, WINDOW_HEIGHT/2 - 200);

	sf::Text credits_label("", monofont, 20);
	int credits_index = 0;
	int credits_count = 3;
	float credits_time = 0.f; // 3 .. 2,5 fade in / 2,5 .. 0,5 show / 0,5 .. 0 fade out
	bool show_credits = false;

	sf::Clock clock;
	clock.Reset();

	sf::Shape black_rect = sf::Shape::Rectangle(0,0,WINDOW_WIDTH, WINDOW_HEIGHT, sf::Color::Black);
	float rect_alpha = 1;
	bool leaving = false;
	bool leaving_value = false;

	while(true) { // will be exited by <return>
		float time_diff = clock.GetElapsedTime();
		clock.Reset();

		bool item_selected = false;

		sf::Event event;
		while(app.GetEvent(event)) {
			if(event.Type == sf::Event::Closed) {
				leaving = true;
				leaving_value = false; // stop program
			} else if(event.Type == sf::Event::MouseMoved) {
				// set active entry by mouse
				for(unsigned int i = 0; i < menu.size(); ++i) {
					if(menu[i].GetRect().Contains(event.MouseMove.X,event.MouseMove.Y)) {
						active_entry = i;
					}
				}
			} else if(event.Type == sf::Event::MouseButtonPressed && event.MouseButton.Button == sf::Mouse::Left) {
				for(unsigned int i = 0; i < menu.size(); ++i) {
					if(menu[i].GetRect().Contains(event.MouseButton.X,event.MouseButton.Y)) {
						active_entry = i;
						item_selected = true;
					}
				}
			} else if(event.Type == sf::Event::KeyPressed) {
				if(event.Key.Code == sf::Key::Escape) {
					leaving = true;
					leaving_value = false;
				} else if(event.Key.Code == sf::Key::M) {
					ToggleMusic();
				} else if(event.Key.Code == sf::Key::Down) {
					active_entry++;
					if(active_entry >= menu.size())
						active_entry = 0;
				} else if(event.Key.Code == sf::Key::Up) {
					active_entry--;
					if(active_entry < 0)
						active_entry = menu.size() - 1;
				} else if(event.Key.Code == sf::Key::Return or event.Key.Code == sf::Key::Space) {
					item_selected = true;
				}
			}
		}

		if(item_selected) {
			game_mode = active_entry;
			if(active_entry == 3) {
				ToggleMusic();
			} else if(active_entry == 4) {
				show_credits = true;
			} else if(active_entry == 5) {
				leaving = true;
				leaving_value = false;
			} else {
				leaving = true;
				leaving_value = true;
			}
			item_selected = true;
		}

		// UPDATE
		// MUSIC FADING
		if(leaving && !leaving_value && music_volume > 0) {
			music_volume -= 2*time_diff;
			if(music_volume < 0)
				music_volume = 0;
			background_music.SetVolume(music_volume*100);
		}

		if(music_running) menu[3].SetString("Music on");
		else menu[3].SetString("Music off");


		for(int i = 0; i < menu.size(); ++i) {
			if(i==active_entry) {
				menu[i].SetStyle(sf::Text::Bold);
				menu[i].SetColor(sf::Color::White);
			} else {
				menu[i].SetStyle(sf::Text::Regular);
				menu[i].SetColor(sf::Color(180,180,180));
			}
			menu[i].SetPosition(WINDOW_WIDTH / 2 - menu[i].GetRect().Width / 2, WINDOW_HEIGHT - menu.size()*40 - 40 + i*40);
		}

		// FADING
		if(!leaving) rect_alpha -= time_diff*0.5;
		else rect_alpha += time_diff;
		if(rect_alpha < 0)
			rect_alpha = 0;
		if(rect_alpha > 1)
			rect_alpha = 1;

		black_rect.SetColor(sf::Color(0,0,0,255*rect_alpha));

		if(rect_alpha >= 1 && leaving && (leaving_value || music_volume == 0)) {
			return leaving_value;
		}


		if(show_credits) {
			credits_time -= time_diff;
			if(credits_time <= 0) {
				credits_time = 3.f;
				if(credits_index < credits_count) {
					credits_label.SetString(credits[credits_index]);
					credits_index++;
					credits_label.SetPosition(WINDOW_WIDTH / 2 - credits_label.GetRect().Width/2, 40);
				} else {
					show_credits = false;
					credits_index = 0;
					credits_time = 0;
				}
			}
		}
		if(show_credits) {
			int alpha = 255;
			if(credits_time > 2.5) alpha = 255 - (credits_time-2.5) / 0.5 * 255;
			if(credits_time < 0.5) alpha = credits_time / 0.5 * 255;
			credits_label.SetColor(sf::Color(255,255,255,alpha));
		}

		// DRAW
		app.Clear();
		for(int i = 0; i < menu.size(); ++i) {
			app.Draw(menu[i]);
		}
		app.Draw(title);
		app.Draw(surtitle);
		if(show_credits) {
			app.Draw(credits_label);
		}
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

	if (!background_music.OpenFromFile("snd/game.ogg")) {
		std::cerr << "Could not load music file. Exiting." << std::endl;
		exit(1);
	}

	thisisyou_image.LoadFromFile("gfx/thisisyou.png");




	// STATE LOOP
	background_music.SetLoop(true);
	background_music.Play();
	bool running = true;
	while(running) {
		if(!menu()) running = false;
		else if(!game()) running = false;
	}


}
