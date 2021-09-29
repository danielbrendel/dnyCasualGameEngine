
string g_szPackagePath = "";

#include "weapon_laser.as"
#include "../../.common/entities/explosion.as"
#include "tankcls.as"

const int BTN_FORWARD = (1 << 0);
const int BTN_BACKWARD = (1 << 1);
const int BTN_MOVELEFT = (1 << 2);
const int BTN_MOVERIGHT = (1 << 3);
const int BTN_TURNLEFT = (1 << 4);
const int BTN_TURNRIGHT = (1 << 5);
const int BTN_SPEED = (1 << 6);
const int BTN_ATTACK = (1 << 7);
class CPlayerEntity : IScriptedEntity, IPlayerEntity
{
	Vector m_vecPos;
	Vector m_vecSize;
	Model m_oModel;
	SpriteHandle m_hSprite;
	float m_fRotation;
	uint32 m_uiButtons;
	uint32 m_uiHealth;
	Timer m_tmrMayDamage;
	Timer m_tmrAttack;
	Timer m_tmrFlicker;
	uint32 m_uiFlickerCount;
	
	CPlayerEntity()
    {
		this.m_uiButtons = 0;
		this.m_uiHealth = 100;
		this.m_vecSize = Vector(59, 52);
		this.m_uiFlickerCount = 0;
    }
	
	//Called when the entity gets spawned. The position on the screen is passed as argument
	void OnSpawn(const Vector& in vec)
	{
		this.m_vecPos = vec;
		this.m_fRotation = 0.0f;
		this.m_hSprite = R_LoadSprite(g_szPackagePath + "gfx\\mech.png", 1, 59, 52, 1, true);
		this.m_tmrMayDamage.SetDelay(2000);
		this.m_tmrMayDamage.Reset();
		this.m_tmrMayDamage.SetActive(true);
		this.m_tmrAttack.SetDelay(500);
		this.m_tmrAttack.Reset();
		this.m_tmrAttack.SetActive(true);
		this.m_tmrFlicker.SetDelay(250);
		this.m_tmrFlicker.Reset();
		this.m_tmrFlicker.SetActive(false);
		BoundingBox bbox;
		bbox.Alloc();
		bbox.AddBBoxItem(Vector(0, 0), this.m_vecSize);
		this.m_oModel.Alloc();
		this.m_oModel.Initialize2(bbox, this.m_hSprite);
	}
	
	//Called when the entity gets released
	void OnRelease()
	{
	}
	
	//Process entity stuff
	void OnProcess()
	{
		//Process movement

		if ((this.m_uiButtons & BTN_FORWARD) == BTN_FORWARD) {
			Ent_Move(this, 10, MOVE_FORWARD);
		} 
		
		if ((this.m_uiButtons & BTN_BACKWARD) == BTN_BACKWARD) {
			Ent_Move(this, 10, MOVE_BACKWARD);
		} 
		
		if ((this.m_uiButtons & BTN_MOVELEFT) == BTN_MOVELEFT) {
			Ent_Move(this, 10, MOVE_LEFT);
		} 
		
		if ((this.m_uiButtons & BTN_MOVERIGHT) == BTN_MOVERIGHT) {
			Ent_Move(this, 10, MOVE_RIGHT);
		} 

		if ((this.m_uiButtons & BTN_TURNLEFT) == BTN_TURNLEFT) {
			this.m_fRotation += 0.05f;
		} 

		if ((this.m_uiButtons & BTN_TURNRIGHT) == BTN_TURNRIGHT) {
			this.m_fRotation -= 0.05f;
		}
		
		HUD_UpdateHealth(this.m_uiHealth);

		//Process attacking
		if ((this.m_uiButtons & BTN_ATTACK) == BTN_ATTACK) {
			this.m_tmrAttack.Update();
			if (this.m_tmrAttack.IsElapsed()) {
				this.m_tmrAttack.Reset();
				
				CLaserEntity @laser = CLaserEntity();
				
				Vector vecLaserPos = Vector(this.m_vecPos[0] + 59 / 2, this.m_vecPos[1] + 52 / 2);
				laser.SetRotation(this.m_fRotation);
				
				Ent_SpawnEntity("weapon_laser", @laser, vecLaserPos);
				
				SoundHandle hSound = S_QuerySound(g_szPackagePath + "sound\\laser.wav");
				S_PlaySound(hSound, 10);
			}
		}
		
		//Update damage permission timer
		this.m_tmrMayDamage.Update();
		
		//Process flickering
		if (this.m_tmrFlicker.IsActive()) {
			this.m_tmrFlicker.Update();
			if (this.m_tmrFlicker.IsElapsed()) {
				this.m_tmrFlicker.Reset();
				
				this.m_uiFlickerCount++;
				if (this.m_uiFlickerCount >= 6) {
					this.m_tmrFlicker.SetActive(false);
					this.m_uiFlickerCount = 0;
				}
			}
		}
	}
	
	//Entity can draw everything in default order here
	void OnDraw()
	{
	}
	
	//Entity can draw on-top stuff here
	void OnDrawOnTop()
	{
		bool bDrawCustomColor = false;
		
		if ((this.m_tmrFlicker.IsActive()) && (this.m_uiFlickerCount % 2 == 0)) {
			bDrawCustomColor = true;
		}
		
		Color sDrawingColor = (this.m_tmrFlicker.IsActive()) ? Color(255, 0, 0, 150) : Color(0, 0, 0, 0);
		
		R_DrawSprite(this.m_hSprite, Vector(Wnd_GetWindowCenterX() - 59 / 2, Wnd_GetWindowCenterY() - 52 / 2), 0, this.m_fRotation, Vector(-1, -1), 0.0f, 0.0f, bDrawCustomColor, sDrawingColor);
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
		if (this.m_tmrMayDamage.IsElapsed()) {
			if (this.m_uiHealth < damageValue) {
				this.m_uiHealth = 0;
			} else {
				this.m_uiHealth -= damageValue;
			}
			
			this.m_tmrMayDamage.Reset();
			
			this.m_tmrFlicker.Reset();
			this.m_tmrFlicker.SetActive(true);
			this.m_uiFlickerCount = 0;
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
	
	//Set health
	void SetHealth(uint health)
	{
		this.m_uiHealth = health;
	}
	
	//Return a name string here, e.g. the class name or instance name. This is used when DAMAGE_NOTSQUAD is defined as damage-type, but can also be useful to other entities
	string GetName()
	{
		return "player";
	}
	
	//Called for wall collisions
	void OnWallCollided()
	{
	}
	
	//Called for key presses
	void OnKeyPress(int vKey, bool bDown)
	{
		if (vKey == GetKeyBinding("TURN_LEFT")) {
			if (bDown) {
				if (!((this.m_uiButtons & BTN_TURNLEFT) == BTN_TURNLEFT)) {
					this.m_uiButtons |= BTN_TURNLEFT;
				}
			} else {
				if ((this.m_uiButtons & BTN_TURNLEFT) == BTN_TURNLEFT) {
					this.m_uiButtons &= ~BTN_TURNLEFT;
				}
			}
		} else if (vKey == GetKeyBinding("TURN_RIGHT")) {
			if (bDown) {
				if (!((this.m_uiButtons & BTN_TURNRIGHT) == BTN_TURNRIGHT)) {
					this.m_uiButtons |= BTN_TURNRIGHT;
				}
			} else {
				if ((this.m_uiButtons & BTN_TURNRIGHT) == BTN_TURNRIGHT) {
					this.m_uiButtons &= ~BTN_TURNRIGHT;
				}
			}
		} 
		
		if (vKey == GetKeyBinding("MOVE_FORWARD")) {
			if (bDown) {
				if (!((this.m_uiButtons & BTN_FORWARD) == BTN_FORWARD)) {
					this.m_uiButtons |= BTN_FORWARD;
				}
			} else {
				if ((this.m_uiButtons & BTN_FORWARD) == BTN_FORWARD) {
					this.m_uiButtons &= ~BTN_FORWARD;
				}
			}
		} else if (vKey == GetKeyBinding("MOVE_BACKWARD")) {
			if (bDown) {
				if (!((this.m_uiButtons & BTN_BACKWARD) == BTN_BACKWARD)) {
					this.m_uiButtons |= BTN_BACKWARD;
				}
			} else {
				if ((this.m_uiButtons & BTN_BACKWARD) == BTN_BACKWARD) {
					this.m_uiButtons &= ~BTN_BACKWARD;
				}
			}
		} else if (vKey == GetKeyBinding("MOVE_LEFT")) {
			if (bDown) {
				if (!((this.m_uiButtons & BTN_MOVELEFT) == BTN_MOVELEFT)) {
					this.m_uiButtons |= BTN_MOVELEFT;
				}
			} else {
				if ((this.m_uiButtons & BTN_MOVELEFT) == BTN_MOVELEFT) {
					this.m_uiButtons &= ~BTN_MOVELEFT;
				}
			}
		} else if (vKey == GetKeyBinding("MOVE_RIGHT")) {
			if (bDown) {
				if (!((this.m_uiButtons & BTN_MOVERIGHT) == BTN_MOVERIGHT)) {
					this.m_uiButtons |= BTN_MOVERIGHT;
				}
			} else {
				if ((this.m_uiButtons & BTN_MOVERIGHT) == BTN_MOVERIGHT) {
					this.m_uiButtons &= ~BTN_MOVERIGHT;
				}
			}
		}
		
		if (vKey == GetKeyBinding("ATTACK")) {
			if (bDown) {
				if (!((this.m_uiButtons & BTN_ATTACK) == BTN_ATTACK)) {
					this.m_uiButtons |= BTN_ATTACK;
				}
			} else {
				if ((this.m_uiButtons & BTN_ATTACK) == BTN_ATTACK) {
					this.m_uiButtons &= ~BTN_ATTACK;
				}
			}
		}
	}
	
	//Called for mouse presses
	void OnMousePress(int key, bool bDown)
	{
	}
	
	//Called for getting current cursor position
	void OnUpdateCursor(const Vector &in pos)
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
	
	//Called for returning the current score
	int GetPlayerScore()
	{
		return 0;
	}
}

//Create the associated entity here
void CreateEntity(const Vector &in vecPos, float fRot, const string &in szIdent, const string &in szPath)
{
	g_szPackagePath = szPath;
	
	CPlayerEntity @player = CPlayerEntity();
	Ent_SpawnEntity(szIdent, @player, vecPos);
	player.SetRotation(fRot);
	
	HUD_AddAmmoItem("laser", GetPackagePath() + "gfx\\laserhud.png");
	HUD_UpdateAmmoItem("laser", 1000, 5000);
	HUD_SetAmmoDisplayItem("laser");
}

//Restore game state
void RestoreState(const string &in szIdent, const string &in szValue)
{
	string id = Sav_GetValueFromProperties(szValue, "id");
	if (id != "") {
		IScriptedEntity@ ent = Ent_GetEntityHandle(parseInt(id));
		if (@ent != null) {
			int x = parseInt(Sav_GetValueFromProperties(szValue, "x"));
			int y = parseInt(Sav_GetValueFromProperties(szValue, "y"));
			float rot = parseFloat(Sav_GetValueFromProperties(szValue, "rot"));
			
			ent.SetPosition(Vector(x, y));
			ent.SetRotation(rot);
			
			string health = Sav_GetValueFromProperties(szValue, "health");
			if (health != "") {
				if (ent.GetName() == "player") {
					CPlayerEntity@ casted = cast<CPlayerEntity>(ent);
					casted.SetHealth(parseInt(health));
				} else if (ent.GetName() == "tank") {
					CTankEntity@ casted = cast<CTankEntity>(ent);
					casted.SetHealth(parseInt(health));
				}
			}
		}
	} else {
		int x = parseInt(Sav_GetValueFromProperties(szValue, "x"));
		int y = parseInt(Sav_GetValueFromProperties(szValue, "y"));
		float rot = parseFloat(Sav_GetValueFromProperties(szValue, "rot"));
	
		if (szIdent == "decal") {
			CDecalEntity @dcl = CDecalEntity();
			dcl.SetRotation(rot);
			Ent_SpawnEntity("decal", @dcl, Vector(x, y));
		} else if (szIdent == "explosion") {
			CExplosionEntity @expl = CExplosionEntity();
			expl.SetRotation(rot);
			Ent_SpawnEntity("explosion", @expl, Vector(x, y));
		} else {
			Print("Unknown spawnable entity ident: " + szIdent);
		}
	}
}

//Save game state to disk
bool SaveGame()
{
	Print("Saving game...");
	
	SaveGameWriter writer;
	writer.BeginSaveGame();
	writer.WritePackage(GetPackageName());
	writer.WriteMap(GetCurrentMap());
	
	for (size_t i = 0; i < Ent_GetEntityCount(); i++) {
		IScriptedEntity@ ent = Ent_GetEntityHandle(i);
		if (@ent != null) {
			writer.WriteAttribute(ent.GetName(), ent.GetSaveGameProperties());
		}
	}
	
	writer.EndSaveGame();
	
	return true;
}
