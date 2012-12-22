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

#include "HBSingleton.h"

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

CCPoint getPositionByPercent(float x, float y);

CCSprite* createImageWithFrameName(const char* name, float x, float y, CCNode* parent);
CCLabelAtlas* createLabelAtlas(const char* label, const char* fontName, int width, int height, char startChar, float x, float y, const CCPoint& anchor, CCNode* parent);


//////////////////////////////////
// dependence by platform
//////////////////////////////////

#include "HBKeys.h"

class HBScore
{
public:
    static void initScore();
    
    static void showBoard(const char* boardName);
    static void reportScore(const char* boardName, int score);
};

class HBUmeng : public HBSingleton<HBUmeng>
{
public:
    static void startup();
    static void updateConfig();
    static int getParamValue(const char* name);
    static void event(const char* name, const char* value = NULL);
};


const char* getLocalizationString(const char* str);
void gotoReview();
void gotoUrl(const char* url);

#endif
