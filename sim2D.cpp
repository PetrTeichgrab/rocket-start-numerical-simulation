
#include <iostream>
#include <math.h>

#include "simlib.h"
#include "simlib2D.h"

// mass
double initial_mass = 0.5;

// thrust force
bool use_thrust_curve = false;
std::string thrust_curve_input = "data/f15_thrust_curve.dat";
double thrust_force = 14.4;
double burnout_time = 3.5;
double specific_impulse = 826.666666667;

// gravity force
double gravity_constant = 9.7803267715;

// drag force
double air_density_constant = 1.2;
double drag_coeficient = 0.3;
double rocket_area = 0.000377;

Constant2D zero(0, 0);

double launch_angle = 0.174532925199433;
double launch_pole_length = 2;

class ThrustForce : public aContiBlock2D {

	Expression2D flight_angle;

	public:
	ThrustForce() : flight_angle(zero) {}

	Value2D Value() {
		if (T.Value() < burnout_time) {			
			return Value2D(thrust_force * sin(flight_angle.Value().x()), thrust_force * cos(flight_angle.Value().x()));
		}
		return Value2D(0, 0);
	}

	void SetInput(Input2D i) {
		flight_angle.SetInput(i);
	}

};

class DragForce : public aContiBlock2D {

	Expression2D flight_angle;
	Expression2D velocity;

	public:
	DragForce() : flight_angle(zero), velocity(zero) {}

	Value2D Value() {
		double mag = sqrt(pow(velocity.Value().x(), 2) + pow(velocity.Value().y(), 2));
		double calc = 0.5 * air_density_constant * drag_coeficient * rocket_area * pow(mag, 2);

		return Value2D(calc * sin(flight_angle.Value().x()), calc * cos(flight_angle.Value().x()));
	}

	void SetInput(Input2D angle, Input2D v) {
		flight_angle.SetInput(angle);
		velocity.SetInput(v);
	}
};

class Mass : public aContiBlock {

	Expression2D thrust_force;
	Integrator mass;

	public:
	Mass(Input2D _tf) : 
		thrust_force(_tf),
		mass(-sqrt(pow(thrust_force.Value().x(), 2) + pow(thrust_force.Value().y(), 2)) / specific_impulse, initial_mass) 
	{}

	double Value() {
		return mass.Value();
	}
};

class GravityForce : public aContiBlock2D {

	Expression2D flight_angle;
	Expression mass;

	public:
	GravityForce(Input _m) : flight_angle(zero), mass(_m) {}

	Value2D Value() {
		double calc = mass.Value() * gravity_constant;
		return Value2D(calc * sin(flight_angle.Value().x()), calc * cos(flight_angle.Value().x()));
	}

	void SetInput(Input2D angle) {
		flight_angle.SetInput(angle);
	}
};

class NetForce : public aContiBlock2D {

	Expression2D ft;
	Expression2D fd;
	Expression2D fg;

	public:
	NetForce(Input2D _ft, Input2D _fd, Input2D _fg) : ft(_ft), fd(_fd), fg(_fg) {}

	Value2D Value() {
		return ft.Value() - fd.Value() - fg.Value();
	}
};

class Velocity : public aContiBlock2D {

	Integrator2D v;

	public:
	Velocity(Input2D F) : v(F, Value2D(0, 0)) {}

	Value2D Value() {
		return v.Value();
	}
};

class Position : public aContiBlock2D {

	Integrator2D p;

	public:
	Position(Input2D v) : p(v, Value2D(0, 0)) {}

	Value2D Value() {

		if (p.Value().y() < -10) {
			Stop();
		}

		return p.Value();
	}
};

class FlightAngle : public aContiBlock2D {

	Expression2D v;
	Expression2D p;

	public:
	FlightAngle(Input2D _v, Input2D _p) : v(_v), p(_p) {}

	Value2D Value() {
		double angle = atan(v.Value().x() / v.Value().y());

		if (isnan(angle)) {
			angle = 0;
		}

		if (p.Value().x() < launch_pole_length * sin(launch_angle) || p.Value().y() < launch_pole_length * cos(launch_angle)) {
			angle = launch_angle;
		}

		return Value2D(angle, 0);
	}
};


ThrustForce F_t;
DragForce F_d;
Mass m(F_t);
GravityForce F_g(m);
NetForce F(F_t, F_d, F_g);
Velocity velocity(F);
Position position(velocity);
FlightAngle flight_angle(velocity, position);

void Sample() {	
	// Print("thrust force: %g %g\n", F_t.Value().x(), F_t.Value().y());
	// Print("drag force: %g %g\n", F_d.Value().x(), F_d.Value().y());
	// Print("mass: %g\n", m.Value());
	// Print("gravity force: %g %g\n", F_g.Value().x(), F_g.Value().y());
	// Print("net force: %g %g\n", F.Value().x(), F.Value().y());
	// Print("velocity: %g %g\n", velocity.Value().x(), velocity.Value().y());
	// Print("position: %g %g\n", position.Value().x(), position.Value().y());
	// Print("angle: %g\n", flight_angle.Value().x());

	Print("%g %g %g\n", T.Value(), position.Value().x(), position.Value().y());
}

Sampler S(Sample, 1e-1);

int main() {
	SetOutput("sim.dat");
	Init(0, 100);
	SetAccuracy(1e-3);

	F_t.SetInput(flight_angle);
	F_d.SetInput(flight_angle, velocity);
	F_g.SetInput(flight_angle);

	Run();
}