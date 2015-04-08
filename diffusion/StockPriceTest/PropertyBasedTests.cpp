#include "quickcheck/quickcheck.hh" //http://software.legiasoft.com/git/quickcheck.git
#include "../Lib/PriceSimulation.h"
#include <numeric>
#include <stdexcept>
#include <vector>

using namespace quickcheck;

class ZeroVolPriceGenerator
{
public:
	ZeroVolPriceGenerator() :
		start_price_(0),
		drift_(0.1),
		dt_(1),
		sims_(1),
		price_(0, drift_, 0.0, 1)
	{
	}

	void reset(double start_price, double drift, double dt, int sims)
	{
		//if (sims <= 0) throw std::range_error("More than zero simulations required"); //it might try this
		start_price_ = start_price;
		drift_ = drift;
		dt_ = dt;
		sims_ = sims;
		price_ = MC101::PriceSimulation(start_price_, drift_, 0.0, dt_, 1);//what about seed?
	}


	double StartPrice() const { return start_price_; }
	double Drift() const { return drift_; }
	double Dt() const { return dt_; }
	int Sims() const { return sims_; }

	std::vector<double> prices() const 
	{
		std::vector<double> prices;

		for(int i=0; i<sims_; ++i)
		{
			prices.push_back(price_.update());
		}
		return prices;
	}

private:
	double start_price_;
	double drift_;
	double dt_;
	int sims_;
	mutable MC101::PriceSimulation price_;
};

std::ostream & operator << (std::ostream & os, const ZeroVolPriceGenerator & gen)
{
	os << "Start price " << gen.StartPrice() << ", drift " << gen.Drift() << ", dt " << gen.Dt() << ", sims " << gen.Sims() << '\n';
    for(const auto & price : gen.prices())
	{
		os << price << '\n';
	}
	return os;
}

void generate(size_t n, ZeroVolPriceGenerator & out)
{
	double price;
	double drift;
	double dt;
	int sims;
	generate(n, price);
	generate(n, drift);
	//generate(n, dt);
	dt = 0.1;
	generate(n, sims);
	out.reset(price, drift, dt, sims);
}

class ZeroVolGivesDrift : public Property<ZeroVolPriceGenerator> {

	bool holdsFor(const ZeroVolPriceGenerator& gen)
	{
		//Aside - closed form v sims: http://quant.stackexchange.com/questions/4589/how-to-simulate-stock-prices-with-a-geometric-brownian-motion
			//to answer BK's exp question
		//"price returns are normally distributed with expected return (mean) "m" and standard deviation "s"."
		//http://www.investopedia.com/articles/07/montecarlo.asp
		//Price returns are normally distributed.
		//The *prices* are log normal
		//Does this mean over several sims?
		//Should I take into account the time step here?
		std::vector<double> xs = gen.prices();
		double tot = 0;
		double previous = gen.StartPrice();
		for(const auto & p : xs)
		{
			tot += (p - previous)/previous;
			previous = p;
		}
		auto average = tot/xs.size();//TODO - drift v. mu
		auto scaled = std::abs(average/gen.Dt() - gen.Drift())/gen.Drift();
		return scaled <= 1.0;
	}

	bool accepts(const ZeroVolPriceGenerator& gen)
	{
		return gen.Sims() >= 1 //is 0 ok?
			&& gen.Dt() > 0.0
			&& std::abs(gen.Drift()) < 50.0;
		//Beware - we can fall out of the range of a double:
			// 5.41822e+307, -143.251
			//-1.#INF, -1.#INF
			//-1.#IND, -1.#IND
			//-1.#IND, -1.#IND
			//-1.#IND, -1.#IND
			//-1.#IND, -1.#IND
			//-1.#IND, -1.#IND
			//-1.#IND, -1.#IND
			//-1.#IND, -1.#IND
			//-1.#IND, -1.#IND
			//-1.#IND, -1.#IND
			//Falsifiable after 300 tests for input:
			//  0: Start price 100, drift -143.251, dt 1, sims 151
		//but then it can't find anything suitable:
		//Arguments exhausted after 88 tests.

	}
};

class ZeroStartPriceGenerator
{
public:
	ZeroStartPriceGenerator() :
		drift_(0.1),
		dt_(1),
		sims_(1),
		price_(0, drift_, 0.0, 1)
	{
	}

	void reset(double drift, double dt, int sims, unsigned int seed)
	{
		//if (sims <= 0) throw std::range_error("More than zero simulations required"); //it might try this
		drift_ = drift;
		dt_ = dt;
		//hmmm - poisson doesn't like negative numbers
		if (dt < 0) dt *= 1;
		sims_ = sims;
		if (seed == 0) seed = 1;//calls the "holds for" *after this?! Mind you, stoopid VS2012
		seed_ = seed;
		price_ = MC101::PriceSimulation(0.0, drift_, 0.0, dt_, seed);
	}


	double Seed() const { return seed_; }
	double Drift() const { return drift_; }
	double Dt() const { return dt_; }
	int Sims() const { return sims_; }

	std::vector<double> prices() const 
	{
		std::vector<double> prices;

		for(int i=0; i<sims_; ++i)
		{
			prices.push_back(price_.update());
		}
		return prices;
	}

private:
	double drift_;
	double dt_;
	int sims_;
	double seed_;
	mutable MC101::PriceSimulation price_;
};

std::ostream & operator << (std::ostream & os, const ZeroStartPriceGenerator & gen)
{
	os << "Seed " << gen.Seed() << ", drift " << gen.Drift() << ", dt " << gen.Dt() << ", sims " << gen.Sims() << '\n';
    for(const auto & price : gen.prices())
	{
		os << price << '\n';
	}
	return os;
}

void generate(size_t n, ZeroStartPriceGenerator & out)
{
	double drift;
	double dt;
	int sims;
	unsigned int seed;
	generate(n, drift);
	generate(n, dt);
	generate(n, sims);
	generate(n, seed);
	if (dt < 0) dt *= -1;//hmmm
	out.reset(drift, dt, sims, seed);
}

class ZeroStartPriceGivesZero : public Property<ZeroStartPriceGenerator> {

	bool holdsFor(const ZeroStartPriceGenerator& gen)
	{
		std::vector<double> xs = gen.prices();
		for(const auto & p : xs)
		{
			if (p != 0.0) return false;
		}
		return true;
	}

	bool accepts(const ZeroStartPriceGenerator& gen)
	{
		return gen.Dt() > 0.0
			&& gen.Seed() > 0;//for VS2012
//		return gen.Sims() >= 1 //is 0 ok?
	//		&& gen.Dt() > 0.0
		//	&& std::abs(gen.Drift()) < 50.0;
	}
};



int main()
{
	//check<ZeroVolGivesDrift>("testing", 10, true);//verbose output *before* testing
	ZeroVolGivesDrift zeroVol;
	zeroVol.check(250);

	ZeroStartPriceGivesZero zeroStartPrice;
	zeroStartPrice.check(100);

	//Could add mean jumps test
}
