
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "simlib.h"

// mass
double initial_mass = 0.5;

// thrust force
bool use_thrust_curve = false;
std::string thrust_curve_input = "data/f15_thrust_curve.dat";
double thrust_force = 14.4;
double burnout_time = 3.5;
double specific_impulse = 826.666666667;

// gravity force
double gravity_constant = 10;

// drag force
double air_density_constant = 1.2;
double drag_coeficient = 0.3;
double rocket_area = 0.000377;

class ThrustForce : public aContiBlock {
	
	int index;
	std::vector<std::pair<double, double>> thrust_curve_values;

	public:
	ThrustForce() : index(0) {
		if (use_thrust_curve) {
			std::ifstream file(thrust_curve_input);
			
			if (!file.is_open()) {
				std::cerr << "Error opening the file!" << std::endl;
				return;
			}

			std::string line;
			while (std::getline(file, line)) {
				std::istringstream iss(line);
				double value1, value2;

				if (iss >> value1 >> value2) {
					thrust_curve_values.push_back(std::make_pair(value1, value2));
				}
			}

			file.close();
		}
	}

	double Value() {
		if (use_thrust_curve) {
			std::pair<double, double> value = thrust_curve_values[index];
			if (T.Value() < burnout_time && T.Value() < value.first) {
				return value.second;
			} else if (T.Value() < burnout_time) {
				value = thrust_curve_values[index++];
				return value.second;
			}
		} else {
			if (T.Value() < burnout_time) {
				return thrust_force;
			}
		}
		return 0;
	}
};

class Rocket {

	Expression drag_force;
	Expression gravity_force;
	Expression net_force;

	Integrator mass;

	Expression a;
	Integrator v;
	Integrator y;

	public:
	Rocket(
		Input thrust_force
	) : 
		mass(- thrust_force / specific_impulse, initial_mass),
		gravity_force(mass * gravity_constant),
		drag_force(0.5 * air_density_constant * drag_coeficient * rocket_area * v * v),
		net_force(thrust_force - gravity_force - drag_force),
		a(net_force / mass),
		v(a, 0),
		y(v, 0)
	{}

	void Out() {
		if (mass.Value() < 0) {
			Abort();
		}

		if (y.Value() < 0) {
			Stop();
		}

		Print("%g %g %g\n", T.Value(), v.Value(), y.Value());
	}
};

ThrustForce tf;
Rocket r(tf);

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