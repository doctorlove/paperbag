#ifndef BEE_INCLUDED
#define BEE_INCLUDED

#include <random>
#include <vector>

namespace BeeColony
{

	enum Job 
	{
		WORKER,
		INACTIVE,
		SCOUT
	};

	class Hive;
	BeeColony::Hive makeHive(int totalBees, int numberActive, int numberScout, int maxVisits, int steps, float width, float buzz, float beeSize, float min_x, float max_x);

	//overall quality of solution - the higher the better - leave y a big is good then do height minus to draw it
	double quality(double x, double y);

	class Bee;
	std::vector<size_t> wagglingBees(const std::vector<BeeColony::Bee> & bees);

	class Bee
	{
	public:
		explicit Bee(Job job, double x = 0.0, double y = 0.0, double buzz = 5.0, double food_x = 0.0, double food_y = 0.0)
			: job_(job),
			x_(x),
			y_(y),
			home_x_(x),
			home_y_(y),
			buzz_(buzz),
			food_x_(food_x),
			food_y_(food_y)
		{
		}

		Job job() const
		{
			return job_;
		}

		void job(Job job, double new_food_x, double new_food_y)
		{
			job_ = job;
			if (BeeColony::quality(new_food_x, new_food_y) > quality(food_x_, food_y_))
			{
				food_x_ = new_food_x;
				food_y_ = new_food_y;
			}
			x_ = home_x_;
			y_ = home_y_;
		}

		void scout(double x_move, double y_move);
		void work(double x_move, double y_move);
		
		void goHome();
	
		bool home() const
		{
//			return (y_ <= home_y_) && (x_ > home_x_ - buzz_) && (x_ < home_x_ + buzz_);
			return (y_ > home_y_ - buzz_) && (y_ < home_y_ + buzz_) 
				&& (x_ > home_x_ - buzz_) && (x_ < home_x_ + buzz_);
		}

		void waggle(double jiggle);

		double x() const { return x_; }
		double y() const { return y_; }
		double food_x() const { return food_x_; }
		double food_y() const { return food_y_; }
		void moveHome(double new_x, double new_y) 
		{
			home_x_ = new_x;
			home_y_ = new_y;
			//and new food?
		}

	private:
		Job job_;
		double x_;
		double y_;
		double home_x_;
		double home_y_;
		double buzz_;
		double food_x_;
		double food_y_;
	};

	class Hive
	{
	public: 
		Hive(std::vector<BeeColony::Bee> bees, int steps)
			: bees_(bees), steps_(steps), step_(0), waggles_(25),
				engine(std::random_device()()),
				uniform_dist(0, bees.size()-1)
		{
		}

		std::vector<BeeColony::Bee> bees()
		{
			return bees_;
		}

		void updateBees();

		bool home() const;
		void swarm();


	private:
		std::vector<BeeColony::Bee> bees_;
		int steps_;
		int step_;
		int waggles_;
		std::mt19937 engine;
		std::normal_distribution<double> normal_dist;
		std::uniform_int_distribution<> uniform_dist;

		void reset()
		{
			step_ = 0;
			waggles_ = 25;
		}
	};

	void swap(Bee & lhs, Bee & rhs);
	bool shouldSwarm(const std::vector<BeeColony::Bee> & bees, double target);
}

#endif
