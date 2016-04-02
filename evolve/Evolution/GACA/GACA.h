#ifndef GACA_INCLUDED
#define GACA_INCLUDED

#include <functional>
#include <map>
#include <random>
#include <vector>

#include "../world.h"

namespace GACA
{
	typedef std::vector<size_t> Lookup;
	typedef std::vector<bool> Cells;
	typedef std::function<Cells(const Cells &)> FunctionRule;

	Lookup crossover(Lookup rule1, Lookup rule2);
	Cells crossover(Cells cell1, Cells cell2);

	Lookup::value_type bits_to_number(const Cells & cells);
	Cells number_to_bits(Lookup::value_type number);
	FunctionRule lookup_to_rule(const Lookup & lookup);

	Cells random_cell_gen(std::random_device::result_type seed, size_t len);
	Lookup random_lookup_gen(std::random_device::result_type seed, size_t cell_size);
	Lookup random_lookup_gen_biased_to_die(std::random_device::result_type seed, size_t cell_size);


	class CellGenerator
	{
	public:
		CellGenerator(std::random_device::result_type seed, size_t cell_size);
		Cells generate();
	private:
		std::default_random_engine gen;
		std::uniform_int_distribution<size_t> uniform_dist;
		size_t cell_size;
	};

	class Rule
	{
	public:
		virtual Cells operator()(const Cells & cells) const = 0;
	};

	class FullRule : public Rule
	{
	public:
		explicit FullRule(std::function<Cells(const Cells &)> fn) : fn(fn)
		{
		}
		virtual Cells operator()(const Cells & cells) const
		{
			return fn(cells);
		}
	private:
		std::function<Cells(const Cells &)> fn;
	};

	class JitRule : public Rule
	{
	public:
		JitRule(std::random_device::result_type seed, size_t cell_size);
		virtual Cells operator()(const Cells & cells) const;
	private:
		mutable std::map<Cells, Cells> lookup;
		mutable CellGenerator gen;
	};

	inline size_t fitest(const World & lhs, const World & rhs)
	{
		if(lhs.Alive() > rhs.Alive())
			return 0;
		return 1;
	}

	size_t tournament(std::vector<size_t> fitness, size_t index1, size_t index2, size_t index3);

	class Crossover
	{
	public:
		Crossover(std::random_device::result_type seed, size_t population_size);
		std::vector<Cells> operator()(const std::vector<Cells> & pop,
							 const std::vector<size_t> & fitness,
							 const Cells & best_cell,
							 bool output);
	private:
		std::default_random_engine gen;
		std::uniform_int_distribution<size_t> uniform_dist;
	};

	class Mutation
	{
	public:
		Mutation(std::random_device::result_type seed, size_t cell_size);
		Cells mutate(Cells cell);
	private:
		std::default_random_engine gen;
		std::uniform_int_distribution<size_t> uniform_dist;
	};

	class CAWorld : public World
	{
	public:
		CAWorld(const Rule & rules, Cells cells, size_t epochs, size_t lag = 0);

		bool Update();

		size_t Alive() const;
		bool Alive(size_t x, size_t y) const;

		size_t Width() const;
		size_t Height() const;

	private:
		const Rule & rules;
		Cells cell;
		size_t epochs;
		size_t lag;
		std::vector<Cells> history;
	};
}

#endif
