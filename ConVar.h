namespace ValveSDK
{
	class ConVar
	{
	public:
		int GetInt()
		{
			PVOID pThisPtr = (PVOID)this;

			int iRet;

			__asm
			{
				MOV ECX, pThisPtr
				MOV EDX, [ECX + 0x1C]
				MOV EDX, [EDX + 0x30]
				MOV iRet, EDX
			}

			return iRet;
		}

		void SetValue(int iValue)
		{
			PVOID pThisPtr = (PVOID)this;

			__asm
			{
				PUSH iValue
				MOV ECX, pThisPtr
				MOV EDX, [ECX + 0x18]
				ADD ECX, 0x18
				CALL [EDX]
			}
		}
	};

	class IAppSystem
	{
	public:
		// Here's where the app systems get to learn about each other 
		virtual void func00() = 0;
		virtual void func01() = 0;
		virtual void func02() = 0;
		virtual void func03() = 0;
		virtual void func04() = 0;
	};

	class ICvar : public IAppSystem
	{
	public:
		virtual void funcy() = 0;
		virtual void funcy1() = 0;
		virtual void funcy2() = 0;
		virtual void funcy3() = 0;
		virtual void funcy4() = 0;
		virtual void funcy5() = 0;
		virtual void funcy6() = 0;
		virtual ConVar *FindVar(const char *var_name) = 0;
		virtual const ConVar *FindVar(const char *var_name) const = 0;
	};
}