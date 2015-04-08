#define CATCH_CONFIG_MAIN
#include <catch.hpp> //https://github.com/philsquared/Catch.git

//first parametrer is test name
//second is a tag for filtering, like
//UnitTests.exe [Bees]

#include "../Lib/PriceSimulation.h"
#include "../Bees/Bee.h"

#include <random>


TEST_CASE("Test that random numbers in have mean 0", "[Simples]")
{
	std::mt19937 engine;
	std::normal_distribution<> normal_dist;

	REQUIRE(normal_dist.mean() == 0.0);
}

TEST_CASE("Test that random numbers in have standard deviation 1", "[Simples]")
{
	std::mt19937 engine;
	std::normal_distribution<> normal_dist;

	REQUIRE(normal_dist.stddev() == 1.0);
}

//TODO - do I want (-inf, +inf) or what?

TEST_CASE("Test that a stock price simulation starting at 0 remains at 0", "[Property]")
{
	const double start_price = 0.0;
	const double dt    = 0.1;//or whatever
	const unsigned int seed = 1;//or whatever
	MC101::PriceSimulation price(start_price, 0.3, 0.2, dt, seed);

	REQUIRE(price.update() == 0.0);
}

TEST_CASE("Test that dt parameter gets set to requested value", "[ParseArgs]")
{
	int sims = 1;
	double dt = 0.5;
	double drift = 0.2;
	double vol = 0.3;
	double jump = 0.0;
	double mean_jump_per_unit_time = 0.1;
	
	int argc = 3;
	double expected_value = 0.400000006;
	const char *argv[] = {"prog name", "sims", "0.4"};
	dt = get_value(argc, &argv[0], 2, dt);
	//REQUIRE(expected_value == dt);//more precision needed
//	Catch::toString
	REQUIRE(Catch::toString(expected_value) == Catch::toString(dt));
}

//Note run just the bees like this
//Debug\UnitTests.exe [Bees]
TEST_CASE("Test that bee has initially stated job", "[Bees]")
{
	using namespace BeeColony;
	Job job = WORKER;
	Bee bee(job);
	REQUIRE(bee.job() == job);
}

TEST_CASE("Test that updateBees keeps same number of scout bees", "[Bees][Update]")
{
	using namespace BeeColony;
	std::vector<BeeColony::Bee> bees;
	bees.push_back(Bee(WORKER));
	bees.push_back(Bee(WORKER));
	bees.push_back(Bee(SCOUT));
	bees.push_back(Bee(INACTIVE));
	Hive hive(bees, 1);
	hive.updateBees();
	auto updated = hive.bees();
	int scout = 0;
	for(const auto & bee : updated) //count_if
	{
		if (bee.job() == SCOUT)
			++scout;
	}
	REQUIRE(scout == 1);
}

TEST_CASE("Test that inactive bee reports it is home", "[Bees][Home]")
{
	using namespace BeeColony;
	Bee bee(INACTIVE);
	REQUIRE(bee.home());
}

TEST_CASE("Test that hive of inactive bee reports they are home", "[Bees][Home]")
{
	using namespace BeeColony;
	std::vector<BeeColony::Bee> bees;
	bees.push_back(Bee(INACTIVE));
	Hive hive(bees, 1);
	REQUIRE(hive.home());
}

TEST_CASE("Test that makeHive starts with bees at home", "[Bees][Home]")
{
	const float width = 500.0f;
	BeeColony::Hive hive = BeeColony::makeHive(3, 1, 1, 5, 100, width, 10.0, 10.0, 0, 100);
	REQUIRE(hive.home());
}

TEST_CASE("Test that bees will all go home in the end", "[Bees][Home]")
{
	const float width = 500.0f;
	int steps = 100;
	BeeColony::Hive hive = BeeColony::makeHive(3, 1, 1, 5, steps, width, 10.0, 10.0, 0, 100);
	hive.updateBees();	//bool home = hive.home();
	//REQUIRE(!home); //sometimes fails since they may not actually move
	while (!hive.home())
		hive.updateBees();
	REQUIRE(hive.home());
}

TEST_CASE("Test that a scout bee becoming a worker bee remembers it's best food source", "[Bees][JobUpdate]")
{
	using namespace BeeColony;
	double start_food_x = 100.0;
	double start_food_y = 300.0;
	Bee bee(SCOUT, 0.0, 0.0, 5.0, start_food_x, start_food_y);
	double initialQuality = quality(bee.x(), bee.y());
	bee.job(WORKER, 200, 0);
	REQUIRE(BeeColony::quality(bee.food_x(), bee.food_y()) > initialQuality);
}

TEST_CASE("Test that a worker bee becoming a scout bee takes on board better food source", "[Bees][JobUpdate]")
{
	using namespace BeeColony;
	double start_food_x = 100.0;
	double start_food_y =   0.0;
	Bee bee(WORKER, 0.0, 0.0, 5.0, start_food_x, start_food_y);
	double initialQuality = quality(bee.x(), bee.y());
	bee.job(SCOUT, 200, 400);
	REQUIRE(BeeColony::quality(bee.food_x(), bee.food_y()) > initialQuality);
}

TEST_CASE("Test that scout will try anything when on first outing", "[Bees][Scout]")
{
	using namespace BeeColony;
	const double buzz = 5.0;
	Bee bee(SCOUT, 0.0, 0.0, buzz);
	const double move = 10;
	bee.scout(0, move);
	REQUIRE(!bee.home());
	REQUIRE(quality(bee.x(), bee.y()) == move * buzz);
	while(!bee.home())
		bee.goHome();
	REQUIRE(bee.home());
	bee.job(SCOUT, 0, 0);
	bee.scout(0, move/2);
	REQUIRE(!bee.home());
	REQUIRE(quality(bee.x(), bee.y()) == move/2 * buzz);
}

TEST_CASE("Test that wagglingBees only contains non-INACTIVE bees", "[Bees][Waggle]")
{
	using namespace BeeColony;
	std::vector<BeeColony::Bee> bees;
	bees.push_back(Bee(INACTIVE));
	bees.push_back(Bee(INACTIVE));
	std::vector<size_t> someBees = wagglingBees(bees);
	REQUIRE(someBees.size() == 0);
}

TEST_CASE("Test that wagglingBees puts best bee first", "[Bees][Waggle]")
{
	using namespace BeeColony;
	std::vector<BeeColony::Bee> bees;
	const double buzz = 5.0;
	bees.push_back(Bee(SCOUT, 0.0, 0.0, buzz));
	bees.push_back(Bee(SCOUT, 0.0, 0.0, buzz));
	bees[0].scout(0, 10);
	bees[1].scout(0, 20);
	std::vector<size_t> someBees = wagglingBees(bees);
	REQUIRE(someBees.size() == 2);
	REQUIRE(bees[someBees[0]].y() == 20*buzz);
	REQUIRE(bees[someBees[1]].y() == 10*buzz);
}

TEST_CASE("Test that worker bees will go home", "[Bees][Home]")
{
	using namespace BeeColony;
	const double buzz = 5.0;
	Bee bee(WORKER, 0.0, 0.0, buzz);
	bee.work(10, 5);
	REQUIRE(!bee.home());
	while(!bee.home())
	{
		double last_x = bee.x();
		bee.goHome();
		REQUIRE(bee.x() < last_x);
	}
}

TEST_CASE("Test that swapping bees swaps jobs", "[Bees][Swap]")
{
	using namespace BeeColony;
	const double buzz = 5.0;
	Bee bee(WORKER, 0.0, 0.0, buzz);
	Bee bee2(INACTIVE, 0.0, 0.0, buzz);
	swap(bee, bee2);
	REQUIRE( bee.job() == INACTIVE);
	REQUIRE(bee2.job() == WORKER);
}

TEST_CASE("Test that swapping bees keeps best food source", "[Bees][Swap]")
{
	using namespace BeeColony;
	const double buzz = 5.0;
	Bee bee(WORKER, 0.0, 0.0, buzz, 1.0, 1.0);
	Bee bee2(INACTIVE, 0.0, 0.0, buzz, 2.0, 2.0);
	swap(bee, bee2);
	REQUIRE( bee.food_x() == 2.0);
	REQUIRE( bee.food_y() == 2.0);
	REQUIRE(bee2.food_x() == 2.0);
	REQUIRE(bee2.food_y() == 2.0);
}

TEST_CASE("Test that rubbish bees do not swarm", "[Bees][Swarm]")
{
	using namespace BeeColony;
	std::vector<Bee> bees;
	bees.push_back(Bee(WORKER, 0.0, 0.0, 0.0, 0.0, 0.0));
	REQUIRE(!shouldSwarm(bees, 10));
}

TEST_CASE("Test that good bees do swarm", "[Bees][Swarm]")
{
	using namespace BeeColony;
	std::vector<Bee> bees;
	bees.push_back(Bee(WORKER, 0.0, 0.0, 0.0, 15.0, 15.0));
	REQUIRE(shouldSwarm(bees, 10));
}

void testHome(BeeColony::Job job)
{
	using namespace BeeColony;
	const double buzz = 5.0;
	Bee bee(SCOUT, 5.0, 5.0, buzz, 8.0, 8.0);
	bee.moveHome(10.0, 10.0);
	REQUIRE(bee.food_x() == 8.0);
	REQUIRE(bee.food_y() == 8.0);
	bee.goHome();
	bee.goHome();
	REQUIRE(bee.x() == 10.0);
	REQUIRE(bee.y() == 10.0);
}

TEST_CASE("Test that bee below a new home goes towards new home when told", "[Bees][goHome]")
{
	testHome(BeeColony::WORKER);
	testHome(BeeColony::INACTIVE);
	testHome(BeeColony::SCOUT);
}
