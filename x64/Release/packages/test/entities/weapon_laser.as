
class CLaserEntity : IScriptedEntity
{
	Vector m_vecPos;
	Vector m_vecSize;
	Model m_oModel;
	SpriteHandle m_hLaser;
	float m_fRotation;
	float m_fSpeed;
	bool m_bRemove;
	Timer m_tmrAlive;
	
	CLaserEntity()
    {
		this.m_vecSize = Vector(60, 99);
		this.m_fSpeed = 35.0;
		this.m_bRemove = false;
    }
	
	//Called when the entity gets spawned. The position on the screen is passed as argument
	void OnSpawn(const Vector& in vec)
	{
		this.m_vecPos = vec;
		this.m_hLaser = R_LoadSprite(GetPackagePath() + "gfx\\laser.png", 1, 60, 99, 1, true);
		this.m_tmrAlive.SetDelay(10000);
		this.m_tmrAlive.Reset();
		this.m_tmrAlive.SetActive(true);
		this.m_oModel.Alloc();
		BoundingBox bbox;
		bbox.Alloc();
		bbox.AddBBoxItem(Vector(0, 0), Vector(60, 99));
		this.m_oModel.Alloc();
		this.m_oModel.SetCenter(Vector(60 / 2, 99 / 2));
		this.m_oModel.Initialize2(bbox, this.m_hLaser);
	}
	
	//Called when the entity gets released
	void OnRelease()
	{
	}
	
	//Process entity stuff
	void OnProcess()
	{
		Ent_Move(this, this.m_fSpeed, MOVE_FORWARD);
		
		this.m_tmrAlive.Update();
		if (this.m_tmrAlive.IsElapsed()) {
			this.m_bRemove = true;
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
		
		R_DrawSprite(this.m_hLaser, vOut, 0, this.m_fRotation, Vector(-1, -1), 0.0, 0.0, false, Color(0, 0, 0, 0));
	}
	
	//Called for wall collisions
	void OnWallCollided()
	{
		this.m_bRemove = true;
	}
	
	//Indicate whether this entity shall be removed by the game
	bool NeedsRemoval()
	{
		return (this.m_bRemove); //Remove laser when reached position
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
		this.m_bRemove = true;
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
		return "weapon_laser";
	}
	
	//This vector is used for drawing the selection box
	Vector& GetSize()
	{
		return this.m_vecPos;
	}
}