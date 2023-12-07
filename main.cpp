
#include <iostream>
#include "simlib.h"

// mass
double initial_mass = 0.15;

// thrust force
double thrust_force = 100;
double burnout_time = 10;
double specific_impulse = 2500;

// gravity force
double gravity_constant = 10;

// drag force
double air_density_constant = 1.2;
double drag_coeficient = 0.3;
double rocket_area = 0.000377;

class Rocket {

	Integrator mass;
	Integrator velocity;
	Integrator altitude;

	public:

	Rocket() :
		mass(- current_thrust_force() / specific_impulse, initial_mass),
		velocity(get_acceleration(), 0),
		altitude(velocity, 0)
	{}

	double current_thrust_force() {
		if (T.Value() < burnout_time) {
			return thrust_force;
		}
		return 0;
	}

	double current_gravity_force() {
		return mass.Value() * gravity_constant;
	}

	double current_drag_force() {
		return 0.5 * air_density_constant * drag_coeficient * rocket_area * velocity.Value() * velocity.Value();
	}

	double current_net_force() {
		return current_thrust_force() - current_drag_force() - current_gravity_force();
	}

	double get_acceleration() {
		return current_net_force() / mass.Value();
	}

	void out() {
		Print("%g %g %g %g\n", T.Value(), velocity.Value(), altitude.Value(), current_gravity_force());
	}
};

Rocket rocket;

void Sample() { 
  rocket.out();
}

Sampler S(Sample, 1e-1);

int main() {
	SetOutput("sim.dat");
	Init(0, 15);
	SetAccuracy(1e-3);

	Run();
}