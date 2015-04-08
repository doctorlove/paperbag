#include "Bee.h"

#include <algorithm>
#include <iostream>

std::vector<size_t> BeeColony::wagglingBees(const std::vector<BeeColony::Bee> & bees)
{
	std::vector<size_t> sorted;
	for(size_t i = 0; i < bees.size(); ++i)
	{
		if(bees[i].job() != BeeColony::INACTIVE)
		{
			sorted.push_back(i);
		}
	}
	std::sort(sorted.begin(), sorted.end(), 
		[=](const size_t & lhs, const size_t & rhs)
		{
			auto lhsBee = bees[lhs];
			auto rhsBee = bees[rhs];
			//TODO the food or the current pos?
			return quality(lhsBee.food_x(), lhsBee.food_y())
					> quality(rhsBee.food_x(), rhsBee.food_y());
		});
	return sorted;
}


//TODO - Use max visits
BeeColony::Hive BeeColony::makeHive(int totalBees, int numberActive, int numberScout, int maxVisits, int steps, float width, float buzz, float beeSize, float min_x, float max_x)
{
	using namespace BeeColony;
	if (totalBees < numberActive + numberScout)
		throw std::out_of_range("active bees and scout bees must be fewer than total bees");
	std::mt19937 engine;
	std::uniform_real_distribution<> food(min_x, max_x);
	std::vector<Bee> bees;
	double x = width/2.0;
	double y = 10.0f;
	for(int i=0; i < numberActive; ++i)
	{
		//double food_x = food(engine);
		double food_x = max_x;
		bees.push_back(Bee(WORKER, x, y, buzz, food_x, y));
	}
	for(int i=0; i < numberScout; ++i)
	{
		bees.push_back(Bee(SCOUT, x, y, buzz));
	}
	for(int i=0; i < totalBees - numberActive - numberScout; ++i)
	{
		bees.push_back(Bee(INACTIVE, x, y, buzz));
	}
	return BeeColony::Hive(bees, steps);
}


void BeeColony::Bee::waggle(double jiggle) 
{
	//depends on type
	//depends on quality of solution - so why not just return quality?
	//something must show this to all inactive bees
	if (job() == INACTIVE)
		return;
	
	//could send in best so far
	x_ += jiggle;

}

double BeeColony::quality(double x, double y)
{
	return y;
}

void BeeColony::Bee::scout(double x_move, double y_move)
{
	//check quality and if better do it
	double new_x = x_ + buzz_ * x_move;
	double new_y = y_ + buzz_ * y_move;
	double new_quality = BeeColony::quality(new_x, new_y);
	if (new_quality > quality(x_, y_))
	{
		x_ = new_x;
		y_ = new_y;
	}
	if (new_quality >= quality(food_x_, food_y_))
	{
		food_x_ = x_;
		food_y_ = y_;
	}
}

void BeeColony::Bee::work(double x_move, double y_move)
{
	//need some idea of a food source to go to and choose a neighbour (solution) possibly mutated
	//how do we go to food initially? Make a bee-line for it. Boom-Tish.
	if (y_ > food_y_)
		y_ -= buzz_/2;
	if (y_ < food_y_)
		y_ += buzz_/2;
	if (x_ < food_x_)
		x_ += buzz_/2;
	if (x_ > food_x_)
		x_ -= buzz_/2;
	double new_quality = BeeColony::quality(x_+x_move, y_ + y_move);
	if (new_quality >= BeeColony::quality(x_, y_))
	{
		x_ += x_move;
		y_ += y_move;
	}
}

void BeeColony::Bee::goHome()
{
	if (!home())
	{
		if (BeeColony::quality(x_, y_) > BeeColony::quality(food_x_, food_y_))
		{
			food_x_ = x_;
			food_y_ = y_;
		}
		if (y_ > home_y_)
			y_ -= buzz_;
		if (y_ < home_y_)
			y_ += buzz_;
		if (x_ < home_x_)
			x_ += buzz_;
		if (x_ > home_x_)
			x_ -= buzz_;
	}
}

void BeeColony::Hive::updateBees()
{
	//either moving 
	//or waggling then updating
	if (step_ < steps_)
	{
		++step_;
		for(auto & bee : bees_)
		{
			//TODO - let the bees decide what state they are in e.g. scouts can finish
			switch(bee.job())
			{
			case WORKER:
				bee.work(normal_dist(engine), normal_dist(engine));
				break;
			case SCOUT:
				bee.scout(normal_dist(engine), normal_dist(engine));
				break;
			}
		}
	}
	else if(!home())
	{
		for(auto & bee : bees_)
		{
			if (!bee.home())
				bee.goHome();
			else
				bee.waggle(normal_dist(engine));
		}
	}
	else if(waggles_)
	{
		for(auto & bee : bees_)
		{
			bee.waggle(normal_dist(engine));
		}
		--waggles_;
	}
	else 
	{
		//maybe abandom some rubbish ones workers were using
		//just let this happen by ordering the best bees and choosing one of the top few
		//swap bee roles
		std::vector<size_t> choices = wagglingBees(bees_);
		auto old_bees = bees_;

		for(auto & bee : bees_)
		{
			const auto currentJob = bee.job();
			size_t choice = uniform_dist(engine);
			if (choice < choices.size())
			{
				//TODO maybe just do this sometimes
				swap(bees_[choice], bee);
			}
			else
			{
				//fudge to keep it moving for now
				bee.job(currentJob, bee.food_x(), bee.food_y());
			}
			//std::cout << choice << ", " << bee.job() << ", " << bee.food_x() << ", " <<  bee.food_y() << '\n';
		}
		reset();
		//std::cout << '\n';
	}
}

bool BeeColony::Hive::home() const 
{
	return bees_.end() == std::find_if(bees_.begin(), bees_.end(), [](const Bee & bee) { return !bee.home(); });
}

void BeeColony::Hive::swarm()
{
	//new home is a worker bee's position
	double best_x = -1.0, best_y = -1.0;
	//TODO which home is voted for? Just go for the best one
	for(const auto & bee : bees_)
	{
		if(bee.job() == WORKER)
		{
			if(bee.food_y() > best_y)
			{
				best_y = bee.food_y();
				best_x = bee.food_x();
			}
		}
	}
	for(auto & bee : bees_)
	{
		bee.moveHome(best_x, best_y);
	}
	//reset();
	//actually... try to stop the inactive ones just jumping
	step_ = steps_;
}

void BeeColony::swap(BeeColony::Bee & lhs, BeeColony::Bee & rhs)
{
	auto temp = lhs;

	//the job only updates the food if it's better
	 lhs.job( rhs.job(), rhs.food_x(), rhs.food_y());
	temp.job(temp.job(), rhs.food_x(), rhs.food_y());

	rhs = temp;
}

bool BeeColony::shouldSwarm(const std::vector<BeeColony::Bee> & bees, double target)
{
	return bees.end() == std::find_if(bees.begin(), bees.end(), 
		[target](const Bee & bee) {
			return bee.food_y() < target;
	});
}
