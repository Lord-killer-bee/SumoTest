#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

#include "GameEntity.h"
#include "Bullet.h"
#include "Game.h"
#include <map>
#include <list>
#include <chrono>

class TimerManager : public GameEntity
{
public:
	TimerManager();
	~TimerManager();

	struct GenericTimer
	{
		int timerID_;
		std::chrono::system_clock::time_point start_;
		std::chrono::duration<float> duration_;
	};

	void Update(System* system);
	int StartTimer(float duration);
	bool IsTimerRunning(int timerID);

private:
	int latestGeneratedTimerID;

	typedef std::map<int, GenericTimer*> GenericTimerList;
	GenericTimerList genericTimerList_;
	int GenerateGenericTimerID();
};

#endif //TIMER_H_INCLUDED