#include "DllMain.h"


ValveSDK::ISurface* g_pSurface = NULL;

VOID CDraw::InitFonts()
{
	g_pSurface = g_Valve.pSurface;

	m_ESPFont = g_pSurface->CreateFont();
	g_pSurface->SetFontGlyphSet(m_ESPFont, /*Calibri*/XorStr<0xD7,8,0xBA08B53A>("\x94\xB9\xB5\xB3\xB9\xAE\xB4"+0xBA08B53A).s, 13, FW_DONTCARE, 0, 0, 0x200);

	m_WatermarkFont = g_pSurface->CreateFont();
	g_pSurface->SetFontGlyphSet(m_WatermarkFont, /*Calibri*/XorStr<0xD7,8,0xBA08B53A>("\x94\xB9\xB5\xB3\xB9\xAE\xB4"+0xBA08B53A).s, 20, FW_DONTCARE, 0, 0, 0x200);

	m_MenuFont = g_pSurface->CreateFont();
	g_pSurface->SetFontGlyphSet(m_MenuFont, /*Visitor TT2 BRK*/XorStr<0x83,16,0x5F987AA5>("\xD5\xED\xF6\xEF\xF3\xE7\xFB\xAA\xDF\xD8\xBF\xAE\xCD\xC2\xDA"+0x5F987AA5).s, 15, 500, 0, 0, 0x200);

	m_ListItemFont = g_pSurface->CreateFont();
	g_pSurface->SetFontGlyphSet(m_ListItemFont, /*Visitor TT2 BRK*/XorStr<0x83,16,0x5F987AA5>("\xD5\xED\xF6\xEF\xF3\xE7\xFB\xAA\xDF\xD8\xBF\xAE\xCD\xC2\xDA"+0x5F987AA5).s, 13, 500, 0, 0, 0x010);
}

VOID CDraw::DrawString(unsigned long font, int x, int y, int r, int g, int b, const wchar_t *pszText)
{
	if (pszText == NULL)
		return;

	g_pSurface->DrawSetTextPos(x, y);
	g_pSurface->DrawSetTextFont(font);
	g_pSurface->DrawSetTextColor(ValveSDK::Color(r, g, b, 255));
	g_pSurface->DrawPrintText(pszText, wcslen(pszText));
}

std::wstring CDraw::stringToWide(const std::string& text)
{
	std::wstring wide(text.length(), L' ');
	std::copy(text.begin(), text.end(), wide.begin());

	return wide;
}

int CDraw::getWidht(unsigned long font, const char* input)
{
	INT iWide = 0;
	INT iTall = 0;
	INT iBufSize = MultiByteToWideChar(CP_UTF8, 0x0, input, -1, NULL, 0);

	wchar_t* pszUnicode = new wchar_t[iBufSize];

	MultiByteToWideChar(CP_UTF8, 0x0, input, -1, pszUnicode, iBufSize);

	g_pSurface->GetTextSize(font, pszUnicode, iWide, iTall);

	delete[] pszUnicode;

	return iWide;
}


void CDraw::DrawStringA(unsigned long font, bool center, int x, int y, int r, int g, int b, int a, const char *input, ...)
{
	CHAR szBuffer[MAX_PATH];

	if (!input)
		return;

	vsprintf(szBuffer, input, (char*)&input + _INTSIZEOF(input));

	if (center)
		x -= getWidht(font,szBuffer) / 2;

	g_pSurface->DrawSetTextColor(ValveSDK::Color(r,g,b,a));
	g_pSurface->DrawSetTextFont(font);
	g_pSurface->DrawSetTextPos(x, y);
	std::wstring wide = stringToWide(std::string(szBuffer));
	g_pSurface->DrawPrintText(wide.c_str(), wide.length());
}


void CDraw::FillRGBA(int x, int y, int w, int h, int r, int g, int b, int a)
{
	g_pSurface->DrawSetColor(r, g, b, a);
	g_pSurface->DrawFilledRect(x, y, x + w, y + h);
}

VOID CDraw::DrawHeader(INT x, INT y, INT w, INT r, INT g, INT b, INT HealthBarWidth)
{
	INT i = 0;
	INT xCoord = x;
	INT yCoord = y;

	// Male Male :)
	for (i = 0; i < 5; i++)
	{
		FillRGBA(x - i, y + i, w, 1, 0, 0, 0, 255);
		FillRGBA(x - i, y + i, HealthBarWidth + 2, 1, r, g, b, 255);

		FillRGBA(x - 3, y + 3, HealthBarWidth + 2, 1, (INT)(r / 1.5f),
			(INT)(g / 1.5f), (INT)(b / 1.5f), 255);

		FillRGBA(x - 4, y + 4, HealthBarWidth + 2, 1, (INT)(r / 1.5f),
			(INT)(g / 1.5f), (INT)(b / 1.5f), 255);
	}

	// Oben
	FillRGBA(x, y, w, 1, 255, 255, 255, 255);

	// Unten
	FillRGBA((x + 1) - 5, y + 5, w, 1, 255, 255, 255, 255);

	for (i = 0; i < 5; i++)
	{
		// Schräg links
		FillRGBA(x, y, 1, 1, 255, 255, 255, 255);
		x--;
		y++;
	}

	x = xCoord;
	y = yCoord;

	for (i = 0; i < 5; i++)
	{
		// Schräg rechts
		if (i != 0)
			FillRGBA(x + w, y, 1, 1, 255, 255, 255, 255);

		x--;
		y++;
	}

}


/*VOID CDraw::DrawHeader2 ( INT x, INT y, INT w, INT r, INT g, INT b, INT HealthBarWidth )
{
INT i = 0;
INT xCoord = x;
INT yCoord = y;

// Male Male :)
for ( i = 0 ; i < 5 ; i++ )
{
FillRGBA ( x - i, y + i, w, 1, 0, 0, 0, 255 );
FillRGBA ( x - i, y + i, HealthBarWidth + 2, 1, r, g, b, 255 );

FillRGBA ( x - 3, y + 3, HealthBarWidth + 2, 1, ( INT ) ( r / 1.5f ),
( INT ) ( g / 1.5f ), ( INT ) ( b / 1.5f ), 255 );

FillRGBA ( x - 4, y + 4, HealthBarWidth + 2, 1, ( INT ) ( r / 1.5f ),
( INT ) ( g / 1.5f ), ( INT ) ( b / 1.5f ), 255 );
}

// Oben
FillRGBA ( x, y, w, 1, 255, 255, 255, 255 );

// Unten
FillRGBA ( ( x + 1 ) - 5, y + 5, w, 1, 255, 255, 255, 255 );

for ( i = 0 ; i < 5 ; i++ )
{
// Schräg links
FillRGBA ( x, y, 1, 1, 255, 255, 255, 255 );
x--;
y++;
}

x = xCoord;
y = yCoord;

for ( i = 0 ; i < 5 ; i++ )
{
// Schräg rechts
if ( i != 0 )
FillRGBA ( x + w, y, 1, 1, 255, 255, 255, 255 );

x--;
y++;
}

}*/




void CDraw::boxESP(int x, int y, int radius, int R, int G, int B)
{
	int iStep = (radius * 4);

	for (int i = 0; i < radius; i++){

		//Background
		FillRGBA((x - iStep + i) - 1, (y - iStep) - 1, 3, 3, 0, 0, 0, 255);
		FillRGBA((x - iStep) - 1, (y - iStep + i) - 1, 3, 3, 0, 0, 0, 255);

		FillRGBA((x + iStep - i) - 1, (y - iStep) - 1, 3, 3, 0, 0, 0, 255);
		FillRGBA((x + iStep) - 1, (y - iStep + i) - 1, 3, 3, 0, 0, 0, 255);

		FillRGBA((x - iStep + i) - 1, (y + iStep) - 1, 3, 3, 0, 0, 0, 255);
		FillRGBA((x - iStep) - 1, (y + iStep - i) - 1, 3, 3, 0, 0, 0, 255);

		FillRGBA((x + iStep - i) - 1, (y + iStep) - 1, 3, 3, 0, 0, 0, 255);
		FillRGBA((x + iStep) - 1, (y + iStep - i) - 1, 3, 3, 0, 0, 0, 255);

	}

	for (int i = 0; i < radius; i++){

		//Simple
		FillRGBA(x - iStep + i, y - iStep, 1, 1, R, G, B, 255);
		FillRGBA(x - iStep, y - iStep + i, 1, 1, R, G, B, 255);

		FillRGBA(x + iStep - i, y - iStep, 1, 1, R, G, B, 255);
		FillRGBA(x + iStep, y - iStep + i, 1, 1, R, G, B, 255);

		FillRGBA(x - iStep + i, y + iStep, 1, 1, R, G, B, 255);
		FillRGBA(x - iStep, y + iStep - i, 1, 1, R, G, B, 255);

		FillRGBA(x + iStep - i, y + iStep, 1, 1, R, G, B, 255);
		FillRGBA(x + iStep, y + iStep - i, 1, 1, R, G, B, 255);

	}
}


VOID CDraw::drawCrosshair(int x, int y, int r, int g, int b)
{
	//Right
	FillRGBA(x, y, 15, 2, r, g, b, 255);
	//Bottom
	FillRGBA(x, y, 2, 15, r, g, b, 255);
	//Left
	FillRGBA(x - 15, y, 15, 2, r, g, b, 255);
	//Top
	FillRGBA(x, y - 15, 2, 15, r, g, b, 255);
}

VOID CDraw::DrawBox(int x, int y, int w, int h, int lw, int r, int g, int b, int a)
{
	FillRGBA(x, y, w, lw, r, g, b, a); // top
	FillRGBA(x, y + lw, lw, h - lw, r, g, b, a); // left
	FillRGBA(x + w - lw, y + lw, lw, h - lw, r, g, b, a); // right
	FillRGBA(x + lw, y + h - lw, w - lw * 2, lw, r, g, b, a); // bottom
}

VOID CDraw::DrawBoxHealth(int x, int y, int w, int h, int lw, int r, int g, int b, int a, int targetId, int damageDealt, int animTimer, int index)
{
	FillRGBA(x, y, w, lw, 0, 0, 0, 255); // top
	FillRGBA(x, y + lw, lw, h - lw, 0, 0, 0, 255); // left
	FillRGBA(x + w - lw, y + lw, lw, h - lw, 0, 0, 0, 255); // right
	FillRGBA(x + lw, y + h - lw, w - lw * 2, lw, 0, 0, 0, 255); // 
	// fill

	DWORD currentTime = GetTickCount();
	bool timerNotElapsed = ((currentTime - animTimer) + 2000) > GetTickCount();
	///*
	// raste aus lol
	//if (targetId && damageDealt && timerNotElapsed )
	//{
	//char szString[200];
	//sprintf(szString, "target: %i, damageDealt: %i", targetId, damageDealt);
	//Base::Debug::LOG(szString);
	//r = 255, g = 255; b = 255;
	//}

	/*else*/ if (a == 0)
		r = 50, g = 50; b = 50;
	//	if ( a == 0 ) r = 50, g = 50; b = 50;

	FillRGBA(x + lw, y + lw, w - (lw * 2), h - (lw * 2), r, g, b, 255);
}

void CDraw::DrawOutlinedRect(int x0, int y0, int x1, int y1, int R, int G, int B)
{
	int BoxWidth = x1 - x0;
	int BoxHeight = y1 - y0;

	if (BoxWidth < 10) BoxWidth = 10;
	if (BoxHeight < 15) BoxHeight = 15;


	g_pSurface->DrawSetColor(R, G, B, 255);

	g_pSurface->DrawFilledRect(x0, y0, x0 + (BoxWidth / 5), y0 + 1); //left top
	g_pSurface->DrawFilledRect(x0, y0, x0 + 1, y0 + (BoxHeight / 6)); //left top

	g_pSurface->DrawFilledRect(x1 - (BoxWidth / 5) + 1, y0, x1, y0 + 1); //right top
	g_pSurface->DrawFilledRect(x1, y0, x1 + 1, y0 + (BoxHeight / 6)); //right top

	g_pSurface->DrawFilledRect(x0, y1, x0 + (BoxWidth / 5), y1 + 1); //left bottom
	g_pSurface->DrawFilledRect(x0, y1 - (BoxHeight / 6) + 1, x0 + 1, y1 + 1); //left bottom

	g_pSurface->DrawFilledRect(x1 - (BoxWidth / 5) + 1, y1, x1, y1 + 1); //right bottom
	g_pSurface->DrawFilledRect(x1, y1 - (BoxHeight / 6) + 1, x1 + 1, y1 + 1); //right bottom

	int HealthWidth = ((x1 - x0) - 2);
	int HealthHeight = (y1 - y0) - 2;
	//DrawGUIBoxHorizontal( x0, y0 - (HealthHeight + 5), HealthWidth, HealthHeight, R, G, B, Health );
}






