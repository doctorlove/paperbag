#include "GoL.h"

#include <random>

namespace GoL
{
	void walk_neighbours(size_t x, size_t y, size_t max_x, size_t max_y,
					std::function<void(size_t, size_t)> action)
	{
		//can I write an iterator instead?
		if(y>0)
		{
			if(x>0)
				action(x-1,y-1);
			action(x,y-1);
			if(x<max_x-1)
				action(x+1,y-1);
		}
		if(x>0)
			action(x-1,y);
		if(x<max_x-1)
			action(x+1,y);
		if(y<max_y-1)
		{
			if(x>0)
				action(x-1,y+1);
			action(x,y+1);
			if(x<max_x-1)
				action(x+1,y+1);
		}
	}

	void walk_neighbours_with_wrapping(size_t x, size_t y, size_t max_x, size_t max_y,
					std::function<void(size_t, size_t)> action)
	{
		size_t row = y>0? y-1 : max_y -1;
		action(x>0? x-1 : max_x -1, row);
		action(                  x, row);
		action(x<max_x-1? x + 1 : 0, row);
		row = y;
		action(x>0? x-1 : max_x -1, row);
		action(x<max_x-1? x + 1 : 0, row);
		row = y<max_y-1? y+1 : 0;
		action(x>0? x-1 : max_x -1, row);
		action(                  x, row);
		action(x<max_x-1? x + 1 : 0, row);
	}

	void glider(GoLWorld & GoLWorld, size_t corner_x, size_t corner_y)
	{
		GoLWorld.Spark(corner_x,     corner_y);
		GoLWorld.Spark(corner_x + 1, corner_y);
		GoLWorld.Spark(corner_x + 2, corner_y);
		GoLWorld.Spark(corner_x + 2, corner_y + 1);
		GoLWorld.Spark(corner_x + 1, corner_y + 2);
	}

	void reflected_glider(GoLWorld & GoLWorld, size_t corner_x, size_t corner_y)
	{
		GoLWorld.Spark(corner_x,     corner_y);
		GoLWorld.Spark(corner_x + 1, corner_y);
		GoLWorld.Spark(corner_x + 2, corner_y);
		GoLWorld.Spark(corner_x,     corner_y + 1);
		GoLWorld.Spark(corner_x + 1, corner_y + 2);
	}

	void inverted_glider(GoLWorld & GoLWorld, size_t corner_x, size_t corner_y)
	{
		GoLWorld.Spark(corner_x,     corner_y + 2);
		GoLWorld.Spark(corner_x + 1, corner_y + 2);
		GoLWorld.Spark(corner_x + 2, corner_y + 2);
		GoLWorld.Spark(corner_x,     corner_y + 1);
		GoLWorld.Spark(corner_x + 1, corner_y);
	}

	GoLWorld random_setup(size_t max_x,
						size_t max_y,
						size_t GoLWorld_x,
						size_t GoLWorld_y,
						size_t number,
						bool wrap)

	{
		if(GoLWorld_x< max_x)
			throw std::invalid_argument("GoLWorld width too small");
		if(GoLWorld_y< max_y)
			throw std::invalid_argument("GoLWorld height too small");
		GoLWorld GoLWorld(GoLWorld_x, GoLWorld_y, wrap);
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis_x(1, max_x-1);
		std::uniform_int_distribution<> dis_y(1, max_y-1);
 
		//TODO - surely there's a quicker way?
		//Like std::shuffle on a vector and send it in
		for (size_t n=0; n<number; ++n)
		{
			try
			{
				GoLWorld.Spark(dis_x(gen), dis_y(gen));
			}
			catch(...)//TODO Please!
			{
			}
		}
		return GoLWorld;
	}

	size_t GoLWorld::Alive() const 
	{
		size_t alive = 0;
		for(size_t y=0; y<Height(); ++y)
		{
			for(size_t x=0; x<Width(); ++x)
			{
				if(Alive(x, y))
					++alive;
			}
		}
		return alive;
	}

}
