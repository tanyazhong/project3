#include "Actor.h"
#include "StudentWorld.h"
#include "Level.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
Actor::Actor(int imageID, double startX, double startY, Direction dir, int depth, double size)
	: GraphObject(imageID, startX, startY, dir, depth, size)
{
	
}

StudentWorld* Actor::getWorld() const
{
	return m_world;
}


//Wall functions
Wall::Wall(int x, int y)
	: Actor(IID_WALL, SPRITE_WIDTH * x, SPRITE_HEIGHT * y, right, 0, 1.0)
{}

void Wall::doSomething()
{
	return;
}

Human::Human(int imageID, int x, int y)
	: Actor(imageID, SPRITE_WIDTH * x, SPRITE_HEIGHT * y, right, 0, 1.0),
	m_alive(true), m_infected(false), m_nInfections(0)
{}

void Human::getInfected()
{

}

bool Human::alive() const
{
	return m_alive;
}

bool Human::infected() const
{
	return m_infected;
}

int Human::infections() const
{
	return m_nInfections;
}

Penelope::Penelope(int x, int y) 
	: Human(IID_PLAYER, x, y)
{}

void Penelope::doSomething() 
{
	int ch;
	if (getWorld()->getKey(ch)) { // user hit a key during this tick!
		switch (ch)
		{ //need to check that location is not blokced!
		case KEY_PRESS_LEFT:
			setDirection(left);
			moveTo(getX() - 4, getY());
			break;
		case KEY_PRESS_RIGHT:
			setDirection(right);
			moveTo(getX() + 4, getY());
			break;
		case KEY_PRESS_UP:
			setDirection(up);
			moveTo(getX(), getY() + 4);
			break;
		case KEY_PRESS_DOWN:
			setDirection(down);
			moveTo(getX(), getY() - 4);
		}
	}
}

