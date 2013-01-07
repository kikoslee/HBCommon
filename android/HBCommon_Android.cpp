#include "HBCommon.h"
#include "main.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

//////////////////////////////////////////////////////////
// HBScore
//////////////////////////////////////////////////////////

void HBScore::initScore()
{
}

void HBScore::showBoard(const char* boardName)
{
	::showLeaderBoard(boardName);
}

void HBScore::reportScore(const char* boardName, int score)
{
	::reportScore(boardName, score);
}


//////////////////////////////////////////////////////////
// HBScore
//////////////////////////////////////////////////////////

// #include "MobClick.h"

void HBUmeng::startup()
{
}

void HBUmeng::updateConfig()
{
}

void HBUmeng::event(const char* name, const char* value)
{
	::umengCustomEvent(name, value);
}

int HBUmeng::getParamValue(const char* name)
{
	return ::umengGetParamValue(name);
}

#endif