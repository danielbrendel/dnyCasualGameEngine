
string g_szPackagePath = "";

/* 
	Scripted entity 
	
	A scripted entity is a dynamic interface class for implementing
	custom entities such as explosions, decals, etc
	
	It is derived from the IScriptedEntity interface and must define
	all the following interface functions. Also it must have at least
	one Vector and Model which are returned in certain methods. Tho
	you don't need to use them. A scripted entity is passed to the 
	game engine via the SpawnEntity(<object handle>, <spawn position>) function. 
*/
class CPlayerEntity : IScriptedEntity, IPlayerEntity
{
	Vector m_vecPos;
	Vector m_vecSelSize;
	Model m_oModel;
	SpriteHandle m_hSprite;
	float m_fRotation;
	
	CPlayerEntity()
    {
		this.m_vecSelSize = Vector(0, 0);
    }
	
	//Called when the entity gets spawned. The position on the screen is passed as argument
	void OnSpawn(const Vector& in vec)
	{
		this.m_vecPos = vec;
		this.m_fRotation = 0.0f;
		this.m_hSprite = R_LoadSprite(g_szPackagePath + "gfx\\mech.png", 1, 59, 52, 1, true);
		this.m_oModel.Alloc();
	}
	
	//Called when the entity gets released
	void OnRelease()
	{
	}
	
	//Process entity stuff
	void OnProcess()
	{
	}
	
	//Entity can draw everything in default order here
	void OnDraw()
	{
	}
	
	//Entity can draw on-top stuff here
	void OnDrawOnTop()
	{
		R_DrawString(R_GetDefaultFont(), "LOL TEST", this.m_vecPos, Color(255, 0, 0, 150));
		bool r = R_DrawSprite(this.m_hSprite, this.m_vecPos, 0, this.m_fRotation, Vector(-1, -1), 0.0f, 0.0f, false, Color(0, 0, 0, 0));
		//Print((r == true) ? "true" : "false");
	}
	
	//Indicate whether this entity shall be removed by the game
	bool NeedsRemoval()
	{
		return false;
	}
	
	//Indicate whether this entity is damageable. Damageable entities can collide with other
	//entities (even with entities from other tools) and recieve and strike damage. 
	//0 = not damageable, 1 = damage all, 2 = not damaging entities with same name
	DamageType IsDamageable()
	{
		return DAMAGEABLE_NOTSQUAD;
	}
	
	//Called when the entity recieves damage
	void OnDamage(DamageValue dv)
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
	
	//Return the rotation. This is actually not used by the host application, but might be useful to other entities
	float GetRotation()
	{
		return 0.0;
	}
	
	//Called for querying the damage value for this entity
	DamageValue GetDamageValue()
	{
		return 0;
	}
	
	//Return a name string here, e.g. the class name or instance name. This is used when DAMAGE_NOTSQUAD is defined as damage-type, but can also be useful to other entities
	string GetName()
	{
		return "player";
	}
	
	//This vector is used for drawing the selection box
	Vector& GetSelectionSize()
	{
		return this.m_vecSelSize;
	}
	
	//Called for key presses
	void OnKeyPress(int vKey, bool bDown)
	{
		Print("Pressing key: " + formatInt(vKey));
		if (vKey == 37) {
			this.m_fRotation += 0.05f;
		} else if (vKey == 39) {
			this.m_fRotation -= 0.05f;
		}
	}
	
	//Called for mouse presses
	void OnMousePress(int key, bool bDown)
	{
		Print("Pressing key: " + formatInt(key));
	}
	
	//Called for getting current cursor position
	void OnUpdateCursor(const Vector &in pos)
	{
	
	}
}


void OnSpawn(const Vector &in vecPos, const string &in szIdent, const string &in szPath)
{
	g_szPackagePath = szPath;

	CPlayerEntity @player = CPlayerEntity();
	Ent_SpawnEntity(szIdent, @player, vecPos);
}

