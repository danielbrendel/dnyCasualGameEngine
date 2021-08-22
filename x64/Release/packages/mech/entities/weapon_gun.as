#include "explosion.as"

const uint32 GUN_SHOT_DAMAGE = 35;

class CGunEntity : IScriptedEntity
{
	Vector m_vecPos;
	Vector m_vecSize;
	Model m_oModel;
	SpriteHandle m_hShot;
	float m_fRotation;
	float m_fSpeed;
	bool m_bRemove;
	Timer m_tmrAlive;
	
	CGunEntity()
    {
		this.m_vecSize = Vector(60, 99);
		this.m_fSpeed = 35.0;
		this.m_bRemove = false;
    }
	
	//Called when the entity gets spawned. The position on the screen is passed as argument
	void OnSpawn(const Vector& in vec)
	{
		this.m_vecPos = vec;
		this.m_hShot = R_LoadSprite(GetPackagePath() + "gfx\\gunshot.png", 1, 3, 12, 1, true);
		this.m_tmrAlive.SetDelay(10000);
		this.m_tmrAlive.Reset();
		this.m_tmrAlive.SetActive(true);
		BoundingBox bbox;
		bbox.Alloc();
		bbox.AddBBoxItem(Vector(0, 0), Vector(60, 99));
		this.m_oModel.Alloc();
		this.m_oModel.SetCenter(Vector(60 / 2, 99 / 2));
		this.m_oModel.Initialize2(bbox, this.m_hShot);
	}
	
	//Called when the entity gets released
	void OnRelease()
	{
		CExplosionEntity @expl = CExplosionEntity();
		Ent_SpawnEntity("explosion", @expl, this.m_vecPos);
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
		
		R_DrawSprite(this.m_hShot, vOut, 0, this.m_fRotation, Vector(-1, -1), 0.0, 0.0, false, Color(0, 0, 0, 0));
	}
	
	//Called for wall collisions
	void OnWallCollided()
	{
		this.m_bRemove = true;
	}
	
	//Indicate whether this entity shall be removed by the game
	bool NeedsRemoval()
	{
		return this.m_bRemove; //Remove laser when reached position
	}
	
	//Indicate whether this entity is collidable
	bool IsCollidable()
	{
		return true;
	}
	
	//Called when the entity collided with another entity
	void OnCollided(IScriptedEntity@ ref)
	{
		if (ref.GetName() == "player") {
			ref.OnDamage(GUN_SHOT_DAMAGE);
		}
		
		this.m_bRemove = true;
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
	
	//Return a name string here, e.g. the class name or instance name. This is used when DAMAGE_NOTSQUAD is defined as damage-type, but can also be useful to other entities
	string GetName()
	{
		return "weapon_gun";
	}
	
	//This vector is used for drawing the selection box
	Vector& GetSize()
	{
		return this.m_vecPos;
	}
}