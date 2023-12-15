#include <SFML/Graphics.hpp>
#include <chrono>
#include <iostream>
#include "EventReader.h"

int main(int argc, const char* const * const argv)
{
	constexpr unsigned width = 1920;
	constexpr unsigned height = 1080;
	sf::String app_name(argv[0]);

	{
		sf::RenderWindow window = sf::RenderWindow(sf::VideoMode(width, height), app_name, sf::Style::Fullscreen);
		my_ns::EventReader event_reader(window);
		my_ns::ButtonKB F_reader(event_reader, sf::Keyboard::Scancode::F);
		sf::CircleShape circle(100);

		circle.setPosition({ 300, 300 });
		circle.setFillColor(sf::Color::Green);

		while (true)
		{
			event_reader.Tick();

			F_reader.Tick();

			if (event_reader.CountEvents(sf::Event::Closed))
			{
				break;
			}

			window.display();
		}
	} 
}