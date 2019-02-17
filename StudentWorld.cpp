#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <list>
#include "Level.h"
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);                      //creates a new student world, returns pointer to a gameworld
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)       //constructor
: GameWorld(assetPath)
{
	
}

int StudentWorld::init()
{
	Level curLev(assetPath());
	curLev.loadLevel("level01.txt");
	Level::MazeEntry curSpot;

	for (int i = 0; i < LEVEL_WIDTH; i++) 
	{
		for (int j = 0; j < LEVEL_HEIGHT; j++) 
		{
			curSpot = curLev.getContentsOf(i, j);
			switch (curSpot)
			{
				case Level::player:
					m_pen = new Penelope(i, j);
					break;
				case Level::wall:
					m_actors.push_back(new Wall(i, j));
					break;
			}
		}
	}
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
	

// Give each actor a chance to do something, including Penelope
	if (m_pen->alive()) {
		m_pen->doSomething();
	}

	for each of the actors in the game world
	{
	 if (actor[i] is still alive)
	 {
		 // tell each actor to do something (e.g. move)
		 actor[i]->doSomething();
		if (m_pen)
			decLives();
		return GWSTATUS_PLAYER_DIED;

		if (Penelope completed the current level)
		return GWSTATUS_FINISHED_LEVEL;
		 }
	}
		// Remove newly-dead actors after each tick
		Remove and delete dead game objects
		// Update the game status line
		Update Display Text // update the score/lives/level text at screen top
		// the player hasn’t completed the current level and hasn’t died, so
		// continue playing the current level
		return GWSTATUS_CONTINUE_GAME;

}

void StudentWorld::cleanUp()
{
}
