#include <random>
#include <cmath>

#include "particle.hpp"

Particle::Particle() :
	m_mass(1.0f)
	, m_position({0.0f, 0.0f})
	, m_velocity({0.0f, 0.0f})
	, m_acceleration({0.0f, 0.0f})
	, m_force({0.0f, 0.0f})
	, m_color({0.0f, 0.0f, 0.0f})
	, m_circle(sf::CircleShape(1.0f))
{
	
}

void Particle::init() {
	m_circle.setRadius(m_mass); // 1 to 1 scaling from mass to radius.
	m_circle.setFillColor(sf::Color(m_color.x, m_color.y, m_color.z));
	m_circle.setPosition(sf::Vector2f(m_position.x, m_position.y));
}

void Particle::update(double dt) {
	m_acceleration.x = m_force.x / m_mass;
	m_acceleration.y = m_force.y / m_mass;

	m_velocity.x += m_acceleration.x * dt;
	m_velocity.y += m_acceleration.y * dt;

	m_position.x += m_velocity.x * dt;
	m_position.y += m_velocity.y * dt;
	
	m_circle.setPosition(sf::Vector2f(m_position.x, m_position.y));
}

void Particle::compute_force_exerted_by_other_particle(const Particle& other_particle) {
	constexpr double UNIVERSAL_GRAVITATIONAL_CONSTANT = 1.0e3; // 6.67e-11

	double dx = -(m_position.x - other_particle.m_position.x);
	double dy = -(m_position.y - other_particle.m_position.y);

	double distance_squared = dx * dx + dy * dy;
	double distance = std::sqrt(distance_squared);

	// F = (G * m_1 * m_2) / r*r, where G is universal gravitation constant, 
	// r is the distance between the two objects 
	// with masses m_1 and m_2, respectively.
	double total_force = (1.0f / distance_squared) * UNIVERSAL_GRAVITATIONAL_CONSTANT * m_mass * other_particle.m_mass; 

	m_force.x += total_force * (dx / distance);
	m_force.y += total_force * (dy / distance);
}

ParticleSystem::ParticleSystem(unsigned int max_x_coordinate, unsigned int max_y_coordinate) : 
	m_particle_vector({})
	, m_max_x_coordinate(max_x_coordinate)
	, m_max_y_coordinate(max_y_coordinate)
{
	
}

void ParticleSystem::draw_all_particles(sf::RenderWindow& window) {
	for (Particle& particle: m_particle_vector) {
		window.draw(particle.m_circle);
	}
}

void ParticleSystem::update(double dt) {
	update_forces_of_all_particles();

	for(Particle& particle: m_particle_vector) {
		particle.update(dt);
	}
	int k = 1;
}

void ParticleSystem::update_forces_of_all_particles() {

	// reset forces of all particles
	for(Particle& particle: m_particle_vector) {
		particle.m_force = { 0.0f, 0.0f };
	}

	// update forces of all particles
	for (int i = 0; i < m_particle_vector.size(); i++) {
		for (int j = 0; j < m_particle_vector.size(); j++) {
			if(i != j) {
				m_particle_vector[i].compute_force_exerted_by_other_particle(m_particle_vector[j]);
			}
		}
	}
}


void ParticleSystem::create_random_particles(unsigned int number_of_particles) {
	std::mt19937 generator(42.0f); // Standard mersenne_twister_engine 
	
	std::normal_distribution<> random_normal_mass(1.0f, 10.0f);

	std::uniform_int_distribution<> random_uniform_x_position(0, m_max_x_coordinate);
	std::uniform_int_distribution<> random_uniform_y_position(0, m_max_y_coordinate);
	std::uniform_int_distribution<> random_uniform_color(0, 255);

	m_particle_vector.reserve(number_of_particles);
	for (int i = 0; i < number_of_particles; i++) {
		float mass = random_normal_mass(generator);
		if(mass < 1.0f) {
			mass = 1.0f;
		}
		
		float position_x = static_cast<float>(random_uniform_x_position(generator));
		float position_y = static_cast<float>(random_uniform_y_position(generator));

		float color_r = static_cast<float>(random_uniform_color(generator));
		float color_g = static_cast<float>(random_uniform_color(generator));
		float color_b = static_cast<float>(random_uniform_color(generator));

		Particle particle;
		particle.m_mass = mass;
		particle.m_position = {position_x, position_y};
		particle.m_color = {color_r, color_g, color_b};

		m_particle_vector.push_back(particle);
	}

	for(Particle& particle: m_particle_vector) {
		particle.init();
	}
}