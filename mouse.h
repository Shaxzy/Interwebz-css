#define MAX_DRAG_OBJECTS 3

class cMouse
{
public:

	//called after menu drawn
	void DrawMouse();

	//called when menu on/whatever
	void Update(ValveSDK::ConVar *mousevar);

	bool LeftClick(int x,int y,int w,int h);
	bool OneLeftClick(int x,int y,int w,int h);
	bool RightClick(int x,int y,int w,int h);
	bool OneRightClick(int x,int y,int w,int h);
	bool IsOver(int x,int y,int w,int h);

	void Drag(bool& bDrag, bool bCheck, bool bDragCheck,int& x, int& y, int& xdif, int& ydif);

	void GetMousePosition(int &posx, int &posy)
	{
		posx = mouse_x;
		posy = mouse_y;
	}

	bool HasMouseOneJustBeenReleased()
	{
		return mouse1released;
	}

	bool IsMouseTwoBeingHeld()
	{
		return mouse2pressed;
	}

	bool bReturn;

	bool bDragged[MAX_DRAG_OBJECTS];
	int iDiffX[MAX_DRAG_OBJECTS];
	int iDiffY[MAX_DRAG_OBJECTS];

private:
	int mouse_x, mouse_y;

	bool mouse1pressed;
	bool mouse1released;
	bool mouse2pressed;
	bool mouse2released;
};

extern cMouse g_Mouse;
