#ifndef GoL_INCLUDED
#define GoL_INCLUDED

#include <algorithm>
#include <functional>
#include <set>
#include <stdexcept>
#include <vector>

#include "../world.h"

namespace GoL
{
	void walk_neighbours(size_t x, size_t y, size_t max_x, size_t max_y,
					std::function<void(size_t, size_t)> action);
	void walk_neighbours_with_wrapping(size_t x, size_t y, size_t max_x, size_t max_y,
					std::function<void(size_t, size_t)> action);

	///
	//There are many know shapes that become stable, in an unbounded GoLWorld
	//See http://www.conwaylife.com/wiki/Glider for details
	///
	class GoLWorld;
	void glider(GoLWorld & GoLWorld,
 				size_t corner_x,
				size_t corner_y);
	void reflected_glider(GoLWorld & GoLWorld,
							size_t corner_x,
							size_t corner_y);
	void inverted_glider(GoLWorld & GoLWorld,
							size_t corner_x,
							size_t corner_y);

	GoLWorld random_setup(size_t max_x,
						size_t max_y,
						size_t GoLWorld_x,
						size_t GoLWorld_y,
						size_t number,
						bool wrap = false);


	class GoLWorld : public World
	{
	public:
		GoLWorld(size_t max_x, size_t max_y, bool wrap) :
			max_x(max_x),
			max_y(max_y),
			state(max_x*max_y, false),
			wrap(wrap)
		{
		}

		size_t Alive(void) const;

		bool Alive(size_t x, size_t y) const
		{
			return state[y*max_x + x];
		}

		size_t Width() const
		{
			return max_x;
		}

		size_t Height() const
		{
			return max_y;
		}

		void Spark(size_t x, size_t y)  //intended for setup really.
		{
			if(Alive(x,y))
				throw std::invalid_argument("Cell already alive");
			//TODO - edge case
			//TODO - extract function and check for off by ones errors...
			state[y*max_x + x] = true;
		}

		///
		//Return false if it matches the last state 
		//   should probably consider last two or three states
		///
		bool Update()
		{
			std::vector<bool> new_state(max_x*max_y, false);
			for(size_t y=0; y<max_y; ++y)
			{
				for(size_t x=0; x<max_x; ++x)
				{
					new_state[y*max_x + x] = StayAlive(x, y);
				}
			}
			bool stable = new_state == state;
			bool stable_again =  new_state == previous_state;
			previous_state = state;//TODO - how long oscillator periods
			state.swap(new_state);

			return !stable && !stable_again;
		}

		bool Dead()
		{
			return std::none_of(state.begin(), state.end(), [](bool b) {return b;});
		}

	private:
		const size_t max_x;
		const size_t max_y;
		std::vector<bool> state;//evil
		std::vector<bool> previous_state;//evil TODO - make this a set
		bool wrap;

		bool StayAlive(size_t x, size_t y)
		{
			size_t countAlive = 0;
			if(wrap)
				walk_neighbours_with_wrapping(x, y, max_x, max_y, [&](size_t xi, size_t yi)
					{
						countAlive += Alive(xi, yi);
					});
			else
				walk_neighbours(x, y, max_x, max_y, [&](size_t xi, size_t yi)
					{
						countAlive += Alive(xi, yi);
					});
			if(Alive(x, y))
			{
				return countAlive==2 || countAlive==3;
			}
			else 
				return countAlive == 3;
		}
	};
}

#endif

