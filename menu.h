#define NUM_OF_TABS 5
#define NUM_OF_CFGS 6
#define NUM_OF_TOTAL_LISTITEMS 13


enum
{
	FL_DISABLEDRAG = (1 << 1)
};

enum
{
	DEFAULTCFG,
	LEGITCFG,
	RAGECFG,
	HVHCFG,
	MMCFG,
	CUSTOMCFG
};

enum
{
	AIMBOTTAB,
	ESPTAB,
	MISCTAB,
	SPECIALTAB,
	COLORTAB
};

enum
{
	SMALL_CHAMSTAB,
	SMALL_GLOWTAB,
	SMALL_ESPTAB,
	SMALL_PLAYERLISTTAB,
	SMALL_MODIFIEDTAB
};

enum
{
	ONOFF,
	DROPDOWN,
	SLIDER,
	BUTTON,
	ADDER
};

typedef struct
{
	char szName[128];
	int iSpecialValue;
} ListItem_t;

//pseudo itemlist by kolo ehi ehi
//this will be dynamic so we will refresh it every frame. i know its slow but who carezz ?
class cListItem
{
public:
	void Draw(int x, int y, int w, int h);

	void OnlyList()
	{
		bOnlyListOnPage = true;
	}

	bool IsOnlyListOnPage()
	{
		return bOnlyListOnPage;
	}

	void ClearList()
	{
		vecListItems.clear();

		bOnlyListOnPage = false;
	}

	int GetNumberOfItems()
	{
		return vecListItems.size();
	}

	void AddItem(const char *pszName, int iSpecialValue, bool bSelected = false)
	{
		ListItem_t liNew;

		strcpy(liNew.szName,pszName);

		liNew.iSpecialValue = iSpecialValue;

		vecListItems.push_back(liNew);
	}

	bool IsScrollingNeeded()
	{
		return bScrollingNeeded;
	}

	bool IsFocused()
	{
		return bListInFocus;
	}

	void SetFocus(bool bCondition)
	{
		bListInFocus = bCondition;
	}

	ListItem_t GetSelectedItem()
	{
		return vecListItems[iSelectedIndex];
	}

	ListItem_t GetTopItem()
	{
		return vecListItems[iTopIndex];
	}

	ListItem_t GetItemFromIndex(int index)
	{
		return vecListItems[index];
	}

	int GetSpecialValueFromIndex(int index)
	{
		return vecListItems[index].iSpecialValue;
	}

	void Select(int index)
	{
		iSelectedIndex = index;
	}

	bool IsSelected(int index)
	{
		return (iSelectedIndex == index);
	}

	void MoveUp(int amount = 2)
	{
		iTopIndex -= amount;

		if(iTopIndex < 0)
			iTopIndex = 0;
	}

	void MoveDown(int amount = 2)
	{
		iTopIndex += amount;

		//just following rules of itemlists which are scrollable
		int iLastPossibleTopIndex = abs(GetNumberOfItems() - HowManyToDisplayPerPage()) - 1;

		if(iTopIndex > iLastPossibleTopIndex)
			iTopIndex = iLastPossibleTopIndex;
	}

	int HowManyToDisplayPerPage()
	{
		return iDisplayPerPage;
	}

	void DisplayPerPage(int amount)
	{
		iDisplayPerPage = amount;
	}

private:
	std::vector<ListItem_t> vecListItems;

	int iTopIndex;
	int iSelectedIndex;
	int iDisplayPerPage;

	bool bListInFocus;
	bool bOnlyListOnPage;
	bool bScrollingNeeded;
};

typedef struct DropDown_s
{
	int x, y;
	int iCount;

	float *fValues;
	float *cvar;

	char **ppszNames;
} DropDown_t;

class cMenuSection
{
public:
	void Draw(int x, int y, int w, int h, const char *pszSectionName);

	void AddElement(int iType, int add_to_x, const char *pszElementName, const char *pszComment, float *cvar, float min = 0, float max = 1, float step = 1, float fToValue = -1, int increment = 1);

	void DrawAllDropDowns();

	void PostSection()
	{
		DrawAllDropDowns();
	}

	void ClearSection()
	{
		vecDropDowns.clear();

		RestartSection();
	}

	void RestartSection()
	{
		iSection = 0;
	}

	void PostElement(int increment = 1)
	{
		iSection += increment;
	}
	
	void SetISection(int newvalue = 1)
	{
		iSection = newvalue;
	}

	void SetColorDummy(float r, float g, float b, float a)
	{
		fColDummy[0] = r;
		fColDummy[1] = g;
		fColDummy[2] = b;
		fColDummy[3] = a;
	}

	void SetSlider(int &iCurrentSlider, bool bValue = false, bool bColor = false, int iColIdx = -1, bool bSetDummy = false, int r = 255, int g = 255, int b = 255, int a = 255, bool bCham = false, float fLen = 360.0f)
	{
		if(bSetDummy)
			SetColorDummy(r,g,b,a);

		iColorIndex = iColIdx;

		bColored = bColor;
		bShowValue = bValue;
		bChams = bCham;

		fLength = fLen;
		
		iCurrentSlider++;
	}

	void SetSectionPos(int x, int y)
	{
		s_x = x;
		s_y = y;
	}

	void SetValueNames(char **szArrayArray, float *fArray, int iCount)
	{
		ppszValueNames = szArrayArray;
		fValueList = fArray;
		iValueCount = iCount;
	}

	void ClearAllElementSpecifics()
	{
		SetValueNames(NULL,NULL,0);
	}

	//slider specifics
	//----------------------
	float fColDummy[4];
	//----------------------

private:
	void AddDropDownToDrawList(DropDown_t ddNew)
	{
		vecDropDowns.push_back(ddNew);
	}

	int iSection;
	int s_x, s_y;

	//dropdown specifics
	//----------------------
	float *fValueList;
	char **ppszValueNames;
	int iValueCount;
	bool bActiveDropDown[50];
	std::vector<DropDown_t> vecDropDowns;
	//----------------------

	//slider specifics
	//----------------------
	int iColorIndex;

	bool bColored;
	bool bShowValue;
	bool bChams;

	float fLength;
	//----------------------
};

class cMenu
{
public:
	cMenu(int x, int y, int w, int h);

	//this will differ in different games
	void InitMenuElements();

	void DrawMenu();

	void DrawHeaderComment(int e_x, int e_y, const char *pszComment, bool bSlider = false);

	void SetMenuPos(int x, int y)
	{
		m_x = x;
		m_y = y;
	}

	void GetMenuPos(int &out_x, int &out_y)
	{
		out_x = m_x;
		out_y = m_y;
	}

	void GetMenuSize(int &out_w, int &out_h)
	{
		out_w = m_w;
		out_h = m_h;
	}

	bool IsHandlingItem()
	{
		return (iMenuFlags & FL_DISABLEDRAG);
	}

	void AddMenuFlag(int iFlag)
	{
		iMenuFlags |= iFlag;
	}

	void RemoveMenuFlag(int iFlag)
	{
		iMenuFlags &= ~iFlag;
	}

	void HandlingItem()
	{
		AddMenuFlag(FL_DISABLEDRAG);
	}

	void NotHandlingItem()
	{
		RemoveMenuFlag(FL_DISABLEDRAG);
	}

	int GetConfigIndex(int iCfgIdx = 0)
	{
		return iConfigIndex[iCfgIdx];
	}

	void SetConfigIndex(int index, int iCfgIdx = 0)
	{
		iConfigIndex[iCfgIdx] = index;
	}

	const char *GetCurrentConfigName(int iCfgIdx = 0)
	{
		return sCurrentConfigName[iConfigIndex[iCfgIdx]].data();
	}

	std::string GetCurrentConfig(int iCfgIdx = 0)
	{
		return sCurrentConfig[iConfigIndex[iCfgIdx]];
	}

	int GetTabIndex()
	{
		return iTabIndex;
	}

	void SetTabIndex(int index)
	{
		iTabIndex = index;
	}

	int GetSmallTabIndex(int i = 0)
	{
		return iSmallTab[i];
	}

private:
	void DrawTab(int index, int &setindex, int x, int y, const char *pszTitle);
	void DrawSmallTab(int index, int &setindex, int x, int y, const char *pszTitle);
	void DrawAConfig(int index, int cfgidx, int x, int y, std::string sString);

	int m_x, m_y, m_w, m_h;
	int iMenuFlags;
	int iTabIndex;
	int iConfigIndex[2];
	int iSmallTab[5];

	std::string sCurrentConfig[NUM_OF_CFGS];
	std::string sCurrentConfigName[NUM_OF_CFGS];
};

extern cMenu g_Menu;
extern cListItem ListItemArray[NUM_OF_TOTAL_LISTITEMS];