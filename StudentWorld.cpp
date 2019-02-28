#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <vector>
#include "Level.h"
#include "Actor.h"
#include <iostream> // defines the overloads of the << operator
#include <sstream>  // defines the type std::ostringstream
#include <iomanip>
using namespace std;
class Actor;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);                      //creates a new student world, returns pointer to a gameworld
}


// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)       //constructor
: GameWorld(assetPath), m_actors(), m_nCitizens(0)
{
}

int StudentWorld::init()
{	
	int level =  getLevel();
	if (level == 99)
		return GWSTATUS_PLAYER_WON;
	ostringstream levtxt;
	levtxt.fill('0');
	levtxt << "level";
	levtxt << setw(2) << level << ".txt";
	string leveltext = levtxt.str();
	Level curLev(assetPath());
	Level::LoadResult result = curLev.loadLevel(leveltext);
	if (result == Level::load_fail_file_not_found)
		return GWSTATUS_PLAYER_WON;
	else if (result == Level::load_fail_bad_format)
		return GWSTATUS_LEVEL_ERROR;

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
					m_pen = new Penelope(this, i * SPRITE_WIDTH, j* SPRITE_HEIGHT);
					break;
				case Level::wall: 
					a = new Wall(this, i* SPRITE_WIDTH, j* SPRITE_HEIGHT);
					m_actors.push_back(a);
					break; 
				case Level::exit:
					a = new Exit(this, i* SPRITE_WIDTH, j* SPRITE_HEIGHT);
					m_actors.push_back(a);
					break;
				case Level::pit:
					a = new Pit(this, i* SPRITE_WIDTH, j* SPRITE_HEIGHT);
					m_actors.push_back(a);
					break;
				case Level::citizen:
					a = new Citizen(this, i* SPRITE_WIDTH, j* SPRITE_HEIGHT);
					m_actors.push_back(a);
					m_nCitizens++;
					break;
				case Level::dumb_zombie:
					a = new DumbZombie(this, i* SPRITE_WIDTH, j* SPRITE_HEIGHT);
					m_actors.push_back(a);
					break;
				case Level::smart_zombie:
					a = new SmartZombie(this, i* SPRITE_WIDTH, j* SPRITE_HEIGHT);
					m_actors.push_back(a);
					break;
				case Level::gas_can_goodie:
					a = new GasCanGoodie(this, i* SPRITE_WIDTH, j* SPRITE_HEIGHT);
					m_actors.push_back(a);
					break;
				case Level::landmine_goodie:
					a = new LandmineGoodie(this, i* SPRITE_WIDTH, j* SPRITE_HEIGHT);
					m_actors.push_back(a);
					break;
				case Level::vaccine_goodie:
					a = new VaccineGoodie(this, i* SPRITE_WIDTH, j* SPRITE_HEIGHT);
					m_actors.push_back(a);
					break;
			}
		}
	}
	m_levFinished = false;
	return GWSTATUS_CONTINUE_GAME;
}


int StudentWorld::move()
{
	if (m_pen->isAlive())
		m_pen->doSomething();
	else {
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}

	if (m_levFinished)
		return GWSTATUS_FINISHED_LEVEL;

	int n = m_actors.size();
	for(int i = 0; i != n; i++)
	{
		m_actors[i]->doSomething();
	}
	
	vector<Actor*>::iterator x = m_actors.begin();
	Actor* temp;
	while (x != m_actors.end()) {
		if (!(*x)->isAlive()){
			temp = *x;
			delete temp;
			x = m_actors.erase(x); //returns an iterator to the next element
		}
		else
			x++;
	}
	
	ostringstream oss;
	oss << "Score: ";
	oss.fill('0');
	if (getScore() < 0) {
		oss << "-";
		oss << setw(5) << -getScore();
	}
	else {
		oss << setw(6) << getScore();
	}
	oss << "  Level: ";
	oss << setw(2) << getLevel();
	oss << "  Lives: " << getLives();
	oss << "  Vaccines: " <<  m_pen->getNumVaccines();
	oss << "  Flames: " <<  m_pen->getNumFlameCharges();
	oss << "  Mines: " <<  m_pen->getNumLandmines();
	oss << "  Infected: " << m_pen->infectionDuration();
	string s = oss.str();
	setGameStatText(s);

	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	if (m_pen != nullptr) {
		delete m_pen;
		m_pen = nullptr;
	}
	Actor* temp;
	vector<Actor*>::iterator it = m_actors.begin(); 
	while (it != m_actors.end()) {
		temp = *it;
		delete temp;
		it = m_actors.erase(it);
	}
}

void StudentWorld::addActor(Actor * a) {
	m_actors.push_back(a);
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

void StudentWorld::recordCitizenGone()
{
	m_nCitizens--;
}

void StudentWorld::recordLevelFinishedIfAllCitizensGone()
{
	m_levFinished = true;
}

bool StudentWorld::citizensLeft() const
{
	return m_nCitizens > 0;
}


bool StudentWorld::canMove(double DestX, double DestY, Actor* a) const      
{
	double b_upperX, b_lowerX, b_upperY, b_lowerY;
	double p_lowerX = DestX;
	double p_upperX = p_lowerX + SPRITE_WIDTH - 1;
	double p_lowerY = DestY;
	double p_upperY = p_lowerY + SPRITE_HEIGHT - 1;

	vector<Actor*>::const_iterator it;
	for (it = m_actors.begin(); it != m_actors.end(); it++)
	{
		if (*it == a)
			continue;
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

	if (m_pen != a && m_pen->isAlive()) {
		b_lowerX = m_pen->getX();
		b_lowerY = m_pen->getY();
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
	double px = 0; double py = 0;
	if (!m_pen->isAlive())
		return false;
	else {
		px = m_pen->getX(); py = m_pen->getY();
	}
	
	double pdx = x - px; double pdy = y - py;
	double pdist = pdx * pdx + pdy * pdy;
	if (locateNearestCitizenThreat(x, y, otherX, otherY, distance)) {  // a zombie exists
		if (distance <= pdist) {
			isThreat = true;
			return true;
		}
	}
	otherX = px; otherY = py;
	distance = pdist;
	isThreat = false;
	return true;
}

bool StudentWorld::locateNearestCitizenThreat(double x, double y, double & otherX, double & otherY, double & distance) const
{
	bool z = false;
	double dx = 256; double dy = 256;
	vector<Actor*>::const_iterator it;
	for (it = m_actors.begin(); it != m_actors.end(); it++) {
		if ((*it)->threatensCitizens()) {     //actor is a zombie
			double zx = (*it)->getX(); double zy = (*it)->getY();
			double zdx = x - zx; double zdy = y - zy;
			if (zdx * zdx + zdy * zdy < dx * dx + dy * dy) {   //zombie is current closest
				dx = zdx; dy = zdy; //set the distance
				otherX = zx; otherY = zy; //set the locations
				distance = dx * dx + dy * dy;
				z = true;
			}
		}
	}
	return z;
}

bool StudentWorld::isZombieVomitTriggerAt(double x, double y) const
{
	const int distance = 10;
	vector<Actor*>::const_iterator it;
	double deltaX, deltaY;
	for (it = m_actors.begin(); it != m_actors.end(); it++)	{
		if ((*it)->triggersZombieVomit()) {
			deltaX = x - (*it)->getX(); deltaY = y - (*it)->getY();
			if (deltaX * deltaX + deltaY * deltaY <= distance * distance)  
				return true;
		}
	}
	deltaX = x - m_pen->getX(); deltaY = y - m_pen->getY();      //now check penelope
	if (deltaX * deltaX + deltaY * deltaY <= distance * distance)
		return true;
	else
		return false;
}

bool StudentWorld::locateNearestVomitTrigger(double x, double y, double & otherX, double & otherY, double & distance)
{ 
	bool c = false;
	double dx = 256; double dy = 256;
	vector<Actor*>::const_iterator it;
	for (it = m_actors.begin(); it != m_actors.end(); it++) {
		if ((*it)->triggersZombieVomit()) {
			double cx = (*it)->getX(); double cy = (*it)->getY();
			double cdx = x - cx; double cdy = y - cy;
			if (cdx * cdx + cdy * cdy < dx * dx + dy * dy) {   //zombie is current closest
				dx = cdx; dy = cdy; //set the distance
				otherX = cx; otherY = cy; //set the locations
				distance = dx * dx + dy * dy;
				c = true;
			}
		}
	}
	double pdx = x - m_pen->getX(); double pdy = y - m_pen->getY();
	double pdist = pdx * pdx + pdy * pdy;
	if (c) 
		if (distance <= pdist) 
			return true;
	otherX = m_pen->getX(); otherY = m_pen->getY();
	distance = pdist;
	return true;
}


