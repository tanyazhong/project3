#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

class StudentWorld;
class Goodie;
class Penelope;

class Actor : public GraphObject{
public: 
	Actor(StudentWorld* sw, int imageID, double startX, double startY, Direction dir, int depth);
	virtual void doSomething() = 0;
	StudentWorld* getWorld() const;
	bool isAlive() const;
	void setDead();
	virtual void activateIfAppropriate(Actor* a);
	virtual void useExitIfAppropriate();
	virtual void dieByFallOrBurnIfAppropriate();
	virtual void beVomitedOnIfAppropriate();
	virtual void pickUpGoodieIfAppropriate(Goodie * g);
	virtual bool blocksMovement() const;       //automatically false
	virtual bool blocksFlame() const;          //auto false
	virtual bool triggersOnlyActiveLandmines() const;
	virtual bool triggersZombieVomit() const;
	virtual bool threatensCitizens() const;
	virtual bool triggersCitizens() const;
	virtual ~Actor();
private:
	StudentWorld* m_world;
	bool m_alive = true;
};

class Wall : public Actor {
public:
	Wall(StudentWorld* sw, double x, double y);
	virtual void doSomething();
	virtual bool blocksMovement() const;
	virtual bool blocksFlame() const;
};

class Exit : public Actor{
public:
	Exit(StudentWorld* sw, double x, double y);
	virtual void doSomething();
	virtual void activateIfAppropriate(Actor* a);
	virtual bool blocksFlame() const;
};

class Pit : public Actor{
public:
	Pit(StudentWorld* sw, double x, double y);
	virtual void doSomething();
	virtual void activateIfAppropriate(Actor* a);
};

class Flame : public Actor{
public:
	Flame(StudentWorld* sw, double x, double y, int dir);
	virtual void doSomething();
	virtual void activateIfAppropriate(Actor* a);
private:
	int m_lifeSpan = 0;
};

class Vomit : public Actor{
public:
	Vomit(StudentWorld* sw, double x, double y, int dir);
	virtual void doSomething();
	virtual void activateIfAppropriate(Actor* a);
private:
	int m_lifeSpan = 0;
};

class Landmine : public Actor{
public:
	Landmine(StudentWorld* sw, double x, double y);
	virtual void doSomething();
	virtual void activateIfAppropriate(Actor* a);
	virtual void dieByFallOrBurnIfAppropriate();
private:
	void explode(double x, double y);
	bool active() const;
	int m_nSafeTicks = 30;
};

class Goodie : public Actor{
public:
	Goodie(StudentWorld* sw, int imageID, double x, double y, int gp);
	virtual void activateIfAppropriate(Actor* a);
	virtual void dieByFallOrBurnIfAppropriate();
	virtual void pickUp(Penelope* p) = 0; // Have p pick up this goodie.
	virtual ~Goodie();
protected: 
	void decGraceP();
	int getGraceP();
private:
	int m_graceP;
};

class VaccineGoodie : public Goodie{
public:
	VaccineGoodie(StudentWorld* sw, double x, double y);
	virtual void doSomething();
	virtual void pickUp(Penelope* p);
};

class GasCanGoodie : public Goodie {
public:
	GasCanGoodie(StudentWorld* sw, double x, double y);
	virtual void doSomething();
	virtual void pickUp(Penelope* p);
};

class LandmineGoodie : public Goodie {
public:
	LandmineGoodie(StudentWorld* sw, double x, double y);
	virtual void doSomething();
	virtual void pickUp(Penelope* p);
};

class Agent : public Actor{
public:
	Agent(StudentWorld* w, int imageID, double x, double y, int dir);
	virtual void moveAgent(Direction d, double x, double y);
	virtual bool blocksMovement() const;
	virtual bool triggersOnlyActiveLandmines() const;
	virtual ~Agent();
};

class Zombie : public Agent{
public:
	Zombie(StudentWorld* sw, double x, double y);
	virtual bool triggersCitizens() const;
	virtual bool threatensCitizens() const;
	virtual ~Zombie();
protected:
	int getPlan() const;
	void setPlan(int p);
	void decPlan();
	bool shouldIVomit(double vx, double vy, Direction zd) const;
private:
	int m_planDist = 0;
};

class DumbZombie : public Zombie{
public:
	DumbZombie(StudentWorld* sw, double x, double y);
	virtual void doSomething();
	virtual void dieByFallOrBurnIfAppropriate();
private:
	bool m_paralyzed = false;
};

class SmartZombie : public Zombie
{
public:
	SmartZombie(StudentWorld* sw, double x, double y);
	virtual void doSomething();
	virtual void dieByFallOrBurnIfAppropriate();
private:
	bool m_paralyzed = false;
};

class Human : public Agent {       //penelope, citizns
public:
	Human(StudentWorld* sw, int imageID, double x, double y);
	virtual void beVomitedOnIfAppropriate();
	virtual bool triggersZombieVomit() const;
	bool isInfected() const;          //returns m_ninfections > 0
	int infectionDuration() const;    //returns m_ninfections
	void clearInfection();            //sets to zero
	void increaseInfections();       //increases by 1
private:
	int m_nInfections = 0;
};

class Penelope : public Human {
public:
	Penelope(StudentWorld * sw, double x, double y);
	virtual void doSomething();
	virtual void useExitIfAppropriate();
	virtual void dieByFallOrBurnIfAppropriate();
	virtual void pickUpGoodieIfAppropriate(Goodie* g);
	virtual bool triggersCitizens() const;
	virtual void moveAgent(Direction d, double x, double y);
	void increaseVaccines();
	void increaseFlameCharges();
	void increaseLandmines();
	int getNumVaccines() const;
	int getNumFlameCharges() const;
	int getNumLandmines() const;
	
private:
	void deployFlames(Direction d, double x, double y);
	void deployLandmine(double x, double y);
	void vaccinate();
	int m_nLandmines = 0;
	int m_nFlamethrowers = 0;
	int m_nVaccines = 0;
};

class Citizen : public Human
{
public:
	Citizen(StudentWorld* sw, double x, double y);
	virtual void doSomething();
	virtual void useExitIfAppropriate();
	virtual void dieByFallOrBurnIfAppropriate();
private:
	double findZDist(double x, double y, bool b) const;
	Direction findBestPath(double upd, double downd, double rightd, double leftd, double& cx, double& cy);
	bool m_paralyzed = false;
};


#endif // ACTOR_H_
