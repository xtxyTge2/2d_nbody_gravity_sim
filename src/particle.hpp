#include <SFML/Graphics.hpp>

struct Vec2d {
	float x;
	float y;
};

struct Vec3d {
	float x;
	float y;
	float z;
};


class Particle {
	public:
	Particle();

	void update();

	float m_mass;
	Vec2d m_position;
	Vec2d m_velocity;
	Vec2d m_acceleration;
	Vec3d m_color;
	sf::CircleShape m_circle;
};

class ParticleSystem {
	public:
	ParticleSystem();

	void draw_all_particles(sf::RenderWindow& window);

	void create_random_particles(unsigned int number_of_particles, unsigned int maximum_x, unsigned int maximum_y);

	std::vector < Particle > m_particle_vector;
};