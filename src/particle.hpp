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

	void init();

	void update(double dt);

	void compute_force_exerted_by_other_particle(const Particle& other_particle);


	float m_mass;
	Vec2d m_position;
	Vec2d m_velocity;
	Vec2d m_acceleration;
	Vec2d m_force;
	Vec3d m_color;
	sf::CircleShape m_circle;
};

class ParticleSystem {
	public:
	ParticleSystem(unsigned int max_x_coordinate, unsigned int max_y_coordinate);

	void draw_all_particles(sf::RenderWindow& window);

	void update(double dt);

	void update_forces_of_all_particles();

	void create_random_particles(unsigned int number_of_particles);

	std::vector < Particle > m_particle_vector;
	unsigned int m_max_x_coordinate;
	unsigned int m_max_y_coordinate;
};