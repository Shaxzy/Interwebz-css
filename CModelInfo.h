namespace ValveSDK
{
	class CModelInfo
	{
	public:
		const char* GetModelName(const model_t* pModel)
		{
			typedef const char* (__thiscall* OriginalFn)(PVOID, const model_t*);
			return getvfunc<OriginalFn>(this, 3)(this, pModel);
		}

		studiohdr_t* GetStudiomodel(const model_t *pModel)
		{
			typedef studiohdr_t* (__thiscall* OriginalFn)(PVOID, const model_t*);
			return getvfunc< OriginalFn >(this, 28)(this, pModel);
		}

		void GetModelMaterials(const model_t *pModel, int count, IMaterial** ppMaterial)
		{
			typedef void (__thiscall* OriginalFn)(PVOID, const model_t*, int, IMaterial**);
			getvfunc< OriginalFn >(this, 16)(this, pModel, count, ppMaterial);
		}
	};
}