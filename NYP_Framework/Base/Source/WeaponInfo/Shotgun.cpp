#include "Shotgun.h"
#include "../Lua/LuaInterface.h"

CShotGun::CShotGun()
{
}


CShotGun::~CShotGun()
{
}

// Initialise this instance to default values
void CShotGun::Init(void)
{
	// Call the parent's Init method
	CWeaponInfo::Init();
	lua_getglobal(CLuaInterface::GetInstance()->theLuaState, "shotguninfo");

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
	elapsedTime = 1.0;
	// Boolean flag to indicate if weapon can fire now
	bFire = true;
}
