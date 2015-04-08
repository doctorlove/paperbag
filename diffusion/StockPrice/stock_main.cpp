#include "../Lib/PriceSimulation.h"
#include <cassert>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <thread>

//disable link warning... LNK4099
#include <SFML/Graphics.hpp>

inline sf::Vertex to_vertex(const sf::Vertex & point, const sf::Color & colour, float edge, float x_scale, float height)
{
	return sf::Vertex(sf::Vector2f(point.position.x * x_scale + edge, height - point.position.y), colour);
}

void draw_it(const std::vector<std::vector<sf::Vertex>> & sims, float time, float height, std::string title)
{
	//http://www.sfml-dev.org/tutorials/2.1/start-vc.php
	//nb -s for static libs
	const float edge = 30.0f;
	const float lineWidth = 5.0f;
	const float width = 500.0f;
	const float x_scale = width/time;
	const auto bagColour = sf::Color(180, 120, 60);
    sf::RenderWindow window(sf::VideoMode(static_cast<int>(width + 2*edge), static_cast<int>(height + 2*edge)), title);
	window.clear(sf::Color::Black);

	sf::RectangleShape  left(sf::Vector2f(lineWidth, static_cast<float>(height)));
	left.setFillColor(bagColour);
	left.setPosition(edge, edge);
	window.draw(left);

	sf::RectangleShape  right(sf::Vector2f(lineWidth, static_cast<float>(height)));
	right.setFillColor(bagColour);
	right.setPosition(edge + width, edge);
	window.draw(right);

	sf::RectangleShape  base(sf::Vector2f(static_cast<float>(width) + lineWidth, lineWidth));
	base.setFillColor(bagColour);
	base.setPosition(edge, edge + height);
	window.draw(base);

	assert(sims.begin()->size());
	//TODO - and they are all the same
	size_t penultimate = sims.begin()->size() - 1;
	size_t last = 1;
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
			break;
        }

        window.clear();
		window.draw(left);
		window.draw(right);
		window.draw(base);
		last = std::min(++last, sims.begin()->size() - 1);
		for(const auto & points: sims)
		{
			bool out = false;
			for(size_t i=0; i < last; ++i)
			{
				out |= (points[i].position.y > height);//what about the edge?
				sf::Color colour = out ? sf::Color::Green: sf::Color::White;
				auto scaled_start = to_vertex(points[i], colour, edge + lineWidth, x_scale, height);
				auto scaled_point = to_vertex(points[i+1], colour, edge + lineWidth, x_scale, height);
				sf::Vertex line[] = {scaled_start, scaled_point};
				window.draw(line, 2, sf::Lines);
			}
		}
		window.display();
		std::this_thread::sleep_for(std::chrono::milliseconds(50));//set render rate... or whatever it's called
    }

}


std::vector<sf::Vertex> price_demo(unsigned int seed, double drift, double vol, double time, double dt, double jump, double mean_jump_per_unit_time)
{
	const double start_price = 50.0;
	MC101::PriceSimulation price(start_price, drift, vol, dt, seed, jump, mean_jump_per_unit_time);

	std::vector<sf::Vertex> points;
	int count = static_cast<int>(time/dt);
	for(int i=0; i <= count+1; ++i)
	{
		auto point = sf::Vector2f(static_cast<float>(i*dt), static_cast<float>(price.update()));
		points.push_back(point);
	}
	return points;
}

int main(int argc, char ** argv)
{
	int sims = 1;
	double dt = 0.5;
	double drift = 0.2;
	double vol = 0.3;
	double jump = 0.0;
	double mean_jump_per_unit_time = 0.1;

	if (argc == 1)
	{
		std::cout << "usage:\n\tsims = " << sims << "\tdt = " << dt << "\tdrift = " << drift << "\tvol = " << vol
				  << "\n\tjump = " << jump << "\tjump probability = " << mean_jump_per_unit_time << "\n";
		return -1;
	}


	if (argc >= 2)
	{
		try
		{
			sims = std::stoi(argv[1]);
			if (sims < 1)
			{
				std::cout << "sims must be positive.\n";
				return 1;
			}
		}
		catch(const std::exception &)
		{
			//I feel bad...
		}
	}
	dt = get_value(argc, const_cast<const char **>(argv), 2, dt);
	drift = get_value(argc, const_cast<const char **>(argv), 3, dt);
	vol = get_value(argc, const_cast<const char **>(argv), 4, dt);
	jump = get_value(argc, const_cast<const char **>(argv), 5, dt);
	if (argc >= 7)
	{
		try
		{
			//not jump prob
			mean_jump_per_unit_time = std::stof(argv[6]);
			if (mean_jump_per_unit_time == 0.0)
			{
				//ok, bad
				mean_jump_per_unit_time = 1;
				jump = 0;
				if (jump != 0.0)
					std::cout << "WARNING - work round jump\n";
			}
		}
		catch(const std::exception &)
		{
			//I feel bad...
		}
	}

	std::vector<std::vector<sf::Vertex>> allPoints;
	float time = 4.0f;
	float final_height = 0.0;
	int out = 0;
	const float height = 400.0f;
	for (int i=0; i<sims; ++i)
	{
		unsigned int seed = std::chrono::high_resolution_clock::now().time_since_epoch().count() + i;
		auto sim = price_demo(seed, drift, vol, time, dt, jump, mean_jump_per_unit_time);
		allPoints.push_back(sim);
		auto price = sim.back().position.y;
		final_height += price;
		if (price > height)
			++out;
	}
	std::cout << "average " << final_height/sims << "\nnumber out " << out << '\n';;

	std::stringstream title;
	title << "Stock prices. sims = " << sims << ", dt = " << dt << ", drift " << drift << ", vol " << vol << ", dt " << dt << ", jump size " << jump << ", mean jumps " << mean_jump_per_unit_time;
	draw_it(allPoints, time, height, title.str());
}
