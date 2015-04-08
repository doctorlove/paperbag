#ifndef PRICE_SIMULATION_INCLUDED
#define PRICE_SIMULATION_INCLUDED

#include <chrono>
#include <random>

namespace MC101
{
	class PriceSimulation
	{
	public:
		PriceSimulation(double price, double drift, double vol, double dt,
			unsigned int seed = std::chrono::high_resolution_clock::now().time_since_epoch().count(),
			double jump = 0.0, double mean_jump_per_unit_time = 0.1);

		double update();

	private:
		double price;
		double drift;
		double vol;
		double dt;
		double jump;

		std::mt19937 engine;
		std::normal_distribution<> normal_dist;//note defaults are good for us
		std::poisson_distribution<> poisson_dist;
	};
}

double get_value(int argc, const char **argv, int pos, double value);

#endif
