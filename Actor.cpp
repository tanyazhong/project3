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

bool Actor::isAlive() const{
	return m_alive;
}

void Actor::setDead(){
	m_alive = false;
}

bool Actor::blocksMovement() const{
	return false;
}

bool Actor::blocksFlame() const{
	return false;
}

void Actor::activateIfAppropriate(Actor * a){}
void Actor::useExitIfAppropriate(){}
void Actor::dieByFallOrBurnIfAppropriate(){}
void Actor::beVomitedOnIfAppropriate(){}
void Actor::pickUpGoodieIfAppropriate(Goodie * g){}

bool Actor::actorCanMove(double dest_x, double dest_y) const{
	return getWorld()->canMove(dest_x, dest_y);
}

Actor::~Actor(){}

StudentWorld* Actor::getWorld() const{
	return m_world;
}

//Wall functions
Wall::Wall(StudentWorld* sw, double x, double y)
	: Actor(sw, IID_WALL, SPRITE_WIDTH * x, SPRITE_HEIGHT * y, right, 0)
{}

void Wall::doSomething(){}

bool Wall::blocksMovement() const {
	return true;
}

bool Wall::blocksFlame() const{
	return true;
}

//exit funcs
Exit::Exit(StudentWorld * sw, double x, double y)
	: Actor(sw, IID_EXIT, SPRITE_WIDTH * x, SPRITE_HEIGHT * y, right, 1)
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
	: Actor(sw, IID_PIT, SPRITE_WIDTH * x, SPRITE_HEIGHT * y, right, 0)
{}

void Pit::doSomething(){
	getWorld()->activateOnAppropriateActors(this);
}

void Pit::activateIfAppropriate(Actor * a){
	a->dieByFallOrBurnIfAppropriate();
}

Flame::Flame(StudentWorld * sw, double x, double y, int dir)
	:Actor(sw, IID_FLAME, SPRITE_WIDTH * x, SPRITE_HEIGHT * y, dir, 0)
{}

void Flame::doSomething(){}

void Flame::activateIfAppropriate(Actor * a)
{}

Vomit::Vomit(StudentWorld * sw, double x, double y, int dir)
	:Actor(sw, IID_VOMIT, SPRITE_WIDTH * x, SPRITE_HEIGHT * y, dir, 0)
{}

void Vomit::doSomething()
{}

void Vomit::activateIfAppropriate(Actor * a)
{}

Landmine::Landmine(StudentWorld * sw, double x, double y)
	:Actor(sw, IID_LANDMINE, SPRITE_WIDTH * x, SPRITE_HEIGHT * y, right, 1)
{}

void Landmine::doSomething()
{}

void Landmine::activateIfAppropriate(Actor * a)
{}

void Landmine::dieByFallOrBurnIfAppropriate()
{}

Goodie::Goodie(StudentWorld * sw, int imageID, double x, double y)
	:Actor(sw, imageID, x, y, right, 1)
{}

void Goodie::activateIfAppropriate(Actor * a){
	a->pickUpGoodieIfAppropriate(this);
}

void Goodie::dieByFallOrBurnIfAppropriate()
{}

VaccineGoodie::VaccineGoodie(StudentWorld * sw, double x, double y)
	:Goodie(sw, IID_VACCINE_GOODIE, SPRITE_WIDTH * x, SPRITE_WIDTH * y)
{}

void VaccineGoodie::doSomething()
{
	if (!isAlive())
		return;
	getWorld()->activateOnAppropriateActors(this);
}

void VaccineGoodie::pickUp(Penelope * p)
{
	p->increaseVaccines();
	this->setDead();
}

GasCanGoodie::GasCanGoodie(StudentWorld * sw, double x, double y)
	:Goodie(sw, IID_GAS_CAN_GOODIE, SPRITE_WIDTH * x, SPRITE_HEIGHT * y)
{}

void GasCanGoodie::doSomething()
{}

void GasCanGoodie::pickUp(Penelope * p) 
{}

LandmineGoodie::LandmineGoodie(StudentWorld * sw, double x, double y)
	:Goodie(sw, IID_LANDMINE_GOODIE, SPRITE_WIDTH * x, SPRITE_HEIGHT * y)
{}

void LandmineGoodie::doSomething()
{}

void LandmineGoodie::pickUp(Penelope * p)
{}


Agent::Agent(StudentWorld* sw, int imageID, double x, double y, int dir) 
	:Actor(sw, imageID, x, y, dir, 0)
{}

bool Agent::blocksMovement() const{
	return true;
}

bool Agent::triggersOnlyActiveLandmines() const{
	return false;
}

Human::Human(StudentWorld* sw, int imageID, double x, double y)
	: Agent(sw, imageID,  x, y, right)
{}

void Human::getVomitedOn(){
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
	: Human(sw, IID_PLAYER, SPRITE_WIDTH * x, SPRITE_HEIGHT * y)
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

void Penelope::useExitIfAppropriate(){
	cerr << "exited" << endl;
}

void Penelope::dieByFallOrBurnIfAppropriate(){
	cerr << "penelope died" << endl;
}

void Penelope::pickUpGoodieIfAppropriate(Goodie * g)
{
	getWorld()->increaseScore(50);
	getWorld()->playSound(SOUND_GOT_GOODIE);
	g->pickUp(this);
}

void Penelope::increaseVaccines()
{
	m_nVaccines++;
}

void Penelope::increaseFlameCharges()
{
	m_nFlamethrowers++;
}

void Penelope::increaseLandmines()
{
	m_nLandmines++;
}

int Penelope::getNumVaccines() const
{
	return m_nVaccines;
}

int Penelope::getNumFlameCharges() const
{
	return m_nFlamethrowers;
}

int Penelope::getNumLandmines() const
{
	return m_nLandmines;
}

void Penelope::movePenelope(Direction d, double x, double y){
	setDirection(d);
	if (actorCanMove(x, y)) 
		moveTo(x, y);
}

Citizen::Citizen(StudentWorld * sw, double x, double y)
	:Human(sw,IID_CITIZEN, SPRITE_WIDTH * x, SPRITE_HEIGHT * y)
{}

void Citizen::doSomething(){}

void Citizen::useExitIfAppropriate(){}

void Citizen::dieByFallOrBurnIfAppropriate(){}

Zombie::Zombie(StudentWorld * sw, double x, double y)
	:Agent(sw, IID_ZOMBIE, x, y, right)
{}

DumbZombie::DumbZombie(StudentWorld * sw, double x, double y)
	:Zombie(sw, SPRITE_WIDTH * x, SPRITE_HEIGHT * y)
{}

void DumbZombie::doSomething()
{}

void DumbZombie::dieByFallOrBurnIfAppropriate()
{}

SmartZombie::SmartZombie(StudentWorld * sw, double x, double y)
	: Zombie(sw, SPRITE_WIDTH*x, SPRITE_HEIGHT* y)
{}

void SmartZombie::doSomething()
{}

void SmartZombie::dieByFallOrBurnIfAppropriate()
{}
