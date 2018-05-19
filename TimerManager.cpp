#include "TimerManager.h"
#include <chrono>
#include <thread>

TimerManager::TimerManager() : latestGeneratedTimerID(0)
{}

TimerManager::~TimerManager()
{

}

int TimerManager::StartTimer(float duration)
{
	GenericTimer* timer = new GenericTimer();
	timer->start_ = std::chrono::high_resolution_clock::now();
	timer->duration_ = std::chrono::duration<float>(duration * 1000);

	int timerID = GenerateGenericTimerID();

	genericTimerList_.insert(std::pair<int, GenericTimer*>(timerID, timer));

	return timerID;
}

void TimerManager::Update(System* system)
{
	for(GenericTimerList::const_iterator timerIt = genericTimerList_.begin(), end = genericTimerList_.end();
		timerIt != end;
		++timerIt)
	{
		GenericTimer* timerObj = timerIt->second;

		if(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - timerObj->start_).count() > (timerObj->duration_).count())
		{
			genericTimerList_.erase(timerIt);
			break;
		}
	}
}

bool TimerManager::IsTimerRunning(int timerID)
{
	return (genericTimerList_.find(timerID) != genericTimerList_.end());
}

int TimerManager::GenerateGenericTimerID()
{
	return ++latestGeneratedTimerID;
}