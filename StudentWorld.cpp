#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <vector>
#include "Level.h"
#include "Actor.h"
using namespace std;
class Actor;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);                      //creates a new student world, returns pointer to a gameworld
}


// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)       //constructor
: GameWorld(assetPath), m_actors()
{
}

int StudentWorld::init()
{	
	Level curLev(assetPath());
	
	Level::LoadResult result = curLev.loadLevel("level03.txt");
	if (result == Level::load_fail_file_not_found)
		cerr << "Cannot find data file" << endl;
	else if (result == Level::load_fail_bad_format)
		cerr << "Your level was improperly formatted" << endl;
	else if (result == Level::load_success)
		cerr << "Successfully loaded level" << endl;

	Level::MazeEntry curSpot;
	for (int i = 0; i < LEVEL_WIDTH; i++) 
	{
		for (int j = 0; j < LEVEL_HEIGHT; j++) 
		{
			curSpot = curLev.getContentsOf(i, j);
			Actor* a;
			switch (curSpot)
			{
				case Level::player:
					m_pen = new Penelope(this, i, j);
					break;
				case Level::wall: 
					a = new Wall(this, i, j);
					m_actors.push_back(a);
					break; 
				case Level::exit:
					a = new Exit(this, i, j);
					m_actors.push_back(a);
					break;
				case Level::pit:
					a = new Pit(this, i, j);
					m_actors.push_back(a);
					break;
				case Level::citizen:
					a = new Citizen(this, i, j);
					m_actors.push_back(a);
					break;
				case Level::dumb_zombie:
					a = new DumbZombie(this, i, j);
					m_actors.push_back(a);
					break;
				case Level::smart_zombie:
					a = new SmartZombie(this, i, j);
					m_actors.push_back(a);
					break;
				case Level::gas_can_goodie:
					a = new GasCanGoodie(this, i, j);
					m_actors.push_back(a);
					break;
				case Level::landmine_goodie:
					a = new LandmineGoodie(this, i, j);
					m_actors.push_back(a);
					break;
				case Level::vaccine_goodie:
					a = new VaccineGoodie(this, i, j);
					m_actors.push_back(a);
					break;
			}
		}
	}
	return GWSTATUS_CONTINUE_GAME;
}


int StudentWorld::move()
{
// Give each actor a chance to do something, including Penelope
	if (m_pen->isAlive()) {
		m_pen->doSomething();
	}
	
	vector<Actor*>::iterator it;
	for(it = m_actors.begin(); it != m_actors.end(); it++)
	{
		(*it)->doSomething();
	}

	it = m_actors.begin();
	Actor* temp;
	while (it != m_actors.end()) {
		if (!(*it)->isAlive()){
			temp = *it;
			delete temp;
			it = m_actors.erase(it); //returns an iterator to the next element
		}
		else
			it++;
	}
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	delete m_pen;
	Actor* temp;
	vector<Actor*>::iterator it = m_actors.begin(); 
	while (it != m_actors.end()) {
		temp = *it;
		delete temp;
		it = m_actors.erase(it);
	}
}

void StudentWorld::addActor(Actor * a)
{
	m_actors.push_back(a);
	a->moveTo(a->getX()/SPRITE_WIDTH, a->getY()/SPRITE_HEIGHT);
}


StudentWorld::~StudentWorld() {
	cleanUp();
}

void StudentWorld::activateOnAppropriateActors(Actor* a)
{
	const int distance = 10;
	vector<Actor*>::const_iterator it;
	double actX = a->getX();
	double actY = a->getY();
	double deltaX, deltaY;
	for (it = m_actors.begin(); it != m_actors.end(); it++)
	{
		deltaX = actX - (*it)->getX();
		deltaY = actY - (*it)->getY();
		if (deltaX * deltaX + deltaY * deltaY <= distance * distance)  //actor overlaps
			a->activateIfAppropriate(*it);
	}

	deltaX = actX - m_pen->getX();
	deltaY = actY - m_pen->getY();
	if (deltaX * deltaX + deltaY * deltaY <= distance * distance)   //pen overlaps
		a->activateIfAppropriate(m_pen);
}


bool StudentWorld::canMove(double penDestX, double penDestY) const      
{
	double b_upperX, b_lowerX, b_upperY, b_lowerY;

	double p_lowerX = penDestX;
	double p_upperX = p_lowerX + SPRITE_WIDTH - 1;
	double p_lowerY = penDestY;
	double p_upperY = p_lowerY + SPRITE_HEIGHT - 1;

	vector<Actor*>::const_iterator it;
	for (it = m_actors.begin(); it != m_actors.end(); it++)
	{
		if ((*it)->blocksMovement())
		{
			b_lowerX = (*it)->getX();
			b_lowerY = (*it)->getY();
			b_upperX = b_lowerX + SPRITE_WIDTH - 1;
			b_upperY = b_lowerY + SPRITE_HEIGHT - 1;

			if (p_lowerX >= b_lowerX && p_lowerX <= b_upperX && 
				p_lowerY >= b_lowerY && p_lowerY <= b_upperY)
				return false;
			if (p_upperX >= b_lowerX && p_upperX <= b_upperX &&
				p_upperY >= b_lowerY && p_upperY <= b_upperY)
				return false; 
			if (p_lowerX >= b_lowerX && p_lowerX <= b_upperX &&
				p_upperY >= b_lowerY && p_upperY <= b_upperY)
				return false;
			if (p_upperX >= b_lowerX && p_upperX <= b_upperX &&
				p_lowerY >= b_lowerY && p_lowerY <= b_upperY)
				return false;
		}
	}
	return true;
}

bool StudentWorld::isFlameBlockedAt(double x, double y) const
{
	const int distance = 10;
	vector<Actor*>::const_iterator it;
	double deltaX, deltaY;
	for (it = m_actors.begin(); it != m_actors.end(); it++)
	{
		if ((*it)->blocksFlame()) {
			deltaX = x - (*it)->getX();
			deltaY = y - (*it)->getY();
			if (deltaX * deltaX + deltaY * deltaY <= distance * distance)  // overlaps
				return true;
		}
	}
	return false;
}


bool StudentWorld::locateNearestCitizenTrigger(double x, double y, double & otherX, double & otherY, double & distance, bool & isThreat) const
{
	// Return true if there is a living zombie or Penelope, otherwise false.
// If true, otherX, otherY, and distance will be set to the location and
// distance of the one nearest to (x,y), and isThreat will be set to true
// if it's a zombie, false if a Penelope.
	double px, py = 0;
	if (!m_pen->isAlive())
		return false;
	else {
		px = m_pen->getX();
		py = m_pen->getY();
	}

	vector<Actor*>::const_iterator it;
	double dx = 0;
	double dy = 0;     //distances
	double curdX, curdY;
	bool noZomb = true;
	for (it = m_actors.begin(); it != m_actors.end(); it++)
	{
		if ((*it)->triggersCitizens()) {
			noZomb = false;
			curdX = x - (*it)->getX();
			curdY = y - (*it)->getY();
			if (curdX * curdX + curdY * curdY <= dx * dx + dy * dy) {     //this is the currecnt closest zomb
				dx = curdX;
				dy = curdY;
				otherX = (*it)->getX();
				otherY = (*it)->getY();
			}
		}
	}

	if (noZomb || px * px + py * py < dx*dx + dy * dy) {   //pen is closer than zomb
		distance = px * px + py * py;
		otherX = px;
		otherY = py;
		isThreat = false;
		return true;
	}
	else{
		distance = dx * dx + dy * dy;
		otherX = dx;
		otherY = dy;
		isThreat = true;
		return true;
	}
	return false;
}