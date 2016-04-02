#include "GACA.h"

#include <algorithm>
#include <bitset>
#include <iostream>
#include <stdexcept>
#include <sstream>

using namespace GACA;

CAWorld::CAWorld(const Rule & rules, Cells cell, size_t epochs, size_t lag) :
	rules(rules),
	cell(cell),
	epochs(epochs),
	lag(lag)
{
}

bool CAWorld::Update()
{
	if(history.size() == epochs)
		return false;
	history.push_back(cell);
	size_t previous_size = cell.size();
	cell = rules(cell);
	if(cell.size() != previous_size)
	{
		std::stringstream ss;
		ss << "Size of cells changed on update.\nPreviously: ";
		for(const auto & item : history.back())
			ss << item << " ";
		ss << "\nNow: ";
		for(const auto & item : cell)
			ss << item << " ";
		throw std::logic_error(ss.str());
	}
	return true;
}

size_t CAWorld::Alive() const 
{
	return std::count(cell.begin(), cell.end(), true);
}

bool CAWorld::Alive(size_t x, size_t y) const
{
	size_t available = history.size();
	if(y > available)
		return false;
	if(y < available)
	{
		if(lag && available > lag)
		{
			if(y < available-lag)
				return false;
		}
		return history[y][x];
	}
	return cell[x];
}

size_t CAWorld::Height() const
{
	///
	//In theory we would not set a ceiling/number of epochs for cellular automaton
	// however, combining this with a genetic algorithm means we want a run to stop at some point
	// in order to "improve" (better rules, or better starting cells)
	///
	return epochs;
}

size_t CAWorld::Width() const
{
	return cell.size();
}

Lookup GACA::crossover(Lookup rule1, Lookup rule2)
{
	//TODO - consider having a ga class, to seed random? (if we do a random crossover point)
	if( rule1.size() != rule2.size() )
		throw std::invalid_argument("rule input sizes must match");

	size_t split = rule1.size()/2;
	Lookup new_rule;
	new_rule.assign(rule1.begin(), rule1.begin() + split);
	new_rule.insert(new_rule.end(), rule2.begin() + split, rule2.end());

	return new_rule;
}

Cells GACA::crossover(Cells cell1, Cells cell2)
{
	if( cell1.size() != cell2.size() )
		throw std::invalid_argument("cell sizes must match");

	size_t split = cell1.size()/2;
	Cells new_cell;
	new_cell.assign(cell1.begin(), cell1.begin() + split);
	new_cell.insert(new_cell.end(), cell2.begin() + split, cell2.end());

	return new_cell;
}

Crossover::Crossover(std::random_device::result_type seed, size_t population_size) :
    gen(seed),
    uniform_dist(0, population_size-1)
{
}

std::vector<Cells> Crossover::operator()(const std::vector<Cells> & pop,
							 const std::vector<size_t> & fitness,
							 const Cells & best_cell,
							 bool output)
{
	std::vector<Cells> pop_new;
	const size_t size = pop.size();
	pop_new.push_back(best_cell);
	while(pop_new.size() < size)
	{
		auto first = uniform_dist(gen);
		auto second = uniform_dist(gen);
		auto third = uniform_dist(gen);
		if(output)
		{
			std::cout << "\tMum: " << first << ":" << fitness[first] << ", "
                << second << ":" << fitness[second] << ", "
                << third <<":" << fitness[third] << '\n';
		}
		size_t mum = tournament(fitness, first, second, third);
		if(output)
		{
			std::cout << "\tChose " << mum << ":" << fitness[mum] << '\n';
		}
		first = uniform_dist(gen);
		second = uniform_dist(gen);
		third = uniform_dist(gen);
		if(output)
		{
	        std::cout << "\tDad: " << first << ":" << fitness[first] << ", "
                << second << ":" << fitness[second] << ", "
                << third <<":" << fitness[third] << '\n';
		}
		size_t dad = tournament(fitness, first, second, third);
		if(output)
		{
			std::cout << "\tChose " << dad << ":" << fitness[dad] << '\n';
		}
		Cells new_cell = GACA::crossover(pop[mum], pop[dad]);
		size_t new_cell_fitness = std::count(new_cell.begin(), new_cell.end(), true);
		if(output)
		{
			std::cout << "\t\tnew cell " << new_cell_fitness << '\n';
		}
		if(new_cell_fitness >= fitness[mum] && new_cell_fitness >= fitness[dad])
		{
			pop_new.push_back(new_cell);
		}
		else 
		{
			if(output)
			{
				std::cout << "\t\t REJECTED \n";
			}
			if(new_cell_fitness >= fitness[mum] || new_cell_fitness >= fitness[dad])
				pop_new.push_back(new_cell);
			else if(fitness[mum] >= fitness[dad])
				pop_new.push_back(pop[mum]);
			else
				pop_new.push_back(pop[dad]);
		}
	}
	return pop_new;
}

size_t GACA::tournament(std::vector<size_t> fitness, size_t index1, size_t index2, size_t index3)
{
	size_t alive1 = fitness[index1];
	size_t alive2 = fitness[index2];
	size_t alive3 = fitness[index3];
	if(alive1 < alive2)
	{
		if(alive1 < alive3)
			return alive2 < alive3 ? index3 : index2;
		return index2;//TODO can surely tidy this
	}
	if(alive2 < alive3)
		return alive1 < alive3 ? index3 : index1;
	return index1;
}

Mutation::Mutation(std::random_device::result_type seed, size_t cell_size) :
    gen(seed),
    uniform_dist(0, cell_size-1)
{
}

Cells Mutation::mutate(Cells cell)
{
	auto maybe = uniform_dist(gen);
	if (maybe < cell.size()/2)
	{
		auto index = uniform_dist(gen);
		cell[index] = !cell[index];
	}
	return cell;
}


Lookup::value_type GACA::bits_to_number(const Cells & cells)
{
	Lookup::value_type key = 0;
	int i = 1;
	for(auto bit = cells.crbegin(), end = cells.crend(); bit!=end; ++bit)
	{
		if(*bit)
			key += i;
		i <<= 1;
	}
	return key;
}

Cells GACA::number_to_bits(Lookup::value_type number)
{
	Cells cells;
	Lookup::value_type i = 1;
	while(i<=number)
	{
		cells.push_back((i & number) != 0);
		i = i << 1;
	}
	std::reverse(cells.begin(), cells.end());
	return cells;
}


FunctionRule GACA::lookup_to_rule(const Lookup & lookup)
{
	FunctionRule rule = [=] (const Cells & cells) {
		Lookup lookup_copy = lookup;//is this ok? or even required?
		Lookup::value_type key = 0;
		int i = 1;
		for(auto bit = cells.crbegin(), end = cells.crend(); bit!=end; ++bit)
		{
			if(*bit)
				key += i;
			i <<= 1;
		}
		if(key > lookup_copy.size())
		{
			std::stringstream ss;
			ss << "unsupported cells pattern:" << key << " with lookup size " << lookup_copy.size(); 
			throw std::invalid_argument(ss.str());
		}
		Cells new_cells = number_to_bits(lookup_copy[key]);
		if(new_cells.size() < cells.size())
			new_cells.insert(new_cells.begin(), cells.size() - new_cells.size(), false);

		return new_cells;
	};

	return rule;
}

Cells GACA::random_cell_gen(std::random_device::result_type seed, size_t len)
{
	using namespace GACA;
	CellGenerator gen(seed, len);
	return gen.generate();
}


CellGenerator::CellGenerator(std::random_device::result_type seed,
							 size_t cell_size) :
				gen(seed),
				uniform_dist(0, 1),
				cell_size(cell_size)
{
}

Cells CellGenerator::generate()
{
	Cells cells;
	for(size_t i = 0; i < cell_size; ++i)
	{
		cells.push_back(uniform_dist(gen) == 0);
	}
	return cells;
}

Lookup GACA::random_lookup_gen(std::random_device::result_type seed, size_t cell_size)
{
    std::default_random_engine gen(seed);
	auto max = (1ul << (cell_size)) - 1;//TODO rolls over for 32: need 64 bits
	if(max < (1ull << (cell_size)) - 1)
		throw std::length_error("size too large");
	Lookup lookup(max);//won't take a 64 bit number
    std::uniform_int_distribution<size_t> uniform_dist(0, max);
	for(size_t i = 0; i < max; ++i)
	{
		auto value = uniform_dist(gen);
		lookup[i] = value;
	}
	return lookup;
}

JitRule::JitRule(std::random_device::result_type seed, size_t cell_size) :
	gen(seed, cell_size)
{
}

Cells JitRule::operator()(const Cells & cells) const
{
	auto it = lookup.find(cells);
	if(it != lookup.end())
		return it->second;
	Cells return_cell = gen.generate();
	lookup[cells] = return_cell;
	return return_cell;
}



Lookup GACA::random_lookup_gen_biased_to_die(std::random_device::result_type seed, size_t cell_size)
{
	Lookup lookup;
    std::default_random_engine gen(seed);
	size_t max = (1 << (cell_size)) - 1;
	std::bernoulli_distribution dist(0.2);
	for(size_t i = 0; i < max; ++i)
	{
		auto value = 0;
		for(size_t j=0; j<cell_size; ++j)
		{
			auto flip = dist(gen);
			auto column = 1<<j;
			value += (column * flip);
			
		}
		lookup.push_back(value);
	}
	return lookup;
}

