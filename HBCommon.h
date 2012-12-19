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
#include "HBWebBrowser.h"

#define fcs(format,...) CCString::createWithFormat(format,## __VA_ARGS__)->getCString()
#define gls(id) getLocalizationString(id)

const CCPoint gPointCenter = ccp(240, 160);
const CCPoint gAnchorCenter = ccp(0.5, 0.5);
const CCPoint gAnchorLeft = ccp(0, 0.5);
const CCPoint gAnchorRight = ccp(1, 0.5);
const CCPoint gAnchorTop = ccp(0.5, 0);
const CCPoint gAnchorBottom = ccp(0.5, 1);

CCScene* HBSceneLoader(const char* name, CCNodeLoader* loader);
CCLayer* HBLayerLoader(const char* name, CCNodeLoader* loader);

const char* getLocalizationString(const char* str);

CCPoint getPositionByPercent(const CCPoint& p);

CCSprite* createImageWithFrameName(const char* name, float x, float y, CCNode* parent);
CCLabelAtlas* createLabelAtlas(const char* label, const char* fontName, int width, int height, char startChar, float x, float y, const CCPoint& anchor, CCNode* parent);

#endif
