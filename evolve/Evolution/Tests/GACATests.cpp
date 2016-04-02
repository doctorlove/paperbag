#include "catch.hpp"

#include "../GACA/GACA.h"
#include <bitset>
#include <sstream> 
using namespace GACA;

namespace Catch {
	//TODO get Cells to show in INFO
	//This doesn't work
	//See https://github.com/philsquared/Catch/blob/master/docs/tostring.md
    std::string toString(const GACA::Cells & cells) {
		////can't find 
		////binary '<<' : no operator found which takes a right-hand operand of type 
		////'const GACA::Cells' (or there is no acceptable conversion)
		//But there isa bool one - error report?
		//and this doesn't match
		std::stringstream ss;
		for(const auto & item : cells)
		{
			ss << std::boolalpha << item << " ";
		}
		return ss.str();
    }
}


TEST_CASE("FunctionRule of zeros means all die", "[alive]")
{
	FunctionRule fn = [](const Cells & cells)
		{
			return Cells(cells.size(), false);
		};
	Cells cells;
	cells.push_back(true);
	cells.push_back(false);//and more magic
	FullRule rule(fn);
	CAWorld world(rule, cells, 10);//yet another magic number
	world.Update();
	REQUIRE(0 == world.Alive());
}


TEST_CASE("All live when told to", "[alive]")
{
	FunctionRule fn = [](const Cells & cells)
		{
			return Cells(cells.size(), true);
		};
	Cells cells;
	cells.push_back(true);
	cells.push_back(false);//and more magic
	FullRule rule(fn);
	CAWorld world(rule, cells, 10);//yet another magic number
	world.Update();
	REQUIRE(2 == world.Alive());
}

///
//FunctionRule tells us for a given gene/phenotype what we get next
//000 -> xxx
//001 -> xxx
//010 -> xxx
//011 -> xxx
//100 -> xxx
//101 -> xxx
//110 -> xxx
//111 -> xxx
//
//The following test is more of an example rule than a required test per se
///

TEST_CASE("FunctionRule leaving cells as was when just one is alive does just that", "[rule]")
{
	FunctionRule fn = [](const Cells & cells)
		{
			Cells out(cells.size());
			bool as_was = 1 == std::count(cells.begin(), cells.end(), true);
			for(size_t i = 0; i < cells.size(); ++i)
			{
				out[i] = as_was ? cells[i] : false;
			}
			return out;
		};
	Cells cells;
	cells.push_back(true);
	cells.push_back(false);//and more magic
	FullRule rule(fn);
	CAWorld world(rule, cells, 0);//yet another magic number
	world.Update();
	REQUIRE(1 == world.Alive());
}


TEST_CASE("All come to life rule remembers previous state", "[alive]")
{
	FunctionRule fn = [](const Cells & cells)
		{
			return Cells(cells.size(), true);
		};
	Cells cells;
	cells.push_back(true);
	cells.push_back(false);//and more magic
	FullRule rule(fn);
	CAWorld world(rule, cells, 10);//yet another magic number
	REQUIRE( world.Alive(0, 0));
	REQUIRE(!world.Alive(1, 0));
	world.Update();
	REQUIRE( world.Alive(0, 0));
	REQUIRE(!world.Alive(1, 0));
	REQUIRE( world.Alive(0, 1));
	REQUIRE( world.Alive(1, 1));
}

TEST_CASE("Zero lag shows earlier cells as they were", "[fade]")
{
	FunctionRule fn = [](const Cells & cells)
		{
			return cells;
		};
	Cells cells;
	cells.push_back(true);
	cells.push_back(false);
	size_t lag = 0;
	size_t epochs = 5;
	FullRule unchanging(fn);
	CAWorld world(unchanging, cells, epochs, lag);
	const size_t alive = world.Alive();
	for(size_t i = 0; i < epochs; ++i)
	{
		world.Update();
		for(size_t y = 0; y < i; ++y)
		{
			for(size_t x = 0; x < cells.size(); ++x)
			{
				std::stringstream ss;
				ss << "Updates " << i << ", asking for history " << y << " at position " << x;
				INFO(ss.str());
				REQUIRE(world.Alive(x, y) == cells[x]);
			}
		}
	}
}

TEST_CASE("Cells 'in the future' i.e. for later epochs are dead", "[fade][alive]")
{
	FunctionRule fn = [](const Cells & cells)
		{
			return cells;
		};
	Cells cells;
	cells.push_back(true);
	cells.push_back(false);
	size_t lag = 1;
	size_t epochs = 5;
	FullRule unchanging(fn);
	CAWorld world(unchanging, cells, epochs, lag);
	const size_t alive = world.Alive();
	for(size_t i = 0; i < epochs; ++i)
	{
		world.Update();
		for(size_t y = i+2; y < epochs; ++y)
		{
			for(size_t x = 0; x < cells.size(); ++x)
			{
				std::stringstream ss;
				ss << "Updates " << i << ", asking for future " << y << " at position " << x;
				INFO(ss.str());
				REQUIRE(!world.Alive(x, y));
			}
		}
	}
}

TEST_CASE("Lag fades earlier cells", "[fade]")
{
	FunctionRule fn = [](const Cells & cells)
		{
			return cells;
		};
	Cells cells;
	cells.push_back(true);
	cells.push_back(false);
	FullRule unchanging(fn);
	for(size_t lag = 1; lag < 5; ++lag)//yet another magic number
	{
		size_t epochs = 5;
		CAWorld world(unchanging, cells, epochs, lag);
		const size_t alive = world.Alive();
		for(size_t updates = 0; updates < epochs; ++updates)
		{
			world.Update();
			for(size_t y = 0; y < updates; ++y)
			{
				for(size_t x = 0; x < cells.size(); ++x)
				{
					std::stringstream ss;
					ss << "Lag " << lag << ": Updates " << updates+1 << ", asking for history " << y << " at position " << x;
					INFO(ss.str());
					if((updates+1-y)>lag)
						REQUIRE(!world.Alive(x, y));
					else
						REQUIRE(cells[x] == world.Alive(x, y));
				}
			}
		}
	}
}


TEST_CASE("Mismatched input gene sizes not allow", "[crossover]")
{
	Lookup rule_short(3, 1);
	Lookup rule_long(4, 1);
	REQUIRE_THROWS_AS(crossover(rule_short, rule_long), std::invalid_argument);
}

//rule1, rule1 ->  identical rule after crossover
TEST_CASE("A rule mated with itself gives the same rule back", "[crossover]")
{
	Lookup rule(3, 1);
	//rule[x] = y means bit pattern x ends at bit pattern y
	//This test' rule therefore means all live

	Lookup new_rule = crossover(rule, rule);
	REQUIRE(new_rule == rule);
}

TEST_CASE("Mismatched gene sizes not allow", "[crossover]")
{
	Cells cell_short(3, false);
	Cells cell_long(4, false);
	REQUIRE_THROWS_AS(crossover(cell_short, cell_long), std::invalid_argument);
}

TEST_CASE("A cell mated with itself gives the same cells back", "[crossover]")
{
	Cells cell;
	cell.push_back(false);
	cell.push_back(false);
	cell.push_back(false);
	Cells new_cell = crossover(cell, cell);
	REQUIRE(new_cell == cell);
}

TEST_CASE("3-on cell mated with 3-off cell gives 100 cell", "[crossover]")
{
	Cells cell1(3, true);
	Cells cell2(3, false);
	Cells new_cell = crossover(cell1, cell2);
	Cells expected;
	expected.push_back(true);
	expected.push_back(false);
	expected.push_back(false);
	REQUIRE(new_cell == expected);
}

TEST_CASE("4-on cell mated with 4-off cell gives 1100 cell", "[crossover]")
{
	Cells cell1(4, true);
	Cells cell2(4, false);
	Cells new_cell = crossover(cell1, cell2);
	Cells expected;
	expected.push_back(true);
	expected.push_back(true);
	expected.push_back(false);
	expected.push_back(false);
	REQUIRE(new_cell == expected);
}

TEST_CASE("Crossover returns same size population as initial", "[crossover]")
{
	const size_t seed = 1;
	const size_t population_size = 4;
	Crossover crossover(seed, population_size);
	std::vector<size_t> fitness(population_size, 0);
	std::vector<Cells> initial_cells(population_size);
	std::vector<Cells> cells = crossover(initial_cells, fitness, initial_cells[0], false);
	REQUIRE(initial_cells.size() == cells.size());
}

TEST_CASE("binary 1000 is decimal 8", "[bits]")
{
	Cells cells;
	cells.push_back(true);
	cells.push_back(false);
	cells.push_back(false);
	cells.push_back(false);
	Lookup::value_type key = bits_to_number(cells);
	REQUIRE(key == 8);
}

TEST_CASE("decimal 8 is binary 1000", "[bits]")
{
	Cells cells;
	cells.push_back(true);
	cells.push_back(false);
	cells.push_back(false);
	cells.push_back(false);
	REQUIRE(cells == number_to_bits(8));
}

TEST_CASE("Lookup of zeros means everything dies", "[gene_rep]")
{
	Cells cell;
	cell.push_back(true);
	cell.push_back(false);
	cell.push_back(false);
	Lookup lookup(2 << cell.size(), 0);
	FunctionRule rule = lookup_to_rule(lookup);
	Cells next_cell = rule(cell);
	CHECK(!next_cell[0]);
	CHECK(!next_cell[1]);
	CHECK(!next_cell[2]);
}

TEST_CASE("Lookup of ones means everything lives", "[gene_rep]")
{
	Cells cell;
	cell.push_back(true);
	cell.push_back(false);
	cell.push_back(false);
	Lookup lookup(2 << cell.size(), (2 << (cell.size()+1)) - 1);
	FunctionRule rule = lookup_to_rule(lookup);
	Cells next_cell = rule(cell);
	CHECK(next_cell[0]);
	CHECK(next_cell[1]);
	CHECK(next_cell[2]);
}

TEST_CASE("FunctionRule missing for cells pattern is reported", "[gene_rep]")
{
	Cells cell;
	cell.push_back(false);
	cell.push_back(true);
	cell.push_back(false);
	Lookup lookup;
	lookup.push_back(0);
	FunctionRule rule = lookup_to_rule(lookup);
	REQUIRE_THROWS_AS(rule(cell), std::invalid_argument);
}

//TODO Consider using sections from Catch
void check_lookups(const Lookup & lookup, const Cells & cell)
{
	for(const auto & value : lookup)
	{
		auto byte_length = log(value)/log(2);//n.b. 7 doesn't come out as 3 so this is wrong, unless 2.80735 is close enough
		std::stringstream ss;
		//TODO what size for bitset?
		ss << "Got " << value << "=" << std::bitset<20>(value).to_string()<< ", want max " << cell.size();
		INFO(ss.str());
		REQUIRE(floor(byte_length) <= cell.size());
	}
	FunctionRule rule = lookup_to_rule(lookup);
	size_t original_size = cell.size();

	//TODO pulled from lookup_to_rule
	//either extract and test or get better tests round that
	Lookup::value_type key = 0;
	int i = 1;
	for(auto bit = cell.crbegin(), end = cell.crend(); bit!=end; ++bit)
	{
		if(*bit)
			key += i;
		i <<= 1;
	}

	Cells new_cells = number_to_bits(lookup[key]);
	REQUIRE(original_size >= new_cells.size());
}

TEST_CASE("Calling rule returns same sized cells as input cells","[update]")
{
    Cells cell;
	cell.push_back(false);
	cell.push_back(false);
	cell.push_back(false);
	cell.push_back(false);
	cell.push_back(true);
	auto seed = 942882080;//captured from a regression
	Lookup lookup = random_lookup_gen(seed, cell.size());
	check_lookups(lookup, cell);
}

TEST_CASE("Calling rule returns same sized cells as input cells v2","[update]")
{
	auto seed = 1851365057;
    Cells cell = random_cell_gen(seed, 5);
	seed = 1173577156;
	Lookup lookup = random_lookup_gen(seed, cell.size());
	check_lookups(lookup, cell);
}


TEST_CASE("Full rule cannot generate all the possible things for 32 bit inputs","[update]")
{
	//TODO - this is bad, so use JitRule instead
	auto seed = 2730990557;
	REQUIRE_THROWS_AS(random_lookup_gen(seed, 32), std::length_error);
}

TEST_CASE("31 bits is too long for the Lookup rule style","[update]")
{
	auto seed = 2730990557;
	Cells cell(31, false);
	REQUIRE_THROWS_AS(random_lookup_gen(seed, cell.size()), std::length_error);
}

TEST_CASE("Calling most_die rule returns same sized cells as input cells","[update]")
{
	auto seed = 3593929597;
    Cells cell = random_cell_gen(seed, 4);
	seed = 1311583758;
	Lookup lookup = random_lookup_gen_biased_to_die(seed, cell.size());
	check_lookups(lookup, cell);
}

TEST_CASE("Updating 1 0 0 0 0 returns 5 cells","[update]")
{
	Cells cell;
	cell.push_back(true);
	cell.push_back(false);
	cell.push_back(false);
	cell.push_back(false);
	cell.push_back(false);

	auto seed = 1173577156;
	Lookup lookup = random_lookup_gen(seed, cell.size());

	FunctionRule rule = lookup_to_rule(lookup);
	auto new_cell = rule(cell);
	auto s = Catch::toString(new_cell);
	INFO(s);
	REQUIRE(new_cell.size() == cell.size());
}

TEST_CASE("Lookup for 10000 is max 5 bytes","[update]")
{
	auto bits = std::bitset<5>(16);
	auto seed = 1173577156;
	Lookup lookup = random_lookup_gen(seed, 5);
	auto value = lookup[bits.to_ulong()];//TODO concern: size_t v ulong
	REQUIRE(value < 32);
}

TEST_CASE("JitRule adds items as required","[update]")
{
	size_t cell_size = 32;
	size_t seed = 2;
	JitRule rule(seed, cell_size);
	Cells cell1 = rule(Cells(cell_size, false));
	Cells cell2 = rule(Cells(cell_size, false));
	REQUIRE(cell1 == cell2);
}

TEST_CASE("More alive is fitter", "[fitness]")
{
	FunctionRule fn = [](const Cells & cells)
		{
			return Cells(cells.size(), true);
		};
	Cells cells;
	cells.push_back(true);
	cells.push_back(false);//and more magic
	FullRule rule(fn);
	CAWorld world1(rule, cells, 10);
	cells[1] = true;
	CAWorld world2(rule, cells, 10);
	REQUIRE(1 == fitest(world1, world2));
}

TEST_CASE("Tournament selection chooses the best of a given subset", "[fitness]")
{
	std::vector<size_t> fitness;
	fitness.push_back(0);
	fitness.push_back(10);
	fitness.push_back(5);
	fitness.push_back(100);
	REQUIRE(1 == tournament(fitness, 0, 1, 2));
	REQUIRE(1 == tournament(fitness, 0, 2, 1));
	REQUIRE(1 == tournament(fitness, 1, 0, 2));
	REQUIRE(1 == tournament(fitness, 1, 2, 0));
	REQUIRE(1 == tournament(fitness, 2, 0, 1));
	REQUIRE(1 == tournament(fitness, 2, 1, 0));
}

TEST_CASE("Tournament selection chooses either of equally good inputs", "[fitness]")
{
	std::vector<size_t> fitness;
	fitness.push_back(0);
	fitness.push_back(0);
	fitness.push_back(10);
	fitness.push_back(10);
	REQUIRE(3 == tournament(fitness, 3, 3, 1));
	REQUIRE(3 == tournament(fitness, 3, 1, 3));
	REQUIRE(3 == tournament(fitness, 1, 3, 3));
	REQUIRE(3 == tournament(fitness, 1, 1, 3));
	REQUIRE(3 == tournament(fitness, 1, 3, 3));
	REQUIRE(3 == tournament(fitness, 1, 3, 1));
}

TEST_CASE("Mutation returns same gene size as it starts with", "[mutation]")
{
	const size_t length = 3;
	Mutation mutation(1, length);
	Cells cell = mutation.mutate(Cells(length, false));
	REQUIRE(cell.size() == length);
}

TEST_CASE("Mutation might not change anything", "[mutation]")
{
	//We might want to change more than one in the long run
	const size_t length = 3;
	Mutation mutation(1, length);
	Cells cell = mutation.mutate(Cells(length, false));
	Cells expected;
	expected.push_back(false);
	expected.push_back(false);
	expected.push_back(false);
	REQUIRE(cell == expected);
}

