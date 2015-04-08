#include "../Bees/Bee.h"

#include <iostream>
#include <stdexcept>
#include <thread>
#include <vector>

#include <SFML/Graphics.hpp>

void drawBees(const std::vector<BeeColony::Bee> & bees, sf::RenderWindow & window, float size, float edge, float width, float height)
{
	for(const auto & bee : bees)
	{
	    sf::CircleShape shape(size);
		switch (bee.job())
		{
		case BeeColony::WORKER:
			shape.setFillColor(sf::Color::Yellow);
			break;
		case BeeColony::INACTIVE:
			shape.setFillColor(sf::Color::Cyan);
			break;
		case BeeColony::SCOUT:
			shape.setFillColor(sf::Color::Magenta);
		}

		float best = 2*size;//stop going off the page
		//TODO _ this changes the x each time - need to decide when to stop and return home
		float x = static_cast<float>(edge + size + bee.x());
		if (x > edge + width - 2*size)
			x = edge + width - 2*size;
		if (x < edge + 2*size)
			x = edge + 2*size;
		shape.setPosition(x, std::max(height - 2*size - static_cast<float>(bee.y()), best));
		window.draw(shape);
		//TODO maybe make more bee like
	}
}

void draw_it(BeeColony::Hive hive, float width, float edge, float beeSize = 10.0f)
{
	bool started = false;
	const float lineWidth = 10.0f;
	const float height = 400.0f;
	const auto bagColour = sf::Color(180, 120, 60);
	//TODO Note edge in the height - but not used when draweing bees
    sf::RenderWindow window(sf::VideoMode(static_cast<int>(width + 2*edge), static_cast<int>(height + 2*edge)), "ABC");
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

	bool paused = false;
	bool swarmed = false;
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

		if (!started)
			started = true;
		else if (!paused)
		{
			hive.updateBees();
			if (!swarmed && shouldSwarm(hive.bees(), height + beeSize))
			{
				std::cout << "SWARM\n";
				hive.swarm();
				swarmed = true;
			}
		}
		drawBees(hive.bees(), window, beeSize, edge, width, height + edge);


		window.display();
		std::this_thread::sleep_for(std::chrono::milliseconds(50));//set render rate... or whatever it's called
	}
}

int get_value(int argc, const char **argv, int pos, int value)
{
	if (argc >= pos+1)
	{
		try
		{
			value = std::stoi(argv[pos]);
		}
		catch(const std::exception &)
		{
			//I feel bad...
		}
	}
	return value;
}


int main(int argc, char** argv)
{
	try
	{
		int totalBees = 10;
		int numberActive = 5;
		int numberScout = 3;
		const int maxVisits = 5;//TODO - use this
		const int steps = 100;
		totalBees = get_value(argc,  const_cast<const char **>(argv), 1, totalBees);
		numberActive = get_value(argc,  const_cast<const char **>(argv), 2, numberActive);
		numberScout = get_value(argc,  const_cast<const char **>(argv), 3, numberScout);
		const float width = 500.0f;
		const float edge = 150.0f;
		const float buzz = 10.0f;
		const float beeSize = 10.0f;
		std::cout << "Bees " << totalBees << "\n\tworkers " << numberActive << "\n\tscouts " << numberScout << '\n';
		BeeColony::Hive hive = BeeColony::makeHive(totalBees, numberActive, numberScout, maxVisits, steps, width, buzz, beeSize, edge + beeSize, edge + width - 2*beeSize);//TODO Am I using max visits?
		draw_it(hive, width, edge, beeSize);
	}
	catch(const std::exception & e)
	{
		std::cout << e.what() << '\n';
	}
}
