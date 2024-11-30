#ifndef PARTICLE_HEADER
#define PARTICLE_HEADER

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

struct Particle {
  enum Type { Propellant, Smoke, ParticleCount };

  sf::Vector2f position;
  sf::Color color;
  sf::Time lifetime;
};

#endif  // PARTICLE_HEADER
