#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>
#include <ctime>

#include "Particle.h"

//disbale link warning... LNK4099
#include <SFML/Graphics.hpp>

struct Coord
{
	int x;
	int y;
	Coord(int x,int y) : x(x), y(y) {}
};

//C:\Dev\SFML-2.1\include

void drawBag(sf::RenderWindow  & window, sf::RectangleShape & left, sf::RectangleShape & right, sf::RectangleShape & base)
{
	window.draw(left);
	window.draw(right);
	window.draw(base);
}

void draw_it(size_t count, float root_t, bool breakout, std::string title)
{
	//http://www.sfml-dev.org/tutorials/2.1/start-vc.php
	//nb -s for static libs
	const unsigned int height = 500;
	const unsigned int width = 500;
	const float edge = 50.0f;
	const float lineWidth = 5.0f;
	const auto bagColour = sf::Color(180, 120, 60);

    sf::RenderWindow window(sf::VideoMode(static_cast<int>(width + edge), static_cast<int>(height + edge)), title);
	window.clear(sf::Color::Black);

    sf::CircleShape shape(lineWidth);
    shape.setFillColor(sf::Color::Green);
	auto win_pos = window.getSize();
	int max_x = win_pos.x;
	int max_y = win_pos.y;
	shape.setPosition(max_x/2.0f, max_y/2.0f);
	auto pos = shape.getPosition();

	sf::RectangleShape  left(sf::Vector2f(lineWidth, static_cast<float>(height)));
	left.setFillColor(bagColour);
	left.setPosition(edge/2, edge/2);

	sf::RectangleShape  right(sf::Vector2f(lineWidth, static_cast<float>(height)));
	right.setFillColor(bagColour);
	right.setPosition(edge/2 + width, edge/2);

	sf::RectangleShape  base(sf::Vector2f(static_cast<float>(height) + lineWidth, lineWidth));
	base.setFillColor(bagColour);
	base.setPosition(edge/2, edge/2 + height);

	drawBag(window, left, right, base);

	//TODO - several particles - do they interact?
	std::vector<MC101::Particle<sf::CircleShape, float>> particles;
	std::random_device rd;
	for(size_t i = 0; i < count; ++i)
	{
		//emplace_back wan't deal with 6 args
		particles.push_back(MC101::Particle<sf::CircleShape, float>(shape, root_t,
			[&] (sf::Shape & shape) //TODO capture some by ref and some by value
			{
				window.draw(shape);
			},
			pos.x,
			pos.y,
			edge + lineWidth,
			edge/2 + width - 2*lineWidth,
			edge/2 + height - 2*lineWidth,
			rd(),
			breakout
		));
	}

	bool paused = false;
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
	    drawBag(window, left, right, base);
		for(auto & particle: particles)
		{
			if (!paused)
				particle.Update();
			particle.Draw();
		}
		window.display();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));//set render rate... or whatever it's called
    }


	//animations
	//https://github.com/LaurentGomila/SFML/wiki/Source:-AnimatedSprite
	//http://en.sfml-dev.org/forums/index.php?topic=5698.0 - thor links
	//or just 
	//http://www.reddit.com/r/gamedev/comments/1qqkt8/a_cool_sfml_project_follow_the_bouncing_ball/

	//Brownian motion
	//http://www.stat.berkeley.edu/~aldous/Research/Ugrad/ZY1.pdf

}

int main(int argc, char ** argv)
{
	int particles = 25;
	bool breakout = true;
	if (argc >= 2)
	{
		try
		{
			particles = std::stoi(argv[1]);

			if (particles < 1)
			{
				std::cout << "Particles must be positive.\n";
				return 0;
			}
			if (argc >= 3)
			{
				breakout = std::atoi(argv[2]) != 0;
			}
		}
		catch(const std::exception &)
		{
			//I feel bad...
		}
	}

	const float root_t = 7.5f;//TODO why not send this in too?
	std::stringstream title;
	title << "2D Brownian motion " << particles << ", breakout " << breakout;

	draw_it(particles, root_t, breakout, title.str());
}
