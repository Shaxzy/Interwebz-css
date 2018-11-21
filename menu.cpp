#include "DllMain.h"

cMenu g_Menu(400,200,490,365);
cListItem ListItemArray[NUM_OF_TOTAL_LISTITEMS];

bool bSliderFix[50];

int iSliderIndex = -1;
int iCurrSlider;

//getasynckeystate fixing...
DWORD dwWait;

//menu colors (format: RGBA)
#define COLOR1 60, 60, 60, 255
#define COLOR2 85, 85, 85, 255
#define COLOR3 0, 0, 0, 255
#define COLOR4 35, 35, 35, 255
#define COLOR5 50, 50, 50, 255
#define COLOR6 100, 100, 100, 255
#define COLOR7 73, 73, 73, 255
#define COLOR8 65, 65, 65, 255
#define COLOR9 140, 0, 0, 255
#define COLOR10 80, 0, 0, 255

#define COLOR_CFG_POINTERS COLOR1
#define COLOR_CFG_POINTERS_BORDER 140, 0, 0, 255

#define TEXTCOLOR1 180, 0, 0, 255

#define LISTITEM_TEXTCOLOR 255, 255, 255, 255

#define ELEMENT_SEPERATION 20
#define CHECKBOX_SEPERATION_FROM_TEXT 100
#define CHECKBOX_SIZE 13

#define ADDER_SIZE 12
#define ADDER_SEPERATE_FROM_BOXES 44 

#define DEFAULT_X_TO_ADD (CHECKBOX_SEPERATION_FROM_TEXT + CHECKBOX_SIZE + 40)
#define SLIDER_X_TO_ADD 40

#define DROPDOWN_WIDTH 60
#define DROPDOWN_HEIGHT 15

#define BUTTON_WIDTH 60
#define BUTTON_HEIGHT 20

cMenu::cMenu(int x, int y, int w, int h)
{
	m_x = x;
	m_y = y;
	m_w = w;
	m_h = h;
	
	sCurrentConfig[DEFAULTCFG] = /*DEFAULT*/XorStr<0xB5,8,0x0CFF78ED>("\xF1\xF3\xF1\xF9\xEC\xF6\xEF"+0x0CFF78ED).s;
	sCurrentConfigName[DEFAULTCFG] = /*default.cfg*/XorStr<0xDB,12,0xBE256A50>("\xBF\xB9\xBB\xBF\xAA\x8C\x95\xCC\x80\x82\x82"+0xBE256A50).s;

	sCurrentConfig[LEGITCFG] = /*LEGIT*/XorStr<0xDE,6,0x4D77A239>("\x92\x9A\xA7\xA8\xB6"+0x4D77A239).s;
	sCurrentConfigName[LEGITCFG] = /*legit.cfg*/XorStr<0x63,10,0x00E34DD1>("\x0F\x01\x02\x0F\x13\x46\x0A\x0C\x0C"+0x00E34DD1).s;

	sCurrentConfig[RAGECFG] = /*RAGE*/XorStr<0x70,5,0xDE01A3E8>("\x22\x30\x35\x36"+0xDE01A3E8).s;
	sCurrentConfigName[RAGECFG] = /*rage.cfg*/XorStr<0xAD,9,0x254F9886>("\xDF\xCF\xC8\xD5\x9F\xD1\xD5\xD3"+0x254F9886).s;

	sCurrentConfig[HVHCFG] = /*HVH*/XorStr<0x8E,4,0x4BF1F019>("\xC6\xD9\xD8"+0x4BF1F019).s;
	sCurrentConfigName[HVHCFG] = /*hvh.cfg*/XorStr<0xE6,8,0xDD645D38>("\x8E\x91\x80\xC7\x89\x8D\x8B"+0xDD645D38).s;

	sCurrentConfig[MMCFG] = /*MM*/XorStr<0xE4,3,0x13CDC05B>("\xA9\xA8"+0x13CDC05B).s;
	sCurrentConfigName[MMCFG] = /*mm.cfg*/XorStr<0xDA,7,0xB5BDEE3A>("\xB7\xB6\xF2\xBE\xB8\xB8"+0xB5BDEE3A).s;

	sCurrentConfig[CUSTOMCFG] = /*CUSTOM*/XorStr<0xCB,7,0x1CF4708E>("\x88\x99\x9E\x9A\x80\x9D"+0x1CF4708E).s;
	sCurrentConfigName[CUSTOMCFG] = /*custom.cfg*/XorStr<0xA9,11,0x79FFFB24>("\xCA\xDF\xD8\xD8\xC2\xC3\x81\xD3\xD7\xD5"+0x79FFFB24).s;
}

char *pszAimspotNames[] = {"Pelvis","L Chest","Chest","Neck","Head"};
float fAimspotValues[] = {0,9,10,11,12};

char *pszAimbotNames[] = {"No","Always","Key"};
float fAimbotValues[] = {0,1,2};

char *pszTargetSelectionNames[] = {"Cross","Distance"};
float fTargetSelectionNames[] = {0,1};

char *pszEspBoxNames[] = {"Off","Frame","Normal"};
float fEspBoxValues[] = {0,1,2};

char *pszChamsNames[] = {"No","Visual","Full"};
float fChamsValues[] = {0,1,2};

char *pszSpecialSpotNames[] = {"Off","Pelvis","L Chest","Chest","Neck","Head"};
float fSpecialSpotValues[] = {-1,0,9,10,11,12};

char *pszPitchNames[] = {"Off","Up","Down","Zero"};
float fPitchValues[] = {0,1,2,3};

char *pszTriggNames[] = { "No","Always","Key" };
float fTriggValues[] = { 0,1,2 };

char *pszTriggSeedNames[] = { "Off","Low","Medium", "High" };
float fTriggSeedValues[] = { 0,1,2,3 };

char *pszKnifeBotNames[] = { "Off","Right","Left" };
float fKnifeBotValues[] = { 0,1,2 };

char *pszNoSpreadNames[] = { "Off","Normal","Roll" };
float fNoSpreadValues[] = { 0,1,2 };

char *pszAntiAimXNames[] = { "Off", "Emotion", "Down", "Magicpitch" };
float fAntiAimXValues[] = { 0, 1, 2, 3 };

char *pszAntiAimYNames[] = { "Off", "Backwards" };
float fAntiAimYValues[] = { 0, 1 };

void cMenu::InitMenuElements()
{
	//all menu sections
	static cMenuSection msAimbot;
	static cMenuSection msAimbotSliders;
	static cMenuSection msEsp;
	static cMenuSection msRender;
	static cMenuSection msPlayerList;
	static cMenuSection msPlayerListSliders;
	static cMenuSection msMisc;
	static cMenuSection msMiscOther;
	static cMenuSection msRemovals;
	static cMenuSection msVisualsSliders;
	static cMenuSection msColor;

	int iRowTwo = DEFAULT_X_TO_ADD;
	int iRowThree = (DEFAULT_X_TO_ADD) * 2;

	//strings here
	static std::string sAutoshootComment = /*Automatically shoot at target.*/XorStr<0xE6,31,0x5C434F2D>("\xA7\x92\x9C\x86\x87\x8A\x98\x84\x8D\x8E\x9C\x9D\x8B\xD3\x87\x9D\x99\x98\x8C\xD9\x9B\x8F\xDC\x89\x9F\x8D\x67\x64\x76\x2D"+0x5C434F2D).s,
						sMain = /*MAIN*/XorStr<0x4B,5,0xC0D3B3F2>("\x06\x0D\x04\x00"+0xC0D3B3F2).s,
						sAutoshoot = /*AIM_AUTOSHOOT*/XorStr<0x8F,14,0x2B76C6C3>("\xCE\xD9\xDC\xCD\xD2\xC1\xC1\xD9\xC4\xD0\xD6\xD5\xCF"+0x2B76C6C3).s,
						sAimspotComment = /*At what point should the aimbot aim at.*/XorStr<0xCA,40,0xC9774422>("\x8B\xBF\xEC\xBA\xA6\xAE\xA4\xF1\xA2\xBC\xBD\xBB\xA2\xF7\xAB\xB1\xB5\xAE\xB0\xB9\xFE\xAB\x88\x84\xC2\x82\x8D\x88\x84\x88\x9C\xC9\x8B\x82\x81\xCD\x8F\x9B\xDE"+0xC9774422).s,
						sAimspot = /*AIM_SPOT*/XorStr<0x00,9,0x0602055B>("\x41\x48\x4F\x5C\x57\x55\x49\x53"+0x0602055B).s,
						sSliders = /*SLIDERS*/XorStr<0xF1,8,0xA1FABC00>("\xA2\xBE\xBA\xB0\xB0\xA4\xA4"+0xA1FABC00).s,
						sAimfov = /*AIM_FOV*/XorStr<0x0F,8,0x0879EB81>("\x4E\x59\x5C\x4D\x55\x5B\x43"+0x0879EB81).s,
						sAimfovComment = /*The field of view in which the aimbot will aim.*/XorStr<0x99,48,0x60F8F031>("\xCD\xF2\xFE\xBC\xFB\xF7\xFA\xCC\xC5\x82\xCC\xC2\x85\xD0\xCE\xCD\xDE\x8A\xC2\xC2\x8D\xD9\xC7\xD9\xD2\xDA\x93\xC0\xDD\xD3\x97\xD9\xD0\xD7\xD9\xD3\xC9\x9E\xC8\xA9\xAD\xAE\xE3\xA5\xAC\xAB\xE9"+0x60F8F031).s,
						sChamsTab = /*CHAMS*/XorStr<0xEC,6,0xD91710DF>("\xAF\xA5\xAF\xA2\xA3"+0xD91710DF).s,
						sGlowTab = /*GLOW*/XorStr<0xC9,5,0xE6A07AB2>("\x8E\x86\x84\x9B"+0xE6A07AB2).s,
						sEspTab = /*ESP*/XorStr<0xEB,4,0xB7767B5C>("\xAE\xBF\xBD"+0xB7767B5C).s,
						sTerroristVisible = /*Terrorist Visible*/XorStr<0x3C,18,0x702C3C14>("\x68\x58\x4C\x4D\x2F\x33\x2B\x30\x30\x65\x10\x2E\x3B\x20\x28\x27\x29"+0x702C3C14).s,
						sTerroristInVisible = /*Terrorist Invisible*/XorStr<0xEC,20,0x3EB6C73E>("\xB8\x88\x9C\x9D\x9F\x83\x9B\x80\x80\xD5\xBF\x99\x8E\x90\x89\x92\x9E\x91\x9B"+0x3EB6C73E).s,
						sCTVisible = /*Counter-Terrorist Visible*/XorStr<0xBB,26,0x3B832B00>("\xF8\xD3\xC8\xD0\xCB\xA5\xB3\xEF\x97\xA1\xB7\xB4\xA8\xBA\xA0\xB9\xBF\xEC\x9B\xA7\xBC\xB9\xB3\xBE\xB6"+0x3B832B00).s,
						sCTInVisible = /*Counter-Terrorist Invisible*/XorStr<0x6C,28,0xB1763540>("\x2F\x02\x1B\x01\x04\x14\x00\x5E\x20\x10\x04\x05\x17\x0B\x13\x08\x08\x5D\x37\x11\xF6\xE8\xF1\xEA\xE6\xE9\xE3"+0xB1763540).s,
						sColRed = /*COLOR_RED*/XorStr<0x8A,10,0xF99A6EC0>("\xC9\xC4\xC0\xC2\xDC\xD0\xC2\xD4\xD6"+0xF99A6EC0).s,
						sColRedComment = /*The amount of red to use in this color.*/XorStr<0x57,40,0x9298B7D0>("\x03\x30\x3C\x7A\x3A\x31\x32\x2B\x31\x14\x41\x0D\x05\x44\x17\x03\x03\x48\x1D\x05\x4B\x19\x1E\x0B\x4F\x19\x1F\x52\x07\x1C\x1C\x05\x57\x1B\x16\x16\x14\x0E\x53"+0x9298B7D0).s,
						sColGreen = /*COLOR_GREEN*/XorStr<0x53,12,0xF3AA112C>("\x10\x1B\x19\x19\x05\x07\x1E\x08\x1E\x19\x13"+0xF3AA112C).s,
						sColGreenComment = /*The amount of green to use in this color.*/XorStr<0x2D,42,0x1CDEA292>("\x79\x46\x4A\x10\x50\x5F\x5C\x41\x5B\x42\x17\x57\x5F\x1A\x5C\x4E\x58\x5B\x51\x60\x35\x2D\x63\x31\x36\x23\x67\x21\x27\x6A\x3F\x24\x24\x3D\x6F\x33\x3E\x3E\x3C\x26\x7B"+0x1CDEA292).s,
						sColBlue = /*COLOR_BLUE*/XorStr<0x0B,11,0x66BA5607>("\x48\x43\x41\x41\x5D\x4F\x53\x5E\x46\x51"+0x66BA5607).s,
						sColBlueComment = /*The amount of blue to use in this color.*/XorStr<0x41,41,0xAF5C3D90>("\x15\x2A\x26\x64\x24\x2B\x28\x3D\x27\x3E\x6B\x23\x2B\x6E\x2D\x3C\x24\x37\x73\x20\x3A\x76\x22\x2B\x3C\x7A\x32\x32\x7D\x2A\x37\x09\x12\x42\x00\x0B\x09\x09\x15\x46"+0xAF5C3D90).s,
						sColAlpha = /*COLOR_ALPHA*/XorStr<0xD3,12,0x87E30CC6>("\x90\x9B\x99\x99\x85\x87\x98\x96\x8B\x94\x9C"+0x87E30CC6).s,
						sColAlphaComment = /*The amount of alpha to use in this color.*/XorStr<0xFD,42,0xFE6820C9>("\xA9\x96\x9A\x20\x60\x6F\x6C\x71\x6B\x72\x27\x67\x6F\x2A\x6A\x60\x7D\x66\x6E\x30\x65\x7D\x33\x61\x66\x73\x37\x71\x77\x3A\x6F\x74\x74\x6D\x3F\x43\x4E\x4E\x4C\x56\x0B"+0xFE6820C9).s,
						sAimbot = /*AIM_ENABLED*/XorStr<0x68,12,0x48AB1BFD>("\x29\x20\x27\x34\x29\x23\x2F\x2D\x3C\x34\x36"+0x48AB1BFD).s,
						sAimbotComment = /*Enable auto aiming. 'Always' will auto-aim and 'Key' is aim-key.*/XorStr<0x66,65,0x40B467AE>("\x23\x09\x09\x0B\x06\x0E\x4C\x0C\x1B\x1B\x1F\x51\x13\x1A\x19\x1C\x18\x10\x56\x59\x5D\x3A\x10\x0A\x1F\x06\xF3\xA6\xA2\xF4\xED\xE9\xEA\xA7\xE9\xFC\xFE\xE4\xA1\xEC\xE7\xE2\xB0\xF0\xFC\xF7\xB4\xB2\xDD\xF2\xE1\xBE\xBA\xF2\xEF\xBD\xFF\xF6\xCD\x8C\xC9\xC6\xDD\x8B"+0x40B467AE).s,
						sAutowall = /*AIM_WALL*/XorStr<0xA4,9,0xFA12B58E>("\xE5\xEC\xEB\xF8\xFF\xE8\xE6\xE7"+0xFA12B58E).s,
						sAutowallComment = /*Aimbot will aim through penetrable walls.*/XorStr<0xB3,42,0x4A428125>("\xF2\xDD\xD8\xD4\xD8\xCC\x99\xCD\xD2\xD0\xD1\x9E\xDE\xA9\xAC\xE2\xB7\xAC\xB7\xA9\xB2\xAF\xA1\xEA\xBB\xA9\xA3\xAB\xBB\xA2\xB0\xB0\xBF\xB1\xF5\xA1\xB6\xB4\xB5\xA9\xF5"+0x4A428125).s,
						sSilentaim = /*AIM_SILENT*/XorStr<0x5E,11,0xB4B59EC4>("\x1F\x16\x2D\x3E\x31\x2A\x28\x20\x28\x33"+0xB4B59EC4).s,
						sSilentaimComment = /*Aimbot will be invisible. For others to also not see it you should turn on 'Psilent'.*/XorStr<0xBF,86,0x33F10EFD>("\xFE\xA9\xAC\xA0\xAC\xB0\xE5\xB1\xAE\xA4\xA5\xEA\xA9\xA9\xED\xA7\xA1\xA6\xB8\xA1\xBA\xB6\xB9\xB3\xF9\xF8\x9F\xB5\xA9\xFC\xB2\xAA\xB7\x85\x93\x91\xC3\x90\x8A\xC6\x86\x84\x9A\x85\xCB\x82\x82\x9A\xCF\x83\x94\x97\xD3\x9D\x81\xD6\x8E\x97\x8C\xDA\x88\x94\x92\x8B\x93\x64\x21\x76\x76\x76\x6B\x26\x68\x66\x29\x2D\x5B\x7F\x64\x62\x6A\x7E\x65\x35\x3D"+0x33F10EFD).s,
						sAimteam = /*AIM_TEAM*/XorStr<0xB8,9,0xB6280200>("\xF9\xF0\xF7\xE4\xE8\xF8\xFF\xF2"+0xB6280200).s,
						sAimteamComment = /*Aim at everyone. Including teammates.*/XorStr<0x6B,38,0x8E148926>("\x2A\x05\x00\x4E\x0E\x04\x51\x17\x05\x11\x07\x0F\x18\x16\x1C\x54\x5B\x35\x13\x1D\x13\xF5\xE5\xEB\xED\xE3\xA5\xF2\xE2\xE9\xE4\xE7\xEA\xF8\xE8\xFD\xA1"+0x8E148926).s,
						sSmartaim = /*AIM_SMART*/XorStr<0x56,10,0xF1D06EF2>("\x17\x1E\x15\x06\x09\x16\x1D\x0F\x0A"+0xF1D06EF2).s,
						sSmartaimComment = /*After this many bullets aimbot will start aiming at chest.*/XorStr<0x56,59,0xD4B38C73>("\x17\x31\x2C\x3C\x28\x7B\x28\x35\x37\x2C\x40\x0C\x03\x0D\x1D\x45\x04\x12\x04\x05\x0F\x1F\x1F\x4D\x0F\x06\x1D\x13\x1D\x07\x54\x02\x1F\x1B\x14\x59\x09\x0F\x1D\x0F\x0A\x5F\xE1\xE8\xEF\xEA\xEA\xE2\xA6\xE6\xFC\xA9\xE9\xE3\xE9\xFE\xFA\xA1"+0xD4B38C73).s,
						sRCS = /*RCS_ENABLED*/XorStr<0x59,12,0x38A4C58E>("\x0B\x19\x08\x03\x18\x10\x1E\x22\x2D\x27\x27"+0x38A4C58E).s,
						sRCSComment = /*Aimbot with smooth recoil compensation.*/XorStr<0xC1,40,0x6811D4E7>("\x80\xAB\xAE\xA6\xAA\xB2\xE7\xBF\xA0\xBE\xA3\xEC\xBE\xA3\xA0\xBF\xA5\xBA\xF3\xA6\xB0\xB5\xB8\xB1\xB5\xFA\xB8\xB3\xB0\xAE\xBA\x8E\x92\x83\x97\x8D\x8A\x88\xC9"+0x6811D4E7).s,
						sSCS = /*SCS_ENABLED*/XorStr<0x7F,12,0xF2D9539E>("\x2C\xC3\xD2\xDD\xC6\xCA\xC4\xC4\xCB\xCD\xCD"+0xF2D9539E).s,
						sSCSComment = /*Along with RCS enabled this will smoothly compensate for spread.*/XorStr<0x1A,65,0x50C3B4BB>("\x5B\x77\x73\x73\x79\x3F\x57\x48\x56\x4B\x04\x77\x65\x74\x08\x4C\x44\x4A\x4E\x41\x4B\x4B\x10\x45\x5A\x5A\x47\x15\x41\x5E\x54\x55\x1A\x48\x51\x52\x51\x4B\x28\x2D\x3B\x63\x27\x2A\x2B\x37\x2D\x27\x39\x2A\x38\x28\x6E\x29\x3F\x23\x72\x20\x24\x27\x33\x36\x3C\x77"+0x50C3B4BB).s,
						sAntiSMAC = /*AIM_SMAC*/XorStr<0xFD,9,0x9970C3FF>("\xBC\xB7\xB2\x5F\x52\x4F\x42\x47"+0x9970C3FF).s,
						sAntiSMACComment = /*This will make sure you won't get banned by SMAC.*/XorStr<0x75,50,0xA228DA2C>("\x21\x1E\x1E\x0B\x59\x0D\x12\x10\x11\x5E\x12\xE1\xEA\xE7\xA3\xF7\xF0\xF4\xE2\xA8\xF0\xE5\xFE\xAC\xFA\xE1\xE1\xB7\xE5\xB2\xF4\xF1\xE1\xB6\xF5\xF9\xF7\xF4\xFE\xF8\xBD\xFC\xE6\x80\xF2\xEF\xE2\xE7\x8B"+0xA228DA2C).s,
						sAntiDM = /*AIM_DM*/XorStr<0x11,7,0x9D2A4E16>("\x50\x5B\x5E\x4B\x51\x5B"+0x9D2A4E16).s,
						sAntiDMComment = /*Aimbot will ignore players who are spawn protected.*/XorStr<0x3A,52,0x028B4E34>("\x7B\x52\x51\x5F\x51\x4B\x60\x36\x2B\x2F\x28\x65\x2F\x20\x26\x26\x38\x2E\x6C\x3D\x22\x2E\x29\x34\x20\x20\x74\x22\x3E\x38\x78\x38\x28\x3E\x7C\x2E\x2E\x3E\x17\x0F\x42\x13\x16\x0A\x12\x02\x0B\x1D\x0F\x0F\x42"+0x028B4E34).s,
						sAimOnFire = /*AIM_ONFIRE*/XorStr<0xC7,11,0x40145D8A>("\x86\x81\x84\x95\x84\x82\x8B\x87\x9D\x95"+0x40145D8A).s,
						sAimOnFireComment = /*RAGEONLY: This will help fps when raging. It will only aim when you can shoot.*/XorStr<0x38,79,0x8FEAA71F>("\x6A\x78\x7D\x7E\x73\x73\x72\x66\x7A\x61\x16\x2B\x2D\x36\x66\x30\x21\x25\x26\x6B\x24\x28\x22\x3F\x70\x37\x22\x20\x74\x22\x3E\x32\x36\x79\x28\x3A\x3B\x34\x30\x38\x4E\x41\x2B\x17\x44\x12\x0F\x0B\x04\x49\x05\x05\x00\x14\x4E\x0E\x19\x1C\x52\x04\x1C\x10\x18\x57\x01\x16\x0F\x5B\x1F\x1C\x10\x5F\xF3\xE9\xED\xEC\xF0\xAB"+0x8FEAA71F).s,
						sRCSFov = /*RCS_FOV*/XorStr<0x0A,8,0x05CE8119>("\x58\x48\x5F\x52\x48\x40\x46"+0x05CE8119).s,
						sRCSFovComment = /*The field of view in which the RCS aimbot will aim in.*/XorStr<0xB4,55,0xDC1CC725>("\xE0\xDD\xD3\x97\xDE\xD0\xDF\xD7\xD8\x9D\xD1\xD9\xE0\xB7\xAB\xA6\xB3\xE5\xAF\xA9\xE8\xBE\xA2\xA2\xAF\xA5\xEE\xBB\xB8\xB4\xF2\x81\x97\x86\xF6\xB6\xB1\xB4\xB8\xB4\xA8\xFD\xA9\xB6\x8C\x8D\xC2\x82\x8D\x88\xC6\x8E\x86\xC7"+0xDC1CC725).s,
						sAimSmooth = /*AIM_SMOOTH*/XorStr<0xE6,11,0x7A0894CE>("\xA7\xAE\xA5\xB6\xB9\xA6\xA3\xA2\xBA\xA7"+0x7A0894CE).s,
						sAimSmoothComment = /*Lower the aimbot's speed by this amount.*/XorStr<0xEF,41,0x2FA55D8E>("\xA3\x9F\x86\x97\x81\xD4\x81\x9E\x92\xD8\x98\x93\x96\x9E\x92\x8A\xD8\x73\x21\x71\x73\x61\x60\x62\x27\x6A\x70\x2A\x7F\x64\x64\x7D\x2F\x71\x7C\x7D\x66\x7A\x61\x38"+0x2FA55D8E).s,
						sRCSSMooth = /*RCS_SMOOTH*/XorStr<0x27,11,0xBA65BEAF>("\x75\x6B\x7A\x75\x78\x61\x62\x61\x7B\x78"+0xBA65BEAF).s,
						sRCSSmoothComment = /*Lower the RCS aimbot's speed by this amount.*/XorStr<0xEC,45,0x20C6E02D>("\xA0\x82\x99\x8A\x82\xD1\x86\x9B\x91\xD5\xA4\xB4\xAB\xD9\x9B\x92\x91\x9F\x91\x8B\x27\x72\x22\x70\x74\x60\x63\x63\x28\x6B\x73\x2B\x78\x65\x67\x7C\x30\x70\x7F\x7C\x61\x7B\x62\x39"+0x20C6E02D).s,
						sRCSSpot = /*RCS_SPOT*/XorStr<0x80,9,0xA1815C4C>("\xD2\xC2\xD1\xDC\xD7\xD5\xC9\xD3"+0xA1815C4C).s,
						sRCSSpotComment = /*At what point should the RCS aimbot aim at.*/XorStr<0x5D,44,0x08554C5B>("\x1C\x2A\x7F\x17\x09\x03\x17\x44\x15\x09\x0E\x06\x1D\x4A\x18\x04\x02\x1B\x03\x14\x51\x06\x1B\x11\x55\x24\x34\x2B\x59\x1B\x12\x11\x1F\x11\x0B\xA0\xE0\xEB\xEE\xA4\xE4\xF2\xA9"+0x08554C5B).s,
						sAimTime = /*AIM_TIME*/XorStr<0x6C,9,0x7B45C300>("\x2D\x24\x23\x30\x24\x38\x3F\x36"+0x7B45C300).s,
						sAimTimeComment = /*Requires 'Key' aimbot mode.*/XorStr<0x5B,28,0x7A0E7A85>("\x09\x39\x2C\x2B\x36\x12\x04\x11\x43\x43\x2E\x03\x1E\x4F\x49\x0B\x02\x01\x0F\x01\x1B\x50\x1C\x1D\x17\x11\x5B"+0x7A0E7A85).s,
						sTargetSelection = /*AIM_SELECTION*/XorStr<0x75,14,0x8B482B20>("\x34\x3F\x3A\x27\x2A\x3F\x37\x39\x3E\x2A\x36\xCF\xCF"+0x8B482B20).s,
						sTargetSelectionComment = /*Cross = closest to crosshair... Dist = nearest player.*/XorStr<0x6C,55,0x16442003>("\x2F\x1F\x01\x1C\x03\x51\x4F\x53\x17\x19\x19\x04\x1D\x0A\x0E\x5B\x08\x12\x5E\x1C\xF2\xEE\xF1\xF0\xEC\xE4\xEF\xF5\xA6\xA7\xA4\xAB\xC8\xE4\xFD\xFB\xB0\xAC\xB2\xFD\xF1\xF4\xE4\xF2\xEB\xED\xBA\xEB\xF0\xFC\xE7\xFA\xD2\x8F"+0x16442003).s,
						sEsp = /*ESP_ENABLE*/XorStr<0x42,11,0x6B94B013>("\x07\x10\x14\x1A\x03\x09\x09\x0B\x06\x0E"+0x6B94B013).s,
						sEspComment = /*The main switch for ESP.*/XorStr<0x2C,25,0x6268CED9>("\x78\x45\x4B\x0F\x5D\x50\x5B\x5D\x14\x46\x41\x5E\x4C\x5A\x52\x1B\x5A\x52\x4C\x1F\x05\x12\x12\x6D"+0x6268CED9).s,
						sBoxESP = /*ESP_BOX*/XorStr<0xA9,8,0xD6930272>("\xEC\xF9\xFB\xF3\xEF\xE1\xF7"+0xD6930272).s,
						sBoxESPComment = /*Draws a 2D bounding box around players.*/XorStr<0x2E,40,0xA7EC8CA7>("\x6A\x5D\x51\x46\x41\x13\x55\x15\x04\x73\x18\x5B\x55\x4E\x52\x59\x57\x51\x27\x61\x20\x2C\x3C\x65\x27\x35\x27\x3C\x24\x2F\x6C\x3D\x22\x2E\x29\x34\x20\x20\x7A"+0xA7EC8CA7).s,
						sHealthESP = /*ESP_HEALTH*/XorStr<0x00,11,0x554D0D2B>("\x45\x52\x52\x5C\x4C\x40\x47\x4B\x5C\x41"+0x554D0D2B).s,
						sHealthESPComment = /*Displays the player's current health.*/XorStr<0xDF,38,0x1C7F0D1F>("\x9B\x89\x92\x92\x8F\x85\x9C\x95\xC7\x9C\x81\x8F\xCB\x9C\x81\x8F\x96\x95\x83\xD5\x80\xD4\x96\x83\x85\x8A\x9C\x94\x8F\xDC\x95\x9B\x9E\x6C\x75\x6A\x2D"+0x1C7F0D1F).s,
						sNameESP = /*ESP_NAME*/XorStr<0xF4,9,0x8304F728>("\xB1\xA6\xA6\xA8\xB6\xB8\xB7\xBE"+0x8304F728).s,
						sNameESPComment = /*Displays the player's current name.*/XorStr<0x5C,36,0x2CAFE3B8>("\x18\x34\x2D\x2F\x0C\x00\x1B\x10\x44\x11\x0E\x02\x48\x19\x06\x0A\x15\x08\x1C\x48\x03\x51\x11\x06\x06\x07\x13\x19\x0C\x59\x14\x1A\x11\x18\x50"+0x2CAFE3B8).s,
						sWeaponESP = /*ESP_WEAPON*/XorStr<0xFF,11,0x79286248>("\xBA\x53\x51\x5D\x54\x41\x44\x56\x48\x46"+0x79286248).s,
						sWeaponESPComment = /*Displays the player's current active weapon.*/XorStr<0x1E,45,0x529635C8>("\x5A\x76\x53\x51\x4E\x42\x5D\x56\x06\x53\x40\x4C\x0A\x5B\x40\x4C\x57\x4A\x42\x16\x41\x13\x57\x40\x44\x45\x5D\x57\x4E\x1B\x5D\x5E\x4A\x56\x36\x24\x62\x34\x21\x24\x36\x28\x26\x67"+0x529635C8).s,
						sHelmetESP = /*ESP_HELMET*/XorStr<0xDF,11,0x06414254>("\x9A\xB3\xB1\xBD\xAB\xA1\xA9\xAB\xA2\xBC"+0x06414254).s,
						sHelmetESPComment = /*Displays whether or not the player has a helmet equipped.*/XorStr<0xE1,58,0x8806CFA6>("\xA5\x8B\x90\x94\x89\x87\x9E\x9B\xC9\x9D\x83\x89\x99\x86\x8A\x82\xD1\x9D\x81\xD4\x9B\x99\x83\xD8\x8D\x92\x9E\xDC\x8D\x92\x9E\x79\x64\x70\x23\x6C\x64\x75\x27\x69\x29\x62\x6E\x60\x60\x6B\x7B\x30\x74\x63\x66\x7D\x65\x66\x72\x7C\x37"+0x8806CFA6).s,
						sBombESP = /*ESP_BOMB*/XorStr<0xFA,9,0x53D60D9F>("\xBF\xA8\xAC\xA2\xBC\xB0\x4D\x43"+0x53D60D9F).s,
						sBombESPComment = /*Displays where bomb is (either carried or not).*/XorStr<0x0C,48,0x89C34D4F>("\x48\x64\x7D\x7F\x7C\x70\x6B\x60\x34\x62\x7E\x72\x6A\x7C\x3A\x79\x73\x70\x7C\x3F\x49\x52\x02\x0B\x41\x4C\x52\x4F\x4D\x5B\x0A\x48\x4D\x5F\x5C\x46\x55\x55\x12\x5C\x46\x15\x58\x58\x4C\x10\x14"+0x89C34D4F).s,
						sDetonation = /*ESP_DETONATION*/XorStr<0x3F,15,0x457EDA3D>("\x7A\x13\x11\x1D\x07\x01\x11\x09\x09\x09\x1D\x03\x04\x02"+0x457EDA3D).s,
						sDetonationComment = /*Displays the detonation information in top right corner.*/XorStr<0xDD,57,0x89D42ED4>("\x99\xB7\xAC\x90\x8D\x83\x9A\x97\xC5\x92\x8F\x8D\xC9\x8E\x8E\x98\x82\x80\x8E\x84\x98\x9D\x9D\xD4\x9C\x98\x91\x97\x8B\x97\x9A\x88\x94\x91\x91\x20\x68\x6C\x23\x70\x6A\x76\x27\x7A\x60\x6D\x63\x78\x2D\x6D\x60\x62\x7F\x77\x61\x3A"+0x89D42ED4).s,
						sDefuseESP = /*ESP_DEFUSE*/XorStr<0xAB,11,0xDAB08BC1>("\xEE\xFF\xFD\xF1\xEB\xF5\xF7\xE7\xE0\xF1"+0xDAB08BC1).s,
						sDefuseESPComment = /*If a player defuses the bomb you'll get a notice on screen.*/XorStr<0xF6,60,0x779A8D1C>("\xBF\x91\xD8\x98\xDA\x8B\x90\x9C\x87\x9A\x72\x21\x66\x66\x62\x70\x75\x62\x7B\x29\x7E\x63\x69\x2D\x6C\x60\x7D\x73\x32\x6A\x7B\x60\x31\x7B\x74\x39\x7D\x7E\x68\x3D\x7F\x3F\x4E\x4E\x56\x4A\x47\x40\x06\x48\x46\x09\x59\x48\x5E\x48\x4B\x41\x1E"+0x779A8D1C).s,
						sEntESP = /*ESP_ENT*/XorStr<0x1A,8,0xC8D320D8>("\x5F\x48\x4C\x42\x5B\x51\x74"+0xC8D320D8).s,
						sEntESPComment = /*Displays weapons on ground.*/XorStr<0x78,28,0x3C46493A>("\x3C\x10\x09\x0B\x10\x1C\x07\x0C\xA0\xF6\xE7\xE2\xF4\xEA\xE8\xF4\xA8\xE6\xE4\xAB\xEB\xFF\xE1\xFA\xFE\xF5\xBC"+0x3C46493A).s,
						sFlashedESP = /*ESP_FLASHED*/XorStr<0x9C,12,0x39C2B792>("\xD9\xCE\xCE\xC0\xE6\xED\xE3\xF0\xEC\xE0\xE2"+0x39C2B792).s,
						sFlashedESPComment = /*Displays whether or not a player is flashed.*/XorStr<0x8A,45,0xEFCE928F>("\xCE\xE2\xFF\xFD\xE2\xEE\xE9\xE2\xB2\xE4\xFC\xF0\xE2\xFF\xFD\xEB\xBA\xF4\xEE\xBD\xF0\xF0\xD4\x81\xC3\x83\xD4\xC9\xC7\xDE\xCD\xDB\x8A\xC2\xDF\x8D\xC8\xC3\xD1\xC2\xDA\xD6\xD0\x9B"+0xEFCE928F).s,
						sGrenadeESP = /*ESP_GRENADE*/XorStr<0x29,12,0x2789F73B>("\x6C\x79\x7B\x73\x6A\x7C\x6A\x7E\x70\x76\x76"+0x2789F73B).s,
						sGrenadeESPComment = /*Displays all thrown grenades and their types.*/XorStr<0xEF,46,0xCBB65248>("\xAB\x99\x82\x82\x9F\x95\x8C\x85\xD7\x99\x95\x96\xDB\x88\x95\x8C\x90\x77\x6F\x22\x64\x76\x60\x68\x66\x6C\x6C\x79\x2B\x6D\x63\x6A\x2F\x64\x79\x77\x7A\x66\x35\x62\x6E\x68\x7C\x69\x35"+0xCBB65248).s,
						sVisChecks = /*ESP_VISUAL*/XorStr<0x03,11,0x345FF824>("\x46\x57\x55\x59\x51\x41\x5A\x5F\x4A\x40"+0x345FF824).s,
						sVisChecksComment = /*Change color of ESP depending on visibility.*/XorStr<0x20,45,0xD2AA3660>("\x63\x49\x43\x4D\x43\x40\x06\x44\x47\x45\x45\x59\x0C\x42\x48\x0F\x75\x62\x62\x13\x50\x50\x46\x52\x56\x5D\x53\x55\x5B\x1D\x51\x51\x60\x37\x2B\x30\x2D\x27\x2F\x2B\x21\x3D\x33\x65"+0xD2AA3660).s,
						sESPEnemyOnly = /*ESP_ENEMYONLY*/XorStr<0xFA,14,0x13041940>("\xBF\xA8\xAC\xA2\xBB\xB1\x45\x4C\x5B\x4C\x4A\x49\x5F"+0x13041940).s,
						sESPEnemyOnlyComment = /*Only display ESP for enemies.*/XorStr<0x84,30,0x41074F88>("\xCB\xEB\xEA\xFE\xA8\xED\xE3\xF8\xFC\xE1\xEF\xF6\xB0\xD4\xC1\xC3\xB4\xF3\xF9\xE5\xB8\xFC\xF4\xFE\xF1\xF4\xFB\xEC\x8E"+0x41074F88).s,
						sHeadESP = /*ESP_HEAD*/XorStr<0xDA,9,0x432F5761>("\x9F\x88\x8C\x82\x96\x9A\xA1\xA5"+0x432F5761).s,
						sHeadESPComment = /*Draws the player's head hitbox.*/XorStr<0xF4,32,0xEFDE7FF3>("\xB0\x87\x97\x80\x8B\xD9\x8E\x93\x99\xDD\x8E\x93\x61\x78\x67\x71\x23\x76\x26\x6F\x6D\x68\x6E\x2B\x64\x64\x7A\x6D\x7F\x69\x3C"+0xEFDE7FF3).s,
						sEspSection = /*ESP*/XorStr<0xC0,4,0xB8C6CCF0>("\x85\x92\x92"+0xB8C6CCF0).s,
						sRender = /*RENDER*/XorStr<0xD3,7,0x92C39FA0>("\x81\x91\x9B\x92\x92\x8A"+0x92C39FA0).s,
						sChams = /*CHAMS_ENABLE*/XorStr<0x01,13,0x368645DF>("\x42\x4A\x42\x49\x56\x59\x42\x46\x48\x48\x47\x49"+0x368645DF).s,
						sChamsComment = /*Draws a colored overlay over player models.*/XorStr<0x45,44,0xA49CA6C9>("\x01\x34\x26\x3F\x3A\x6A\x2A\x6C\x2E\x21\x23\x3F\x23\x37\x37\x74\x3A\x20\x32\x2A\x35\x3B\x22\x7C\x32\x28\x3A\x12\x41\x12\x0F\x05\x1C\x03\x15\x48\x04\x05\x0F\x09\x01\x1D\x41"+0xA49CA6C9).s,
						sBrightChams = /*CHAMS_BRIGHT*/XorStr<0xCD,13,0xEB4BD568>("\x8E\x86\x8E\x9D\x82\x8D\x91\x86\x9C\x91\x9F\x8C"+0xEB4BD568).s,
						sBrightChamsComment = /*Removes the shadowing of the colored overlay.*/XorStr<0xCA,46,0x2A978F9A>("\x98\xAE\xA1\xA2\xB8\xAA\xA3\xF1\xA6\xBB\xB1\xF5\xA5\xBF\xB9\xBD\xB5\xAC\xB5\xB3\xB9\xFF\x8F\x87\xC2\x97\x8C\x80\xC6\x84\x87\x85\x85\x99\x89\x89\xCE\x80\x86\x94\x80\x9F\x95\x8C\xD8"+0x2A978F9A).s,
						sXQZ = /*XQZ_ENABLE*/XorStr<0xA9,11,0xDC4F0F46>("\xF1\xFB\xF1\xF3\xE8\xE0\xEE\xF2\xFD\xF7"+0xDC4F0F46).s,
						sXQZComment = /*Displays player models through walls.*/XorStr<0xBA,38,0xC59ADD25>("\xFE\xD2\xCF\xCD\xD2\xDE\xB9\xB2\xE2\xB3\xA8\xA4\xBF\xA2\xBA\xE9\xA7\xA4\xA8\xA8\xA2\xBC\xF0\xA5\xBA\xA1\xBB\xA0\xB1\xBF\xF8\xAE\xBB\xB7\xB0\xAE\xF0"+0xC59ADD25).s,
						sGlow = /*GLOW_ENABLE*/XorStr<0x6C,12,0x0566AFDB>("\x2B\x21\x21\x38\x2F\x34\x3C\x32\x36\x39\x33"+0x0566AFDB).s,
						sGlowComment = /*Lights up player models with different colors.*/XorStr<0x2B,47,0x08A58251>("\x67\x45\x4A\x46\x5B\x43\x11\x47\x43\x14\x45\x5A\x56\x41\x5C\x48\x1B\x51\x52\x5A\x5A\x2C\x32\x62\x34\x2D\x31\x2E\x67\x2C\x20\x2C\x2D\x29\x3F\x2B\x21\x24\x71\x31\x3C\x38\x3A\x24\x24\x76"+0x08A58251).s,
						sAsus = /*WALL_ALPHA*/XorStr<0xE7,11,0xE7E1308A>("\xB0\xA9\xA5\xA6\xB4\xAD\xA1\xBE\xA7\xB1"+0xE7E1308A).s,
						sAsusComment = /*The alpha value for walls.*/XorStr<0x4C,27,0x45D6E065>("\x18\x25\x2B\x6F\x31\x3D\x22\x3B\x35\x75\x20\x36\x34\x2C\x3F\x7B\x3A\x32\x2C\x7F\x17\x00\x0E\x0F\x17\x4B"+0x45D6E065).s,
						sRadar = /*RADAR_ENABLE*/XorStr<0xFF,13,0x8519FB86>("\xAD\x41\x45\x43\x51\x5B\x40\x48\x46\x4A\x45\x4F"+0x8519FB86).s,
						sRadarComment = /*Enables radar.*/XorStr<0xFA,15,0x62DB86CC>("\xBF\x95\x9D\x9F\x92\x9A\x73\x21\x70\x62\x60\x64\x74\x29"+0x62DB86CC).s,
						sRadarVisuals = /*RADAR_VISUALS*/XorStr<0xBA,14,0x3B9482B8>("\xE8\xFA\xF8\xFC\xEC\xE0\x96\x88\x91\x96\x85\x89\x95"+0x3B9482B8).s,
						sRadarVisualsComment = /*Enable radar border and background display.*/XorStr<0x15,44,0x0CD6A75F>("\x50\x78\x76\x7A\x75\x7F\x3B\x6E\x7C\x7A\x7E\x52\x01\x40\x4C\x56\x41\x43\x55\x08\x48\x44\x4F\x0C\x4F\x4F\x4C\x5B\x56\x40\x5C\x41\x5B\x52\x17\x5C\x50\x49\x4B\x50\x5C\x47\x11"+0x0CD6A75F).s,
						sRadarRange = /*RADAR_RANGE*/XorStr<0xAC,12,0xD89B267A>("\xFE\xEC\xEA\xEE\xE2\xEE\xE0\xF2\xFA\xF2\xF3"+0xD89B267A).s,
						sRadarRangeComment = /*The radar range.*/XorStr<0xC9,17,0x663F034E>("\x9D\xA2\xAE\xEC\xBF\xAF\xAB\xB1\xA3\xF2\xA1\xB5\xBB\xB1\xB2\xF6"+0x663F034E).s,
						sRadarWorld = /*RADAR_WORLD*/XorStr<0xB1,12,0xF1E71513>("\xE3\xF3\xF7\xF5\xE7\xE9\xE0\xF7\xEB\xF6\xFF"+0xF1E71513).s,
						sRadarWorldComment = /*Display M4. Galil. AK. AWP. FAMAS and C4 on radar.*/XorStr<0xDE,51,0xC48EA281>("\x9A\xB6\x93\x91\x8E\x82\x9D\xC5\xAB\xD3\xC6\xC9\xAD\x8A\x80\x84\x82\xC1\xD0\xB0\xB9\xDD\xD4\xB4\xA1\xA7\xD6\xD9\xBC\xBA\xB1\xBC\xAD\xDF\x61\x6F\x66\x23\x47\x31\x26\x68\x66\x29\x78\x6A\x68\x6C\x7C\x21"+0xC48EA281).s,
						sRadarHeight = /*RADAR_HEIGHT*/XorStr<0x51,13,0x02B39914>("\x03\x13\x17\x15\x07\x09\x1F\x1D\x10\x1D\x13\x08"+0x02B39914).s,
						sRadarHeightComment = /*Displays if the player is above or below you on radar.*/XorStr<0x1E,55,0x9FF59A9F>("\x5A\x76\x53\x51\x4E\x42\x5D\x56\x06\x4E\x4E\x09\x5E\x43\x49\x0D\x5E\x43\x51\x48\x57\x41\x14\x5C\x45\x17\x59\x5B\x55\x4D\x59\x1D\x51\x4D\x60\x23\x27\x2F\x2B\x32\x66\x3E\x27\x3C\x6A\x24\x22\x6D\x3C\x2E\x34\x30\x20\x7D"+0x9FF59A9F).s,
						sChamsEnemyOnly = /*CHAMS_ENEMYONLY*/XorStr<0xD6,16,0xEB1B05AD>("\x95\x9F\x99\x94\x89\x84\x99\x93\x9B\x92\xB9\xAE\xAC\xAF\xBD"+0xEB1B05AD).s,
						sChamsEnemyOnlyComment = /*Only display chams for enemies.*/XorStr<0x37,32,0xBA932EE2>("\x78\x56\x55\x43\x1B\x58\x54\x4D\x4F\x2C\x20\x3B\x63\x27\x2D\x27\x2A\x3B\x69\x2C\x24\x3E\x6D\x2B\x21\x35\x3C\x3B\x36\x27\x7B"+0xBA932EE2).s,
						sGlowEnemyOnly = /*GLOW_ENEMYONLY*/XorStr<0x63,15,0x22601824>("\x24\x28\x2A\x31\x38\x2D\x27\x2F\x26\x35\x22\x20\x23\x29"+0x22601824).s,
						sGlowEnemyOnlyComment = /*Only display glow for enemies.*/XorStr<0xFF,31,0xD64F5F48>("\xB0\x6E\x6D\x7B\x23\x60\x6C\x75\x77\x64\x68\x73\x2B\x6B\x61\x61\x78\x30\x77\x7D\x61\x34\x70\x78\x72\x75\x70\x7F\x68\x32"+0xD64F5F48).s,
						sRemovals = /*REMOVALS*/XorStr<0xAA,9,0x5AD8E834>("\xF8\xEE\xE1\xE2\xF8\xEE\xFC\xE2"+0x5AD8E834).s,
						sList = /*LIST*/XorStr<0xBA,5,0x770DDD13>("\xF6\xF2\xEF\xE9"+0x770DDD13).s,
						sPlayerList = /*PLAYER*/XorStr<0x50,7,0x2729E6E0>("\x00\x1D\x13\x0A\x11\x07"+0x2729E6E0).s,
						sPlayerListDisabled = /*PLAYERLIST IS DISABLED WHEN NOT CONNECTED*/XorStr<0xC9,42,0xCCB30071>("\x99\x86\x8A\x95\x88\x9C\x83\x99\x82\x86\xF3\x9D\x86\xF6\x93\x91\x8A\x9B\x99\x90\x98\x9A\xFF\xB7\xA9\xA7\xAD\xC4\xAB\xA9\xB3\xC8\xAA\xA5\xA5\xA2\xA8\xAD\xBB\xB5\xB5"+0xCCB30071).s,
						sSomething = /*: */XorStr<0x36,3,0xED21FEF0>("\x0C\x17"+0xED21FEF0).s,
						sBest = sPlayerList + sSomething,
						sWhitelist = /*LIST_WHITELIST*/XorStr<0xFC,15,0xD4A1FDC5>("\xB0\xB4\xAD\xAB\x5F\x56\x4A\x4A\x50\x40\x4A\x4E\x5B\x5D"+0xD4A1FDC5).s,
						sWhitelistComment = /*This player will be ignored by aimbot, knifebot and triggerbot.*/XorStr<0x9E,64,0x2EA85D8A>("\xCA\xF7\xC9\xD2\x82\xD3\xC8\xC4\xDF\xC2\xDA\x89\xDD\xC2\xC0\xC1\x8E\xCD\xD5\x91\xDB\xD4\xDA\xDA\xC4\xD2\xDC\x99\xD8\xC2\x9C\xDC\xD7\xD2\xA2\xAE\xB6\xED\xE4\xAE\xA8\xAE\xAE\xAC\xA8\xA4\xB8\xED\xAF\xA1\xB4\xF1\xA6\xA1\xBD\xB2\xB1\xB2\xAA\xBB\xB5\xAF\xF2"+0x2EA85D8A).s,
						sSpecialSpot = /*LIST_AIMSPOT*/XorStr<0xC8,13,0xCCFEEAF5>("\x84\x80\x99\x9F\x93\x8C\x87\x82\x83\x81\x9D\x87"+0xCCFEEAF5).s,
						sSpecialSpotComment = /*The aimbot will use this aimspot for only this player.*/XorStr<0xE0,55,0xA828DFFB>("\xB4\x89\x87\xC3\x85\x8C\x8B\x85\x87\x9D\xCA\x9C\x85\x81\x82\xCF\x85\x82\x97\xD3\x80\x9D\x9F\x84\xD8\x98\x93\x96\x8F\x8D\x91\x8B\x20\x67\x6D\x71\x24\x6A\x68\x6B\x71\x29\x7E\x63\x65\x7E\x2E\x7F\x7C\x70\x6B\x76\x66\x3B"+0xA828DFFB).s,
						sPitchMod = /*LIST_PITCH*/XorStr<0x6D,11,0x8183AD9C>("\x21\x27\x3C\x24\x2E\x22\x3A\x20\x36\x3E"+0x8183AD9C).s,
						sPitchModComment = /*ADVANCED: Modify someones 'pitch'.*/XorStr<0xE7,35,0x3D3F67D4>("\xA6\xAC\xBF\xAB\xA5\xAF\xA8\xAA\xD5\xD0\xBC\x9D\x97\x9D\x93\x8F\xD7\x8B\x96\x97\x9E\x93\x93\x9B\x8C\x20\x26\x72\x6A\x70\x66\x6E\x20\x26"+0x3D3F67D4).s,
						sEnableAngle = /*LIST_ANGLE*/XorStr<0x6A,11,0x3DE0DC5A>("\x26\x22\x3F\x39\x31\x2E\x3E\x36\x3E\x36"+0x3DE0DC5A).s,
						sEnableAngleComment = /*Enables pitch and yaw modifications for this player.*/XorStr<0xE7,53,0x858B691B>("\xA2\x86\x88\x88\x87\x89\x9E\xCE\x9F\x99\x85\x91\x9B\xD4\x94\x98\x93\xD8\x80\x9B\x8C\xDC\x90\x91\x9B\x69\x67\x6B\x60\x65\x71\x6F\x68\x66\x7A\x2A\x6D\x63\x7F\x2E\x7B\x78\x78\x61\x33\x64\x79\x77\x6E\x7D\x6B\x34"+0x858B691B).s,
						sYawMod = /*LIST_YAW*/XorStr<0xF2,9,0xA2061FE4>("\xBE\xBA\xA7\xA1\xA9\xAE\xB9\xAE"+0xA2061FE4).s,
						sYawModComment = /*The amount of 'yaw' to add to the angle.*/XorStr<0x5F,41,0x9E354C69>("\x0B\x08\x04\x42\x02\x09\x0A\x13\x09\x1C\x49\x05\x0D\x4C\x4A\x17\x0E\x07\x56\x52\x07\x1B\x55\x17\x13\x1C\x59\x0E\x14\x5C\x09\x16\x1A\xA0\xE0\xEC\xE4\xE8\xE0\xA8"+0x9E354C69).s,
						sResetAll = /*RESET_ALL*/XorStr<0x1A,10,0x679B4341>("\x48\x5E\x4F\x58\x4A\x40\x61\x6D\x6E"+0x679B4341).s,
						sMod = /*MOD*/XorStr<0x74,4,0xDA56E1D3>("\x39\x3A\x32"+0xDA56E1D3).s,
						sNoPlayers = /*NO PLAYERS FOUND*/XorStr<0x74,17,0x8E710758>("\x3A\x3A\x56\x27\x34\x38\x23\x3E\x2E\x2E\x5E\x39\xCF\xD4\xCC\xC7"+0x8E710758).s,

						sTrigg = "TRIGGERBOT",
						sTriggEnable = "TRIGGER_ON",
						sTriggEnableSS = "",
						sTriggGroup = "TRIGGER_GROUP",
						sTriggSeed = "TRIGGER_SEED",
						sTriggBurst = "TRIGGER_BURST",
						sTriggAutowall = "TRIGGER_WALL",
						sTriggAll = "TRIGGER_ALL",
						sTriggDelay = "TRIGGER_DELAY",

						sNoSpread = "NO_SPREAD",
						sNoRecoil = "NO_RECOIL";
	//the usual x position for sections/itemlists
	int iUsualX = m_x + 10;
	int iListY = m_y + 45;

	//playerlist stuff
	static bool bDoOnce = true;
	static int iPlayerIndex;
	static float fResetHandle;
	std::string sCurrPlayer;

	//blabla
	iCurrSlider = 0;
	 
	switch(GetTabIndex())
	{
	case AIMBOTTAB:

		msAimbot.ClearSection();
		msAimbotSliders.ClearSection();

		//----------------------------
		msAimbot.Draw(iUsualX,m_y + 30,m_w - 22,120,sMain.data());

		msAimbot.SetValueNames(pszAimbotNames,fAimbotValues,3);
		msAimbot.AddElement(DROPDOWN,0,sAimbot.data(),sAimbotComment.data(),&g_CVARS.CvarList[EnableAim]);
		
		//msAimbot.AddElement(ONOFF,0,sAutoshoot.data(),sAutoshootComment.data(),&g_CVARS.CvarList[Autoshoot]);
		 
		msAimbot.SetValueNames(pszAimspotNames,fAimspotValues,5);
		msAimbot.AddElement(DROPDOWN,0,sAimspot.data(),sAimspotComment.data(),&g_CVARS.CvarList[AimSpot]);
		
		msAimbot.SetValueNames(pszTargetSelectionNames,fTargetSelectionNames,2);
		msAimbot.AddElement(DROPDOWN,0,sTargetSelection.data(),sTargetSelectionComment.data(),&g_CVARS.CvarList[TargetSelection]);

		msAimbot.AddElement(ONOFF,0,sAutoshoot.data(),sAutoshootComment.data(),&g_CVARS.CvarList[Autoshoot]);
		msAimbot.AddElement(ONOFF,0,sAutowall.data(),sAutowallComment.data(),&g_CVARS.CvarList[Autowall]);
	
		msAimbot.RestartSection();

		msAimbot.AddElement(ONOFF,iRowTwo,sSilentaim.data(),sSilentaimComment.data(),&g_CVARS.CvarList[SilentAim]);	
		msAimbot.AddElement(ONOFF,iRowTwo,sAimteam.data(),sAimteamComment.data(),&g_CVARS.CvarList[AimTeam]);
		msAimbot.AddElement(ONOFF,iRowTwo,sAntiDM.data(),sAntiDMComment.data(),&g_CVARS.CvarList[AntiDM]);
		msAimbot.AddElement(ONOFF,iRowTwo,sAntiSMAC.data(),sAntiSMACComment.data(),&g_CVARS.CvarList[SMACBot]);
		msAimbot.AddElement(ONOFF,iRowTwo,sAimOnFire.data(),sAimOnFireComment.data(),&g_CVARS.CvarList[AimOnFire]);
		
		msAimbot.RestartSection();

		msAimbot.SetValueNames(pszAimspotNames,fAimspotValues,5);
		msAimbot.AddElement(ONOFF,iRowThree,sRCS.data(),sRCSComment.data(),&g_CVARS.CvarList[RCS]);
		msAimbot.AddElement(ONOFF,iRowThree,sSCS.data(),sSCSComment.data(),&g_CVARS.CvarList[SCS]);
		msAimbot.AddElement(DROPDOWN,iRowThree,sRCSSpot.data(),sRCSSpotComment.data(),&g_CVARS.CvarList[RCSSpot]);
		msAimbot.AddElement(ADDER,iRowThree,sSmartaim.data(),sSmartaimComment.data(),&g_CVARS.CvarList[SmartAim],0,15);
		//----------------------------


		//----------------------------
		msAimbotSliders.Draw(iUsualX,m_y + 155,m_w - 22,200,sSliders.data());

		msAimbotSliders.SetSlider(iCurrSlider,true);
		msAimbotSliders.AddElement(SLIDER,SLIDER_X_TO_ADD,sAimfov.data(),sAimfovComment.data(),&g_CVARS.CvarList[AimFOV],0,360);
		msAimbotSliders.SetSlider(iCurrSlider,true);
		msAimbotSliders.AddElement(SLIDER,SLIDER_X_TO_ADD,sAimSmooth.data(),sAimSmoothComment.data(),&g_CVARS.CvarList[SmoothAim],0,150);
		msAimbotSliders.SetSlider(iCurrSlider,true);
		msAimbotSliders.AddElement(SLIDER,SLIDER_X_TO_ADD,sAimTime.data(),sAimTimeComment.data(),&g_CVARS.CvarList[AimTime],0,2000);
		msAimbotSliders.SetSlider(iCurrSlider,true);
		msAimbotSliders.AddElement(SLIDER,SLIDER_X_TO_ADD,sRCSFov.data(),sRCSFovComment.data(),&g_CVARS.CvarList[RCSFOV],0,360);
		msAimbotSliders.SetSlider(iCurrSlider,true);
		msAimbotSliders.AddElement(SLIDER,SLIDER_X_TO_ADD,sRCSSMooth.data(),sRCSSmoothComment.data(),&g_CVARS.CvarList[RCSSmooth],0,150);
		//----------------------------

		//NOTE: you have to call this at the end if youve added a dropdown to this section
		msAimbot.PostSection();
		break;

	case ESPTAB:

		msEsp.ClearSection();
		msRender.ClearSection();
		msVisualsSliders.ClearSection();

		//----------------------------
		msEsp.Draw(iUsualX,m_y + 30,m_w - 22,120,sEspSection.data());

		msEsp.AddElement(ONOFF,0,sEsp.data(),sEspComment.data(),&g_CVARS.CvarList[EnableESP]);

		msEsp.AddElement(ONOFF,0,sNameESP.data(),sNameESPComment.data(),&g_CVARS.CvarList[NameESP]);
		msEsp.AddElement(ONOFF,0,sHealthESP.data(),sHealthESPComment.data(),&g_CVARS.CvarList[HealthESP]);
		msEsp.AddElement(ONOFF,0,sWeaponESP.data(),sWeaponESPComment.data(),&g_CVARS.CvarList[WeaponESP]);
		
		msEsp.SetValueNames(pszEspBoxNames,fEspBoxValues,3);
		msEsp.AddElement(DROPDOWN,0,sBoxESP.data(),sBoxESPComment.data(),&g_CVARS.CvarList[BoxESP]);

		msEsp.RestartSection();
		
		msEsp.AddElement(ONOFF,iRowTwo,sHelmetESP.data(),sHelmetESPComment.data(),&g_CVARS.CvarList[HelmetESP]);
		msEsp.AddElement(ONOFF,iRowTwo,sFlashedESP.data(),sFlashedESPComment.data(),&g_CVARS.CvarList[FlashedESP]);
		msEsp.AddElement(ONOFF,iRowTwo,sDefuseESP.data(),sDefuseESPComment.data(),&g_CVARS.CvarList[DefuseESP]);
		msEsp.AddElement(ONOFF,iRowTwo,sHeadESP.data(),sHeadESPComment.data(),&g_CVARS.CvarList[HeadESP]);
		msEsp.AddElement(ONOFF,iRowTwo,sBombESP.data(),sBombESPComment.data(),&g_CVARS.CvarList[BombESP]);

		msEsp.RestartSection();

		msEsp.AddElement(ONOFF,iRowThree,sEntESP.data(),sEntESPComment.data(),&g_CVARS.CvarList[EntESP]);
		msEsp.AddElement(ONOFF,iRowThree,sGrenadeESP.data(),sGrenadeESPComment.data(),&g_CVARS.CvarList[GrenadeESP]);
		msEsp.AddElement(ONOFF,iRowThree,sDetonation.data(),sDetonationComment.data(),&g_CVARS.CvarList[BombDetonation]);
		msEsp.AddElement(ONOFF,iRowThree,sVisChecks.data(),sVisChecksComment.data(),&g_CVARS.CvarList[VisChecks]);
		msEsp.AddElement(ONOFF,iRowThree,sESPEnemyOnly.data(),sESPEnemyOnlyComment.data(),&g_CVARS.CvarList[ESPEnemyOnly]);
		//----------------------------


		//----------------------------
		msRender.Draw(iUsualX,m_y + 155,m_w - 22,100,sRender.data());

		msRender.SetValueNames(pszChamsNames,fChamsValues,3);
		msRender.AddElement(DROPDOWN,0,sChams.data(),sChamsComment.data(),&g_CVARS.CvarList[Chams]);

		msRender.AddElement(ONOFF,0,sBrightChams.data(),sBrightChamsComment.data(),&g_CVARS.CvarList[BrightChams]);

		msRender.AddElement(ONOFF,0,sChamsEnemyOnly.data(),sChamsEnemyOnlyComment.data(),&g_CVARS.CvarList[ChamsEnemyOnly]);

		msRender.AddElement(ONOFF,0,sXQZ.data(),sXQZComment.data(),&g_CVARS.CvarList[XQZ]);

		msRender.RestartSection();

		msRender.AddElement(ONOFF,iRowTwo,sRadar.data(),sRadarComment.data(),&g_CVARS.CvarList[Radar]);
		msRender.AddElement(ONOFF,iRowTwo,sRadarVisuals.data(),sRadarVisualsComment.data(),&g_CVARS.CvarList[RadarVisuals]);
		msRender.AddElement(ONOFF,iRowTwo,sRadarWorld.data(),sRadarWorldComment.data(),&g_CVARS.CvarList[RadarWorld]);
		msRender.AddElement(ONOFF,iRowTwo,sRadarHeight.data(),sRadarHeightComment.data(),&g_CVARS.CvarList[RadarHeight]);

		msRender.RestartSection();

		msRender.AddElement(ONOFF,iRowThree,sGlow.data(),sGlowComment.data(),&g_CVARS.CvarList[Glow]);
		msRender.AddElement(ONOFF,iRowThree,sGlowEnemyOnly.data(),sGlowEnemyOnlyComment.data(),&g_CVARS.CvarList[GlowEnemyOnly]);

		//----------------------------

		//----------------------------
		msVisualsSliders.Draw(iUsualX,m_y + 260,m_w - 22,95,sSliders.data());

		msVisualsSliders.SetSlider(iCurrSlider,true);
		msVisualsSliders.AddElement(SLIDER,SLIDER_X_TO_ADD,sRadarRange.data(),sRadarRangeComment.data(),&g_CVARS.CvarList[RadarRange],0,100);
		
		msVisualsSliders.SetSlider(iCurrSlider,true);
		msVisualsSliders.AddElement(SLIDER,SLIDER_X_TO_ADD,sAsus.data(),sAsusComment.data(),&g_CVARS.CvarList[AsusAmount],0,255);
		//----------------------------

		msRender.PostSection();
		msEsp.PostSection();
		
		break;
		 
	case MISCTAB:

		//msMisc.ClearSection();
		msRemovals.ClearSection();
		msMiscOther.ClearSection();

		//msMisc.Draw(iUsualX, m_y + 30, m_w - 22, 80, sTrigg.data());

		/* // íå ðàáîòàåò
		msMisc.SetValueNames(pszTriggNames, fTriggValues, 3);
		msMisc.AddElement(DROPDOWN, 0, sTriggEnable.data(), sTriggEnableSS.data(), &g_CVARS.CvarList[Triggerbot]);

		msMisc.SetValueNames(pszTriggSeedNames, fTriggSeedValues, 4);
		msMisc.AddElement(DROPDOWN, 0, sTriggSeed.data(), "", &g_CVARS.CvarList[TriggerSeed]);

		msMisc.AddElement(ONOFF, 0, "TRIGGER_PERFECT", "", &g_CVARS.CvarList[PTrigger]);

		msMisc.RestartSection();

		msMisc.AddElement(ONOFF, iRowTwo, sTriggBurst.data(), "", &g_CVARS.CvarList[TriggerBurst]);
		msMisc.AddElement(ONOFF, iRowTwo, sTriggAutowall.data(), "", &g_CVARS.CvarList[TriggerWall]);

		msMisc.AddElement(ONOFF, iRowTwo, sTriggAll.data(), "", &g_CVARS.CvarList[TriggerAll]);*/

		msRemovals.Draw(iUsualX, m_y + 30, m_w - 22, 100, "REMOVALS");

		msRemovals.SetValueNames(pszNoSpreadNames, fNoSpreadValues, 3);
		msRemovals.AddElement(DROPDOWN, 0, sNoSpread.data(), "", &g_CVARS.CvarList[NoSpread]);
		msRemovals.AddElement(ONOFF, 0, sNoRecoil.data(), "", &g_CVARS.CvarList[NoRecoil]);
		msRemovals.AddElement(ONOFF, 0, "NO_VIS_RECOIL", "", &g_CVARS.CvarList[NoVisRecoil]);
		msRemovals.AddElement(ONOFF, 0, "NO_HANDS", "", &g_CVARS.CvarList[NoHands]);

		/* // íå ðàáîòàåò
		msRemovals.RestartSection();

		msRemovals.AddElement(ONOFF, iRowTwo, "NO_SMOKE", "", &g_CVARS.CvarList[NoSmoke]);
		msRemovals.AddElement(ONOFF, iRowTwo, "NO_FLASH", "", &g_CVARS.CvarList[NoFlash]);*/


		msMiscOther.Draw(iUsualX, m_y + 135, m_w - 22, 135, "OTHER");

		msMiscOther.AddElement(ONOFF, 0, "BUNNY_HOP", "", &g_CVARS.CvarList[Bunnyhop]);
		msMiscOther.AddElement(ONOFF, 0, "STRAFE_BOT", "", &g_CVARS.CvarList[Strafebot]);
		msMiscOther.AddElement(ONOFF, 0, "AUTO_PISTOL", "", &g_CVARS.CvarList[Autopistol]);

		msMiscOther.RestartSection();

		// íå ðàáîòàåò
		/*msMiscOther.SetValueNames(pszKnifeBotNames, fKnifeBotValues, 3);
		msMiscOther.AddElement(DROPDOWN, iRowTwo, "KNIFEBOT", "", &g_CVARS.CvarList[Knifebot]);*/

		msMiscOther.AddElement(ONOFF, iRowTwo, "PSILENT_ON", "", &g_CVARS.CvarList[PSilent]);

		msMiscOther.RestartSection();

		msMiscOther.SetValueNames(pszAntiAimXNames, fAntiAimXValues, 4);
		msMiscOther.AddElement(DROPDOWN, iRowThree, "ANTI_AIM_X", "", &g_CVARS.CvarList[AntiAimX]);

		msMiscOther.SetValueNames(pszAntiAimYNames, fAntiAimYValues, 2);
		msMiscOther.AddElement(DROPDOWN, iRowThree, "ANTI_AIM_Y", "", &g_CVARS.CvarList[AntiAimY]);

		//msMisc.PostSection();
		msRemovals.PostSection();
		msMiscOther.PostSection();
		break;

	case SPECIALTAB:
		msPlayerList.ClearSection();
		msPlayerListSliders.ClearSection();

		//----------------------------
		DrawSmallTab(SMALL_PLAYERLISTTAB,iSmallTab[1],iUsualX, iListY,sList.data());
		DrawSmallTab(SMALL_MODIFIEDTAB,iSmallTab[1],iUsualX + 60, iListY,sMod.data());

		if(bDoOnce)
		{
			iSmallTab[1] = SMALL_PLAYERLISTTAB;
		
			bDoOnce = false;
		}

		//reset everything inside this list
		ListItemArray[iSmallTab[1]].ClearList();
		//this is the only list on this tab
		ListItemArray[iSmallTab[1]].OnlyList();

		if(iSmallTab[1] == SMALL_PLAYERLISTTAB)
		{
			if(g_Valve.pEngine->IsInGame())
			{
				for(int player = 1; player <= g_Valve.pEngine->GetMaxClients(); player++)
				{
					ValveSDK::CEngineClient::player_info_t info;

					if(player == g_Valve.pEngine->GetLocalPlayer()
						|| !g_Valve.pEngine->GetPlayerInfo(player,&info))
						continue;

					ListItemArray[iSmallTab[1]].AddItem(info.name,player);
				}
			}
			else
			{
				fResetHandle = 1;

				ListItemArray[iSmallTab[1]].AddItem(sPlayerListDisabled.data(),65);
			}
		}
		else if(iSmallTab[1] == SMALL_MODIFIEDTAB)
		{
			if(g_Valve.pEngine->IsInGame())
			{
				for(int player = 1; player <= g_Valve.pEngine->GetMaxClients(); player++)
				{
					ValveSDK::CEngineClient::player_info_t info;

					if(player == g_Valve.pEngine->GetLocalPlayer()
						|| !g_Valve.pEngine->GetPlayerInfo(player,&info))
						continue;

					if(g_Aimbot.fWhiteList[player] || g_Aimbot.fIsSelected[player]
					|| g_Aimbot.fPitchMod[player] || g_Aimbot.fYawMod[player] 
					|| g_Aimbot.fSpecialAimspot[player] != -1)
						ListItemArray[iSmallTab[1]].AddItem(info.name,player);
					else
						continue;
				}
			}
			else
			{
				fResetHandle = 1;

				ListItemArray[iSmallTab[1]].AddItem(sPlayerListDisabled.data(),65);
			}
		}

		if(ListItemArray[iSmallTab[1]].GetNumberOfItems() == 0)
			ListItemArray[iSmallTab[1]].AddItem(sNoPlayers.data(),65);

		ListItemArray[iSmallTab[1]].Draw(iUsualX,iListY,m_w - 30,120);
		//----------------------------


		//----------------------------
		//selected player

		if(ListItemArray[iSmallTab[1]].GetNumberOfItems() > 0)
		{
			iPlayerIndex = ListItemArray[iSmallTab[1]].GetSelectedItem().iSpecialValue;
			
			ValveSDK::CEngineClient::player_info_t info;

			if(g_Valve.pEngine->GetPlayerInfo(iPlayerIndex,&info))
				sCurrPlayer = info.name;
		}

		msPlayerList.Draw(iUsualX,iListY + 125,m_w - 22,70,((std::string)(sBest + sCurrPlayer)).data());

		msPlayerList.AddElement(ONOFF,0,sWhitelist.data(),sWhitelistComment.data(),&g_Aimbot.fWhiteList[iPlayerIndex]);
		msPlayerList.AddElement(ONOFF,0,sEnableAngle.data(),sEnableAngleComment.data(),&g_Aimbot.fIsSelected[iPlayerIndex]);
		
		msPlayerList.RestartSection();

		msPlayerList.SetISection();

		msPlayerList.AddElement(BUTTON,iRowTwo + 20,sResetAll.data(),NULL,&fResetHandle);

		if(fResetHandle)
			g_Aimbot.ResetPlayerList(fResetHandle);

		msPlayerList.RestartSection();

		msPlayerList.SetValueNames(pszSpecialSpotNames,fSpecialSpotValues,6);
		msPlayerList.AddElement(DROPDOWN,iRowThree,sSpecialSpot.data(),sSpecialSpotComment.data(),&g_Aimbot.fSpecialAimspot[iPlayerIndex]);
		
		msPlayerList.SetValueNames(pszPitchNames,fPitchValues,4);
		msPlayerList.AddElement(DROPDOWN,iRowThree,sPitchMod.data(),sPitchModComment.data(),&g_Aimbot.fPitchMod[iPlayerIndex]);
		//----------------------------

		//----------------------------
		msPlayerListSliders.Draw(iUsualX,iListY + 200,m_w - 22,110,sSliders.data());

		msPlayerListSliders.SetISection();

		msPlayerListSliders.SetSlider(iCurrSlider,true);
		msPlayerListSliders.AddElement(SLIDER,SLIDER_X_TO_ADD,sYawMod.data(),sYawModComment.data(),&g_Aimbot.fYawMod[iPlayerIndex],0,360);
		//----------------------------

		//ADD FORCEFULLUPDATE HANDLING !!!!

		msPlayerList.PostSection();
		break;

	case COLORTAB:

		//----------------------------

		DrawSmallTab(SMALL_CHAMSTAB,iSmallTab[0],iUsualX, iListY,sChamsTab.data());
		DrawSmallTab(SMALL_GLOWTAB,iSmallTab[0],iUsualX + 60, iListY,sGlowTab.data());
		DrawSmallTab(SMALL_ESPTAB,iSmallTab[0],iUsualX + 120, iListY,sEspTab.data());

		//reset everything inside this list
		ListItemArray[iSmallTab[0]].ClearList();
		//this is the only list on this tab
		ListItemArray[iSmallTab[0]].OnlyList();

		bool bChams = (iSmallTab[0] == SMALL_CHAMSTAB || iSmallTab[0] == SMALL_GLOWTAB);

		if(iSmallTab[0] == SMALL_CHAMSTAB)
		{
			ListItemArray[iSmallTab[0]].AddItem(sTerroristVisible.data(),Chams_T_Vis);
			ListItemArray[iSmallTab[0]].AddItem(sTerroristInVisible.data(),Chams_T_InVis);
			ListItemArray[iSmallTab[0]].AddItem(sCTVisible.data(),Chams_CT_Vis);
			ListItemArray[iSmallTab[0]].AddItem(sCTInVisible.data(),Chams_CT_InVis);
		}
		else if(iSmallTab[0] == SMALL_GLOWTAB)
		{
			ListItemArray[iSmallTab[0]].AddItem(sTerroristVisible.data(),Glow_T_Vis);
			ListItemArray[iSmallTab[0]].AddItem(sTerroristInVisible.data(),Glow_T_InVis);
			ListItemArray[iSmallTab[0]].AddItem(sCTVisible.data(),Glow_CT_Vis);
			ListItemArray[iSmallTab[0]].AddItem(sCTInVisible.data(),Glow_CT_InVis);
		}
		else if(iSmallTab[0] == SMALL_ESPTAB)
		{
			ListItemArray[iSmallTab[0]].AddItem(sTerroristVisible.data(),Esp_T_Vis);
			ListItemArray[iSmallTab[0]].AddItem(sTerroristInVisible.data(),Esp_T_InVis);
			ListItemArray[iSmallTab[0]].AddItem(sCTVisible.data(),Esp_CT_Vis);
			ListItemArray[iSmallTab[0]].AddItem(sCTInVisible.data(),Esp_CT_InVis);
		}

		ListItemArray[iSmallTab[0]].Draw(iUsualX,iListY,m_w - 30,120);
		//----------------------------

		//----------------------------
		int iColorIndex = ListItemArray[iSmallTab[0]].GetSelectedItem().iSpecialValue;

		msColor.ClearSection();

		msColor.Draw(iUsualX,m_y + 170,m_w - 22,185,sSliders.data());

		msColor.SetSlider(iCurrSlider,true,true,-1,true,g_CVARS.CvarList[iColorIndex],g_CVARS.CvarList[iColorIndex + 1],g_CVARS.CvarList[iColorIndex + 2],(bChams ? g_CVARS.CvarList[ModelAlpha] : g_CVARS.CvarList[iColorIndex + 3]),bChams);
		msColor.AddElement(SLIDER,SLIDER_X_TO_ADD,sColRed.data(),sColRedComment.data(),&msColor.fColDummy[0],0,255);

		msColor.SetSlider(iCurrSlider,true,true,-1,false,g_CVARS.CvarList[iColorIndex],g_CVARS.CvarList[iColorIndex + 1],g_CVARS.CvarList[iColorIndex + 2],(bChams ? g_CVARS.CvarList[ModelAlpha] : g_CVARS.CvarList[iColorIndex + 3]),bChams);
		msColor.AddElement(SLIDER,SLIDER_X_TO_ADD,sColGreen.data(),sColGreenComment.data(),&msColor.fColDummy[1],0,255);
		
		msColor.SetSlider(iCurrSlider,true,true,-1,false,g_CVARS.CvarList[iColorIndex],g_CVARS.CvarList[iColorIndex + 1],g_CVARS.CvarList[iColorIndex + 2],(bChams ? g_CVARS.CvarList[ModelAlpha] : g_CVARS.CvarList[iColorIndex + 3]),bChams);
		msColor.AddElement(SLIDER,SLIDER_X_TO_ADD,sColBlue.data(),sColBlueComment.data(),&msColor.fColDummy[2],0,255);
		
		msColor.SetSlider(iCurrSlider,true,true,iColorIndex,false,g_CVARS.CvarList[iColorIndex],g_CVARS.CvarList[iColorIndex + 1],g_CVARS.CvarList[iColorIndex + 2],(bChams ? g_CVARS.CvarList[ModelAlpha] : g_CVARS.CvarList[iColorIndex + 3]),bChams);
		msColor.AddElement(SLIDER,SLIDER_X_TO_ADD,sColAlpha.data(),sColAlphaComment.data(),&msColor.fColDummy[3],0,255);
		//----------------------------

		break;
	}
}

void cMenu::DrawHeaderComment(int e_x, int e_y, const char *pszComment, bool bSlider)
{
	if(g_Mouse.IsOver(e_x,e_y,(bSlider ? 360 : (DEFAULT_X_TO_ADD - 15)),(bSlider ? 18 : 15)))
		g_Draw.DrawStringA(g_Draw.m_ListItemFont,false,m_x + 15,m_y + 7,TEXTCOLOR1,pszComment);
}

int iLoop[] = {15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0};

void cMenu::DrawSmallTab(int index, int &setindex, int x, int y, const char *pszTitle)
{
	static int iWidth = 55;
	static int iHeight = 15;

	bool bOverTri = false;

	bool bOverBack = g_Mouse.IsOver(x + iHeight, y - iHeight,iWidth + 1 - iHeight,iHeight + 1);

	bool bSame = setindex == index;

	for(int ax = 0; ax <= iHeight; ax++)
	{
		if(g_Mouse.IsOver(x + ax,y - ax,2,ax) || g_Mouse.IsOver(x + iWidth + ax,y - iHeight,2,iLoop[ax]))
		{
			bOverTri = true;
			break;
		}
	}

	for (int i = 0; i <= iHeight; i++)
	{
		if(bSame)
		{
			g_Valve.pSurface->DrawSetColor(COLOR7);

			g_Valve.pSurface->DrawLine(x + i, y, x + iHeight + i, y - iHeight);
			g_Valve.pSurface->DrawLine(x + iWidth + iHeight - i, y - iHeight, x + iWidth - i, y);

			if(i == iHeight)
				g_Draw.FillRGBA(x + iHeight,y - iHeight,iWidth - iHeight,iHeight + 1,COLOR7);
		}
		else
		{
			if(bOverBack || bOverTri)
				g_Valve.pSurface->DrawSetColor(COLOR8);
			else
				g_Valve.pSurface->DrawSetColor(COLOR1);

			g_Valve.pSurface->DrawLine(x + i, y, x + iHeight + i, y - iHeight);
			g_Valve.pSurface->DrawLine(x + iWidth + iHeight - i, y - iHeight, x + iWidth - i, y);

			if(g_Mouse.HasMouseOneJustBeenReleased() && (bOverBack || bOverTri))
				setindex = index;

			if(i == iHeight)
			{
				if(bOverBack || bOverTri)
					g_Draw.FillRGBA(x + iHeight,y - iHeight,iWidth - iHeight,iHeight + 1,COLOR8);
				else
					g_Draw.FillRGBA(x + iHeight,y - iHeight,iWidth - iHeight,iHeight + 1,COLOR1);
			}
		}
	}

	g_Valve.pSurface->DrawSetColor(COLOR2);

	g_Valve.pSurface->DrawLine(x, y, x + iHeight, y - iHeight);//  Erste Schräge nach Rechts
	g_Valve.pSurface->DrawLine(x + iHeight, y - iHeight, x + iWidth + iHeight, y - iHeight);  //Gerade Linie von Oben
	g_Valve.pSurface->DrawLine(x + iWidth, y, x + iWidth + iHeight, y - iHeight);//  Zweite Schräge nach Rechts
	g_Valve.pSurface->DrawLine(x, y, x + iWidth, y); // Gerade Linie von Unten
	g_Draw.DrawStringA(g_Draw.m_MenuFont,true,x + (iWidth + iHeight) * 0.5,y - 2 - iHeight,TEXTCOLOR1,pszTitle);
}

int iNubLoop[] = {22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0};

void cMenu::DrawTab(int index, int &setindex, int x, int y, const char *pszTitle)
{
	static int iWidth = 80;
	static int iHeight = 22;

	bool bOverTri = false;

	bool bOverBack = g_Mouse.IsOver(x + iHeight, y - iHeight,iWidth + 1 - iHeight,iHeight + 1);

	bool bSame = setindex == index;

	for(int ax = 0; ax <= iHeight; ax++)
	{
		if(g_Mouse.IsOver(x + ax,y - ax,2,ax) || g_Mouse.IsOver(x + iWidth + ax,y - iHeight,2,iNubLoop[ax]))
		{
			bOverTri = true;
			break;
		}
	}

	for (int i = 0; i <= iHeight; i++)
	{
		if(bSame)
		{
			g_Valve.pSurface->DrawSetColor(COLOR7);

			g_Valve.pSurface->DrawLine(x + i, y, x + iHeight + i, y - iHeight);
			g_Valve.pSurface->DrawLine(x + iWidth + iHeight - i, y - iHeight, x + iWidth - i, y);

			if(i == iHeight)
				g_Draw.FillRGBA(x + iHeight,y - iHeight,iWidth - iHeight,iHeight + 1,COLOR7);
		}
		else
		{
			if(bOverBack || bOverTri)
				g_Valve.pSurface->DrawSetColor(COLOR8);
			else
				g_Valve.pSurface->DrawSetColor(COLOR1);

			g_Valve.pSurface->DrawLine(x + i, y, x + iHeight + i, y - iHeight);
			g_Valve.pSurface->DrawLine(x + iWidth + iHeight - i, y - iHeight, x + iWidth - i, y);

			if(g_Mouse.HasMouseOneJustBeenReleased() && (bOverBack || bOverTri))
				setindex = index;

			if(i == iHeight)
			{
				if(bOverBack || bOverTri)
					g_Draw.FillRGBA(x + iHeight,y - iHeight,iWidth - iHeight,iHeight + 1,COLOR8);
				else
					g_Draw.FillRGBA(x + iHeight,y - iHeight,iWidth - iHeight,iHeight + 1,COLOR1);
			}
		}
	}

	g_Valve.pSurface->DrawSetColor(COLOR2);

	g_Valve.pSurface->DrawLine(x, y, x + iHeight, y - iHeight);//  Erste Schräge nach Rechts
	g_Valve.pSurface->DrawLine(x + iHeight, y - iHeight, x + iWidth + iHeight + 1, y - iHeight);  //Gerade Linie von Oben
	g_Valve.pSurface->DrawLine(x + iWidth, y, x + iWidth + iHeight, y - iHeight);//  Zweite Schräge nach Rechts
	g_Valve.pSurface->DrawLine(x, y, x + iWidth, y); // Gerade Linie von Unten
	g_Draw.DrawStringA(g_Draw.m_MenuFont,true,x + (iWidth + iHeight) * 0.5,y + 1 - iHeight,TEXTCOLOR1,pszTitle);
}

void cMenu::DrawAConfig(int index, int cfgidx, int x, int y, std::string sString)
{
	for(int i = 0; i < sString.size(); i++)
	{
		std::string sCopy = sString;

		sCopy = &sCopy[i]; 
		sCopy[1] = 0;

		g_Draw.DrawStringA(g_Draw.m_MenuFont,false, x, y + (i * 10), TEXTCOLOR1, sCopy.data());
	}

	if(GetConfigIndex(cfgidx) != index)
		SetConfigIndex(index,cfgidx);
}

void OutlinedRectangle(int x,int y,int w,int h, int r, int g, int b, int a)
{
	g_Draw.FillRGBA(x,y,w,1,r,g,b,a);
	g_Draw.FillRGBA(x,y,1,h,r,g,b,a);
	g_Draw.FillRGBA(x+w,y,1,h+1,r,g,b,a);
	g_Draw.FillRGBA(x,y+h,w,1,r,g,b,a);
}

void SoftOutlinedRectangle(int x,int y,int w,int h, int r, int g, int b, int a)
{
	//top
	g_Draw.FillRGBA(x + 2,y,w - 3,1,r,g,b,a);
	g_Draw.FillRGBA(x + 1,y + 1,1,1,r,g,b,a);

	//left
	g_Draw.FillRGBA(x,y + 2,1,h - 3,r,g,b,a);
	g_Draw.FillRGBA(x + 1,y+h - 1,1,1,r,g,b,a);

	//right
	g_Draw.FillRGBA(x+w,y + 2,1,h - 3,r,g,b,a);
	g_Draw.FillRGBA(x+w - 1,y + 1,1,1,r,g,b,a);

	//bottom
	g_Draw.FillRGBA(x + 2,y+h,w - 3,1,r,g,b,a);
	g_Draw.FillRGBA(x+w - 1,y+h - 1,1,1,r,g,b,a);
}

#define ELEMENT_HEIGHT 15

void cListItem::Draw(int x, int y,int w, int h)
{
	int iNumOfItems = GetNumberOfItems();

	if(iNumOfItems == 0)
	{
		g_Draw.FillRGBA(x+w+2,y,8,h,COLOR2);

		OutlinedRectangle(x+w+2,y,8,h,COLOR6);

		OutlinedRectangle(x,y,w,h,COLOR2);

		return;
	}

	//we lost some old members probably... so we gotta make sure we still have some kind of selection
	if(iSelectedIndex >= iNumOfItems)
		iSelectedIndex = (iNumOfItems - 1);

	if(iTopIndex >= iNumOfItems)
		iTopIndex = 0;

	DisplayPerPage(((h / ELEMENT_HEIGHT) - 1));

	bScrollingNeeded = true;

	//we can display all items already so we dont have to change our top index
	if(iDisplayPerPage > iNumOfItems)
		bScrollingNeeded = false;

	//counter for items we will display/draw
	int iDisplayed = 0;

	for(int i = iTopIndex; i < iNumOfItems; i++)
	{
		//once display limit reached (if its even reached), we dont wanna display anymore
		if(iDisplayed > iDisplayPerPage)
			break;

		bool bBlackText = false;

		//current draw item
		//int iItemIndex = iTopIndex + i;
		ListItem_t liItem = GetItemFromIndex(i);

		int iDrawIndex = i - iTopIndex;
		
		//every second-item will have a different color (another optional rule for lists)
		if((iDrawIndex & 2) == 1)//the leftover when dividing (i) by 2 is zero
			g_Draw.FillRGBA(x,y + iDrawIndex * ELEMENT_HEIGHT,w,ELEMENT_HEIGHT,255,255,255,255);

		if(g_Mouse.IsOver(x,y + iDrawIndex * ELEMENT_HEIGHT,w,ELEMENT_HEIGHT + 1))
		{
			//mouse is hoovering over our item ????
			g_Draw.FillRGBA(x,y + iDrawIndex * ELEMENT_HEIGHT,w,ELEMENT_HEIGHT,COLOR9);

			//handle selection upon a mouse click
			if(g_Mouse.HasMouseOneJustBeenReleased())
				this->Select(i);

			bBlackText = true;
		}

		if(iSelectedIndex == i)
		{
			g_Draw.FillRGBA(x,y + iDrawIndex * ELEMENT_HEIGHT,w,ELEMENT_HEIGHT,COLOR9);

			bBlackText = true;
		}

		if(bBlackText)
			g_Draw.DrawStringA(g_Draw.m_ListItemFont,false,x + 5,y + iDrawIndex * ELEMENT_HEIGHT + 1,0,0,0,255,liItem.szName);
		else
			g_Draw.DrawStringA(g_Draw.m_ListItemFont,false,x + 5,y + iDrawIndex * ELEMENT_HEIGHT + 1,LISTITEM_TEXTCOLOR,liItem.szName);

		//we displayed the current item with the index (i), so lets try and display the next item
		iDisplayed++;
	}

	//is this list in focus aka active
	if(g_Mouse.IsOver(x,y,w,h))
	{
		if(!IsFocused())
			SetFocus(true);
	}
	else
	{
		if(!IsOnlyListOnPage())
			bScrollingNeeded = false;//why would we scroll if we list aint active + this isnt only list on menu page

		if(IsFocused())
			SetFocus(false);
	}

	if(bScrollingNeeded)
	{
		int iHowMany = iNumOfItems - iDisplayPerPage;
		int iScrollBar = (iHowMany * 3.5);
		int iWhen = 0;

		if(iScrollBar > (h - 20))
		{
			iScrollBar = h - 20;

			for(int nab = 0; ; nab++)
			{
				int iNew = (nab * 3.5);

				if(iNew > (h - 20))
				{
					iWhen = nab;
					break;
				}
			}
		}

		int iDiff = 0;
		
		if(iWhen > 0)
			iDiff = abs(iHowMany - iWhen) * 3.5;

		int iScrollBarHai = h + 3.5 - iScrollBar;
		int iScrollYAdd = iTopIndex * 3.5 * ((iWhen > 0) ? ((3.5 / iDiff) * 10 + 0.15) : 1);
		int iScrollYPos = y + iScrollYAdd;
		int iSavedScrollYPos = iScrollYPos;
		int iDummy;

		g_Mouse.Drag(g_Mouse.bDragged[1],true/*!g_Radar.IsHandlingItem()*/,
			g_Mouse.LeftClick(x+w+2,y,8,h),iDummy,iScrollYPos,g_Mouse.iDiffX[1],g_Mouse.iDiffY[1]);

		if(g_Mouse.bDragged[1] && !g_Menu.IsHandlingItem())//make sure we disable menu dragging
			g_Menu.AddMenuFlag(FL_DISABLEDRAG);

		if(iScrollYPos != iSavedScrollYPos)//slider was dragged
		{
			int iNewTopIndex = (iScrollYPos - y);
			iNewTopIndex /= 3.5;
			iNewTopIndex /= ((iWhen > 0) ? ((3.5 / iDiff) * 10 + 0.15) : 1);

			//min
			if(iNewTopIndex < 0)
				iNewTopIndex = 0;

			//max
			int iLastPossibleTopIndex = abs(iNumOfItems - iDisplayPerPage) - 1;

			if(iNewTopIndex > iLastPossibleTopIndex)
				iNewTopIndex = iLastPossibleTopIndex;

			//we clamped our new top index now we just gotta clamp our y pos
			iScrollYPos = y + (iNewTopIndex * 3.5 * ((iWhen > 0) ? ((3.5 / iDiff) * 10 + 0.15) : 1));

			//set the top index of the list
			iTopIndex = iNewTopIndex;
		}

		g_Draw.FillRGBA(x+w+2,iScrollYPos,8,iScrollBarHai,COLOR2);
	}
	else
		g_Draw.FillRGBA(x+w+2,y,8,h,COLOR2);

	OutlinedRectangle(x+w+2,y,8,h,COLOR6);

	OutlinedRectangle(x,y,w,h,COLOR2);
}

//SketchFlow Print Normal
void cMenu::DrawMenu()
{
	static std::string sAim = /*AIM*/XorStr<0x97,4,0xB54C0325>("\xD6\xD1\xD4"+0xB54C0325).s,
						sLegit = /*LEGIT*/XorStr<0x22,6,0x4C128A7C>("\x6E\x66\x63\x6C\x72"+0x4C128A7C).s,
						sEsp = /*VISUALS*/XorStr<0x75,8,0x095D04AD>("\x23\x3F\x24\x2D\x38\x36\x28"+0x095D04AD).s,
						sMisc = /*MISC*/XorStr<0x7C,5,0x4162A24B>("\x31\x34\x2D\x3C"+0x4162A24B).s,
						sSpecial = /*SPECIAL*/XorStr<0xCE,8,0x353F70E0>("\x9D\x9F\x95\x92\x9B\x92\x98"+0x353F70E0).s,
						sColor = /*COLORS*/XorStr<0xEF,7,0x8667B464>("\xAC\xBF\xBD\xBD\xA1\xA7"+0x8667B464).s,
						sL = /*L*/XorStr<0x96,2,0x0E0D140A>("\xDA"+0x0E0D140A).s,
						sO = /*O*/XorStr<0xD3,2,0xA3BE135B>("\x9C"+0xA3BE135B).s,
						sA = /*A*/XorStr<0x55,2,0xD2E94581>("\x14"+0xD2E94581).s,
						sD = /*D*/XorStr<0x9C,2,0x12A7B0FD>("\xD8"+0x12A7B0FD).s,
						sS = /*S*/XorStr<0x63,2,0x74AD7D8A>("\x30"+0x74AD7D8A).s,
						sV = /*V*/XorStr<0xD2,2,0x412FCA4F>("\x84"+0x412FCA4F).s,
						sE = /*E*/XorStr<0x7B,2,0x6E24E6F1>("\x3E"+0x6E24E6F1).s;

	/******************************************Tabs**********************************************/
	//Background
	g_Draw.FillRGBA(m_x + 22, m_y - 22, 80 * NUM_OF_TABS + 20, 23, COLOR4);

	g_Valve.pSurface->DrawSetColor(COLOR1);

	//for (int i = 0; i <= 20; i++)
	//{
		//g_Valve.pSurface->DrawLine(m_x + i, m_y, m_x + 20 + i, m_y - 20);
		//g_Valve.pSurface->DrawLine(m_x + 80 * NUM_OF_TABS + 60 - i, m_y - 20, m_x + 80 * NUM_OF_TABS + 40 - i, m_y);
	//}

	DrawTab(AIMBOTTAB,iTabIndex,m_x,m_y,sAim.data());
	DrawTab(ESPTAB,iTabIndex,m_x + 102,m_y,sEsp.data());
	DrawTab(MISCTAB,iTabIndex,m_x + 204,m_y,sMisc.data());
	DrawTab(SPECIALTAB,iTabIndex,m_x + 306,m_y,sSpecial.data());
	DrawTab(COLORTAB,iTabIndex,m_x + 409,m_y,sColor.data());

	/******************************************Menu**********************************************/
	g_Draw.FillRGBA(m_x, m_y + 1, m_w, m_h, COLOR3); // Black outlining of Dark Gray background
	g_Draw.FillRGBA(m_x + 1, m_y + 2, m_w - 2, m_h - 2, COLOR4); // Dark Gray background
	g_Draw.FillRGBA(m_x + 6, m_y + 7, m_w - 12, m_h - 12, COLOR3); // Black outlining of Gray Background
	g_Draw.FillRGBA(m_x + 7, m_y + 8, m_w - 14, m_h - 14, COLOR5);  //Gray Background
	g_Draw.FillRGBA(m_x + 5, m_y + 6, m_w - 10, 17, COLOR3);
	g_Draw.FillRGBA(m_x + 5, m_y + 7, m_w - 10, 15, COLOR5);

//#define BUILDSTAMP ( __DATE__ " @ " __TIME__ )
	//g_Draw.DrawStringA(g_Draw.m_MenuFont,false, m_x + 15, m_y + 7, TEXTCOLOR1, "BINARY BUILD DATE: %s", BUILDSTAMP);
	
	//default config
	static int iCfgIdx = 0;
	//legit config
	static int iCfgIdxTwo = 1;

	int iTriX = m_x - 27;
	int iTriY = m_y + 37;

	if(g_Mouse.OneLeftClick(iTriX - 13,m_y + 31,25,13))
	{
		iCfgIdx++;

		if(iCfgIdx >= NUM_OF_CFGS)
			iCfgIdx = 0;
	}

	g_Draw.FillRGBA(iTriX,iTriY,1,1,COLOR_CFG_POINTERS);
	g_Draw.FillRGBA(iTriX + 1,iTriY - 1,1,3,COLOR_CFG_POINTERS);
	g_Draw.FillRGBA(iTriX + 2,iTriY - 2,1,5,COLOR_CFG_POINTERS);
	g_Draw.FillRGBA(iTriX + 3,iTriY - 3,1,7,COLOR_CFG_POINTERS);
	g_Draw.FillRGBA(iTriX + 4,iTriY - 4,1,9,COLOR_CFG_POINTERS);
	g_Draw.FillRGBA(iTriX + 5,iTriY - 5,1,11,COLOR_CFG_POINTERS);
	g_Draw.FillRGBA(iTriX + 6,iTriY - 6,1,13,COLOR_CFG_POINTERS);

	g_Valve.pSurface->DrawSetColor(COLOR_CFG_POINTERS_BORDER);
	g_Valve.pSurface->DrawLine(iTriX,iTriY,iTriX + 6,iTriY - 6);
	g_Valve.pSurface->DrawLine(iTriX,iTriY,iTriX + 6,iTriY + 6);
	g_Valve.pSurface->DrawLine(iTriX + 6,iTriY - 6,iTriX + 6,iTriY + 7);

	//slower but ok
	DrawAConfig(iCfgIdx,0,m_x - 10, m_y + 7,GetCurrentConfig());

	g_Draw.FillRGBA(m_x - 15, m_y + 87, 15, 55, COLOR3);

	if (g_Mouse.OneLeftClick(m_x - 14, m_y + 88, 14, 53))
		g_CVARS.HandleConfig(GetCurrentConfigName(),LoadConfig);

	if(g_Mouse.LeftClick(m_x - 14, m_y + 88, 14, 53))
		g_Draw.FillRGBA(m_x - 15, m_y + 87, 15, 55, TEXTCOLOR1);

	g_Draw.FillRGBA(m_x - 14, m_y + 88, 14, 53, COLOR6);
	g_Draw.DrawStringA(g_Draw.m_MenuFont,false, m_x - 10, m_y + 87, TEXTCOLOR1, sL.data());
	g_Draw.DrawStringA(g_Draw.m_MenuFont,false, m_x - 10, m_y + 99, TEXTCOLOR1, sO.data());
	g_Draw.DrawStringA(g_Draw.m_MenuFont,false, m_x - 10, m_y + 111, TEXTCOLOR1, sA.data());
	g_Draw.DrawStringA(g_Draw.m_MenuFont,false, m_x - 10, m_y + 123, TEXTCOLOR1, sD.data());
	g_Draw.FillRGBA(m_x - 15, m_y + 193, 15, 55, COLOR3);
	
	if (g_Mouse.OneLeftClick(m_x - 14, m_y + 194, 14, 53))
		g_CVARS.HandleConfig(GetCurrentConfigName(),SaveConfig);

	if (g_Mouse.LeftClick(m_x - 14, m_y + 194, 14, 53))
		g_Draw.FillRGBA(m_x - 15, m_y + 193, 15, 55, TEXTCOLOR1);

	g_Draw.FillRGBA(m_x - 14, m_y + 194, 14, 53, COLOR6);
	g_Draw.DrawStringA(g_Draw.m_MenuFont,false, m_x - 10, m_y + 193, TEXTCOLOR1, sS.data());
	g_Draw.DrawStringA(g_Draw.m_MenuFont,false, m_x - 10, m_y + 205, TEXTCOLOR1, sA.data());
	g_Draw.DrawStringA(g_Draw.m_MenuFont,false, m_x - 10, m_y + 217, TEXTCOLOR1, sV.data());
	g_Draw.DrawStringA(g_Draw.m_MenuFont,false, m_x - 10, m_y + 229, TEXTCOLOR1, sE.data());

	int iTriTwoX = m_x + m_w + 20;
	int iTriTwoY = m_y + 31;

	if(g_Mouse.OneLeftClick(iTriTwoX - 5,iTriTwoY,30,13))
	{
		iCfgIdxTwo++;

		if(iCfgIdxTwo >= NUM_OF_CFGS)
			iCfgIdxTwo = 0;
	}

	g_Draw.FillRGBA(iTriTwoX,iTriTwoY,1,13,COLOR_CFG_POINTERS);
	g_Draw.FillRGBA(iTriTwoX + 1,iTriTwoY + 1,1,11,COLOR_CFG_POINTERS);
	g_Draw.FillRGBA(iTriTwoX + 2,iTriTwoY + 2,1,9,COLOR_CFG_POINTERS);
	g_Draw.FillRGBA(iTriTwoX + 3,iTriTwoY + 3,1,7,COLOR_CFG_POINTERS);
	g_Draw.FillRGBA(iTriTwoX + 4,iTriTwoY + 4,1,5,COLOR_CFG_POINTERS);
	g_Draw.FillRGBA(iTriTwoX + 5,iTriTwoY + 5,1,3,COLOR_CFG_POINTERS);
	g_Draw.FillRGBA(iTriTwoX + 6,iTriTwoY + 6,1,1,COLOR_CFG_POINTERS);

	g_Valve.pSurface->DrawSetColor(COLOR_CFG_POINTERS_BORDER);
	g_Valve.pSurface->DrawLine(iTriTwoX + 6,iTriTwoY + 6,iTriTwoX,iTriTwoY);
	g_Valve.pSurface->DrawLine(iTriTwoX + 6,iTriTwoY + 6,iTriTwoX,iTriTwoY + 12);
	g_Valve.pSurface->DrawLine(iTriTwoX,iTriTwoY,iTriTwoX,iTriTwoY + 13);

	DrawAConfig(iCfgIdxTwo,1,m_x + m_w + 5, m_y + 7,GetCurrentConfig(1));

	g_Draw.FillRGBA(m_x + m_w, m_y + 87, 15, 55, COLOR3);
	
	if (g_Mouse.OneLeftClick(m_x + m_w, m_y + 88, 14, 53))
		g_CVARS.HandleConfig(GetCurrentConfigName(1),LoadConfig);

	if (g_Mouse.LeftClick(m_x + m_w, m_y + 88, 15, 55))
		g_Draw.FillRGBA(m_x + m_w, m_y + 87, 15, 55, TEXTCOLOR1);

	g_Draw.FillRGBA(m_x + m_w, m_y + 88, 14, 53, COLOR6);
	
	g_Draw.DrawStringA(g_Draw.m_MenuFont,false, m_x + m_w + 5, m_y + 87, TEXTCOLOR1, sL.data());
	g_Draw.DrawStringA(g_Draw.m_MenuFont,false, m_x + m_w + 5, m_y + 99, TEXTCOLOR1, sO.data());
	g_Draw.DrawStringA(g_Draw.m_MenuFont,false, m_x + m_w + 5, m_y + 111, TEXTCOLOR1, sA.data());
	g_Draw.DrawStringA(g_Draw.m_MenuFont,false, m_x + m_w + 5, m_y + 123, TEXTCOLOR1, sD.data());
	g_Draw.FillRGBA(m_x + m_w, m_y + 193, 15, 55, COLOR3);
	
	if (g_Mouse.OneLeftClick(m_x + m_w + 5, m_y + 194, 14, 53))
		g_CVARS.HandleConfig(GetCurrentConfigName(1),SaveConfig);

	if (g_Mouse.LeftClick(m_x + m_w + 5, m_y + 194, 14, 53))
		g_Draw.FillRGBA(m_x + m_w, m_y + 193, 15, 55, TEXTCOLOR1);
	
	g_Draw.FillRGBA(m_x + m_w, m_y + 194, 14, 53, COLOR6);
	g_Draw.DrawStringA(g_Draw.m_MenuFont,false, m_x + m_w + 5, m_y + 193, TEXTCOLOR1, sS.data());
	g_Draw.DrawStringA(g_Draw.m_MenuFont,false, m_x + m_w + 5, m_y + 205, TEXTCOLOR1, sA.data());
	g_Draw.DrawStringA(g_Draw.m_MenuFont,false, m_x + m_w + 5, m_y + 217, TEXTCOLOR1, sV.data());
	g_Draw.DrawStringA(g_Draw.m_MenuFont,false, m_x + m_w + 5, m_y + 229, TEXTCOLOR1, sE.data());

	///*********************************************************************************************/

	InitMenuElements();
}

void cMenuSection::Draw(int x, int y, int w, int h, const char *pszSectionName)
{
	int iWidth = g_Draw.getWidht(g_Draw.m_ListItemFont,pszSectionName);

	g_Draw.DrawStringA(g_Draw.m_ListItemFont,false,x+20,y-7,LISTITEM_TEXTCOLOR,pszSectionName);

	g_Draw.FillRGBA(x,y,10,1,COLOR2);
	g_Draw.FillRGBA(x,y,1,h,COLOR2);
	g_Draw.FillRGBA(x,y+h,w,1,COLOR2);
	g_Draw.FillRGBA(x+w,y,1,h+1,COLOR2);
	g_Draw.FillRGBA(x+30+iWidth,y,w-(30+iWidth),1,COLOR2);

	SetSectionPos(x,y);
}

int iRed,iGreen,iBlue;
void GradientVertical(int x, int y, int w, int h, int r, int g, int b, int a,int r2, int g2, int b2)
{
	y += 1;

	for(int i = 1; i < h; i++)
	{
		iRed = (int)((float)i/h*(r - r2));
		iGreen = (int)((float)i/h*(g - g2));
		iBlue = (int)((float)i/h*(b - b2));
		g_Draw.FillRGBA(x,y+i-1,w,1,r-iRed,g-iGreen,b-iBlue,a);
	}
}

void GradientHorizontal(int x, int y, int w, int h, int r, int g, int b, int a, int r2, int g2, int b2)
{
	for(int i=1; i < w; i++)
	{
		iRed = (int)((float)i/w*(r - r2));
		iGreen = (int)((float)i/w*(g - g2));
		iBlue = (int)((float)i/w*(b - b2));
		g_Draw.FillRGBA(x+i-1,y,1,h,r-iRed,g-iGreen,b-iBlue,a);
	}
}

void cMenuSection::DrawAllDropDowns()
{
	int iNumOfDropDowns = vecDropDowns.size();

	int iWhich = -1;

	//AFTERCLICK FIX
	for(int i2 = (iNumOfDropDowns - 1); i2 >= 0; i2--)
	{
		bool bIsActive = bActiveDropDown[i2];

		if(bIsActive)
		{
			iWhich = i2;

			dwWait = GetTickCount() + 700;
			break;
		}
	}

	for(int i = (iNumOfDropDowns - 1); i >= 0; i--)
	{
		int e_x = vecDropDowns[i].x;
		int e_y = vecDropDowns[i].y;
		int iCount = vecDropDowns[i].iCount;

		bool bIsActive = bActiveDropDown[i];

		char **ppszNames = vecDropDowns[i].ppszNames;
		float *fValues  = vecDropDowns[i].fValues;
		float *cvar = vecDropDowns[i].cvar;

		if(bIsActive)
		{
			int string_x = e_x + (DROPDOWN_WIDTH * 0.5);

			for(int ax = 0; ax < iCount; ax++)
			{
				if(g_Mouse.IsOver(e_x,e_y + (ax * DROPDOWN_HEIGHT),DROPDOWN_WIDTH,DROPDOWN_HEIGHT))
				{
					if(!g_Menu.IsHandlingItem())
						g_Menu.AddMenuFlag(FL_DISABLEDRAG);

					g_Draw.FillRGBA(e_x + 1,e_y + 1 + (ax * DROPDOWN_HEIGHT),DROPDOWN_WIDTH - 1,DROPDOWN_HEIGHT - (ax == (iCount - 1) ? 1 : 0),COLOR5);
					
					if(g_Mouse.HasMouseOneJustBeenReleased())
					{
						*cvar = fValues[ax];

						bActiveDropDown[i] = false;
					}
				}
				else
				{
					g_Draw.FillRGBA(e_x + 1,e_y + 1 + (ax * DROPDOWN_HEIGHT),DROPDOWN_WIDTH - 1,DROPDOWN_HEIGHT - (ax == (iCount - 1) ? 1 : 0),COLOR4);
				
					if(g_Mouse.HasMouseOneJustBeenReleased())
					{
						bActiveDropDown[i] = false;
					}
				}

				if(ax == (iCount - 1))//do once only
					SoftOutlinedRectangle(e_x,e_y,DROPDOWN_WIDTH,(ax + 1) * DROPDOWN_HEIGHT,COLOR3);

				//strings here
				if(*cvar == fValues[ax])
					g_Draw.DrawStringA(g_Draw.m_ListItemFont,true,string_x,e_y + (ax * DROPDOWN_HEIGHT),COLOR9,ppszNames[ax]);
				else
					g_Draw.DrawStringA(g_Draw.m_ListItemFont,true,string_x,e_y + (ax * DROPDOWN_HEIGHT),LISTITEM_TEXTCOLOR,ppszNames[ax]);
			}
		}
		else//drop down not active
		{
			if(g_Mouse.IsOver(e_x,e_y,DROPDOWN_WIDTH,DROPDOWN_HEIGHT))
			{
				if(!g_Menu.IsHandlingItem())
					g_Menu.AddMenuFlag(FL_DISABLEDRAG);

				g_Draw.FillRGBA(e_x + 1,e_y + 1,DROPDOWN_WIDTH - 1,DROPDOWN_HEIGHT - 1,COLOR5);

				if(g_Mouse.HasMouseOneJustBeenReleased() && (dwWait < GetTickCount() || iWhich == -1))
				{
					bActiveDropDown[i] = true;
				}
			}
			else
				g_Draw.FillRGBA(e_x + 1,e_y + 1,DROPDOWN_WIDTH - 1,DROPDOWN_HEIGHT - 1,COLOR4);

			SoftOutlinedRectangle(e_x,e_y,DROPDOWN_WIDTH,DROPDOWN_HEIGHT,COLOR3);

			int string_x = e_x + (DROPDOWN_WIDTH * 0.5);
			int string_y = e_y;

			for(int ax = 0; ax < iCount; ax++)
			{
				if(*cvar == fValues[ax])//we display only the cvar matching the value in the list
				{
					g_Draw.DrawStringA(g_Draw.m_ListItemFont,true,string_x,string_y,LISTITEM_TEXTCOLOR,ppszNames[ax]);
					break;
				}
			}
		}
	}

	ClearAllElementSpecifics();
}

void cMenuSection::AddElement(int iType, int add_to_x, const char *pszElementName, const char *pszComment, float *cvar, float min, float max, float step, float fToValue, int increment)
{
	int e_x = s_x + 10 + add_to_x;
	int e_y = s_y + 15 + (iSection * ELEMENT_SEPERATION);

	if(iType != BUTTON)
		g_Menu.DrawHeaderComment(e_x,e_y,pszComment,(iType == SLIDER));

	bool bDropDownActive = false;

	for(int i2 = 0; i2 < 50; i2++)
	{
		bDropDownActive = bActiveDropDown[i2];

		if(bDropDownActive)
			break;
	}

	if(iType == ONOFF)
	{
		g_Draw.DrawStringA(g_Draw.m_ListItemFont,false,e_x,e_y,LISTITEM_TEXTCOLOR,pszElementName);

		e_x += CHECKBOX_SEPERATION_FROM_TEXT;

		if(g_Mouse.OneLeftClick(e_x - 3,e_y - 3,CHECKBOX_SIZE + 5,CHECKBOX_SIZE + 5) && !bDropDownActive)
			*cvar = !*cvar;

		e_y += 1;

		g_Draw.FillRGBA(e_x,e_y,CHECKBOX_SIZE,CHECKBOX_SIZE,COLOR7);

		if(*cvar)
		{
			g_Valve.pSurface->DrawSetColor(COLOR4);

			g_Valve.pSurface->DrawLine(e_x + 2,e_y + 2,e_x + CHECKBOX_SIZE - 1,e_y + CHECKBOX_SIZE - 1);
			g_Valve.pSurface->DrawLine(e_x + CHECKBOX_SIZE - 2,e_y + 2,e_x + 1,e_y + CHECKBOX_SIZE - 1);
		}

		if(g_Mouse.IsOver(e_x,e_y,CHECKBOX_SIZE,CHECKBOX_SIZE))
		{
			OutlinedRectangle(e_x,e_y,CHECKBOX_SIZE,CHECKBOX_SIZE,COLOR9);
			
			if(!g_Menu.IsHandlingItem())
				g_Menu.AddMenuFlag(FL_DISABLEDRAG);
		}
		else
			OutlinedRectangle(e_x,e_y,CHECKBOX_SIZE,CHECKBOX_SIZE,COLOR4);
	}
	else if(iType == DROPDOWN)
	{
		g_Draw.DrawStringA(g_Draw.m_ListItemFont,false,e_x,e_y,LISTITEM_TEXTCOLOR,pszElementName);

		e_y += 1;

		DropDown_t ddNew;

		ddNew.x = e_x + CHECKBOX_SEPERATION_FROM_TEXT - 20;
		ddNew.y = e_y - 2;
		ddNew.fValues = fValueList;
		ddNew.cvar = cvar;
		ddNew.ppszNames = ppszValueNames;
		ddNew.iCount = iValueCount;

		AddDropDownToDrawList(ddNew);
	}
	else if(iType == SLIDER)
	{
		float Scale = max/fLength;

		bool bOver = false;

		if(bShowValue)
		{
			char szCopy[256];

			strcpy(szCopy,pszElementName);
			sprintf(szCopy,/*%s %2.2f*/XorStr<0xEB,9,0xE008EA4B>("\xCE\x9F\xCD\xCB\xDD\xDE\xC3\x94"+0xE008EA4B).s,pszElementName,*cvar);
			
			g_Draw.DrawStringA(g_Draw.m_ListItemFont,false,e_x,e_y-7,LISTITEM_TEXTCOLOR,szCopy);
		}
		else
			g_Draw.DrawStringA(g_Draw.m_ListItemFont,false,e_x,e_y-7,LISTITEM_TEXTCOLOR,pszElementName);

		e_y += 8;

		if(bColored)
		{
			g_Draw.FillRGBA(e_x,e_y+4,fLength+2,5,fColDummy[0],fColDummy[1],fColDummy[2],fColDummy[3]);		

			OutlinedRectangle(e_x,e_y+4,fLength+2,5,70,70,70,255);

			//Bottom effect
			GradientHorizontal(e_x,e_y+9,58,1,70,70,70,255,115,120,123);
			g_Draw.FillRGBA(e_x+57,e_y+9,240,1,115,120,123,185);
			GradientHorizontal(e_x+297,e_y+9,67,1,115,120,123,255,70,70,70);

			GradientHorizontal(e_x,e_y+4,58,1,70,70,70,255,115,120,123);
			g_Draw.FillRGBA(e_x+57,e_y+4,240,1,115,120,123,185);
			GradientHorizontal(e_x+297,e_y+4,67,1,115,120,123,255,70,70,70);
		}
		else
		{
			SoftOutlinedRectangle(e_x,e_y+4,fLength+2,5,70,70,70,255);

			//Bottom effect
			GradientHorizontal(e_x + 2,e_y+9,56,1,70,70,70,255,115,120,123);
			g_Draw.FillRGBA(e_x+57,e_y+9,240,1,115,120,123,185);
			GradientHorizontal(e_x+297,e_y+9,65,1,115,120,123,255,70,70,70);

			GradientHorizontal(e_x + 2,e_y+4,56,1,70,70,70,255,115,120,123);
			g_Draw.FillRGBA(e_x+57,e_y+4,240,1,115,120,123,185);
			GradientHorizontal(e_x+297,e_y+4,65,1,115,120,123,255,70,70,70);
		}

		int mouse_x, mouse_y;
		g_Mouse.GetMousePosition(mouse_x,mouse_y);

		if ( mouse_x >= ( e_x - 3 ) && mouse_x <= ( e_x + fLength + 3 ) && mouse_y >= ( e_y ) && mouse_y <= ( e_y + 10 ) && GetAsyncKeyState( VK_LBUTTON ) && (iSliderIndex == -1) && dwWait < GetTickCount() && !bSliderFix[iCurrSlider] )
		{
			iSliderIndex = iCurrSlider;
			bSliderFix[iCurrSlider] = true;

			if(!g_Menu.IsHandlingItem())
				g_Menu.AddMenuFlag(FL_DISABLEDRAG);
		}
		
		if ( mouse_x >= ( e_x - 1000 ) && mouse_x <= ( e_x + fLength + 1000 ) && mouse_y >= ( e_y - 1000 ) && mouse_y <= ( e_y + 1000 ) && GetAsyncKeyState( VK_LBUTTON ) && !bDropDownActive)
		{
			if(bSliderFix[iCurrSlider])
			{
				*cvar = min + (mouse_x - e_x) * Scale;
				
				bOver = true;

				if(!g_Menu.IsHandlingItem())
					g_Menu.AddMenuFlag(FL_DISABLEDRAG);
			}
		}
		else if(bSliderFix[iCurrSlider])
		{
			iSliderIndex = -1;
			bSliderFix[iCurrSlider] = false;
		}

		if(*cvar > max)
			*cvar = max;

		if(*cvar < min)
			*cvar = min;

		static int iSizeOf = 6;

		for(int nu = 0; nu <= iSizeOf; nu++)
		{
			g_Draw.FillRGBA(e_x+(*cvar/Scale)-iSizeOf+nu,e_y+iSizeOf-nu,(iSizeOf+1)-nu,1,COLOR4);
			g_Draw.FillRGBA(e_x+(*cvar/Scale)-iSizeOf+nu,e_y+iSizeOf+nu,(iSizeOf+1)-nu,1,COLOR4);

			g_Draw.FillRGBA(e_x+(*cvar/Scale),e_y+iSizeOf-nu,(iSizeOf+1)-nu,1,COLOR4);
			g_Draw.FillRGBA(e_x+(*cvar/Scale),e_y+iSizeOf+nu,(iSizeOf+1)-nu,1,COLOR4);
		}

		if(g_Mouse.IsOver(e_x+(*cvar/Scale)-iSizeOf,e_y-1,10,14) || bOver)
			g_Valve.pSurface->DrawSetColor(COLOR9);
		else
			g_Valve.pSurface->DrawSetColor(COLOR6);

		g_Valve.pSurface->DrawLine(e_x+(*cvar/Scale)-(iSizeOf+1),e_y+iSizeOf,e_x+(*cvar/Scale),e_y-1);
		g_Valve.pSurface->DrawLine(e_x+(*cvar/Scale),e_y-1,e_x+(*cvar/Scale)+(iSizeOf+1),e_y+iSizeOf);
		g_Valve.pSurface->DrawLine(e_x+(*cvar/Scale)+(iSizeOf+1),e_y+iSizeOf,e_x+(*cvar/Scale),e_y+(iSizeOf*2+1));
		g_Valve.pSurface->DrawLine(e_x+(*cvar/Scale),e_y+(iSizeOf*2+1),e_x+(*cvar/Scale)-(iSizeOf+1),e_y+iSizeOf);

		if(iColorIndex != -1)
		{
			g_CVARS.CvarList[iColorIndex] = fColDummy[0];
			g_CVARS.CvarList[iColorIndex + 1] = fColDummy[1];
			g_CVARS.CvarList[iColorIndex + 2] = fColDummy[2];
			g_CVARS.CvarList[(bChams ? ModelAlpha : (iColorIndex + 3))] = fColDummy[3];
		}
	}
	else if(iType == BUTTON)
	{
		bool bLeftClick = g_Mouse.LeftClick(e_x,e_y,BUTTON_WIDTH,BUTTON_HEIGHT);

		if(fToValue == -1)
		{
			if(*cvar)
				GradientVertical(e_x,e_y,BUTTON_WIDTH,BUTTON_HEIGHT,COLOR4,140,0,0);
			else
			{
				if(bLeftClick)
					GradientVertical(e_x,e_y,BUTTON_WIDTH,BUTTON_HEIGHT,COLOR9,35,35,35);			
				else
					GradientVertical(e_x,e_y,BUTTON_WIDTH,BUTTON_HEIGHT,COLOR4,50,50,50);
			}
		}
		else
		{
			if(*cvar == fToValue)
				GradientVertical(e_x,e_y,BUTTON_WIDTH,BUTTON_HEIGHT,COLOR4,140,0,0);
			else
			{
				if(bLeftClick)
					GradientVertical(e_x,e_y,BUTTON_WIDTH,BUTTON_HEIGHT,COLOR9,35,35,35);			
				else
					GradientVertical(e_x,e_y,BUTTON_WIDTH,BUTTON_HEIGHT,COLOR4,50,50,50);
			}
		}

		g_Draw.DrawStringA(g_Draw.m_ListItemFont,true,e_x + BUTTON_WIDTH * 0.5,e_y + 2,LISTITEM_TEXTCOLOR,pszElementName);

		if(g_Mouse.IsOver(e_x,e_y,BUTTON_WIDTH,BUTTON_HEIGHT))
		{
			if(!bLeftClick)
				SoftOutlinedRectangle(e_x,e_y,BUTTON_WIDTH,BUTTON_HEIGHT,COLOR9);
			else
				SoftOutlinedRectangle(e_x,e_y,BUTTON_WIDTH,BUTTON_HEIGHT,COLOR3);

			if(g_Mouse.HasMouseOneJustBeenReleased() && !bDropDownActive)
			{
				if(fToValue == -1)
					*cvar = !*cvar;
				else
					*cvar = fToValue;
			}
		}
		else
			SoftOutlinedRectangle(e_x,e_y,BUTTON_WIDTH,BUTTON_HEIGHT,COLOR3);
	}
	else if(iType == ADDER)
	{
		g_Draw.DrawStringA(g_Draw.m_ListItemFont,false,e_x,e_y,LISTITEM_TEXTCOLOR,pszElementName);

		e_y += 1;

		e_x += (CHECKBOX_SEPERATION_FROM_TEXT - 20);

		g_Draw.FillRGBA(e_x,e_y,ADDER_SIZE,ADDER_SIZE,COLOR7);

		if(g_Mouse.IsOver(e_x,e_y,ADDER_SIZE,ADDER_SIZE))
		{
			OutlinedRectangle(e_x,e_y,ADDER_SIZE,ADDER_SIZE,COLOR9);

			if(g_Mouse.HasMouseOneJustBeenReleased() && !bDropDownActive)
				*cvar -= step;

			if(g_Mouse.IsMouseTwoBeingHeld() && !bDropDownActive)
				*cvar -= 20;
		}
		else
			OutlinedRectangle(e_x,e_y,ADDER_SIZE,ADDER_SIZE,COLOR4);

		static std::string sLeft = /*<*/XorStr<0xB1,2,0x3FF043A5>("\x8D"+0x3FF043A5).s;

		g_Draw.DrawStringA(g_Draw.m_ListItemFont,true,e_x + ADDER_SIZE * 0.5,e_y - 3,COLOR10,sLeft.data());

		static std::string sValue = /*%2.0f*/XorStr<0x9A,6,0x7D470981>("\xBF\xA9\xB2\xAD\xF8"+0x7D470981).s;

		g_Draw.DrawStringA(g_Draw.m_ListItemFont,true,e_x - 6 + ADDER_SIZE + ADDER_SEPERATE_FROM_BOXES * 0.5,e_y - 2,LISTITEM_TEXTCOLOR,sValue.data(),*cvar);

		e_x += ADDER_SEPERATE_FROM_BOXES;

		g_Draw.FillRGBA(e_x,e_y,ADDER_SIZE,ADDER_SIZE,COLOR7);

		if(g_Mouse.IsOver(e_x,e_y,ADDER_SIZE,ADDER_SIZE))
		{
			OutlinedRectangle(e_x,e_y,ADDER_SIZE,ADDER_SIZE,COLOR9);

			if(g_Mouse.HasMouseOneJustBeenReleased() && !bDropDownActive)
				*cvar += step;

			if(g_Mouse.IsMouseTwoBeingHeld() && !bDropDownActive)
				*cvar += 20;
		}
		else
			OutlinedRectangle(e_x,e_y,ADDER_SIZE,ADDER_SIZE,COLOR4);

		static std::string sRight = /*>*/XorStr<0x38,2,0x84EC1D65>("\x06"+0x84EC1D65).s;

		g_Draw.DrawStringA(g_Draw.m_ListItemFont,true,e_x + ADDER_SIZE * 0.5,e_y - 3,COLOR10,sRight.data());

		//clamp but go to min,max instead of not allowing to increase/decrease
		if(*cvar < min)
			*cvar = max;

		if(*cvar > max)
			*cvar = min;
	}

	PostElement((iType == SLIDER ? 2 : 1));
}




