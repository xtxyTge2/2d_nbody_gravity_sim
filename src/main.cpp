#include <SFML/Graphics.hpp>

#include "particle.hpp"

int main(int argc, char** argv) {
	sf::VideoMode video_mode = sf::VideoMode::getDesktopMode();
	sf::RenderWindow window(video_mode, "2D n-body simulation");
	
	constexpr unsigned int NUMBER_OF_PARTICLES = 5000;
	ParticleSystem particle_system;
	particle_system.create_random_particles(NUMBER_OF_PARTICLES, video_mode.size.x, video_mode.size.y);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		particle_system.draw_all_particles(window);
		window.display();
	}

	return 0;
}