#include <cstdlib>
#include <ctime>
#include <functional>
#include <iostream>
#include <map>
#include <random>
#include <string>

int bad_mod()
{
	return rand() % 100;
}

int bad_mod_101()
{
	return rand() % 101;
}

int bad_mod_ranged()
{
	auto value = (rand() * 1.0/RAND_MAX);
	return static_cast<int>(value * 100);
}

int rejection_sample()
{
	int value;
	do {  
	  value = rand();
	}
	while (value > 100);
	return value;
}

int bad_die()
{
	auto value = (rand() * 1.0/RAND_MAX);
	return static_cast<int>(value * 6) + 1;
}

void sample(std::function<int()> fn)
{
	for(int i=0; i<10000; ++i)
	{
		auto value = fn();
		std::cout << value << '\n';
	}
}

int main(int argc, char ** argv)
{
	if (argc != 2)
	{
		std::cout << "choose one of " << 
			"bad_mod\n" <<
			"bad_mod_ranged\n" <<
			"rejection_sample\n" <<
			"better\n";
		return 1;
	}
	srand(time(NULL));
	std::string choice(argv[1]);
	if (choice == std::string("bad_mod"))
		sample(bad_mod);

	if (choice == std::string("bad_mod_101"))
		sample(bad_mod_101);

	if (choice == std::string("bad_mod_ranged"))
		sample(bad_mod_ranged);

	if (choice == std::string("rejection_sample"))
		sample(rejection_sample);

	if (choice == std::string("bad_die"))
		sample(bad_die);
	

	if (choice == std::string("uniform"))
	{
		std::random_device rseed;
		std::mt19937 rgen(rseed()); // mersenne_twister 
		std::uniform_int_distribution<int> dist(0,100);
		for(int i=0; i<10000; ++i)
		{
			auto value = dist(rgen);
			std::cout << value << '\n';
		}
	}
}
