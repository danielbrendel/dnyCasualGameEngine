/*
	Casual Game Engine: Casual Pixel Warrior
	
	A game for Casual Game Engine
	
	(C) 2021 by Daniel Brendel

	Contact: dbrendel1988<at>gmail<dot>com
	GitHub: https://github.com/danielbrendel/

	Released under the MIT license
*/

#include "blooddecal.as"

const float C_HEADCRAB_DEFAULT_SPEED = 8;
const int C_HEADCRAB_REACT_RANGE = 250;
const int C_HEADGRAB_ATTACK_RANGE = 50;
const DamageValue C_HEADGRAB_DAMAGE_VALUE = 5;

/* Headcrab entity */
class CHeadcrabEntity : IScriptedEntity
{
	Vector m_vecPos;
	Vector m_vecSize;
	Model m_oModel;
	SpriteHandle m_hSprite;
	float m_fRotation;
	uint32 m_uiHealth;
	Timer m_oMovement;
	Timer m_oDirChange;
	Timer m_oEnemyCheck;
	Timer m_oShaking;
	Timer m_oWalkSound;
	Timer m_oAttack;
	bool m_bGotEnemy;
	float m_fShakeRot;
	float m_fSpeed;
	SoundHandle m_hWalkSound;
	SoundHandle m_hPainSound;
	SoundHandle m_hAttackSound;
	
	void LookAt(const Vector &in vPos)
	{
		//Look at position
		float flAngle = atan2(float(vPos[1] - this.m_vecPos[1]), float(vPos[0] - this.m_vecPos[0]));
		this.m_fRotation = flAngle + 6.30 / 4;
	}
	
	void CheckForEnemiesInRange()
	{
		//Check for enemies in close range and act accordingly
		
		this.m_bGotEnemy = false;
		IScriptedEntity@ pEntity = null;
		
		for (size_t i = 0; i < Ent_GetEntityCount(); i++) {
			@pEntity = @Ent_GetEntityHandle(i);
			if ((@pEntity != null) && (pEntity.GetName() == "player")) {
				if (this.m_vecPos.Distance(pEntity.GetPosition()) <= C_HEADCRAB_REACT_RANGE) {
					this.m_bGotEnemy = true;
					break;
				}
			}
		}
		
		if (this.m_bGotEnemy) {
			if (this.m_fSpeed == C_HEADCRAB_DEFAULT_SPEED)
				this.m_fSpeed *= 2;
				
			if (pEntity.GetName().length() > 0) {
				this.LookAt(pEntity.GetPosition());
			}

			if (this.m_vecPos.Distance(pEntity.GetPosition()) <= C_HEADGRAB_ATTACK_RANGE) {
				this.m_oAttack.Update();
				if (this.m_oAttack.IsElapsed()) {
					pEntity.OnDamage(C_HEADGRAB_DAMAGE_VALUE);
					S_PlaySound(this.m_hAttackSound, S_GetCurrentVolume());
					this.m_oAttack.Reset();
				}
			}
		} else {
			if (this.m_fSpeed != C_HEADCRAB_DEFAULT_SPEED)
				this.m_fSpeed = C_HEADCRAB_DEFAULT_SPEED;
		}
	}
	
	CHeadcrabEntity()
    {
		this.m_uiHealth = 100;
		this.m_vecSize = Vector(59, 52);
    }
	
	//Called when the entity gets spawned. The position on the screen is passed as argument
	void OnSpawn(const Vector& in vec)
	{
		this.m_vecPos = vec;
		this.m_fRotation = 0.0f;
		this.m_hSprite = R_LoadSprite(GetPackagePath() + "gfx\\headcrab.png", 1, 32, 32, 1, true);
		this.m_oMovement.SetDelay(100);
		this.m_oMovement.Reset();
		this.m_oMovement.SetActive(true);
		this.m_oDirChange.SetDelay(5000);
		this.m_oDirChange.Reset();
		this.m_oDirChange.SetActive(true);
		this.m_oEnemyCheck.SetDelay(1);
		this.m_oEnemyCheck.Reset();
		this.m_oEnemyCheck.SetActive(true);
		this.m_oShaking.SetDelay(1000);
		this.m_oShaking.Reset();
		this.m_oShaking.SetActive(true);
		this.m_oWalkSound.SetDelay(1000 + Util_Random(1, 2000));
		this.m_oWalkSound.Reset();
		this.m_oWalkSound.SetActive(true);
		this.m_oAttack.SetDelay(1000);
		this.m_oAttack.Reset();
		this.m_oAttack.SetActive(true);
		this.m_hWalkSound = S_QuerySound(GetPackagePath() + "sound\\hc_walk.wav");
		this.m_hPainSound = S_QuerySound(GetPackagePath() + "sound\\hc_hurt.wav");
		this.m_hAttackSound = S_QuerySound(GetPackagePath() + "sound\\hc_attack.wav");
		BoundingBox bbox;
		bbox.Alloc();
		bbox.AddBBoxItem(Vector(0, 0), this.m_vecSize);
		this.m_oModel.Alloc();
		this.m_oModel.Initialize2(bbox, this.m_hSprite);
	}
	
	//Called when the entity gets released
	void OnRelease()
	{
		CBloodSplash @obj = CBloodSplash();
		Ent_SpawnEntity("blooddecal", @obj, this.m_vecPos);
		
		SoundHandle hSplash = S_QuerySound(GetPackagePath() + "sound\\hc_splash.wav");
		S_PlaySound(hSplash, S_GetCurrentVolume());
	}
	
	//Process entity stuff
	void OnProcess()
	{
		this.m_oShaking.Update();
		if (this.m_oShaking.IsElapsed()) {
			if (!this.m_bGotEnemy)
				this.m_fShakeRot = -0.25 + float(Util_Random(1, 5)) / 10.0;
		}
		
		this.m_oDirChange.Update();
		if (this.m_oDirChange.IsElapsed()) {
			this.m_oDirChange.Reset();
			if (!this.m_bGotEnemy)
				this.m_fRotation = float(Util_Random(1, 360));
		}
		
		this.m_oWalkSound.Update();
		if (this.m_oWalkSound.IsElapsed()) {
			this.m_oWalkSound.Reset();
			S_PlaySound(this.m_hWalkSound, 8);
		}
		
		this.CheckForEnemiesInRange();
		
		this.m_oMovement.Update();
		if (this.m_oMovement.IsElapsed()) {
			this.m_oMovement.Reset();
			
			Ent_Move(this, this.m_fSpeed, MOVE_FORWARD);
		}
	}
	
	//Entity can draw everything in default order here
	void OnDraw()
	{
	}
	
	//Entity can draw on-top stuff here
	void OnDrawOnTop()
	{
		if (!R_ShouldDraw(this.m_vecPos, this.m_vecSize))
			return;
			
		Vector vOut;
		R_GetDrawingPosition(this.m_vecPos, this.m_vecSize, vOut);
		
		R_DrawSprite(this.m_hSprite, vOut, 0, this.m_fRotation, Vector(-1, -1), 0.0, 0.0, false, Color(0, 0, 0, 0));
	}
	
	//Indicate whether this entity shall be removed by the game
	bool NeedsRemoval()
	{
		return this.m_uiHealth == 0;
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
		if (this.m_uiHealth < damageValue) {
			this.m_uiHealth = 0;
		} else {
			this.m_uiHealth -= damageValue;
		}
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
	
	//Return the rotation.
	float GetRotation()
	{
		return this.m_fRotation;
	}
	
	//Set new rotation
	void SetRotation(float fRot)
	{
		this.m_fRotation = fRot;
	}
	
	//Return a name string here, e.g. the class name or instance name.
	string GetName()
	{
		return "headcrab";
	}
	
	//Called for wall collisions
	void OnWallCollided()
	{
	}
	
	//Return save game properties
	string GetSaveGameProperties()
	{
		return Sav_CreateProperty("id", formatInt(Ent_GetId(@this))) +
				Sav_CreateProperty("x", formatInt(this.m_vecPos[0])) +
				Sav_CreateProperty("y", formatInt(this.m_vecPos[1])) +
				Sav_CreateProperty("rot", formatFloat(this.m_fRotation)) +
				Sav_CreateProperty("health", formatInt(this.m_uiHealth));
	}
}

//Create headcrab entity
void CreateEntity(const Vector &in vecPos, float fRot, const string &in szIdent, const string &in szPath)
{
	CHeadcrabEntity @headcrab = CHeadcrabEntity();
	Ent_SpawnEntity(szIdent, @headcrab, vecPos);
	headcrab.SetRotation(fRot);
}

