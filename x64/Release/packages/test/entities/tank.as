
string g_szPackagePath = "";

#include "weapon_gun.as"

class CTankEntity : IScriptedEntity
{
	Vector m_vecPos;
	Vector m_vecSize;
	float m_fRotation;
	float m_fHeadRot;
	Model m_oModel;
	SpriteHandle m_hBody;
	SpriteHandle m_hHead;
	SpriteHandle m_hMuzzle;
	Timer m_tmrSpriteChange;
	Timer m_tmrAttack;
	Timer m_tmrMuzzle;
	SoundHandle m_hFireSound;
	
	CTankEntity()
    {
		this.m_vecSize = Vector(64, 64);
    }
	
	void AimTo(const Vector& in vecPos)
	{
		//Init aiming process
		
		//Calculate aim rotation
		float flAngle = atan2(float(vecPos[1] - this.m_vecPos[1]), float(vecPos[0] - this.m_vecPos[0]));
		this.m_fHeadRot = flAngle + 6.30 / 4;
		
		//Activate attack timing if not already
		if (!this.m_tmrAttack.IsActive()) {
			this.m_tmrAttack.SetDelay(Util_Random(1000, 2000));
			this.m_tmrAttack.Reset();
			this.m_tmrAttack.SetActive(true);
		}
	}
	
	void Fire()
	{
		//Fire at target
		
		IScriptedEntity@ player = Ent_GetPlayerEntity();
		if (!Ent_IsValid(@player)) 
			return;
		
		//Play fire sound
		S_PlaySound(this.m_hFireSound, 10);
		
		//Spawn shot
		CGunEntity@ gun = CGunEntity();
		Ent_SpawnEntity("weapon_gun", @gun, this.m_vecPos);
		gun.SetRotation(this.m_fHeadRot);
	}
	
	//Called when the entity gets spawned. The position on the screen is passed as argument
	void OnSpawn(const Vector& in vec)
	{
		this.m_vecPos = vec;
		this.m_fRotation = 0.0f;
		this.m_fHeadRot = 0.0f;
		this.m_hBody = R_LoadSprite(g_szPackagePath + "gfx\\tank_body.png", 4, 42, 49, 4, true);
		this.m_hHead = R_LoadSprite(g_szPackagePath + "gfx\\tank_head.png", 4, 65, 65, 4, true);
		this.m_hMuzzle = R_LoadSprite(g_szPackagePath + "gfx\\muzzle.png", 1, 256, 256, 1, false);
		this.m_hFireSound = S_QuerySound(g_szPackagePath + "sound\\tank_fire.wav");
		this.m_tmrSpriteChange.SetDelay(50);
		this.m_tmrSpriteChange.Reset();
		this.m_tmrSpriteChange.SetActive(true);
		this.m_tmrAttack.SetDelay(1000);
		this.m_tmrAttack.Reset();
		this.m_tmrAttack.SetActive(false);
		this.m_tmrMuzzle.SetActive(false);
		BoundingBox bbox;
		bbox.Alloc();
		bbox.AddBBoxItem(Vector(0, 0), this.m_vecSize);
		this.m_oModel.Alloc();
		this.m_oModel.Initialize2(bbox, this.m_hBody);
	}
	
	//Called when the entity gets released
	void OnRelease()
	{
	}
	
	//Process entity stuff
	void OnProcess()
	{
		this.m_tmrSpriteChange.Update();
		if (this.m_tmrSpriteChange.IsElapsed()) {
			this.m_tmrSpriteChange.Reset();
			
			Ent_Move(this, 15, MOVE_FORWARD);
		}
		
		//Process target search
		const int MAX_CHECK_RANGE = 500;
		IScriptedEntity@ player = Ent_GetPlayerEntity();
		if (Ent_IsValid(@player)) {
			if ((this.m_vecPos.Distance(player.GetPosition()) < MAX_CHECK_RANGE)) {
				this.AimTo(player.GetPosition());
			} else {
				this.m_tmrAttack.SetActive(false);
			}
		}
		
		//Process attacking
		if (this.m_tmrAttack.IsActive()) {
			this.m_tmrAttack.Update();
			if (this.m_tmrAttack.IsElapsed()) {
				this.m_tmrAttack.Reset();
				this.Fire();
				this.m_tmrMuzzle.SetDelay(10);
				this.m_tmrMuzzle.Reset();
				this.m_tmrMuzzle.SetActive(true);
			}
		}
		
		//Process muzzle flash
		if (this.m_tmrMuzzle.IsActive()) {
			this.m_tmrMuzzle.Update();
			if (this.m_tmrMuzzle.IsElapsed()) {
				this.m_tmrMuzzle.SetActive(false);
			}
		}
	}
	
	//Entity can draw everything in default order here
	void OnDraw()
	{
	}
	
	//Called for wall collisions
	void OnWallCollided()
	{
		if (this.m_fRotation == 0.0f) {
			this.m_fRotation = 3.30f;
		} else {
			this.m_fRotation = 0.0f;
		}
	}
	
	//Entity can draw on-top stuff here
	void OnDrawOnTop()
	{
		if (!R_ShouldDraw(this.m_vecPos, this.m_vecSize))
			return;
			
		Vector vOut;
		R_GetDrawingPosition(this.m_vecPos, this.m_vecSize, vOut);
		
		R_DrawSprite(this.m_hBody, vOut, 0, 0.0f, Vector(-1, -1), 0.0f, 0.0f, false, Color(0, 0, 0, 0));
		R_DrawSprite(this.m_hHead, Vector(vOut[0] - 11, vOut[1] - 3), 0, this.m_fHeadRot, Vector(-1, -1), 0.0, 0.0, false, Color(0, 0, 0, 0));
		
		if (this.m_tmrMuzzle.IsActive()) {
			Vector vecCenter = this.GetModel().GetCenter();
			Vector vecMuzzlePos(this.m_vecPos[0] + vecCenter[0] - 128, this.m_vecPos[1] + vecCenter[1] - 128);
			vecMuzzlePos[0] += int(sin(this.m_fHeadRot) * (65 + 25));
			vecMuzzlePos[1] -= int(cos(this.m_fHeadRot) * (65 + 25));
			
			Vector vMuzzleOut;
			R_GetDrawingPosition(vecMuzzlePos, this.m_vecSize, vMuzzleOut);
			
			R_DrawSprite(this.m_hMuzzle, vMuzzleOut, 0, this.m_fHeadRot, Vector(-1, -1), 0.0, 0.0, false, Color(0, 0, 0, 0));
		}
	}
	
	//Indicate whether this entity shall be removed by the game
	bool NeedsRemoval()
	{
		return false;
	}
	
	//Indicate whether this entity is collidable
	bool IsCollidable()
	{
		return true;
	}
	
	//Called when the entity collided with another entity
	void OnCollided(IScriptedEntity@ ref)
	{
	}
	
	//Called when entity gets damaged
	void OnDamage(uint32 damageValue)
	{
	}
	
	//Called for recieving the model data for this entity. This is only used for
	//damageable entities. 
	Model& GetModel()
	{
		return this.m_oModel;
	}
	
	//Called for recieving the current position. This is useful if the entity shall move.
	Vector& GetPosition()
	{
		return this.m_vecPos;
	}
	
	//Set new position
	void SetPosition(const Vector &in vecPos)
	{
		this.m_vecPos = vecPos;
	}
	
	//This vector is used for getting the overall drawing size
	Vector& GetSize()
	{
		return this.m_vecSize;
	}
	
	//Return the rotation. This is actually not used by the host application, but might be useful to other entities
	float GetRotation()
	{
		return this.m_fRotation;
	}
	
	//Set new rotation
	void SetRotation(float fRot)
	{
		this.m_fRotation = fRot;
	}
	
	//Called for querying the damage value for this entity
	DamageValue GetDamageValue()
	{
		return 0;
	}
	
	//Return a name string here, e.g. the class name or instance name. This is used when DAMAGE_NOTSQUAD is defined as damage-type, but can also be useful to other entities
	string GetName()
	{
		return "plasma_ball";
	}
}


void OnSpawn(const Vector &in vecPos, float fRot, const string &in szIdent, const string &in szPath)
{
	g_szPackagePath = szPath;

	CTankEntity @tank = CTankEntity();
	Ent_SpawnEntity(szIdent, @tank, vecPos);
}

