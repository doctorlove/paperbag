#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "../GameOfLife/GoL.h"

using namespace GoL;

/*
namespace Catch { // CATCH won't find this in the global namespace
	//ignoring this
	std::ostream& operator<<(std::ostream& o, std::vector<int> const& s) {
    for(auto&& i : s)
        o << i << ' ';
    return o;
}
*/
namespace Catch {
	//See https://github.com/philsquared/Catch/blob/master/docs/tostring.md
    std::string toString(std::pair<size_t, size_t> const& value ) {
		return "(" + toString(value.first) + ", " + toString(value.second) + ")";
    }
}

TEST_CASE("Cells start dead by default", "[setup]")
{
	GoLWorld world(1, 1, false);
	REQUIRE(!world.Alive(0, 0));
}

TEST_CASE("Sparking an alive cell reports error", "[setup]")
{
	const size_t max_x = 6;
	const size_t max_y = 6;
	GoLWorld world(max_x, max_y, false);
	world.Spark(2, 5);
	REQUIRE_THROWS_AS(world.Spark(2, 5), std::invalid_argument);
}

TEST_CASE("random_setup detects GoLWorld width too small", "[setup]")
{
	REQUIRE_THROWS_AS(random_setup(2, 10, 1, 10, false), std::invalid_argument);
}

TEST_CASE("random_setup detects GoLWorld height too small", "[setup]")
{
	REQUIRE_THROWS_AS(random_setup(10, 2, 10, 1, false), std::invalid_argument);
}

TEST_CASE("random_setup puts sparks inside bag boundaries", "[setup]")
{
	size_t max_x = 3;
	size_t max_y = 3;
	size_t world_x = 3;
	size_t world_y = 6;
	GoLWorld world = random_setup(max_x, max_y, world_x, world_y, false);//or true -property based tests would be better
	for(size_t y=max_y; y<world.Width(); ++y)
	{
		for(size_t x=0; x<world.Height(); ++x)
		{
			REQUIRE_FALSE(world.Alive(x, y));//TODO or get a list of failures
		}
	}
}

TEST_CASE("Spark brings to life", "[impl]")
{
	GoLWorld world(2, 1, false);
	REQUIRE(!world.Alive(0, 0));
	REQUIRE(!world.Alive(1,0));
	world.Spark(0,0);
	world.Spark(1,0);
	REQUIRE(world.Alive(0,0));
	REQUIRE(world.Alive(1,0));
}

TEST_CASE("Eight neighbours of inner cell found", "[impl]")
{
	size_t neighbours = 0;
	walk_neighbours(1, 1, 3, 3, 
		[&neighbours](size_t, size_t)
		{
			++neighbours;
		}
	);
	REQUIRE(neighbours == 8);
}

TEST_CASE("Eight neighbours of edge cell found when wrapping", "[impl]")
{
	std::vector<std::pair<size_t, size_t>> points;
	walk_neighbours_with_wrapping(1, 0, 3, 3, 
		[&](size_t x, size_t y)
		{
			points.push_back(std::make_pair(x, y));
		}
	);
	std::vector<std::pair<size_t, size_t>> v;
	//meh - EVERYWHERE! (except itself)
	v.push_back(std::make_pair(0,2));
	v.push_back(std::make_pair(1,2));
	v.push_back(std::make_pair(2,2));

	v.push_back(std::make_pair(0,0));
	v.push_back(std::make_pair(2,0));

	v.push_back(std::make_pair(0,1));
	v.push_back(std::make_pair(1,1));
	v.push_back(std::make_pair(2,1));

	REQUIRE(points == v);
}
/*
TEST_CASE("Eight neighbours of inner cell found have correct coords", "[impl]")//TODO - name better
{
	std::vector<std::pair<size_t, size_t>> neighbours;
	walk_neighbours(1, 1, 3, 3, 
		[&neighbours](size_t x , size_t y)
		{
			neighbours.push_back(std::make_pair(x, y));
		}
	);
	std::vector<std::pair<size_t, size_t>> expected;
	expected.push_back(std::make_pair(0,0));
	expected.push_back(std::make_pair(1,0));
	expected.push_back(std::make_pair(2,0));
	expected.push_back(std::make_pair(1,1));
	expected.push_back(std::make_pair(3,1));
	expected.push_back(std::make_pair(1,3));
	expected.push_back(std::make_pair(2,3));
	expected.push_back(std::make_pair(3,3));
	REQUIRE(neighbours == expected);
}
*/

TEST_CASE("Five neighbours of left edge cell found", "[impl]")
{
	size_t neighbours = 0;
	walk_neighbours(0, 1, 3, 3, 
		[&neighbours](size_t, size_t)
		{
			++neighbours;
		}
	);
	REQUIRE(neighbours == 5);
}

TEST_CASE("Eight neighbours of inner cell in default GoLWorld all dead", "[impl]")
{
	const size_t max_x = 3;
	const size_t max_y = 3;
	GoLWorld world(max_x, max_y, false);
	size_t countAlive = 0;
	walk_neighbours(1, 1, max_x, max_y, [&](size_t xi, size_t yi)
		{
			countAlive += world.Alive(xi, yi);
		});
	REQUIRE(countAlive == 0);
}

TEST_CASE("Fewer than two live neighbours dies", "[alive]")
{
	GoLWorld world(3, 3, false);
	REQUIRE(!world.Alive(1,1));
}

TEST_CASE("Live cell with one live neighbours dies", "[alive]")
{
	GoLWorld world(3, 3, false);
	world.Spark(1, 0);
	world.Spark(1, 1);
	world.Update();
	REQUIRE(!world.Alive(1, 1));
}

TEST_CASE("Dead cell with one live neighbours dies", "[alive]")
{
	GoLWorld world(3, 3, false);
	world.Spark(1, 0);
	world.Update();
	REQUIRE(!world.Alive(1, 1));
}

TEST_CASE("Live cell with two live neighbours lives", "[alive]")
{
	GoLWorld world(3, 3, false);
	world.Spark(0, 0);
	world.Spark(1, 0);
	world.Spark(1, 1);
	world.Update();
	REQUIRE(world.Alive(1, 1));
}

TEST_CASE("Dead cell with two live neighbours dies", "[alive]")
{
	GoLWorld world(3, 3, false);
	world.Spark(0, 0);
	world.Spark(1, 0);
	world.Update();
	REQUIRE(!world.Alive(1, 1));
}

TEST_CASE("Live cell with three live neighbours lives", "[alive]")
{
	GoLWorld world(3, 3, false);
	world.Spark(0, 0);
	world.Spark(1, 0);
	world.Spark(2, 0);

	world.Spark(1, 1);

	world.Update();
	REQUIRE(world.Alive(1, 1));
}

TEST_CASE("Dead cell with three live in horizontal line neighbours comes alive", "[alive]")
{
	GoLWorld world(3, 3, false);
	world.Spark(0, 0);
	world.Spark(1, 0);
	world.Spark(2, 0);

	world.Update();
	REQUIRE(world.Alive(1, 1));
}

TEST_CASE("Dead cell with three live in vertical line neighbours comes alive", "[alive]")
{
	GoLWorld world(3, 3, false);
	world.Spark(1, 0);
	world.Spark(1, 1);
	world.Spark(1, 2);

	world.Update();
	REQUIRE( world.Alive(0, 1));
	REQUIRE(!world.Alive(1, 0));
	REQUIRE( world.Alive(1, 1));
	REQUIRE(!world.Alive(1, 2));
	REQUIRE( world.Alive(2, 1));
}

TEST_CASE("More than three live neighbours when live dies", "[alive]")
{
	GoLWorld world(3, 3, false);
	world.Spark(0, 0);
	world.Spark(1, 0);
	world.Spark(2, 0);
	world.Spark(2, 1);

	world.Spark(1, 1);
	
	world.Update();
	REQUIRE(!world.Alive(1, 1));
}

TEST_CASE("More than three live neighbours when dead dies", "[alive]")
{
	GoLWorld world(3, 3, false);
	world.Spark(0, 0);
	world.Spark(1, 0);
	world.Spark(2, 0);
	world.Spark(2, 1);
	world.Update();
	REQUIRE(!world.Alive(1, 1));
}

TEST_CASE("Horizontal line of three along top of bag can go out", "[alive]")
{
	size_t world_x = 50;
	size_t world_y = 60;
	size_t bag_top = 40;
	bool wrap = false;
	GoLWorld world(world_x, world_y, wrap);
	world.Spark(world_x/2-1, bag_top);
	world.Spark(world_x/2, bag_top);
	world.Spark(world_x/2+1, bag_top);
	REQUIRE(3 == world.Alive());
	world.Update();
	REQUIRE(3 == world.Alive());
}

TEST_CASE("First cell of glider dies after first update", "[glider]")
{
	GoLWorld world(10, 10, false);
	glider(world, 1, 1);
	REQUIRE(world.Alive(1,1));
	world.Update();
	REQUIRE(!world.Alive(1,1));
}

TEST_CASE("Second cell of glider alive after first update", "[glider]")
{
	GoLWorld world(10, 10, false);
	glider(world, 1, 1);
	REQUIRE(world.Alive(2, 1));
	world.Update();
	REQUIRE(world.Alive(2, 1));
}

TEST_CASE("Third cell of glider alive after first update", "[glider]")
{
	GoLWorld world(10, 10, false);
	glider(world, 1, 1);
	REQUIRE(world.Alive(3, 1));
	world.Update();
	REQUIRE(world.Alive(3, 1));
}

TEST_CASE("Top of glider dies after first update", "[glider]")//TODO - other steps
{
	GoLWorld world(10, 10, false);
	glider(world, 1, 1);
	REQUIRE(world.Alive(2,3));
	world.Update();
	REQUIRE(!world.Alive(2,3));
}

TEST_CASE("Glider remains with 5 alive cells after each of 5 updates", "[glider]")
{
	//TODO- specify hitting the edges case
	GoLWorld world(10, 10, false);
	glider(world, 1, 5);

	for(int i=0; i<6; ++i)
	{
		size_t alive = world.Alive();
		world.Update();
		INFO(i);
		REQUIRE(alive == 5);
	}
}


TEST_CASE("Glider state 1 moves to state 2", "[glider]")
{
	GoLWorld world(10, 10, false);
	world.Spark(1, 1);
	world.Spark(2, 1);
	world.Spark(3, 1);
	world.Spark(3, 2);
	world.Spark(2, 3);

	world.Update();
	CHECK(!world.Alive(0, 0));
	CHECK(!world.Alive(1, 0));
	CHECK( world.Alive(2, 0));
	CHECK(!world.Alive(3, 0));
	CHECK(!world.Alive(0, 1));
	CHECK(!world.Alive(1, 1));
	CHECK( world.Alive(2, 1));
	CHECK( world.Alive(3, 1));
	CHECK(!world.Alive(0, 2));
	CHECK( world.Alive(1, 2));
	CHECK(!world.Alive(2, 2));
	CHECK( world.Alive(3, 2));
}

TEST_CASE("Glider state 2 moves to state 3", "[glider]")
{
	GoLWorld world(10, 10, false);
	world.Spark(1, 1);
	world.Spark(1, 2);
	world.Spark(2, 2);
	world.Spark(0, 3);
	world.Spark(2, 3);

	world.Update();
	CHECK(!world.Alive(0, 1));
	CHECK( world.Alive(1, 1));
	CHECK( world.Alive(2, 1));
	CHECK( world.Alive(0, 2));
	CHECK(!world.Alive(1, 2));
	CHECK( world.Alive(2, 2));
	CHECK(!world.Alive(0, 3));
	CHECK(!world.Alive(1, 3));
	CHECK( world.Alive(2, 3));
}

TEST_CASE("Glider state 3 moves to state 4", "[glider]")
{
	GoLWorld world(10, 10, false);
	world.Spark(1, 1);
	world.Spark(2, 1);
	world.Spark(0, 2);
	world.Spark(2, 2);
	world.Spark(2, 3);

	world.Update();
	CHECK(!world.Alive(0, 1));
	CHECK( world.Alive(1, 1));
	CHECK( world.Alive(2, 1));
	CHECK(!world.Alive(3, 1));
	CHECK(!world.Alive(0, 2));
	CHECK(!world.Alive(1, 2));
	CHECK( world.Alive(2, 2));
	CHECK( world.Alive(3, 2));
	CHECK(!world.Alive(0, 3));
	CHECK( world.Alive(1, 3));
	CHECK(!world.Alive(2, 3));
	CHECK(!world.Alive(3, 3));
}

TEST_CASE("Glider state 4 moves to state 1", "[glider]")
{
	GoLWorld world(10, 10, false);
	world.Spark(1, 1);
	world.Spark(2, 1);
	world.Spark(2, 2);
	world.Spark(3, 2);
	world.Spark(1, 3);

	world.Update();
	CHECK(!world.Alive(0, 1));
	CHECK( world.Alive(1, 1));
	CHECK( world.Alive(2, 1));
	CHECK( world.Alive(3, 1));
	CHECK(!world.Alive(0, 2));
	CHECK(!world.Alive(1, 2));
	CHECK(!world.Alive(2, 2));
	CHECK( world.Alive(3, 2));
	CHECK(!world.Alive(0, 3));
	CHECK(!world.Alive(1, 3));
	CHECK( world.Alive(2, 3));
	CHECK(!world.Alive(3, 3));
}

TEST_CASE("Three cell vertical line remains at three cells after update", "[stable]")
{
	const size_t size = 3;
	GoLWorld world(size, size, false);
	world.Spark(1, 0);
	world.Spark(1, 1);
	world.Spark(1, 2);

	REQUIRE(world.Update());
	size_t countAlive = world.Alive();
	REQUIRE(countAlive == 3);
}


TEST_CASE("Three cell line is stable", "stable")
{
	const size_t size = 5;
	SECTION("horizontal")
	{
		GoLWorld world(size, size, false);
		world.Spark(0, 1);
		world.Spark(1, 1);
		world.Spark(2, 1);

		REQUIRE( world.Update());
		REQUIRE(!world.Update());
	}
	SECTION("vertical")
	{
		GoLWorld world(size, size, false);
		world.Spark(1, 0);
		world.Spark(1, 1);
		world.Spark(1, 2);

		REQUIRE( world.Update());
		REQUIRE(!world.Update());
	}
}

TEST_CASE("Empty GoLWorld is dead", "[dead]")
{
	GoLWorld world(3, 4, false);
	REQUIRE(world.Dead());
}

TEST_CASE("Non-Empty GoLWorld is not dead", "[dead]")
{
	GoLWorld world(3, 4, false);
	world.Spark(1, 2);
	REQUIRE(!world.Dead());
}

TEST_CASE("GoLWorld with single point dies after update", "[dead]")
{
	GoLWorld world(3, 4, false);
	world.Spark(1, 2);
	world.Update();
	REQUIRE(world.Dead());
}

TEST_CASE("Three cell horizontal line on edge remains at three cells when not wrapping", "[wrapping]")
{
	const size_t size = 3;
	GoLWorld world(size, size, false);
	world.Spark(0, 2);
	world.Spark(1, 2);
	world.Spark(2, 2);

	REQUIRE(world.Update());
	REQUIRE(world.Alive() == 2);
	REQUIRE(world.Update());
	REQUIRE(world.Dead());
}

TEST_CASE("Bottom middle cell has three neighbours from top horizontal row when wrapping", "[wrapping]")
{
	const size_t size = 3;
	GoLWorld world(size, size, true);
	world.Spark(0, 2);
	world.Spark(1, 2);
	world.Spark(2, 2);

	size_t aliveNeighours = 0;
	walk_neighbours_with_wrapping(1, 0, size, size, [&](size_t xi, size_t yi)
		{
			aliveNeighours += world.Alive(xi, yi);
		});
	REQUIRE(aliveNeighours == 3);
}

TEST_CASE("Three cell horizontal line on edge remains at three cells on larger grid when wrapping", "[wrapping]")
{
	const size_t size = 5;
	GoLWorld world(size, size, true);
	world.Spark(1, 4);
	world.Spark(2, 4);
	world.Spark(3, 4);

	std::vector<std::pair<size_t, size_t>> v, expected;
	expected.push_back(std::make_pair(2,0));
	expected.push_back(std::make_pair(2,3));
	expected.push_back(std::make_pair(2,4));
	REQUIRE(world.Update());
	for(size_t y=0; y<world.Width(); ++y)
	{
		for(size_t x=0; x<world.Height(); ++x)
		{
			if(world.Alive(x, y))
				v.push_back(std::make_pair(x,y));
		}
	}
	REQUIRE(v == expected);

	REQUIRE(world.Alive() == 3);
	REQUIRE(!world.Update());//note it now matches what we started with
}

TEST_CASE("Three cell vertical line running over edge remains at three cells on larger grid when wrapping", "[wrapping]")
{
	const size_t size = 5;
	GoLWorld world(size, size, true);
	world.Spark(2, 0);
	world.Spark(2, 3);
	world.Spark(2, 4);

	std::vector<std::pair<size_t, size_t>> v, expected;
	expected.push_back(std::make_pair(1,4));
	expected.push_back(std::make_pair(2,4));
	expected.push_back(std::make_pair(3,4));
	REQUIRE(world.Update());
	for(size_t y=0; y<world.Width(); ++y)
	{
		for(size_t x=0; x<world.Height(); ++x)
		{
			if(world.Alive(x, y))
				v.push_back(std::make_pair(x,y));
		}
	}
	REQUIRE(v == expected);
}

TEST_CASE("Three cell horizontal line on edge makes GoLWorld alive on 3x3 grid when wrapping", "[wrapping]")
{
	const size_t size = 3;
	GoLWorld world(size, size, true);
	world.Spark(0, 2);
	world.Spark(1, 2);
	world.Spark(2, 2);

	std::vector<std::pair<size_t, size_t>> v, expected;
	expected.push_back(std::make_pair(1,0));//not this
	expected.push_back(std::make_pair(1,1));
	expected.push_back(std::make_pair(1,2));
	REQUIRE(world.Update());
	REQUIRE(world.Alive() == 9);
}

