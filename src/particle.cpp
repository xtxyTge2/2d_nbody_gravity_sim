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

void Particle::update() {
	m_circle.setPosition(sf::Vector2f(m_position.x, m_position.y));
}

void Particle::compute_force_exerted_by_other_particle(const Particle& other_particle) {
	constexpr double UNIVERSAL_GRAVITATIONAL_CONSTANT = 1.0e3; // 6.67e-11

	double dx = other_particle.m_position.x - m_position.x;
	double dy = other_particle.m_position.y - m_position.y;

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
	// Leapfrog numerical integration strategy

	// step 1: update velocities of all particles using half step: v += a * 0.5*dt
	update_velocities_of_all_particles(dt);

	// step 2: update all forces of all particles using gravitational law F = (G * m_1 * m_2)/ r*r
	update_forces_of_all_particles(dt);

	// step 3: set acceleration of all particles using a = F/m
	update_acceleration_of_all_particles(dt);

	// step 4: update position of all particles using full step: p += v * dt
	update_position_of_all_particles(dt);

	// step 5: update velocities of all particles using half step: v += a * 0.5*dt
	update_velocities_of_all_particles(dt);

	for (Particle& particle: m_particle_vector) {
		particle.update();
	}
}

void ParticleSystem::update_velocities_of_all_particles(double dt) {
	double half_dt = 0.5f * dt;
	for(Particle& particle: m_particle_vector) {
		particle.m_velocity.x += half_dt * particle.m_acceleration.x;
		particle.m_velocity.y += half_dt * particle.m_acceleration.y;
	}
}

void ParticleSystem::update_acceleration_of_all_particles(double dt) {
	for(Particle& particle: m_particle_vector) {
		particle.m_acceleration.x = particle.m_force.x / particle.m_mass;
		particle.m_acceleration.y = particle.m_force.y / particle.m_mass;
	}
}

void ParticleSystem::update_position_of_all_particles(double dt) {
	for(Particle& particle: m_particle_vector) {
		particle.m_position.x += dt * particle.m_velocity.x;
		particle.m_position.y += dt * particle.m_velocity.y;
	}
}

void ParticleSystem::update_forces_of_all_particles(double dt) {

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
		double mass = random_normal_mass(generator);
		if(mass < 1.0f) {
			mass = 1.0f;
		}
		
		double position_x = static_cast<double>(random_uniform_x_position(generator));
		double position_y = static_cast<double>(random_uniform_y_position(generator));

		double color_r = static_cast<double>(random_uniform_color(generator));
		double color_g = static_cast<double>(random_uniform_color(generator));
		double color_b = static_cast<double>(random_uniform_color(generator));

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

void ParticleSystem::run_simple_system(Vec2d screen_offset) {
	Particle p1;
	p1.m_mass = 1000;
	p1.m_position = { 800.0f, 400.0f};
	p1.m_velocity = { 20.0f, 0.0f };
	p1.m_acceleration = { 0.0f, 0.0f };
	p1.m_force = { 0.0f, 0.0f };
	p1.m_color = { 100.0f, 0.0f, 0.0f };
	
	Particle p2;
	p2.m_mass = 500;
	p2.m_position = { 300.0f, 300.0f};
	p2.m_velocity = { 10.0f, 10.0f };
	p2.m_acceleration = { 0.0f, 0.0f };
	p2.m_force = { 0.0f, 0.0f };
	p2.m_color = { 50.0f, 100.0f, 200.0f };

	Particle p3;
	p3.m_mass = 3000;
	p3.m_position = { 1000.0f, 800.0f};
	p3.m_velocity = { 20.0f, 20.0f };
	p3.m_acceleration = { 0.0f, 0.0f };
	p3.m_force = { 0.0f, 0.0f };
	p3.m_color = { 100.0f, 255.0f, 255.0f };

	m_particle_vector.push_back(p1);
	m_particle_vector.push_back(p2);
	m_particle_vector.push_back(p3);

	for(Particle& particle: m_particle_vector) {
		particle.init();
	}
}