#ifndef WORLD_INCLUDED
#define WORLD_INCLUDED

class World
{
public:
	virtual bool Update() = 0; //returning a bool is stupid here

	virtual bool Alive(size_t x, size_t y) const = 0;
	virtual size_t Alive() const = 0;

	virtual size_t Width() const = 0;
	virtual size_t Height() const = 0;
};

#endif

