extern void FullCham( ValveSDK::IMaterial *pMat, const ValveSDK::ModelRenderInfo_t &pInfo, int r, int g, int b, int r2, int g2, int b2, bool bDeadIgnorez = false);
extern ValveSDK::IMaterial *CreateMaterial(BOOL bFullbright, BOOL bIgnorez);
extern void GenerateRandomString(char *s, const int len);
extern void ChamModel( float r, float g, float b, ValveSDK::IMaterial *pMat);

extern bool bMenu;
extern bool bGetMaterials;