#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include <vector>
#include <string>


// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Actor;
class Penelope;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
	void addActor(Actor* a);
	void activateOnAppropriateActors(Actor* a);
	void recordCitizenGone();
	void recordLevelFinishedIfAllCitizensGone();
	bool citizensLeft() const;
	bool canMove(double x, double y, Actor* a) const;
	bool isFlameBlockedAt(double x, double y) const;
	bool locateNearestCitizenTrigger(double x, double y, double& otherX, double& otherY, double& distance, bool& isThreat) const;
	bool locateNearestCitizenThreat(double x, double y, double& otherX, double& otherY, double& distance) const;
	bool isZombieVomitTriggerAt(double x, double y) const;
	bool locateNearestVomitTrigger(double x, double y, double& otherX, double& otherY, double& distance);
	virtual ~StudentWorld();
	
private:
	std::vector<Actor*> m_actors;
	Penelope* m_pen;
	int m_nCitizens;
	bool m_levFinished = false;
};


#endif // STUDENTWORLD_H_
