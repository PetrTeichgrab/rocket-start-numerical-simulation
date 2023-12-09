
#include <iostream>
#include "simlib.h"

// mass
double initial_mass = 0.5;

// thrust force
double thrust_force = 10;
double burnout_time = 10;
double specific_impulse = 2500;

// gravity force
double gravity_constant = 10;

// drag force
double air_density_constant = 1.2;
double drag_coeficient = 0.3;
double rocket_area = 0.000377;

class ThrustForce : public aContiBlock {	
	
	double thrust_force, burnout_time;
	
	public:
	ThrustForce(double _tf, double _bt) : thrust_force(_tf), burnout_time(_bt) {}

	double Value() {
		if (T.Value() < this->burnout_time) {
			return this->thrust_force;
		}
		return 0;
	}

	void Out() {
		Print("[%g]\tthrust force = %g\n", T.Value(), this->Value());
	}
};

class Mass : public aContiBlock {
	
	Integrator mass;

	public:
	Mass(Input thrust_force, double initial_mass, double specific_impulse) : 
		mass(- thrust_force / specific_impulse, initial_mass) 
	{}

	double Value() {
		if (mass.Value() <= 0) {
			Print("invalid mass\n");
			Abort();
		}
		return mass.Value();
	}

	void Out() {
		Print("[%g]\tmass = %g\n", T.Value(), this->Value());
	}
};

class GravityForce : public aContiBlock {

	Expression gravity_force;

	public:
	GravityForce(Input mass, double gravity_constant) :
		gravity_force(mass * gravity_constant)
	{}

	double Value() {
		return this->gravity_force.Value();
	}

	void Out() {
		Print("[%g]\tgravitaty force = %g\n", T.Value(), this->Value());
	}
};

class Rocket {

	Expression drag_force;
	Expression net_force;

	Expression a;
	Integrator v;
	Integrator y;

	public:
	Rocket(
		Input mass, Input thrust_force, Input gravity_force,
		double air_density_constant, double drag_coeficient, double rocket_area
	) : 
		drag_force(0.5 * air_density_constant * drag_coeficient * rocket_area * v * v),
		net_force(thrust_force - gravity_force - drag_force),
		a(net_force / mass),
		v(a, 0),
		y(v, 0)
	{}

	void Out() {
		if (y.Value() < 0) {
			Stop();
		}
		Print("%g %g %g\n", T.Value(), v.Value(), y.Value());
	}
};

ThrustForce tf(thrust_force, burnout_time);
Mass m(tf, initial_mass, specific_impulse);
GravityForce gf(m, gravity_constant);
Rocket r(m, tf, gf, air_density_constant, drag_coeficient, rocket_area);

void Sample() {
	r.Out();
}

Sampler S(Sample, 1e-1);

int main() {
	SetOutput("sim.dat");
	Init(0, 100);
	SetAccuracy(1e-3);

	Run();
}