void __stdcall hkdCreateMove(int sequence_number, float input_sample_frametime, bool active);
void __stdcall hkdRunCommand(CBaseEntity* pEntity, ValveSDK::CInput::CUserCmd* pUserCmd, void* moveHelper);
void __stdcall hkdFrameStageNotify(ClientFrameStage_t curStage);
void __stdcall hkdPaintTraverse(unsigned int vguiPanel, bool forceRepaint, bool allowForce);
void __stdcall hkdDrawModelExecute(const ValveSDK::DrawModelState_t &state, const ValveSDK::ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld = NULL);
ValveSDK::CInput::CUserCmd* __stdcall hkdGetUserCmd(int sequence_number);
int __stdcall hkdIN_KeyEvent(int eventcode,ValveSDK::ButtonCode_t keynum,const char *pszCurrentBinding);
void __stdcall hkdFinishMove(CBaseEntity *player, ValveSDK::CInput::CUserCmd *ucmd, PVOID move);

extern DWORD dwOriginCreateMove;
extern Base::Utils::CVMTHookManager g_pClientVMT;
extern Base::Utils::CVMTHookManager g_pPanelVMT;
extern Base::Utils::CVMTHookManager g_pPredictionVMT;
extern Base::Utils::CVMTHookManager g_pModelRenderVMT;
extern Base::Utils::CVMTHookManager g_pInputVMT;