#include <SFML/Graphics.hpp>

#include <cmath>

class Vec2d {
public:
	Vec2d() = default;

	Vec2d(double a_x, double a_y) :
		x(a_x), y(a_y)
	{
	
	};

	Vec2d normalize() {
		double d = std::sqrt(x*x + y*y);

		return Vec2d(x / d, y / d);
	};

	double x;
	double y;
};

struct Vec3d {
	double x;
	double y;
	double z;
};


class Particle {
	public:
	Particle();

	void init();

	void update();

	void compute_force_exerted_by_other_particle(const Particle& other_particle);


	double m_mass;
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

	void update_forces_of_all_particles(double dt);

	void update_position_of_all_particles(double dt);

	void update_velocities_of_all_particles(double dt);

	void update_acceleration_of_all_particles(double dt);

	void run_simple_system(Vec2d screen_offset);

	void create_random_particles(unsigned int number_of_particles);

	std::vector < Particle > m_particle_vector;
	unsigned int m_max_x_coordinate;
	unsigned int m_max_y_coordinate;
};