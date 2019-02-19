#include "Actor.h"
#include "StudentWorld.h"
#include "Level.h"
#include <iostream>
using namespace std;

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
Actor::Actor(StudentWorld* sw, int imageID, double startX, double startY, Direction dir, int depth, double size)
	: GraphObject(imageID, startX, startY, dir, depth, size), m_world(sw)
{
}

bool Actor::alive() const
{
	return m_alive;
}

bool Actor::blocker() const
{
	return m_blocker;
}

bool Actor::actorCanMove(double dest_x, double dest_y) const
{
	return getWorld()->canMove(dest_x, dest_y);
}

StudentWorld* Actor::getWorld() const
{
	return m_world;
}


//Wall functions
Wall::Wall(StudentWorld* sw, int x, int y)
	: Actor(sw, IID_WALL, SPRITE_WIDTH * x, SPRITE_HEIGHT * y, right, 0, 1.0)
{}

void Wall::doSomething()
{
	return;
}

Human::Human(StudentWorld* sw, int imageID, int x, int y)
	: Actor(sw, imageID, SPRITE_WIDTH * x, SPRITE_HEIGHT * y, right, 0, 1.0)
{}

void Human::getInfected()
{

}

bool Human::infected() const
{
	return m_infected;
}

int Human::infections() const
{
	return m_nInfections;
}

Penelope::Penelope(StudentWorld* sw, int x, int y) 
	: Human(sw, IID_PLAYER, x, y)
{}

void Penelope::doSomething() 
{
	int ch, x, y;
	if (getWorld()->getKey(ch)) { // user hit a key during this tick!
		switch (ch)
		{ //need to check that location is not blokced!
		case KEY_PRESS_LEFT:
			movePenelope(left, getX() - 4, getY());
			break;
		case KEY_PRESS_RIGHT:
			movePenelope(right, getX() + 4, getY());
			break;
		case KEY_PRESS_UP:
			movePenelope(up, getX(), getY() + 4);
			break;
		case KEY_PRESS_DOWN:
			movePenelope(down, getX(), getY() - 4);
		}
	}
}

void Penelope::movePenelope(Direction d, double x, double y)
{
	setDirection(d);;
	if (actorCanMove(x, y))
		moveTo(x, y);
}



