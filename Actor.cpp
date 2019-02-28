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

bool Actor::isAlive() const {
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
bool Actor::triggersOnlyActiveLandmines() const {
	return false;
}
bool Actor::triggersZombieVomit() const {
	return false;
}
bool Actor::threatensCitizens() const {
	return false;
}
bool Actor::triggersCitizens() const {
	return false;
}
Actor::~Actor(){}
void Actor::activateIfAppropriate(Actor * a){}
void Actor::useExitIfAppropriate(){}
void Actor::dieByFallOrBurnIfAppropriate(){}
void Actor::beVomitedOnIfAppropriate(){}
void Actor::pickUpGoodieIfAppropriate(Goodie * g){}
StudentWorld* Actor::getWorld() const{
	return m_world;
}


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
	m_lifeSpan++;
}

void Vomit::activateIfAppropriate(Actor * a) {
	a->beVomitedOnIfAppropriate();
}


Landmine::Landmine(StudentWorld * sw, double x, double y)
	:Actor(sw, IID_LANDMINE, SPRITE_WIDTH * x, SPRITE_HEIGHT * y, right, 1)
{}

void Landmine::doSomething()
{
	if (!isAlive())
		return;
	if (!active()) {
		m_nSafeTicks--;
		return;
	}
	getWorld()->activateOnAppropriateActors(this);
}

void Landmine::activateIfAppropriate(Actor * a) {
	if (a->triggersOnlyActiveLandmines()) {
		setDead();
		getWorld()->playSound(SOUND_LANDMINE_EXPLODE);
		explode(getX(), getY());
	}
}

void Landmine::dieByFallOrBurnIfAppropriate()
{
	setDead();
	getWorld()->playSound(SOUND_LANDMINE_EXPLODE); 
	explode(getX(), getY());
}

void Landmine::explode(double x, double y)
{
	double fx = x / SPRITE_WIDTH;
	double fy = y / SPRITE_HEIGHT;
	StudentWorld* sw = getWorld();
	//middle flame
	Actor* f = new Flame(sw, fx, fy, up);   
	sw->addActor(f);

	//top row
	f = new Flame(sw, fx - 1, fy + 1, up);
	sw->addActor(f);
	f = new Flame(sw, fx, fy + 1, up);
	sw->addActor(f);
	f = new Flame(sw, fx + 1, fy + 1, up);
	sw->addActor(f);

	//middle row
	f = new Flame(sw, fx - 1, fy, up);
	sw->addActor(f);
	f = new Flame(sw, fx + 1, fy, up);
	sw->addActor(f);

	//bottom row
	f = new Flame(sw, fx - 1, fy - 1, up);
	sw->addActor(f);
	f = new Flame(sw, fx, fy - 1, up);
	sw->addActor(f);
	f = new Flame(sw, fx + 1, fy - 1, up);
	sw->addActor(f);

	f = new Pit(sw, fx, fy);
	sw->addActor(f);
}

bool Landmine::active() const {
	return m_nSafeTicks == 0;
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
Goodie::~Goodie(){}


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
	return true;
}
Agent::~Agent(){}


Human::Human(StudentWorld* sw, int imageID, double x, double y)
	: Agent(sw, imageID,  x, y, right)
{}
void Human::beVomitedOnIfAppropriate(){
	m_nInfections++;
}
bool Human::triggersZombieVomit() const {
	return true;
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
void Agent::moveAgent(Direction d, double x, double y) {
	if (getWorld()->canMove(x, y, this)) {
		setDirection(d);
		moveTo(x, y);
	}
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
			vaccinate();
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
	if (getWorld()->canMove(x, y, this))
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
			fx = x; fy = y + i * SPRITE_HEIGHT;	break;
		case down:
			fx = x; fy = y - i * SPRITE_HEIGHT; break;
		case left:
			fx = x - i * SPRITE_WIDTH; fy = y; break;
		case right:
			fx = x + i * SPRITE_WIDTH; fy = y;
		}
		if (getWorld()->isFlameBlockedAt(fx, fy))
			break;
		Actor* f = new Flame(getWorld(), fx/SPRITE_WIDTH, fy/SPRITE_HEIGHT, up);
		getWorld()->addActor(f);
	}
}

void Penelope::deployLandmine(Direction d, double x, double y)
{
	if (m_nLandmines > 0) {
		Actor* l = new Landmine(getWorld(), x/SPRITE_HEIGHT, y/SPRITE_WIDTH);
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
	if (getWorld()->citizensLeft())
		return;
	getWorld()->recordLevelFinishedIfAllCitizensGone();
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
	double cx = getX(); double cy = getY();
	if (!isAlive())
		return;
	if (isInfected())
		increaseInfections();
	if (infectionDuration() == 500) {
		setDead();
		getWorld()->recordCitizenGone();
		getWorld()->playSound(SOUND_ZOMBIE_BORN);
		getWorld()->increaseScore(-1000);
		int n = randInt(0, 9);
		Actor* z;
		if (n < 3)     //0 1 or 2
			z = new SmartZombie(getWorld(), cx / SPRITE_WIDTH, cy / SPRITE_HEIGHT);
		else
			z = new DumbZombie(getWorld(), cx / SPRITE_WIDTH, cy / SPRITE_HEIGHT);
		getWorld()->addActor(z);
		return;
	}
	if (m_paralyzed) {
		m_paralyzed = false;
		return;
	}
	
	bool isThreat;
	double ox, oy, d, dx, dy;
	if (!getWorld()->locateNearestCitizenTrigger(cx, cy, ox, oy, d, isThreat)) {      //no triggers
		m_paralyzed = true;
		return;
	}

	dx = ox - cx; dy = oy - cy;
	Direction xtoP, ytoP;
	if (dy > 0)
		ytoP = up;
	else
		ytoP = down;
	if (dx > 0)
		xtoP = right;
	else
		xtoP = left;

	if (!isThreat && d <= 80 * 80) {               //penelope is near
		if (cx == ox) {     //same column
			if (ytoP == up)        //pen is above
				cy += 2;
			else
				cy -= 2;
			moveAgent(ytoP, cx, cy);
			m_paralyzed = true;
			return;
		}
		else if (cy == oy){
			if (xtoP == right)        //pen is right
				cx += 2; 
			else
				cx -= 2;
			moveAgent(xtoP, cx, cy);
			m_paralyzed = true;
			return;
		}

		int n = randInt(0, 1);                           //THIS IS IF ELSE HELL
		if (n == 0) {      //move vertically
			if (ytoP == up) 
				cy += 2;
			else
				cy -= 2;

			if (getWorld()->canMove(cx, cy, this))          //if can move there, move
				moveAgent(ytoP, cx, cy);
			else {                                         //else try moving horizontally
				if (xtoP == right)
					cx += 2;
				else
					cx -= 2;
				moveAgent(xtoP, cx, cy);
			}
		}
		else {        //move horizontally
			if (xtoP == right)
				cx += 2;
			else
				cx -= 2;

			if (getWorld()->canMove(cx, cy, this))
				moveAgent(xtoP, cx, cy);
			else {
				if (ytoP == up)        //pen is above
					cy += 2;
				else
					cy -= 2;
				moveAgent(ytoP, cx, cy);
			}
		}
		m_paralyzed = true;
		return;
	}

	bool Bup, Bdown, Bright, Bleft; double upD, downD, rightD, leftD;
	if (getWorld()->locateNearestCitizenThreat(cx, cy, ox, oy, d)) {
		if (d <= 80 * 80) {
			Bup = getWorld()->canMove(cx, cy + 2, this);
			Bdown = getWorld()->canMove(cx, cy - 2, this);
			Bright = getWorld()->canMove(cx + 2, cy - 2, this);
			Bleft = getWorld()->canMove(cx - 2, cy, this);
			upD = findZDist(cx, cy + 2, Bup);
			downD = findZDist(cx, cy - 2, Bdown);
			rightD = findZDist(cx + 2, cy, Bright);
			leftD = findZDist(cx - 2, cy, Bleft);
			if (upD < d && downD < d && rightD < d && leftD < d) {
				m_paralyzed = true;
				return;
			}
			setDirection(findBestPath(upD, downD, rightD, leftD, cx, cy));
			moveTo(cx, cy);
		}
	}
	m_paralyzed = true;
}

double Citizen::findZDist(double x, double y, bool b) const
{
	if (!b)
		return 0;
	double ox, oy, d;
	getWorld()->locateNearestCitizenThreat(x, y, ox, oy, d);
	return d;
}

Direction Citizen::findBestPath(double upd, double downd, double rightd, double leftd, double& cx, double& cy)
{
	double most = upd;
	if (downd > most)
		most = downd;
	if (rightd > most)
		most = rightd;
	if (leftd > most)
		most = leftd;
	
	if (most == upd) {
		cy += 2;
		return up;
	}
	else if (most == downd) {
		cy -= 2;
		return down;
	}
	else if (most == rightd) {
		cx += 2;
		return right;
	}
	else {
		cx -= 2;
		return left;
	}
}

void Citizen::useExitIfAppropriate(){
	getWorld()->increaseScore(500);
	setDead();
	getWorld()->playSound(SOUND_CITIZEN_SAVED);
	getWorld()->recordCitizenGone();
}

void Citizen::dieByFallOrBurnIfAppropriate(){
	getWorld()->increaseScore(-1000);
	setDead();
	getWorld()->playSound(SOUND_CITIZEN_DIE);
	getWorld()->recordCitizenGone();
}

Zombie::Zombie(StudentWorld * sw, double x, double y)
	:Agent(sw, IID_ZOMBIE, x, y, right)
{}

bool Zombie::triggersCitizens() const {
	return true;
}

bool Zombie::threatensCitizens() const {
	return true;
}

int Zombie::getPlan() const {
	return m_planDist;
}

void Zombie::setPlan(int p) {
	m_planDist = p;
}

void Zombie::decPlan() {
	m_planDist--;
}

Zombie::~Zombie() {}

bool Zombie::shouldIVomit(double vx, double vy, Direction zd) const
{
	switch (zd) {
	case up:
		vy += SPRITE_HEIGHT; break;
	case down:
		vy -= SPRITE_HEIGHT; break;
	case right:
		vx += SPRITE_WIDTH; break;
	case left:
		vx -= SPRITE_WIDTH;
	}
	if (getWorld()->isZombieVomitTriggerAt(vx, vy)) {
		int n = randInt(0, 2);
		if (n == 0) {
			Actor* v = new Vomit(getWorld(), vx / SPRITE_WIDTH, vy / SPRITE_HEIGHT, getDirection());
			getWorld()->addActor(v);
			getWorld()->playSound(SOUND_ZOMBIE_VOMIT);
			return true;
		}
	}
	return false;
}


DumbZombie::DumbZombie(StudentWorld * sw, double x, double y)
	:Zombie(sw, SPRITE_WIDTH * x, SPRITE_HEIGHT * y)
{}

void DumbZombie::doSomething()
{
	if (!isAlive())
		return;
	if (m_paralyzed) {
		m_paralyzed = false;
		return;
	}
	if (shouldIVomit(getX(), getY(), getDirection())){
		m_paralyzed = true;
		return;
	}
	if (getPlan() == 0) {
		setPlan(randInt(3, 10));
		int n = randInt(0, 3);
		switch (n) {
		case 0:
			setDirection(up); break;
		case 1:
			setDirection(down); break;
		case 2:
			setDirection(right); break;
		case 3:
			setDirection(left);
		}
	}
	double zx = getX(); double zy = getY();
	Direction zd = getDirection();
	switch (zd) {
	case up:
		zy++; break;
	case down: 
		zy--; break;
	case right: 
		zx++; break;
	case left:
		zx--;
	}
	if (getWorld()->canMove(zx, zy, this)) {
		moveTo(zx, zy);
		decPlan();
	}
	else
		setPlan(0);
	m_paralyzed = true;
}

void DumbZombie::dieByFallOrBurnIfAppropriate()
{
	setDead();
	getWorld()->playSound(SOUND_ZOMBIE_DIE);
	getWorld()->increaseScore(1000);
	int n;
	n = randInt(0, 9);  
	if (n == 0) {
		Actor* v = new VaccineGoodie(getWorld(), getX()/SPRITE_WIDTH, getY()/SPRITE_HEIGHT);
		getWorld()->addActor(v);
	}
}

SmartZombie::SmartZombie(StudentWorld * sw, double x, double y)
	: Zombie(sw, SPRITE_WIDTH * x, SPRITE_HEIGHT * y)
{}

void SmartZombie::doSomething()
{
	if (!isAlive())
		return;
	if (m_paralyzed) {
		m_paralyzed = false;
		return;
	}
	double zx = getX(); double zy = getY();
	double ox, oy, d;
	if (shouldIVomit(zx, zy, getDirection())) {
		m_paralyzed = true;
		return;
	}
	if (getPlan() == 0) {
		setPlan(randInt(3, 10));
		getWorld()->locateNearestVomitTrigger(zx, zy, ox, oy, d);
		if (d > 80 * 80) {
			int n = randInt(0, 3);
			switch (n) {
			case 0:
				setDirection(up); break;
			case 1:
				setDirection(down); break;
			case 2:
				setDirection(right); break;
			case 3:
				setDirection(left);
			}
		}
		else {
			double dx = ox - zx; double dy = oy - zy;
			Direction xtoP, ytoP;
			if (dy > 0)
				ytoP = up;
			else
				ytoP = down;
			if (dx > 0)
				xtoP = right;
			else
				xtoP = left;

			if (zx == ox) //same column
				setDirection(ytoP);
			else if (zy == oy)
				setDirection(xtoP);
			else {
				int n = randInt(0, 1);
				if (n == 0)
					setDirection(ytoP);
				else
					setDirection(xtoP);
			}
		}
	}
	Direction zd = getDirection();
	switch (zd) {
	case up:
		zy++; break;
	case down:
		zy--; break;
	case right:
		zx++; break;
	case left:
		zx--;
	}
	if (getWorld()->canMove(zx, zy, this)) {
		moveTo(zx, zy);
		decPlan();
	}
	else
		setPlan(0);
	m_paralyzed = true;
}

void SmartZombie::dieByFallOrBurnIfAppropriate() {
	setDead();
	getWorld()->playSound(SOUND_ZOMBIE_DIE);
	getWorld()->increaseScore(2000);
}
