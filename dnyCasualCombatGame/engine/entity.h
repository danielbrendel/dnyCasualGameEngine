#pragma once

#include "shared.h"
#include "vars.h"
#include "utils.h"
#include "scriptint.h"

namespace Entity {
	enum DamageType { DAMAGEABLE_NO = 0, DAMAGEABLE_ALL, DAMAGEABLE_NOTSQUAD };

	struct Color {
		Color() {}
		Color(byte cr, byte cg, byte cb, byte ca) : r(cr), g(cg), b(cb), a(ca) {}
		~Color() {}

		byte r, g, b, a;

		byte GetR(void) { return r; }
		byte GetG(void) { return g; }
		byte GetB(void) { return b; }
		byte GetA(void) { return a; }

		//AngelScript interface methods
		void Constr_Bytes(byte cr, byte cg, byte cb, byte ca) { r = cr; g = cg; b = cb; a = ca; }
		void Construct(void* pMemory) { new (pMemory) Color(); }
		void Destruct(void* pMemory) { ((Color*)pMemory)->~Color(); }
	};

	/* Timer utility class */
	class CTimer {
	private:
		bool m_bStarted;
		DWORD m_dwInitial;
		DWORD m_dwCurrent;
		DWORD m_dwDelay;
	public:
		CTimer() {}
		//CTimer() : m_bStarted(false) {}
		CTimer(DWORD dwDelay) { this->SetDelay(dwDelay); this->SetActive(true); }
		~CTimer() {}

		void Reset(void)
		{
			//Reset timer

			this->m_dwCurrent = this->m_dwInitial = GetTickCount();
		}

		void Update(void)
		{
			//Update current value

			this->m_dwCurrent = GetTickCount();
		}

		//Setters
		void SetActive(bool bStatus) { this->m_bStarted = bStatus; }
		void SetDelay(DWORD dwDelay) { this->m_dwDelay = dwDelay; }

		//Getters
		const bool Started(void) const { return this->m_bStarted; }
		const DWORD Delay(void) const { return this->m_dwDelay; }
		const bool Elapsed(void) const { return this->m_dwCurrent >= this->m_dwInitial + this->m_dwDelay; }

		//AngelScript interface methods
		void Constr_Delay(DWORD dwDelay) { this->SetDelay(dwDelay); this->SetActive(true); }
		void Construct(void* pMemory) { new (pMemory) CTimer(); }
		void Destruct(void* pMemory) { ((CTimer*)pMemory)->~CTimer(); }
	};

	/* Entity screen position manager */
	struct Vector {
		//Constructors
		Vector() {}
		Vector(int x, int y) { elem[0] = x; elem[1] = y; }
		Vector(const Vector& ref) { elem[0] = ref.GetX(); elem[1] = ref.GetY(); }

		//Getters
		inline int GetX(void) const { return elem[0]; }
		inline int GetY(void) const { return elem[1]; }

		//Setters
		inline void SetX(int x) { elem[0] = x; }
		inline void SetY(int y) { elem[1] = y; }

		//Overriden operators
		inline int& operator[](int i) { return elem[i]; }
		inline int operator[](int i) const { return elem[i]; }
		inline bool operator==(const Vector& ref) { return (elem[0] == ref[0]) && (elem[1] == ref[1]); }
		inline void operator=(const Vector& ref) { elem[0] = ref.GetX(); elem[1] = ref.GetY(); }
		inline void operator++(int iIndex) { this->elem[iIndex]++; }
		inline void operator--(int iIndex) { this->elem[iIndex]--; }
		inline Vector operator+(const Vector& v) { Vector res; res[0] = this->elem[0] + v[0]; res[1] = this->elem[1] + v[1]; return res; }
		inline Vector operator-(const Vector& v) { Vector res; res[0] = this->elem[0] - v[0]; res[1] = this->elem[1] - v[1]; return res; }
		inline Vector operator*(const Vector& v) { Vector res; res[0] = this->elem[0] * v[0]; res[1] = this->elem[1] * v[1]; return res; }
		inline Vector operator/(const Vector& v) { Vector res; res[0] = this->elem[0] / v[0]; res[1] = this->elem[1] / v[1]; return res; }

		//Utils
		int Distance(const Vector& ref)
		{
			//Get distance from this to reference vector
			Vector vDiff;
			vDiff[0] = ref[0] - elem[0];
			vDiff[1] = ref[1] - elem[1];
			return (int)(sqrt(vDiff[0] * vDiff[0] + vDiff[1] * vDiff[1]));
		}
		void Zero(void)
		{
			//Zero values
			elem[0] = elem[1] = 0;
		}

		//AngelScript interface methods
		void Constr_Class(const Vector& src) { this->SetX(src.GetX()); this->SetY(src.GetY()); }
		void Constr_Ints(int x, int y) { this->SetX(x); this->SetY(y); }
		void Construct(void* pMemory) { new (pMemory) Vector(); }
		void Destruct(void* pMemory) { ((Vector*)pMemory)->~Vector(); }
	private:
		int elem[2];
	};

	/* Sprite info data container */
	struct SpriteInfo {
		SpriteInfo() {}
		SpriteInfo(const Vector& v, int d, int f) : res(v), depth(d), format(f) {}
		~SpriteInfo() {}

		Vector res;
		int depth;
		int format;

		Vector& GetResolution(void) { return res; }
		int GetDepth(void) { return depth; }
		int GetFormat(void) { return format; }

		//AngelScript interface methods
		void Construct(void* pMemory) { new (pMemory) Color(); }
		void Destruct(void* pMemory) { ((Color*)pMemory)->~Color(); }
	};

	/* Bounding Box handler */
	class CBoundingBox {
	private:
		struct bbox_item_s {
			Vector pos;
			Vector size;
		};

		std::vector<bbox_item_s>* m_pvBBoxItems;

		void Release(void)
		{
			//Clear data

			if (!this->m_pvBBoxItems)
				return;

			this->m_pvBBoxItems->clear();

			delete this->m_pvBBoxItems;
		}

		bool IsItemCollided(const Vector& vMyAbsPos, const Vector& vRefAbsPos, const bbox_item_s& rBBoxItem) const
		{
			//Check if bbox item collides with any of own items

			if (!this->m_pvBBoxItems)
				return false;

			for (size_t i = 0; i < this->m_pvBBoxItems->size(); i++) { //Iterate through items
				bbox_item_s item = (*this->m_pvBBoxItems)[i];
				//Check if bbox item is inside current iterated item
				if (((vRefAbsPos[0] + rBBoxItem.pos[0] >= vMyAbsPos[0] + (*this->m_pvBBoxItems)[i].pos[0]) && (vRefAbsPos[0] + rBBoxItem.pos[0] <= vMyAbsPos[0] + (*this->m_pvBBoxItems)[i].pos[0] + (*this->m_pvBBoxItems)[i].size[0])) && ((vRefAbsPos[1] + rBBoxItem.pos[1] >= vMyAbsPos[1] + (*this->m_pvBBoxItems)[i].pos[1]) && (vRefAbsPos[1] + rBBoxItem.pos[1] <= vMyAbsPos[1] + (*this->m_pvBBoxItems)[i].pos[1] + (*this->m_pvBBoxItems)[i].size[1]))) {
					return true;
				}
			}

			return false;
		}
	public:
		CBoundingBox() {}
		~CBoundingBox() { this->Release(); }

		bool Alloc(void)
		{
			//Allocate memory for vector

			this->m_pvBBoxItems = new std::vector<bbox_item_s>;

			return this->m_pvBBoxItems != nullptr;
		}

		void AddBBoxItem(const Vector& pos, const Vector& size)
		{
			//Add bounding box item

			if (!this->m_pvBBoxItems)
				return;

			//Setup data
			bbox_item_s sItem;
			sItem.pos = pos;
			sItem.size = size;

			//Add to list
			this->m_pvBBoxItems->push_back(sItem);
		}

		bool IsCollided(const Vector& vMyAbsPos, const Vector& vRefAbsPos, const CBoundingBox& roBBox)
		{
			//Check if bounding boxes collide with each other

			if (!this->m_pvBBoxItems)
				return false;

			if (roBBox.IsEmpty())
				return false;

			for (size_t i = 0; i < this->m_pvBBoxItems->size(); i++) { //Iterate through own bbox list
				//Check if current item collides ref bbox
				if (roBBox.IsItemCollided(vRefAbsPos, vMyAbsPos, (*this->m_pvBBoxItems)[i]))
					return true;
			}

			return false;
		}

		bool IsInside(const Vector& vMyAbsPos, const Vector& vPosition)
		{
			//Check if position is inside bbox

			if (!this->m_pvBBoxItems)
				return false;

			if (this->IsEmpty())
				return false;

			for (size_t i = 0; i < this->m_pvBBoxItems->size(); i++) { //Loop through all bbox i tems
				//Check if position is inside
				if ((vPosition[0] >= vMyAbsPos[0] + (*this->m_pvBBoxItems)[i].pos[0]) && (vPosition[0] <= vMyAbsPos[0] + (*this->m_pvBBoxItems)[i].pos[0] + (*this->m_pvBBoxItems)[i].size[0]) && (vPosition[1] >= vMyAbsPos[1] + (*this->m_pvBBoxItems)[i].pos[1]) && (vPosition[1] <= vMyAbsPos[1] + (*this->m_pvBBoxItems)[i].pos[1] + (*this->m_pvBBoxItems)[i].size[1])) {
					return true;
				}
			}

			return false;
		}

		inline bool IsEmpty(void) const { return (this->m_pvBBoxItems) ? (this->m_pvBBoxItems->size() == 0) : true; }
		inline void Clear(void) { if (this->m_pvBBoxItems) this->m_pvBBoxItems->clear(); }
		inline size_t Count(void) const { return this->m_pvBBoxItems->size(); }
		inline const bbox_item_s& Item(const size_t uiItem) const { return (*this->m_pvBBoxItems)[uiItem]; }

		void operator=(const CBoundingBox& ref)
		{
			//Copy data from other bbox

			this->m_pvBBoxItems->clear();

			for (size_t i = 0; i < ref.Count(); i++) {
				this->m_pvBBoxItems->push_back(ref.Item(i));
			}
		}

		//AngelScript interface methods
		void Construct(void* pMemory) { new (pMemory) CBoundingBox(); }
		void Destruct(void* pMemory) { ((CBoundingBox*)pMemory)->~CBoundingBox(); }
	};

	/* Sprite component */
	class CTempSprite {
	private:
		bool m_bInfinite;
		CTimer m_oTimer;
		DxRenderer::HD3DSPRITE m_hSprite;
		int m_iCurrentFrame;
		int m_iMaxFrames;
	public:
		CTempSprite() {}
		CTempSprite(const std::string& szTexture, DWORD dwSwitchDelay, bool bInfinite, int iFrameCount, int iFrameWidth, int iFrameHeight, int iFramesPerLine, const bool bForceCustomSize) { this->Initialize(szTexture, dwSwitchDelay, bInfinite, iFrameCount, iFrameWidth, iFrameHeight, iFramesPerLine, bForceCustomSize); }
		~CTempSprite() {}

		bool Initialize(const std::string& szTexture, DWORD dwSwitchDelay, bool bInfinite, int iFrameCount, int iFrameWidth, int iFrameHeight, int iFramesPerLine, const bool bForceCustomSize)
		{
			//Initialize sprite object

			//Load sprite
			this->m_hSprite = pRenderer->LoadSprite(Utils::ConvertToWideString(szTexture), iFrameCount, iFrameWidth, iFrameHeight, iFramesPerLine, bForceCustomSize);
			if (this->m_hSprite == GFX_INVALID_SPRITE_ID)
				return false;

			//Initialize timer
			this->m_oTimer = CTimer(dwSwitchDelay);

			//Store indicator
			this->m_bInfinite = bInfinite;

			//Init frame values
			this->m_iCurrentFrame = 0;
			this->m_iMaxFrames = iFrameCount;

			return true;
		}

		void Draw(const Vector& pos, float fRotation, float fScale1, float fScale2)
		{
			//Draw sprite items

			if (this->m_hSprite == GFX_INVALID_SPRITE_ID)
				return;

			//Draw current sprite frame
			pRenderer->DrawSprite(this->m_hSprite, pos[0], pos[1], this->m_iCurrentFrame, fRotation, fScale1, fScale2);

			//Update timer
			this->m_oTimer.Update();

			//Handle frame change
			if (this->m_oTimer.Elapsed()) {
				this->m_iCurrentFrame++;
				if (this->m_iCurrentFrame >= this->m_iMaxFrames) {
					this->m_iCurrentFrame = 0;

					if (!this->m_bInfinite) {
						this->Release();
					}
				}
			}
		}

		void Release(void)
		{
			//Release resources

			if (this->m_hSprite == GFX_INVALID_SPRITE_ID)
				return;

			//Free sprite
			pRenderer->FreeSprite(this->m_hSprite);

			//Stop timer
			this->m_oTimer.SetActive(false);

			//Clear value
			this->m_hSprite = GFX_INVALID_SPRITE_ID;
		}

		//AngelScript interface methods
		void Constr_Init(const std::string& szTexture, DWORD dwSwitchDelay, bool bInfinite, int iFrameCount, int iFrameWidth, int iFrameHeight, int iFramesPerLine, const bool bForceCustomSize) { this->Initialize(szTexture, dwSwitchDelay, bInfinite, iFrameCount, iFrameWidth, iFrameHeight, iFramesPerLine, bForceCustomSize); }
		void Construct(void* pMemory) { new (pMemory) CTempSprite(); }
		void Destruct(void* pMemory) { ((CTempSprite*)pMemory)->~CTempSprite(); }
	};

	/* Model component */
	class CModel {
	private:
		bool m_bReady;
		DxRenderer::HD3DSPRITE m_hSprite;
		CBoundingBox m_oBBox;
		Vector m_vCenter;
	public:
		CModel() {}
		//CModel(const std::string& szInputFile) { this->Initialize(szInputFile); }
		//CModel(const std::string& szInputFile, bool bForceCustomSize) { this->Initialize(szInputFile, bForceCustomSize); }
		~CModel() { this->Release(); }

		bool Initialize(const std::string& szMdlFile, bool bForceCustomSize = false)
		{
			//Initialize model by using source file as input

			std::ifstream hFile;
			std::string szSpriteFile;
			int iaFrameInfos[4];

			//Allocate bounding box
			//if (!this->m_oBBox.Alloc())
			//	return false;

			//Attempt to open file for reading
			hFile.open(szMdlFile, std::ifstream::in);
			if (!hFile.is_open())
				return false;

			char szCurLine[MAX_PATH];

			//Read first line to get the model sprite file
			hFile.getline(szCurLine, sizeof(szCurLine), '\n');
			szSpriteFile = szCurLine;

			//Read second line to get the sprite infos
			hFile.getline(szCurLine, sizeof(szCurLine), '\n');
			std::vector<std::string> vSplit = Utils::Split(szCurLine, " ");
			if (vSplit.size() != 4) { hFile.close(); return false; }
			iaFrameInfos[0] = atoi(vSplit[0].c_str());
			iaFrameInfos[1] = atoi(vSplit[1].c_str());
			iaFrameInfos[2] = atoi(vSplit[2].c_str());
			iaFrameInfos[3] = atoi(vSplit[3].c_str());

			//Read remaining lines to get the bbox infos
			while (!hFile.eof()) {
				hFile.getline(szCurLine, sizeof(szCurLine), '\n');

				Vector vPos, vSize;

				vSplit = Utils::Split(szCurLine, " ");
				if (vSplit.size() != 4) { hFile.close(); return false; }
				vPos[0] = atoi(vSplit[0].c_str());
				vPos[1] = atoi(vSplit[1].c_str());
				vSize[0] = atoi(vSplit[2].c_str());
				vSize[1] = atoi(vSplit[3].c_str());

				//Add to bbox
				this->m_oBBox.AddBBoxItem(vPos, vSize);
			}

			//Close file
			hFile.close();

			//Load sprite
			this->m_hSprite = pRenderer->LoadSprite(Utils::ConvertToWideString(szSpriteFile), iaFrameInfos[0], iaFrameInfos[1], iaFrameInfos[2], iaFrameInfos[3], bForceCustomSize);
			if (!this->m_hSprite)
				return false;

			//Return set indicator
			return this->m_bReady = true;
		}

		bool Initialize2(const CBoundingBox& bbox, const DxRenderer::HD3DSPRITE hSprite)
		{
			//Initialize model by using pre-initialized data objects

			//Copy data
			this->m_hSprite = hSprite;
			this->m_oBBox = bbox;

			//Return set indicator
			return this->m_bReady = true;
		}

		void Release(void)
		{
			//Release resources

			if (!this->m_bReady)
				return;

			//Free sprite
			pRenderer->FreeSprite(this->m_hSprite);

			//Clear bbox data
			this->m_oBBox.Clear();

			//Clear indicator
			this->m_bReady = false;
		}

		bool IsCollided(const Vector& vMyAbsPos, const Vector& vRefAbsPos, const CModel& rRefMdl)
		{
			//Check for collision

			return this->m_oBBox.IsCollided(vMyAbsPos, vRefAbsPos, rRefMdl.BBox());
		}

		bool IsInside(const Vector& vMyAbsPos, const Vector& vPosition)
		{
			//Check if inside

			return this->m_oBBox.IsInside(vMyAbsPos, vPosition);
		}

		inline bool Alloc(void) { return this->m_oBBox.Alloc(); }
		inline void SetCenter(const Vector& vCenter) { this->m_vCenter = vCenter; }

		//Getters
		inline bool IsValid(void) const { return this->m_bReady; }
		inline const CBoundingBox& BBox(void) const { return this->m_oBBox; }
		inline const DxRenderer::HD3DSPRITE Sprite(void) const { return this->m_hSprite; }
		inline const Vector& Center(void) const { return this->m_vCenter; }

		//Assign operator
		void operator=(const CModel& ref)
		{
			this->m_hSprite = ref.Sprite();
			this->m_oBBox = ref.BBox();
			this->m_vCenter = ref.Center();
		}

		//AngelScript interface methods
		void Constr_IntputStr(const std::string& szInputFile) { this->Initialize(szInputFile); }
		void Constr_IntputStrBool(const std::string& szInputFile, bool bForceCustomSize) { this->Initialize(szInputFile, bForceCustomSize); }
		void Construct(void* pMemory) { new (pMemory) CModel(); }
		void Destruct(void* pMemory) { ((CModel*)pMemory)->~CModel(); }
	};

	/* Managed entity component */
	class CScriptedEntity {
	public:
		typedef byte DamageValue;
	private:
		std::string m_szClassName;
		Scripting::HSISCRIPT m_hScript;
		asIScriptObject* m_pScriptObject;

		void Release(void)
		{
			//Release resources

			this->m_pScriptObject->Release();
			this->m_pScriptObject = nullptr;
		}
	public:
		CScriptedEntity(const Scripting::HSISCRIPT hScript, asIScriptObject* pObject) : m_pScriptObject(pObject), m_hScript(hScript) {}
		CScriptedEntity(const Scripting::HSISCRIPT hScript, const std::string& szClassName) : m_szClassName(szClassName) { this->Initialize(hScript, szClassName); }
		~CScriptedEntity() { this->Release(); }

		bool Initialize(const Scripting::HSISCRIPT hScript, const std::string& szClassName)
		{
			//Initialize scripted entity

			if (!szClassName.length())
				return false;

			//Allocate class instance
			this->m_pScriptObject = pScriptingInt->AllocClass(hScript, szClassName);

			//Store data
			this->m_hScript = hScript;

			return this->m_pScriptObject != nullptr;
		}

		void OnSpawn(const Vector& vAtPos)
		{
			//Inform class instance of event

			Vector v(vAtPos);

			BEGIN_PARAMS(vArgs);
			PUSH_OBJECT(&v);

			pScriptingInt->CallScriptMethod(this->m_hScript, this->m_pScriptObject, "void OnSpawn(const Vector& in)", &vArgs, nullptr);

			END_PARAMS(vArgs);
		}

		void OnRelease(void)
		{
			//Inform class instance of event

			pScriptingInt->CallScriptMethod(this->m_hScript, this->m_pScriptObject, "void OnRelease()", nullptr, nullptr);
		}

		void OnProcess(void)
		{
			//Inform class instance of event

			pScriptingInt->CallScriptMethod(this->m_hScript, this->m_pScriptObject, "void OnProcess()", nullptr, nullptr);
		}

		void OnDraw(void)
		{
			//Inform class instance of event

			pScriptingInt->CallScriptMethod(this->m_hScript, this->m_pScriptObject, "void OnDraw()", nullptr, nullptr);
		}

		void OnDrawOnTop(void)
		{
			//Inform class instance of event

			pScriptingInt->CallScriptMethod(this->m_hScript, this->m_pScriptObject, "void OnDrawOnTop()", nullptr, nullptr);
		}

		byte IsDamageable(void)
		{
			//Query if entity is damageable (0 = non-damageable, 1 = damage all, 2 = damage only entities with different name)

			byte ucResult;

			pScriptingInt->CallScriptMethod(this->m_hScript, this->m_pScriptObject, "DamageType IsDamageable()", nullptr, &ucResult, Scripting::FA_BYTE);

			return ucResult;
		}

		void OnDamage(DamageValue dv)
		{
			//Inform of being damaged

			BEGIN_PARAMS(vArgs);
			PUSH_BYTE(dv);

			pScriptingInt->CallScriptMethod(this->m_hScript, this->m_pScriptObject, "void OnDamage(DamageValue dv)", &vArgs, nullptr);

			END_PARAMS(vArgs);
		}

		CModel* GetModel(void)
		{
			//Query model object pointer

			CModel* pResult;

			pScriptingInt->CallScriptMethod(this->m_hScript, this->m_pScriptObject, "Model& GetModel()", nullptr, &pResult, Scripting::FA_OBJECT);

			return pResult;
		}

		Vector GetPosition(void)
		{
			//Query position

			Vector* pResult;

			pScriptingInt->CallScriptMethod(this->m_hScript, this->m_pScriptObject, "Vector& GetPosition()", nullptr, &pResult, Scripting::FA_OBJECT);

			return *pResult;
		}

		float GetRotation(void)
		{
			//Query rotation

			float flResult;

			pScriptingInt->CallScriptMethod(this->m_hScript, this->m_pScriptObject, "float GetRotation()", nullptr, &flResult, Scripting::FA_FLOAT);

			return flResult;
		}

		bool IsMovable(void)
		{
			//Query movable indicator

			bool bResult;

			pScriptingInt->CallScriptMethod(this->m_hScript, this->m_pScriptObject, "bool IsMovable()", nullptr, &bResult, Scripting::FA_BYTE);

			return bResult;
		}

		Vector GetSelectionSize(void)
		{
			//Query selection size

			Vector* pResult;

			pScriptingInt->CallScriptMethod(this->m_hScript, this->m_pScriptObject, "Vector& GetSelectionSize()", nullptr, &pResult, Scripting::FA_OBJECT);

			return *pResult;
		}

		DamageValue GetDamageValue(void)
		{
			//Get damage value

			DamageValue dvResult;

			pScriptingInt->CallScriptMethod(this->m_hScript, this->m_pScriptObject, "DamageValue GetDamageValue()", nullptr, &dvResult, Scripting::FA_BYTE);

			return dvResult;
		}

		bool NeedsRemoval(void)
		{
			//Inform class instance of event

			bool bResult;

			pScriptingInt->CallScriptMethod(this->m_hScript, this->m_pScriptObject, "bool NeedsRemoval()", nullptr, &bResult, Scripting::FA_BYTE);

			return bResult;
		}

		std::string GetName(void)
		{
			//Get name from class instance

			std::string szResult;

			pScriptingInt->CallScriptMethod(this->m_hScript, this->m_pScriptObject, "string GetName()", nullptr, &szResult, Scripting::FA_STRING);

			return szResult;
		}

		//Getters
		inline bool IsReady(void) const { return (this->m_pScriptObject != nullptr); }
		inline asIScriptObject* Object(void) const { return this->m_pScriptObject; }
	};

	/* Scripted entity manager */
	class CScriptedEntsMgr {
	public:
		struct playerentity_s {
			Scripting::HSISCRIPT hScript;
			asIScriptObject* pObject;
		};
	private:
		std::vector<CScriptedEntity*> m_vEnts;
		playerentity_s m_sPlayerEntity;

		void Release(void)
		{
			//Release resources

			for (size_t i = 0; i < this->m_vEnts.size(); i++) {
				this->m_vEnts[i]->OnRelease();
				delete this->m_vEnts[i];
			}

			this->m_vEnts.clear();
		}
	public:
		CScriptedEntsMgr() {}
		~CScriptedEntsMgr() { this->Release(); }

		bool Spawn(const std::wstring& wszIdent, asIScriptObject* pObject, const Vector& vAtPos);

		void Process(void)
		{
			//Inform entities

			for (size_t i = 0; i < this->m_vEnts.size(); i++) {
				//Let entity process
				this->m_vEnts[i]->OnProcess();

				//Handle damaging
				byte ucDamageType = this->m_vEnts[i]->IsDamageable(); //Query damage type
				if (ucDamageType) {
					//Query model
					CModel* pModel = this->m_vEnts[i]->GetModel();
					if (pModel) {
						//Check for collisions with any other entities
						for (size_t j = 0; j < this->m_vEnts.size(); j++) {
							if ((j != i) && (this->m_vEnts[j]->IsDamageable())) {
								//Check if entity does not damage other entities with the same name and ignore damage handling then
								if (ucDamageType == DAMAGEABLE_NOTSQUAD) {
									if (this->m_vEnts[i]->GetName() == this->m_vEnts[j]->GetName())
										continue;
								}

								CModel* pRef = this->m_vEnts[j]->GetModel();
								if (pRef) {
									//Check if collided
									if (pModel->IsCollided(this->m_vEnts[i]->GetPosition(), this->m_vEnts[j]->GetPosition(), *pRef)) {
										//Inform both of being damaged
										this->m_vEnts[i]->OnDamage(this->m_vEnts[j]->GetDamageValue());
										this->m_vEnts[j]->OnDamage(this->m_vEnts[i]->GetDamageValue());
									}
								}
							}
						}
					}
				}

				//Check for removal
				if (this->m_vEnts[i]->NeedsRemoval()) {
					this->m_vEnts[i]->OnRelease();
					delete this->m_vEnts[i];
					this->m_vEnts.erase(this->m_vEnts.begin() + i);
				}
			}
		}

		void Draw(void)
		{
			//Inform entities

			for (size_t i = 0; i < this->m_vEnts.size(); i++) {
				this->m_vEnts[i]->OnDraw();
			}
		}

		void DrawOnTop(void)
		{
			//Inform entities

			for (size_t i = 0; i < this->m_vEnts.size(); i++) {
				this->m_vEnts[i]->OnDrawOnTop();
			}
		}

		//Entity querying
		size_t GetEntityCount() { return this->m_vEnts.size(); }
		CScriptedEntity* GetEntity(size_t uiEntityId)
		{
			if (uiEntityId >= this->m_vEnts.size())
				return nullptr;

			return this->m_vEnts[uiEntityId];
		}
		asIScriptObject* GetEntityHandle(size_t uiEntityId)
		{
			if (uiEntityId >= this->m_vEnts.size())
				return nullptr;

			if (!this->m_vEnts[uiEntityId])
				return nullptr;

			return this->m_vEnts[uiEntityId]->Object();
		}
		bool IsValidEntity(asIScriptObject* pEntity)
		{
			for (size_t i = 0; i < this->m_vEnts.size(); i++) {
				if (this->m_vEnts[i]->Object() == pEntity)
					return true;
			}

			return false;
		}
		size_t GetEntityId(asIScriptObject* pEntity)
		{
			for (size_t i = 0; i < this->m_vEnts.size(); i++) {
				if (this->m_vEnts[i]->Object() == pEntity)
					return true;
			}

			return (size_t)-1;
		}
		const playerentity_s& GetPlayerEntity(void) const { return this->m_sPlayerEntity; }
	};

	extern CScriptedEntsMgr oScriptedEntMgr;

	class CEntityTrace { //Entity tracing utility class
	private:
		struct tracedata_s {
			Vector vStart;
			Vector vEnd;
			asIScriptObject* pIgnoreEnt;
		};

		std::vector<CScriptedEntity*> m_vEntities;
		tracedata_s m_sTraceData;

		void IncreaseVec(Vector& vVector)
		{
			//Increase vector value to move to the next position in direction to the end position

			if (vVector[0] != this->m_sTraceData.vEnd[0]) { (this->m_sTraceData.vEnd[0] - vVector[0] > 0) ? vVector[0]++ : vVector[0]--; }
			if (vVector[1] != this->m_sTraceData.vEnd[1]) { (this->m_sTraceData.vEnd[1] - vVector[1] > 0) ? vVector[1]++ : vVector[1]--; }
		}

		bool EndReached(const Vector& vCurPos)
		{
			//Indicate whether end has been reached

			return (vCurPos[0] == this->m_sTraceData.vEnd[0]) && (vCurPos[1] == this->m_sTraceData.vEnd[1]);
		}

		void EnumerateEntitiesAtPosition(const Vector& vPosition, asIScriptObject* pIgnoreEnt)
		{
			//Create a list with all entities at the given position

			//Clear list
			this->m_vEntities.clear();

			for (size_t i = 0; i < oScriptedEntMgr.GetEntityCount(); i++) { //Loop through all entities
				CScriptedEntity* pEntity = oScriptedEntMgr.GetEntity(i); //Get entity pointer
				if ((pEntity) && (pEntity->Object() != pIgnoreEnt) && (pEntity->IsDamageable())) { //Use only valid damageable entities
					//Obtain model pointer
					CModel* pModel = pEntity->GetModel();
					if (pModel) {
						if (pModel->IsInside(pEntity->GetPosition(), vPosition)) { //Check if position is inside
							//Add to list
							this->m_vEntities.push_back(pEntity);
						}
					}
				}
			}
		}
	public:
		CEntityTrace() {}
		CEntityTrace(const Vector& vStart, const Vector& vEnd, asIScriptObject* pIgnoreEnt)
		{
			this->SetStart(vStart);
			this->SetEnd(vEnd);
			this->SetIgnoredEnt(pIgnoreEnt);
			this->Run();
		}
		~CEntityTrace() { this->m_vEntities.clear(); }

		//Action
		void Run(void)
		{
			//Run trace computation

			//Initialize with start position
			Vector vCurrentPosition = this->m_sTraceData.vStart;

			while (!this->EndReached(vCurrentPosition)) { //Loop as long as end position has not yet been reached
				//Enumerate all entities at current position
				this->EnumerateEntitiesAtPosition(vCurrentPosition, this->m_sTraceData.pIgnoreEnt);
				if (!this->IsEmpty()) { //Break out if at least one entity has been found
					break;
				}

				//Update current position
				this->IncreaseVec(vCurrentPosition);
			}
		}

		//Setters
		inline void SetStart(const Vector& vVector) { this->m_sTraceData.vStart = vVector; }
		inline void SetEnd(const Vector& vVector) { this->m_sTraceData.vEnd = vVector; }
		inline void SetIgnoredEnt(asIScriptObject* pIgnoreEnt) { this->m_sTraceData.pIgnoreEnt = pIgnoreEnt; }

		//Getters
		inline const bool IsEmpty(void) const { return this->m_vEntities.size() == 0; }
		inline asIScriptObject* EntityObject(const size_t uiId) { if (uiId >= this->m_vEntities.size()) return nullptr; return this->m_vEntities[uiId]->Object(); }
		inline const size_t EntityCount(void) const { return this->m_vEntities.size(); }
	};

	/* Solid sprite class */
	class CSolidSprite {
	private:
		std::vector<DxRenderer::HD3DSPRITE> m_vSprites;
		Vector m_vecPos;
		Vector m_vecSize;
		int m_iDir;
		float m_fRotation;
	public:
		CSolidSprite() {}
		~CSolidSprite() {}

		bool Initialize(int x, int y, int w, int h, const std::wstring& wszFile, int repeat, int dir, float rot)
		{
			//Initialize entity

			if (!repeat) {
				repeat = 1;
			}

			this->m_vecPos = Vector(x, y);
			this->m_vecSize = Vector(w, h);
			this->m_iDir = dir;
			this->m_fRotation = rot;

			for (size_t i = 0; i < repeat; i++) {
				DxRenderer::HD3DSPRITE hSprite = pRenderer->LoadSprite(wszFile, 1, w, h, 1, false);
				if (!hSprite) {
					return false;
				}

				this->m_vSprites.push_back(hSprite);
			}

			return true;
		}

		void Draw(void);

		void Release(void)
		{
			//Free resources

			for (size_t i = 0; i < this->m_vSprites.size(); i++) {
				pRenderer->FreeSprite(this->m_vSprites[i]);
			}

			this->m_vSprites.clear();
		}
	};

	/* File reader class */
	class CFileReader {
	public:
		enum SeekWay_e {
			SEEKW_BEGIN = std::ios_base::beg,
			SEEKW_CURRENT = std::ios_base::cur,
			SEEKW_END = std::ios_base::end
		};
	private:
		std::ifstream* m_poFile;

		void Release(void)
		{
			//Release resources

			if (this->m_poFile) {
				if (this->m_poFile->is_open()) {
					this->m_poFile->close();
				}

				delete this->m_poFile;
				this->m_poFile = nullptr;
			}
		}
	public:
		CFileReader() {}
		~CFileReader() { /*this->Release();*/ }

		bool Open(const std::string& szFile)
		{
			//Allocate object and open stream to file

			//if (this->m_poFile)
			//	return false;

			this->m_poFile = new std::ifstream();
			if (!this->m_poFile)
				return false;

			this->m_poFile->open(Utils::ConvertToAnsiString(wszBasePath) + "tools\\" + szFile, std::ifstream::in);

			return this->m_poFile->is_open();
		}

		bool IsOpen(void)
		{
			//Indicate status

			if (!this->m_poFile)
				return false;

			return this->m_poFile->is_open();
		}

		bool Eof(void)
		{
			//Indicate status

			if (!this->m_poFile)
				return true;

			return this->m_poFile->eof();
		}

		void Seek(int from, int offset)
		{
			//Set file pointer

			if (!this->m_poFile)
				return;

			this->m_poFile->seekg(offset, from);
		}

		std::string GetLine(void)
		{
			//Return current line content

			if (!this->m_poFile)
				return "";

			char szLineBuffer[1024 * 4] = { 0 };

			this->m_poFile->getline(szLineBuffer, sizeof(szLineBuffer), '\n');

			return std::string(szLineBuffer);
		}

		std::string GetEntireContent(bool bSkipNLChar)
		{
			//Return entire file content

			if (!this->m_poFile)
				return "";

			std::string szAllLines;
			this->m_poFile->seekg(0, std::ios_base::beg);
			while (!this->m_poFile->eof()) {
				szAllLines += this->GetLine() + ((bSkipNLChar) ? "" : "\n");
			}

			return szAllLines;
		}

		void Close(void)
		{
			//Close stream and free memory

			this->Release();
		}

		//AngelScript interface methods
		void Constr_Init(const std::string& szFile) { this->Open(szFile); }
		void Construct(void* pMemory) { new (pMemory) CFileReader(); }
		void Destruct(void* pMemory) { ((CFileReader*)pMemory)->~CFileReader(); }
	};

	/* File writer class */
	class CFileWriter {
	public:
		enum SeekWay_e {
			SEEKW_BEGIN = std::ios_base::beg,
			SEEKW_CURRENT = std::ios_base::cur,
			SEEKW_END = std::ios_base::end
		};
	private:
		std::ofstream* m_poFile;

		void Release(void)
		{
			//Release resources

			if (this->m_poFile) {
				if (this->m_poFile->is_open()) {
					this->m_poFile->close();
				}

				delete this->m_poFile;
				this->m_poFile = nullptr;
			}
		}
	public:
		CFileWriter() {}
		~CFileWriter() { /*this->Release();*/ }

		bool Open(const std::string& szFile, bool bAppend = true)
		{
			//Allocate object and open stream to file

			//if (this->m_poFile)
			//	return false;

			this->m_poFile = new std::ofstream();
			if (!this->m_poFile)
				return false;

			int openMode = std::ifstream::out;
			if (bAppend) openMode |= std::ifstream::app;

			this->m_poFile->open(Utils::ConvertToAnsiString(wszBasePath) + "tools\\" + szFile, openMode);

			return this->m_poFile->is_open();
		}

		bool IsOpen(void)
		{
			//Indicate status

			if (!this->m_poFile)
				return false;

			return this->m_poFile->is_open();
		}

		bool Eof(void)
		{
			//Indicate status

			if (!this->m_poFile)
				return true;

			return this->m_poFile->eof();
		}

		void Seek(int from, int offset)
		{
			//Set file pointer

			if (!this->m_poFile)
				return;

			this->m_poFile->seekp(offset, from);
		}

		void Write(const std::string& szText)
		{
			//Write text to file

			if (!this->m_poFile)
				return;

			this->m_poFile->write(szText.data(), szText.length());
		}

		void WriteLine(const std::string& szText)
		{
			//Write text line to file

			this->Write(szText + "\n");
		}

		void Close(void)
		{
			//Close stream and free memory

			this->Release();
		}

		//AngelScript interface methods
		void Constr_Init(const std::string& szFile) { this->Open(szFile); }
		void Construct(void* pMemory) { new (pMemory) CFileWriter(); }
		void Destruct(void* pMemory) { ((CFileWriter*)pMemory)->~CFileWriter(); }
	};

	bool Initialize(void);
}