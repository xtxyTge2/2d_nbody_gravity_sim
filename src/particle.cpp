#include "particle.hpp"
#include <random>


Particle::Particle() :
	m_mass(1.0f)
	, m_position({0.0f, 0.0f})
	, m_velocity({0.0f, 0.0f})
	, m_acceleration({0.0f, 0.0f})
	, m_color({0.0f, 0.0f, 0.0f})
	, m_circle(sf::CircleShape(1.0f))
{

}

void Particle::update() {
	m_circle.setPosition(sf::Vector2f(m_position.x, m_position.y));
	m_circle.setFillColor(sf::Color(m_color.x, m_color.y, m_color.z));
	m_circle.setRadius(m_mass); // 1 to 1 scaling from mass to radius.
}

ParticleSystem::ParticleSystem() : m_particle_vector({}) {
	
}

void ParticleSystem::draw_all_particles(sf::RenderWindow& window) {
	for (Particle& particle: m_particle_vector) {
		window.draw(particle.m_circle);
	}
}

void ParticleSystem::create_random_particles(unsigned int number_of_particles, unsigned int maximum_x, unsigned int maximum_y) {
	std::random_device rd;  // Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
	
	std::normal_distribution<> random_normal_mass(1.0f, 5.0f);

	std::uniform_int_distribution<> random_uniform_x_position(0, maximum_x);
	std::uniform_int_distribution<> random_uniform_y_position(0, maximum_y);
	std::uniform_int_distribution<> random_uniform_color(0, 255);

	m_particle_vector.reserve(number_of_particles);
	for (int i = 0; i < number_of_particles; i++) {
		float mass = random_normal_mass(gen);
		if(mass < 1.0f) {
			mass = 1.0f;
		}
		
		
		float position_x = static_cast<float>(random_uniform_x_position(gen));
		float position_y = static_cast<float>(random_uniform_y_position(gen));

		float color_r = static_cast<float>(random_uniform_color(gen));
		float color_g = static_cast<float>(random_uniform_color(gen));
		float color_b = static_cast<float>(random_uniform_color(gen));

		Particle particle;
		particle.m_mass = mass;
		particle.m_position = {position_x, position_y};
		particle.m_color = {color_r, color_g, color_b};

		m_particle_vector.push_back(particle);
	}

	for (Particle& particle: m_particle_vector) {
		particle.update();
	}
}