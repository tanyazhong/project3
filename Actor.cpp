#include "Actor.h"
#include "StudentWorld.h"
#include "Level.h"
#include <iostream>
using namespace std;

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
Actor::Actor(StudentWorld* sw, int imageID, double startX, double startY, Direction dir, int depth)
	: GraphObject(imageID, startX, startY, dir, depth), m_world(sw)
{
}

bool Actor::isAlive() const
{
	return m_alive;
}

void Actor::setDead()
{
	m_alive = false;
}

bool Actor::blocksMovement() const
{
	return false;
}

bool Actor::blocksFlame() const
{
	return false;
}

void Actor::activateIfAppropriate(Actor * a)
{}

void Actor::useExitIfAppropriate()
{}


bool Actor::actorCanMove(double dest_x, double dest_y) const
{
	return getWorld()->canMove(dest_x, dest_y);
}

Actor::~Actor()
{
}

StudentWorld* Actor::getWorld() const
{
	return m_world;
}

//Wall functions
Wall::Wall(StudentWorld* sw, double x, double y)
	: Actor(sw, IID_WALL, SPRITE_WIDTH * x, SPRITE_HEIGHT * y, right, 0)
{}

void Wall::doSomething()
{}

bool Wall::blocksMovement() const {
	return true;
}

bool Wall::blocksFlame() const{
	return true;
}

ActivatingObject::ActivatingObject(StudentWorld * sw, int imageID, double x, double y, int depth, int dir)
	: Actor(sw, imageID, x, y, depth, dir)
{}

//exit funcs
Exit::Exit(StudentWorld * sw, double x, double y)
	: ActivatingObject(sw, IID_EXIT, SPRITE_WIDTH * x, SPRITE_HEIGHT * y, right, 1)
{}

void Exit::doSomething(){
	getWorld()->activateOnAppropriateActors(this);   
}

void Exit::activateIfAppropriate(Actor * a){
	a->useExitIfAppropriate();
}

bool Exit::blocksFlame() const{
	return true;
}

Pit::Pit(StudentWorld * sw, double x, double y)
	: ActivatingObject(sw, IID_PIT, SPRITE_WIDTH * x, SPRITE_HEIGHT * y, right, 0)
{}

void Pit::doSomething()
{
	getWorld()->activateOnAppropriateActors(this);
}

void Pit::activateIfAppropriate(Actor * a)
{}


Human::Human(StudentWorld* sw, int imageID, double x, double y)
	: Actor(sw, imageID, SPRITE_WIDTH * x, SPRITE_HEIGHT * y, right, 0)
{}

void Human::getInfected(){
	return;
}

bool Human::blocksMovement() const{
	return true;
}

bool Human::isInfected() const{
	return m_infected;
}

int Human::infections() const{
	return m_nInfections;
}

Penelope::Penelope(StudentWorld* sw, double x, double y) 
	: Human(sw, IID_PLAYER, x, y)
{}

void Penelope::doSomething() 
{
	int ch;
	if (getWorld()->getKey(ch)) { // user hit a key during this tick!
		switch (ch)
		{ 
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

void Penelope::useExitIfAppropriate()
{
	cout << "exited" << endl;
}

void Penelope::movePenelope(Direction d, double x, double y)
{
	setDirection(d);
	if (actorCanMove(x, y)) 
		moveTo(x, y);
}
