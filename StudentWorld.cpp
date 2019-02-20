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
	
	Level::LoadResult result = curLev.loadLevel("level01.txt");
	if (result == Level::load_fail_file_not_found)
		cerr << "Cannot find level01.txt data file" << endl;
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
			switch (curSpot)
			{
				case Level::player:
					m_pen = new Penelope(this, i, j);
					break;
				case Level::wall:
					Actor* w = new Wall(this, i, j);
					m_actors.push_back(w);
					break;
			}
		}
	}
	
	//m_pen = new Penelope(100, 200);
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
// Give each actor a chance to do something, including Penelope
	if (m_pen->alive()) {
		m_pen->doSomething();
	}
	
	vector<Actor*>::iterator it;
	for(it = m_actors.begin(); it != m_actors.end(); it++)
	{
		(*it)->doSomething();
		
	}
	
	decLives();
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	delete m_pen;
	vector<Actor*>::iterator it;
	for (it = m_actors.begin(); it != m_actors.end(); it++)
		m_actors.erase(it);
}

StudentWorld::~StudentWorld() {
	cleanUp();
}

bool StudentWorld::canMove(double penDestX, double penDestY) const      
{
	//if the destination is inside a blocker's bounding box retrun false.
	//penelope's bounding box cannot intersect at all with Q's boudning box
	//then pen's bounding box is from (penDestX, penDestY) to (penDestX + spritewidth - 1, penDestY + spritewidth - 1)
	double b_upperX, b_lowerX, b_upperY, b_lowerY;

	double p_lowerX = penDestX;
	double p_upperX = p_lowerX + SPRITE_WIDTH - 1;
	double p_lowerY = penDestY;
	double p_upperY = p_lowerY + SPRITE_HEIGHT - 1;

	vector<Actor*>::const_iterator it;
	for (it = m_actors.begin(); it != m_actors.end(); it++)
	{
		if ((*it)->blocker())
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
		}
	}
	return true;
}
