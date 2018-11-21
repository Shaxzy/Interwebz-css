#ifndef _CGAMEEVENTMANAGER_H_
#define _CGAMEEVENTMANAGER_H_

class cGameEventManager : public ValveSDK::IGameEventListener2
{
public:
	void FireGameEvent(ValveSDK::IGameEvent *event);
	void RegisterSelf();

};

extern cGameEventManager gGameEventManager;


#endif