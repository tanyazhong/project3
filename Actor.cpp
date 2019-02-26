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

bool Actor::triggersOnlyActiveLandmines() const
{
	return false;
}

bool Actor::triggersZombieVomit() const
{
	return false;
}

bool Actor::threatensCitizens() const
{
	return false;
}

bool Actor::triggersCitizens() const {
	return false;
}

void Actor::activateIfAppropriate(Actor * a){}
void Actor::useExitIfAppropriate(){}
void Actor::dieByFallOrBurnIfAppropriate(){}
void Actor::beVomitedOnIfAppropriate(){}
void Actor::pickUpGoodieIfAppropriate(Goodie * g){}

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

void Flame::doSomething(){
	if (!isAlive())
		return;
	if (m_lifeSpan == 2){
		setDead();
		return;
	}
	getWorld()->activateOnAppropriateActors(this);
	m_lifeSpan++;
}

void Flame::activateIfAppropriate(Actor * a){
	a->dieByFallOrBurnIfAppropriate();
}

Vomit::Vomit(StudentWorld * sw, double x, double y, int dir)
	:Actor(sw, IID_VOMIT, SPRITE_WIDTH * x, SPRITE_HEIGHT * y, dir, 0)
{}

void Vomit::doSomething()
{
	if (!isAlive())
		return;
	if (m_lifeSpan == 2){
		setDead();
		return;
	}
	getWorld()->activateOnAppropriateActors(this);
}

void Vomit::activateIfAppropriate(Actor * a)
{
	a->beVomitedOnIfAppropriate();
}

Landmine::Landmine(StudentWorld * sw, double x, double y)
	:Actor(sw, IID_LANDMINE, SPRITE_WIDTH * x, SPRITE_HEIGHT * y, right, 1)
{}

void Landmine::doSomething()
{}

void Landmine::activateIfAppropriate(Actor * a)
{}

void Landmine::dieByFallOrBurnIfAppropriate()
{
	setDead();
	getWorld()->playSound(SOUND_LANDMINE_EXPLODE); 
	//theres more stuff you gotta do here
	//
	//
	//Actor* f = new Flame(getWorld(), this->getX, this->getY)
}

Goodie::Goodie(StudentWorld * sw, int imageID, double x, double y)
	:Actor(sw, imageID, x, y, right, 1)
{}

void Goodie::activateIfAppropriate(Actor * a){
	a->pickUpGoodieIfAppropriate(this);
}

void Goodie::dieByFallOrBurnIfAppropriate(){
	setDead();
}

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
{
	if (!isAlive())
		return;
	getWorld()->activateOnAppropriateActors(this);
}

void GasCanGoodie::pickUp(Penelope * p) 
{
	p->increaseFlameCharges();
	this->setDead();
}

LandmineGoodie::LandmineGoodie(StudentWorld * sw, double x, double y)
	:Goodie(sw, IID_LANDMINE_GOODIE, SPRITE_WIDTH * x, SPRITE_HEIGHT * y)
{}

void LandmineGoodie::doSomething()
{
	if (!isAlive())
		return;
	getWorld()->activateOnAppropriateActors(this);
}

void LandmineGoodie::pickUp(Penelope * p)
{
	p->increaseLandmines();
	this->setDead();
}


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

void Human::beVomitedOnIfAppropriate(){
	m_nInfections++;
}

bool Human::isInfected() const{
	return m_nInfections > 0;
}

int Human::infectionDuration() const{
	return m_nInfections;
}

void Human::clearInfection(){
	m_nInfections = 0;
}

void Human::increaseInfections(){
	m_nInfections++;
}

Penelope::Penelope(StudentWorld* sw, double x, double y) 
	: Human(sw, IID_PLAYER, SPRITE_WIDTH * x, SPRITE_HEIGHT * y)
{}

void Penelope::doSomething() 
{
	if (!isAlive())
		return;

	if (isInfected()){
		increaseInfections();
		if (infectionDuration() == 500) {
			setDead();
			getWorld()->playSound(SOUND_PLAYER_DIE);
			return;
		}
	}

	int ch;
	if (getWorld()->getKey(ch)) { // user hit a key during this tick!
		switch (ch)
		{ 
		case KEY_PRESS_SPACE:
			deployFlames(getDirection(), getX(), getY());
			break;
		case KEY_PRESS_TAB:
			deployLandmine(getDirection(), getX(), getY());
			break;
		case KEY_PRESS_ENTER:
			break;
		case KEY_PRESS_LEFT:
			moveAgent(left, getX() - 4, getY());
			break;
		case KEY_PRESS_RIGHT:
			moveAgent(right, getX() + 4, getY());
			break;
		case KEY_PRESS_UP:
			moveAgent(up, getX(), getY() + 4);
			break;
		case KEY_PRESS_DOWN:
			moveAgent(down, getX(), getY() - 4);
		}
	}
}

void Penelope::moveAgent(Direction d, double x, double y) {
	setDirection(d);
	if (getWorld()->canMove(x, y))
		moveTo(x, y);
}

void Penelope::deployFlames(Direction d, double x, double y) {
	if (m_nFlamethrowers == 0)
		return;
	m_nFlamethrowers--;
	getWorld()->playSound(SOUND_PLAYER_FIRE);
	double fx, fy;
	for (int i = 1; i != 4; i++) {
		switch (d) {
		case up:
			fx = x;
			fy = y + i * SPRITE_HEIGHT;
			break;
		case down:
			fx = x;
			fy = y - i * SPRITE_HEIGHT;
			break;
		case left:
			fx = x - i * SPRITE_WIDTH;
			fy = y;
			break;
		case right:
			fx = x + i * SPRITE_WIDTH;
			fy = y;
		}
		if (getWorld()->isFlameBlockedAt(fx, fy))
			break;

		Actor* f = new Flame(getWorld(), fx, fy, up);
		getWorld()->addActor(f);
	}
}

void Penelope::deployLandmine(Direction d, double x, double y)
{
	if (m_nLandmines > 0) {
		Actor* l = new Landmine(getWorld(), x, y);
		getWorld()->addActor(l);
		m_nLandmines--;
	}
}

void Penelope::vaccinate()
{
	if (m_nVaccines > 0) {
		clearInfection();
		m_nVaccines--;
	}
}

void Penelope::useExitIfAppropriate(){
	cerr << "exited" << endl;
}

void Penelope::dieByFallOrBurnIfAppropriate(){
	setDead();
	getWorld()->playSound(SOUND_PLAYER_DIE);
}

void Penelope::pickUpGoodieIfAppropriate(Goodie * g)
{
	getWorld()->increaseScore(50);
	getWorld()->playSound(SOUND_GOT_GOODIE);
	g->pickUp(this);
}

bool Penelope::triggersCitizens() const {
	return true;
}

void Penelope::increaseVaccines(){
	m_nVaccines++;
}

void Penelope::increaseFlameCharges(){
	m_nFlamethrowers += 5;
}

void Penelope::increaseLandmines(){
	m_nLandmines += 2;
}

int Penelope::getNumVaccines() const{
	return m_nVaccines;
}

int Penelope::getNumFlameCharges() const{
	return m_nFlamethrowers;
}

int Penelope::getNumLandmines() const{
	return m_nLandmines;
}



Citizen::Citizen(StudentWorld * sw, double x, double y)
	:Human(sw,IID_CITIZEN, SPRITE_WIDTH * x, SPRITE_HEIGHT * y)
{}

void Citizen::doSomething(){
	if (!isAlive())
		return;
	if (isInfected())
		increaseInfections();
	if (infectionDuration() == 500) {
		setDead();
		getWorld()->playSound(SOUND_ZOMBIE_BORN);
		getWorld()->increaseScore(-1000);
		int n = randInt(0, 9);
		Actor* z;
		if (n < 3)     //0 1 or 2
			z = new SmartZombie(getWorld(), getX(), getY());
		else
			z = new DumbZombie(getWorld(), getX(), getY());
		getWorld()->addActor(z);
		return;
	}
	if (m_paralyzed) {
		m_paralyzed = false;
		return;
	}
	
	bool isThreat;
	double ox, oy, d, dx, dy;
	if (!getWorld()->locateNearestCitizenTrigger(getX(), getY(), ox, oy, d, isThreat)) {
		m_paralyzed = true;
		return;
	}
	dx = getX() - ox;
	dy = getY() - oy;
	Direction xdir, ydir;
	if (dx <= 0)
		ydir = up;
	else
		ydir = down;

	if (dy <= 0)
		xdir = right;
	else
		xdir = left;

	if (!isThreat && dx * dx + dy * dy <= 80 * 80) {               //penelope is near
		if (getX() == ox) {     //same column
			if (ydir == up)        //pen is above
				moveAgent(up, getX(), getY() + 2);
			else 
				moveAgent(down, getX(), getY() - 2);
			return;
		}
		else if (getY() == oy){
			if (xdir == right)        //pen is right
				moveAgent(right, getX() + 2, getY());
			else 
				moveAgent(left, getX() - 2, getY());
			return;
		}
		else {
			int n = randInt(0, 1);
		

		}
	}
}

void Citizen::moveAgent(Direction d, double x, double y) {
	if (getWorld()->canMove(x, y)) {
		setDirection(d);
		moveTo(x, y);
	}
}

void Citizen::useExitIfAppropriate(){}

void Citizen::dieByFallOrBurnIfAppropriate(){
	setDead();
	getWorld()->playSound(SOUND_CITIZEN_DIE);
}

Zombie::Zombie(StudentWorld * sw, double x, double y)
	:Agent(sw, IID_ZOMBIE, x, y, right)
{}

void Zombie::moveAgent(Direction d, double x, double y)
{
}

bool Zombie::triggersCitizens() const {
	return true;
}


DumbZombie::DumbZombie(StudentWorld * sw, double x, double y)
	:Zombie(sw, SPRITE_WIDTH * x, SPRITE_HEIGHT * y)
{}

void DumbZombie::doSomething()
{}

void DumbZombie::dieByFallOrBurnIfAppropriate()
{
	setDead();
	getWorld()->playSound(SOUND_ZOMBIE_DIE);
	getWorld()->increaseScore(1000);
	int n;
	n = randInt(0, 9);  
	if (n == 0) {
		Actor* v = new VaccineGoodie(getWorld(), getX(), getY());
		getWorld()->addActor(v);
	}

}

SmartZombie::SmartZombie(StudentWorld * sw, double x, double y)
	: Zombie(sw, SPRITE_WIDTH*x, SPRITE_HEIGHT* y)
{}

void SmartZombie::doSomething()
{}

void SmartZombie::dieByFallOrBurnIfAppropriate()
{
	setDead();
	getWorld()->playSound(SOUND_ZOMBIE_DIE);
	getWorld()->increaseScore(2000);
}
