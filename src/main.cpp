#include <SFML/Graphics.hpp>

#include <chrono>
#include <iostream>

#include "particle.hpp"

int main(int argc, char** argv) {
	sf::VideoMode video_mode = sf::VideoMode::getDesktopMode();
	sf::RenderWindow window(video_mode, "2D n-body simulation");
	

	bool run_test_case = false;
	ParticleSystem particle_system(video_mode.size.x, video_mode.size.y);
	if (run_test_case) {
		Vec2d screen_offset = { 0.5f * video_mode.size.x, 0.5f * video_mode.size.y };
		particle_system.run_simple_system(screen_offset);
	} else {
		constexpr unsigned int NUMBER_OF_PARTICLES = 10;
		particle_system.create_random_particles(NUMBER_OF_PARTICLES);
	}
	double dt = 0.0f;
	sf::Clock timer;
	int iteration = 0;
	bool simulate_physics = true;
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			// toggle physics simulation with enter/space.
			if(event.type == sf::Event::KeyPressed) {
				if(event.key.code == sf::Keyboard::Enter || event.key.code == sf::Keyboard::Space) {
					simulate_physics = !simulate_physics;
					dt = 0.000001f; // reset dt and set it to a small value, so we dont get weird physic artifacts, be the fact that dt is 0.
					timer.restart();
				}
			}
		}
		window.clear();
		if(simulate_physics) {
			particle_system.update(dt);		
		}
		particle_system.draw_all_particles(window);
		window.display();

		iteration++;
		dt = timer.restart().asSeconds();

		std::cout << "iteration: " << iteration << ", dt: " << dt << " in seconds.\n";
	}

	return 0;
}