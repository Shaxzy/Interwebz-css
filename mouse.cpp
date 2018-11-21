#include "DllMain.h"

cMouse g_Mouse;

//not mine

//parameters:
//bDrag : pass a new boolean for it as this only handles if drag object is being dragged (change MAX_DRAG_OBJECTS and pass the bDragged from gmouse)
//bCheck : a check if any other drag objects are being dragged (to prevent dragging other objects while current one is being dragged, so pass in other bDrag booleans)
//bDragCheck : basically pass mouse.leftclick(x,y,w,h) as the param(the place where you have to hold down mouse to drag)
//x : dragobject's x axis location on screen which we wanna modify
//y : dragobject's y axis location on screen which we wanna modify
//xdif : pass an empty int (change MAX_DRAG_OBJECTS and pass the iDiffX from gmouse here)
//ydif : pass an empty int (change MAX_DRAG_OBJECTS and pass the iDiffY from gmouse here)

void cMouse::Drag(bool& bDrag, bool bCheck, bool bDragCheck,int& x, int& y, int& xdif, int& ydif)
{
	if(bCheck)								  
	{					
		if(bDragCheck || (mouse1pressed && bDrag))		  
		{
			if(!bDrag)					  
				bDrag = true;

			if(xdif == -1 || ydif == -1)	  
			{							
				xdif = mouse_x - x;	  
				ydif = mouse_y - y;	  
			}

			x += mouse_x - (xdif + x); 
			y += mouse_y - (ydif + y); 
		}									  
		else								  
		{			
			if(bDrag)
				bDrag = false;

			xdif = -1;						  
			ydif = -1;						  
		}									  
	}
}

//not mine
void cMouse::DrawMouse()
{
	//kolonote: we gotta implement pat0's COLORCODE macros
	static int default_r = 3, default_g = 6, default_b = 26, default_a = 215;

	g_Draw.FillRGBA(mouse_x + 1, mouse_y, 1, 17, default_r, default_g, default_b, default_a);

	for(int i = 0; i < 11; i++)
	g_Draw.FillRGBA(mouse_x + 2 + i, mouse_y + 1 + i, 1, 1, default_r, default_g, default_b, default_a);
	g_Draw.FillRGBA(mouse_x + 7, mouse_y + 12, 6, 1, default_r, default_g, default_b, default_a);
	g_Draw.FillRGBA(mouse_x + 6, mouse_y + 12, 1, 1, default_r, default_g, default_b, default_a);
	g_Draw.FillRGBA(mouse_x + 5, mouse_y + 13, 1, 1, default_r, default_g, default_b, default_a);
	g_Draw.FillRGBA(mouse_x + 4, mouse_y + 14, 1, 1, default_r, default_g, default_b, default_a);
	g_Draw.FillRGBA(mouse_x + 3, mouse_y + 15, 1, 1, default_r, default_g, default_b, default_a);
	g_Draw.FillRGBA(mouse_x + 2, mouse_y + 16, 1, 1, default_r, default_g, default_b, default_a);

	static int mouse_r = 255, mouse_g = 255, mouse_b = 255, mouse_a = 255;

	for(int i = 0; i < 4; i++)
	g_Draw.FillRGBA(mouse_x + 2 + i, mouse_y + 2 + i, 1, 14 - (i * 2), mouse_r,mouse_g,mouse_b,mouse_a);
	g_Draw.FillRGBA(mouse_x + 6, mouse_y + 6, 1, 6, mouse_r,mouse_g,mouse_b,mouse_a);
	g_Draw.FillRGBA(mouse_x + 7, mouse_y + 7, 1, 5, mouse_r,mouse_g,mouse_b,mouse_a);	
	g_Draw.FillRGBA(mouse_x + 8, mouse_y + 8, 1, 4, mouse_r,mouse_g,mouse_b,mouse_a);
	g_Draw.FillRGBA(mouse_x + 9, mouse_y + 9, 1, 3, mouse_r,mouse_g,mouse_b,mouse_a);
	g_Draw.FillRGBA(mouse_x + 10, mouse_y + 10, 1, 2, mouse_r,mouse_g,mouse_b,mouse_a);
	g_Draw.FillRGBA(mouse_x + 11, mouse_y + 11, 1, 1, mouse_r,mouse_g,mouse_b,mouse_a);
}

bool cMouse::LeftClick(int x,int y,int w,int h)
{
	return (mouse1pressed && IsOver(x,y,w,h));
}

bool cMouse::RightClick(int x,int y,int w,int h)
{
	return (mouse2pressed && IsOver(x,y,w,h));
}

bool cMouse::OneLeftClick(int x,int y,int w,int h)
{
	return (mouse1released && IsOver(x,y,w,h));
}

bool cMouse::OneRightClick(int x,int y,int w,int h)
{
	return (mouse2released && IsOver(x,y,w,h));
}

bool cMouse::IsOver(int x,int y,int w,int h)
{
	return (mouse_x > x  && w+x > mouse_x && mouse_y > y  && h+y > mouse_y);
}

//kolonote:
//now the credits to this go to someone that posted it on GD in 2004 i think i forgot his name
void cMouse::Update(ValveSDK::ConVar *mousevar)
{
	static ValveSDK::ConVar *pRawInput = g_Valve.pConVar->FindVar(/*m_rawinput*/XorStr<0x63,11,0x160FA730>("\x0E\x3B\x17\x07\x10\x01\x07\x1A\x1E\x18"+0x160FA730).s);	

	//IMPORTANT FIX DONT REMOVE ME
	if(pRawInput->GetInt())
		pRawInput->SetValue(0);

	int width = g_Valve.pClient->GetScreenWidth();
	int height = g_Valve.pClient->GetScreenHeight();

	int winX = width * 0.5;
	int winY = height * 0.5;

	//3 winapi calls is all we need
	tagPOINT tp;
	GetCursorPos(&tp);

	LPPOINT pPoint = &tp;
	ScreenToClient(GetForegroundWindow(),pPoint);

	tp.x = pPoint->x;
	tp.y = pPoint->y;

	if(!g_Valve.pEngine->IsInGame())
	{
		mouse_x = tp.x;
		mouse_y = tp.y;
	}
	else
	{
		if((tp.x - winX) != 0 || (tp.y - winY) != 0)
		{
			mouse_x += (tp.x - winX) * 2;
			mouse_y += (tp.y - winY) * 2;
		}
	}

	//just make sure it knows that it has to revert back mouse... sadly using mouseenable isnt the best way but ok
	if(mousevar->GetInt())
	{
		mousevar->SetValue(0);
		
		mouse_x = winX;
		mouse_y = winY;

		bReturn = true;
	}

	//the mouse coordinates depending on the screen size
	if(mouse_x > width) 
		mouse_x = width;

	if(mouse_x < 0)
		mouse_x = 0;

	if(mouse_y > height)
		mouse_y = height;

	if(mouse_y < 0)
		mouse_y = 0;

	//HANDLING:

	//handle mouse1
	if(GetAsyncKeyState(VK_LBUTTON))
		mouse1pressed = true;
	else if(!GetAsyncKeyState(VK_LBUTTON))
	{
		if(mouse1pressed)
			mouse1released = true;
		else
			mouse1released = false;

		mouse1pressed = false;
	}

	//handle mouse2
	if(GetAsyncKeyState(VK_RBUTTON))
		mouse2pressed = true;
	else if(!GetAsyncKeyState(VK_RBUTTON))
	{
		if(mouse2pressed)
			mouse2released = true;
		else
			mouse2released = false;

		mouse2pressed = false;
	}
}








