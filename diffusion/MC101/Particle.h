#ifndef PARTICLE_INCLUDED
#define PARTICLE_INCLUDED

#include <cmath>
#include <functional>
#include <limits>
#include <random>

namespace MC101
{
	//http://lben.epfl.ch/files/content/sites/lben/files/users/179705/Simulating%20Brownian%20Motion.pdf

	template<typename Shape, typename T = double>
	class Particle
	{
	public:
		Particle(Shape shape, T root_t, std::function<void(Shape &)> draw,
					T x = 0, T y = 0,
					T edge = 0,
					T max_x = std::numeric_limits<T>::max(), T max_y = std::numeric_limits<T>::max(),
					unsigned int seed = 1,
					bool breakout = false) 
				:
			shape(shape),
			root_t(root_t),
			draw(draw),
			engine(seed),
			x(x),
			y(y),
			edge(edge),
			max_x(max_x),
			max_y(max_y),
			done(false),
			breakout(breakout)
			{
			}


		void Update()
		{
			if (done)
				return;

			x += move_step();
			y += move_step();

			if (y < edge/4 )
			{
				done = true;
				return; //stop, as finished
			}

			if(!breakout)
			{
				if (x < edge/2) x = edge/2;
				if (x > max_x) x = max_x;
				if (y > max_y) y = max_y;
			}
			shape.setPosition(x, y);
		}

		void Draw()
		{
			draw(shape);
		}

	private:
		Shape shape;
		T root_t;
		std::function<void(Shape &)> draw;

		std::mt19937 engine;
		std::normal_distribution<T> normal_dist;//note defaults are good for us
		T x;//whole numbers or not? Depends on how we draw them
		T y;
		T edge;
		T max_x;
		T max_y;
		bool done;
		bool breakout;

		T move_step()
		{
			return root_t * normal_dist(engine);
		}
	};

}

#endif
