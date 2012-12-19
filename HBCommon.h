#ifndef _HBCommon_H_
#define _HBCommon_H_

#include <vector>
using namespace std;

#include "cocos2d.h"
USING_NS_CC;

#include "cocos-ext.h"
USING_NS_CC_EXT;

#include "SimpleAudioEngine.h"
using namespace CocosDenshion;
#define Audio SimpleAudioEngine::sharedEngine()

#include "HBUmeng.h"
#include "HBScore.h"

#define fcs(format,...) CCString::createWithFormat(format,## __VA_ARGS__)->getCString()
#define gls(id) getLocalizationString(id)

CCScene* HBSceneLoader(const char* name, CCNodeLoader* loader);
CCLayer* HBLayerLoader(const char* name, CCNodeLoader* loader);

const char* getLocalizationString(const char* str);

CCPoint getPositionByPercent(const CCPoint& p);

CCSprite* createImageWithFrameName(const char* name, float x, float y, CCNode* parent);

#endif
