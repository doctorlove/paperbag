#include "PriceSimulation.h"

#include <string>
using namespace MC101;

//http://en.cppreference.com/w/cpp/numeric/random


PriceSimulation::PriceSimulation(double price, double drift, double vol, double dt, unsigned int seed, double jump, double mean_jump_per_unit_time)
	:
	price(price),
	drift(drift),
	vol(vol),
	dt(dt),
	engine(seed),//TODO send in the seed: also why is 0 not allowed?
	//and see http://stackoverflow.com/questions/15509270/does-stdmt19937-require-warmup
	jump(jump),
	poisson_dist(mean_jump_per_unit_time * dt)
{
}

double PriceSimulation::update()
{
  double stochastic = normal_dist(engine);
  double dn = poisson_dist(engine);//beware http://stackoverflow.com/questions/9870541/using-one-random-engine-for-multi-distributions-in-c11
  //See http://demonstrations.wolfram.com/OptionPricesInMertonsJumpDiffusionModel/ for jump details
  double increment  = drift * dt + vol * sqrt(dt) * stochastic + jump * dn;
  price += price * increment;
  return price;
}

double get_value(int argc, const char **argv, int pos, double value)
{
	if (argc >= pos+1)
	{
		try
		{
			value = std::stof(argv[pos]);
		}
		catch(const std::exception &)
		{
			//I feel bad...
		}
	}
	return value;
}

