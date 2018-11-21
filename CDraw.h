#pragma once

class CDraw
{
public:
	VOID InitFonts();
	INT getWidht(unsigned long font, const char* input);
	std::wstring stringToWide(const std::string& text);
	VOID DrawString(unsigned long font,int x, int y, int r, int g, int b, const wchar_t *pszText);
	VOID DrawStringA(unsigned long font, bool center, int x, int y, int r, int g, int b, int a, const char *input, ...);
	VOID FillRGBA(int x, int y, int w, int h, int r, int g, int b, int a);
	VOID DrawHeader(INT x, INT y, INT w, INT r, INT g, INT b, INT HealthBarWidth);
	VOID DrawHealthBox(int x, int y, int r, int g, int b, int a, int CurHealth, int MaxHealth);
	VOID boxESP(int x, int y, int radius, int R, int G, int B);
	VOID drawCrosshair(int x, int y, int r, int g, int b);
	VOID DrawBox(int x, int y, int w, int h, int lw, int r, int g, int b, int a);
	VOID DrawBoxHealth(int x, int y, int w, int h, int lw, int r, int g, int b, int a, int targetId, int damageDealt, int animTimer, int index);
	VOID DrawOutlinedRect(int x0, int y0, int x1, int y1, int R, int G, int B);
	
	unsigned long m_ESPFont, m_WatermarkFont, m_MenuFont, m_ListItemFont;
private:
};