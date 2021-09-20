#include "tankcls.as"

void CreateEntity(const Vector &in vecPos, float fRot, const string &in szIdent, const string &in szPath)
{
	CTankEntity @tank = CTankEntity();
	Ent_SpawnEntity(szIdent, @tank, vecPos);
}

