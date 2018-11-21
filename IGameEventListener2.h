namespace ValveSDK
{
	class IGameEventListener2
	{
	public:
		virtual	~IGameEventListener2(void) {};

		// FireEvent is called by EventManager if event just occured
		// KeyValue memory will be freed by manager if not needed anymore
		virtual void FireGameEvent(IGameEvent *event) = 0;
	};

}