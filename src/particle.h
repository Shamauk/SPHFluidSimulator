#ifndef PARTICLE_H
#define PARTICLE_H

#include <glm/glm.hpp>

class Particle {
public:
    // Default constructor
    Particle() : position(glm::dvec2(0.0)),
                 velocity(glm::dvec2(0.0)),
                 acceleration(glm::dvec2(0.0)),
                 pressure(0.0),
                 density(0.0) {}

    // Constructor with initial position and velocity
    Particle(const glm::dvec2& initial_position, const glm::dvec2& initial_velocity)
        : position(initial_position),
          velocity(initial_velocity),
          acceleration(glm::dvec2(0.0)),
          pressure(0.0),
          density(0.0) {}

    // Particle properties
    glm::dvec2 position;      
    glm::dvec2 velocity;      
    glm::dvec2 acceleration;  
    double pressure;          
    double density;    
    double mass = 0.1;    
};

#endif