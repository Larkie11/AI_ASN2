#include "Pistol.h"
#include "../Lua/LuaInterface.h"

CPistol::CPistol()
{
}


CPistol::~CPistol()
{
}

// Initialise this instance to default values
void CPistol::Init(void)
{
	// Call the parent's Init method
	CWeaponInfo::Init();
	lua_getglobal(CLuaInterface::GetInstance()->theLuaState, "pistolinfo");
	// The number of ammunition in a magazine for this weapon
	magRounds = CLuaInterface::GetInstance()->GetField("maground");
	// The maximum number of ammunition for this magazine for this weapon
	maxMagRounds = CLuaInterface::GetInstance()->GetField("maxmaground");
	// The current total number of rounds currently carried by this player
	totalRounds = CLuaInterface::GetInstance()->GetField("totalround");
	// The max total number of rounds currently carried by this player
	maxTotalRounds = CLuaInterface::GetInstance()->GetField("maxtotalround");
	soundeffect = CLuaInterface::GetInstance()->GetStringField("se");
	// The time between shots
	timeBetweenShots = CLuaInterface::GetInstance()->GetField("timebetween");
	// The elapsed time (between shots)
	elapsedTime = 0.0;
	// Boolean flag to indicate if weapon can fire now
	bFire = true;
}
