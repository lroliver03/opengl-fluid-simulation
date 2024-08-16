#include "fluid/fluid.h"
#include "utils/macros.h"

Fluid::Fluid() {
  // Does nothing.
}

Fluid::~Fluid() {
  delete particles;
  delete density;
  delete pressure;
}

inline unsigned int Fluid::grid_t::getIndex(const unsigned &x, const unsigned &y) {
  return x * this->height + y;
}

void Fluid::init() {
  // Inits grid.
  
  // Setting dimensions.
  const unsigned grid_width = (this->container.right - this->container.left) / Phy.SMOOTHING_RADIUS;
  const unsigned grid_height = (this->container.top - this->container.bottom) / Phy.SMOOTHING_RADIUS;

  // Grid matrix init.
  this->grid.width = grid_width;
  this->grid.height = grid_height;

  this->grid.vec.resize(grid_width * grid_height);

  return;
}

void Fluid::initParticles(const unsigned int &particle_count) {
  count = particle_count;
  particles = new particle_t[count];
  int temp_counter = 0;
  for (int i = 0; i < 100; i++) {
    for (int j = 0; j < 50; j++) {
      particles[temp_counter] = {
        .position = {
          .x = SETTOINTERVAL(j, 50.f, 0.f, this->container.right/4.f, this->container.left/4.f),
          .y = SETTOINTERVAL(i, 50.f, 0.f, this->container.top/4.f, this->container.bottom/4.f),
          .z = 0.f // SETTOINTERVAL(i, 20.f, 0.f, .3f, -.3f)
        },
        .velocity = {
          .x = 0.f,
          .y = 0.f,
          .z = 0.f
        },
        .grid_position {
          .x = 0,
          .y = 0
        }
      };
      temp_counter++;
      if (!(temp_counter - count)) break;
    }
    if (!(temp_counter - count)) break;
  }

  density = new float[count];
  pressure = new float[count];

  // this->updateGrid();
}

void Fluid::setBoundaries(const float &top, const float &bottom, const float &left, const float &right) {
  container = {
    .top = top,
    .bottom = bottom,
    .left = left,
    .right = right
  };
}

inline void Fluid::accelerate(const unsigned &particle, const vec3f &dv) {
  this->particles[particle].velocity += dv;
}

inline void Fluid::updateGrid() {
  for (unsigned int i = 0; i < this->count; ++i) {

    const unsigned int new_grid_x = (unsigned int)((this->particles[i].position.x - this->container.left) / Phy.SMOOTHING_RADIUS);
    const unsigned int new_grid_y = (unsigned int)((this->particles[i].position.y - this->container.bottom) / Phy.SMOOTHING_RADIUS);

    // If grid position hasn't changed, skip particle.
    if (new_grid_x == this->particles[i].grid_position.x && new_grid_y == this->particles[i].grid_position.y)
      continue;

    // Else...
    unsigned old_index = this->grid.getIndex(this->particles[i].grid_position.x, this->particles[i].grid_position.y);
    unsigned new_index = this->grid.getIndex(new_grid_x, new_grid_y);

    auto old_begin = this->grid.vec.at(old_index).begin(), old_end = this->grid.vec.at(old_index).end();
    auto new_begin = this->grid.vec.at(new_index).begin(), new_end = this->grid.vec.at(new_index).end();

    this->particles[i].grid_position.x = new_grid_x;
    this->particles[i].grid_position.y = new_grid_y;

    this->grid.vec.at(old_index).erase(std::remove(old_begin, old_end, i), old_end);
    this->grid.vec.at(new_index).push_back(i);

  }
}

inline void Fluid::updateDensity(const unsigned &particle) {
  float new_density = 0.f;

  const int x_lower_limit = std::max(this->particles[particle].grid_position.x - 1, 0);
  const int x_upper_limit = std::min(this->particles[particle].grid_position.x + 1, this->grid.width - 1);
  const int y_lower_limit = std::max(this->particles[particle].grid_position.y - 1, 0);
  const int y_upper_limit = std::min(this->particles[particle].grid_position.y + 1, this->grid.height - 1);

  for (int i = x_lower_limit; i <= x_upper_limit; ++i) {
    for (int j = y_lower_limit; j <= y_upper_limit; ++j) {
      for (auto &neighbor : this->grid.vec.at(this->grid.getIndex(i, j))) {
        new_density += Phy.spikyKernel(this->particles[particle].position - this->particles[neighbor].position);
        // std::cout << "Evaluated " << neighbor << "th particle against " << particle << "th particle.\n";
      }
    }
  }

  this->density[particle] = new_density;
}

inline void Fluid::updateAllDensities() {
  // Updates all densities.
  for (int i = 0; i < count; ++i) {
    this->updateDensity(i);
  }
}

inline void Fluid::updateAllPressures() {
  // Simplest update. Gets pressure values from Phy.getPressure.
  for (int i = 0; i < count; ++i) {
    this->pressure[i] = Phy.getPressure(this->density[i]);
    // if (std::isnan(this->pressure[i])) {
    //   std::cout << "Pressure of " << i << "th particle is NaN\n";
    // }
  }
}

inline void Fluid::updateVelocity(const unsigned &particle, const float &timestep) {
  vec3f pressure_force = {.0f, .0f, .0f};
  vec3f viscosity_force = {.0f, .0f, .0f};

  const int x_lower_limit = std::max(this->particles[particle].grid_position.x - 1, 0);
  const int x_upper_limit = std::min(this->particles[particle].grid_position.x + 1, this->grid.width - 1);
  const int y_lower_limit = std::max(this->particles[particle].grid_position.y - 1, 0);
  const int y_upper_limit = std::min(this->particles[particle].grid_position.y + 1, this->grid.height - 1);

  for (int i = x_lower_limit; i <= x_upper_limit; ++i) {
    for (int j = y_lower_limit; j <= y_upper_limit; ++j) {
      for (auto &neighbor : this->grid.vec.at(this->grid.getIndex(i, j))) {
        // Compute pressure forces.
        pressure_force += (
          Phy.gradSpikyKernel(this->particles[particle].position - this->particles[neighbor].position) 
          * -(this->pressure[particle] + this->pressure[neighbor])/(2.f * this->density[neighbor])
        ); // Symmetric pressure force, equation 10 of [1].

        // Compute viscosity forces.
        viscosity_force += (
          (this->particles[neighbor].velocity - this->particles[particle].velocity)
          *
          (Phy.laplSpikyKernel(
              this->particles[particle].position - this->particles[neighbor].position
            ) / this->density[neighbor])
        );
        // std::cout << "Evaluated " << neighbor << "th particle against " << particle << "th particle.\n";
      }
    }
  }

  viscosity_force *= Phy.VISCOSITY_CONSTANT;

  vec3f dv = (pressure_force + viscosity_force + Phy.GRAVITY) * timestep;

  this->accelerate(particle, dv);
}

inline void Fluid::updateAllVelocities(const float &timestep) {
  for (int i = 0; i < this->count; ++i) {
    this->updateVelocity(i, timestep);
  }
}

inline void Fluid::handleBorderCollisions(const unsigned int &particle) {
  // Check x position.
  if (this->particles[particle].position.x < this->container.left + EPSILON) {
    this->particles[particle].position.x = this->container.left + EPSILON;
    this->particles[particle].velocity.x *= -1.f;
  } else if (this->particles[particle].position.x > this->container.right - EPSILON) {
    this->particles[particle].position.x = this->container.right - EPSILON;
    this->particles[particle].velocity.x *= -1.f;
  }
  // Check y position.
  if (this->particles[particle].position.y < this->container.bottom + EPSILON) {
    this->particles[particle].position.y = this->container.bottom + EPSILON;
    this->particles[particle].velocity.y *= -1.f;
  } else if (this->particles[particle].position.y > this->container.top - EPSILON) {
    this->particles[particle].position.y = this->container.top - EPSILON;
    this->particles[particle].velocity.y *= -1.f;
  }
}

inline void Fluid::updateAllPositions(const float &timestep) {
  for (int i = 0; i < this->count; ++i) {
    this->particles[i].position += this->particles[i].velocity * timestep;

    this->handleBorderCollisions(i);
  }
}

void Fluid::update(const float &timestep) {
  this->updateAllDensities();
  this->updateAllPressures();
  this->updateAllVelocities(timestep);
  this->updateAllPositions(timestep);
  this->updateGrid();
}

Fluid fluid;
