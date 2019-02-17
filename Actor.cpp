#include "Actor.h"
#include "StudentWorld.h"
#include "Level.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
Actor::Actor(int imageID, double startX, double startY, Direction dir, int depth, double size)
	: GraphObject(imageID, startX, startY, dir, depth, size)
{
	
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

Penelope::Penelope(int x, int y) 
	: Human(IID_PLAYER, x, y)
{}

void Penelope::doSomething() 
{

}

