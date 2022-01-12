/*
	Casual Game Engine: Casual Pixel Warrior
	
	A sample and test game for Casual Game Engine
	
	(C) 2021 - 2022 by Daniel Brendel

	Contact: dbrendel1988<at>gmail<dot>com
	GitHub: https://github.com/danielbrendel/

	Released under the MIT license
*/

#include "explosion.as"

const uint32 LASER_SHOT_DAMAGE = 20;

/* Laser entity */
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
	IScriptedEntity@ m_pOwner;
	
	//Set owner
	void SetOwner(IScriptedEntity@ pOwner)
	{
		@this.m_pOwner = @pOwner;
	}
	
	CLaserEntity()
    {
		this.m_vecSize = Vector(50, 35);
		this.m_fSpeed = 650.0;
		this.m_bRemove = false;
		@this.m_pOwner = null;
    }
	
	//Called when the entity gets spawned. The position in the map is passed as argument
	void OnSpawn(const Vector& in vec)
	{
		this.m_vecPos = vec;
		this.m_hLaser = R_LoadSprite(GetPackagePath() + "gfx\\laser.png", 1, 60, 99, 1, true);
		this.m_tmrAlive.SetDelay(10000);
		this.m_tmrAlive.Reset();
		this.m_tmrAlive.SetActive(true);
		BoundingBox bbox;
		bbox.Alloc();
		bbox.AddBBoxItem(Vector(0, 0), this.m_vecSize);
		this.m_oModel.Alloc();
		this.m_oModel.SetCenter(Vector(50 / 2, 35 / 2));
		this.m_oModel.Initialize2(bbox, this.m_hLaser);
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
		if (ref.GetName() != "player") {
			this.m_bRemove = true;
			
			ref.OnDamage(LASER_SHOT_DAMAGE);
			
			if (ref.NeedsRemoval()) {
				if (@this.m_pOwner == @Ent_GetPlayerEntity()) {
					IPlayerEntity@ casted = cast<IPlayerEntity>(this.m_pOwner);
					
					if (ref.GetName() == "headcrab") {
						casted.AddPlayerScore(1);
					} else if (ref.GetName() == "tank") {
						casted.AddPlayerScore(15);
					} else if (ref.GetName() == "teslatower") {
						casted.AddPlayerScore(10);
					} else {
						casted.AddPlayerScore(1);
					}
				}
			}
		}
	}
	
	//Called when entity gets damaged
	void OnDamage(uint32 damageValue)
	{
	}
	
	//Called for accessing the model data for this entity.
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
		return "weapon_laser";
	}
	
	//This vector is used for drawing the selection box
	Vector& GetSize()
	{
		return this.m_vecSize;
	}
	
	//Return save game properties
	string GetSaveGameProperties()
	{
		return "";
	}
}