#include "../GameOfLife/GoL.h"
#include "../GACA/GACA.h"

#include <iostream>
#include <thread>

#include <SFML/Graphics.hpp>

using namespace GoL;

void draw(bool out, World & world, size_t top_margin)//TODO either top margin, or out not both
{
	const float blob_size = 10.0f;
	float width = world.Width() * blob_size;
	if(world.Width() < 50)
		width *= 2;
	const float edge = top_margin * blob_size;
	const float line_width = 10.0f;
	const float height = world.Height() * blob_size;
	const float bag_height = out ? (world.Height() - top_margin) * blob_size: world.Height() * blob_size;
	const auto bagColour = sf::Color(180, 120, 60);

	const int window_x = static_cast<int>(width + 2*edge);
	int window_y = static_cast<int>(height + edge);
	if (!out)
		window_y += static_cast<int>(edge);
    sf::RenderWindow window(sf::VideoMode(window_x, window_y), "GoL");//TODO - change title?
	window.clear(sf::Color::Black);

	sf::RectangleShape  left(sf::Vector2f(line_width, bag_height));
	left.setFillColor(bagColour);
	left.setPosition(edge - line_width, edge + line_width);
	window.draw(left);

	sf::RectangleShape  right(sf::Vector2f(line_width, bag_height));
	right.setFillColor(bagColour);
	right.setPosition(edge + width, edge + line_width);
	window.draw(right);

	sf::RectangleShape  base(sf::Vector2f(static_cast<float>(width) + line_width + blob_size, line_width));
	base.setFillColor(bagColour);
	base.setPosition(edge - blob_size, out ? height + blob_size : edge + height + blob_size);
	window.draw(base);

	bool paused = false;
	size_t i = 1;
	while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
			if (event.type == sf::Event::KeyPressed)
				paused = !paused;
        }

        window.clear();
		window.draw(left);
		window.draw(right);
		window.draw(base);

		for(size_t y=0; y<world.Height(); ++y)
		{
			for(size_t x=0; x<world.Width(); ++x)
			{
				if(world.Alive(x, y))
				{
				    sf::CircleShape shape(5);
					shape.setFillColor(sf::Color::Cyan);
					if(world.Width() >= 50)
					{
						shape.setPosition(x * blob_size + edge, 
							out ? height - y * blob_size : height - y * blob_size + edge);
					}
					else
					{
						shape.setPosition(x * 2*blob_size + edge, 
							out ? height - y * blob_size : height - y * blob_size + edge);
					}

					window.draw(shape);
				}
			}
		}

		window.display();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));//set render rate... or whatever it's called
		if(!paused)
		{
			if (world.Update())
			{
				++i;//TODO move to GoLWorld
			}
		}
	}
}


GACA::FunctionRule ga_rules(const GACA::Cells & cell, bool most_die, size_t population)
{
	using namespace GACA;
	FunctionRule best_rule = [](const Cells & )
		{
			std::cout <<"ERROR: No rule set!";
			return Cells();
		};
	size_t best_fitness = 0;
    std::random_device rd;		
	for(size_t i=0; i<population; ++i)
	{
		auto seed = rd();
		std::cout << "Next seed  " << seed << '\n';
		Lookup lookup = most_die ? random_lookup_gen_biased_to_die(seed, cell.size()) :
							random_lookup_gen(seed, cell.size());
		FunctionRule fn = lookup_to_rule(lookup);
		FullRule rule(fn);
		GACA::CAWorld world(rule, cell, 55, 1);
		while(world.Update())
			;
		auto alive = world.Alive();
		if(best_fitness <= alive)
		{
			std::cout << i << ". Alive " << alive << '\n';
			best_rule = rule;
			best_fitness = alive;
		}

		//crossover
		//mutation
	}
	std::cout << "Best fitness " << best_fitness << '\n';
	return best_rule;
}

size_t run_trial(const GACA::Rule & rule, const GACA::Cells & cell)
{
	GACA::CAWorld world(rule, cell, 55, 1);//TODO lag is not working
	while(world.Update())
		;
	return world.Alive();
}

GACA::Cells ga_cells(const GACA::Rule & rule, size_t size)
{
	using namespace GACA;
	size_t best_fitness = 0;
	Cells best_cell;
    std::random_device rd;		
	std::vector<Cells> pop(size);
	const size_t cell_size = 32;
	std::vector<size_t> fitness(size, 0);
	auto seed = rd();
	std::cout << "Next seed  " << seed << '\n';
	CellGenerator cell_generator(seed, cell_size);
	for(size_t i=0; i<size; ++i)
	{
		pop[i] = cell_generator.generate();
	}
	seed = rd();
	std::cout << "Next seed  " << seed << '\n';
	Mutation mutation(seed, cell_size);
	seed = rd();
	std::cout << "Next seed  " << seed << '\n';
	Crossover crossover(seed, size);
	for(size_t epoch = 0; epoch < 20; ++epoch)
	{
		for(size_t i=0; i<size; ++i)
		{
			auto alive = run_trial(rule, pop[i]);
			fitness[i] = alive;
			if(best_fitness <= alive)
			{
				std::cout << epoch << ". Alive " << alive << '\n';
				best_cell = pop[i];
				best_fitness = alive;
			}
		}

		pop = crossover(pop, fitness, best_cell, false);
		for(auto & cell : pop)
		{
			cell = mutation.mutate(cell);
		}
	}
	std::cout << "Best fitness " << best_fitness << '\n';
	return best_cell;
}

void gaca_visual(bool most_die, size_t population, bool find_start, bool rand_rule)
{
	using namespace GACA;
    std::random_device rd;
	auto seed =  rd();
	std::cout << "First seed " << seed << '\n';
    Cells cell;
	FunctionRule fn;

	if (find_start)
	{
		if(!rand_rule)
		{
			fn = [](const Cells & cell)
				{
					return cell;
				};
			FullRule rule(fn);
			cell = ga_cells(rule, population);
		}
		else
		{
			seed = rd();
			std::cout << "Next seed " << seed << '\n';
			JitRule rule(seed, 32);
			cell = ga_cells(rule, population);
			size_t lag = 5;//TODO does lag work?
			GACA::CAWorld world(rule, cell, 55, lag);
			bool whatever = true;
			size_t really = 15;
			draw(whatever, world, really);
			return;
		}
	}
	else
	{
		cell = random_cell_gen(seed, 16);
		fn = ga_rules(cell, most_die, population); //TODO hanged to JitRule and drop lookup?
	}
	size_t lag = 0;
	FullRule rule(fn);
	GACA::CAWorld world(rule, cell, 55, lag);
	bool whatever = true;
	size_t really = 15;
	draw(whatever, world, really);
}

void game_of_life_visual(bool wrap, bool out)
{
	const size_t bag_x = 50;
	const size_t bag_y = 40;
	const size_t edge = 15;

	const size_t world_x = bag_x;
	const size_t world_y = out ? bag_y + edge : bag_y;


	size_t sparks = 800;
	GoLWorld world = random_setup(bag_x, bag_y, world_x, world_y, sparks, wrap);
	//GoLWorld world(x, y, wrap);
	//GoLWorld.Spark(x/2-1, bag_y);//top of bag, not GoLWorld
	//GoLWorld.Spark(x/2, bag_y);
	//GoLWorld.Spark(x/2+1, bag_y);
	draw(out, world, edge);
}

void game_of_life_sparks_versus_stable()
{
	const size_t max_x = 50;
	const size_t max_y = 50;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(1, max_x * max_y);

	for(int i=0; i<250; ++i)
	{
		const int n = dis(gen);
		GoLWorld GoLWorld = random_setup(max_x, max_y, max_x, max_y, n);
		bool stable = false;
		int generation = 0;
		for(; !stable && generation < 1000; ++generation)
		{
			stable = !GoLWorld.Update();
		}
		std::cout << n << ", " << std::boolalpha << stable << ", " << std::boolalpha << GoLWorld.Dead() << ", " << generation << '\n';
	}
}

int main(int argc, char** argv)
{
	//TODO See   inline Clara::CommandLine<ConfigData> makeCommandLineParser() { 
	//in  "catch_commandline.hpp"

	try
	{
		bool sim = false;
		bool wrap = false;
		bool out = false;
		bool gaca = false;
		bool find_start = false;
		bool most_die = false;
		int population = 1;
		bool rand_start = false;
		int arg = 1;
		while(arg < argc)
		{
			if(std::string("sim")==argv[arg])
			{
				std::cout << "Sim\n";
				sim = true;
			}
			else if(std::string("wrap")==argv[arg])
			{
				wrap = true;
				std::cout << "Wrap fully\n";
			}
			else if(std::string("out")==argv[arg])
			{
				out = true;//need to decide what going out the bottom means
				std::cout << "out\n";
			}
			else if(std::string("gaca")==argv[arg])
			{
				gaca = true;
				std::cout << "gaca\n";
			}
			else if(std::string("find_start")==argv[arg])
			{
				find_start = true;
				std::cout << "find_start\n";
			}
			else if(std::string("most_die")==argv[arg])
			{
				most_die = true;
				std::cout << "most_die\n";
			}
			else if(std::string("population=")==std::string(argv[arg]).substr(0, 11))
			{
				auto value = std::string(argv[arg]).substr(11);
				population = std::stoi(value);
				std::cout << "population " << population << "\n";
			}
			else if (std::string("rand_start")==argv[arg])
			{
				rand_start = true;
			}
			else
			{
				std::cout << "Unused parameter:" << argv[arg];
				return -1;
			}
			++arg;
		}
		if (sim)
			game_of_life_sparks_versus_stable();
		else if (gaca)
		{
			//one run might make demo clearer
			gaca_visual(most_die, population, find_start, rand_start);
		}
		else
			game_of_life_visual(wrap, out);
	}
	catch(const std::exception & e)
	{
		std::cerr << "ERROR:\n" << e.what() << '\n';
	}
}
