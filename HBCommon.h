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

const char* HBCommaPrint(int n);

const CCPoint gAnchorCenter = ccp(0.5, 0.5);
const CCPoint gAnchorLeft = ccp(0, 0.5);
const CCPoint gAnchorRight = ccp(1, 0.5);
const CCPoint gAnchorTop = ccp(0.5, 0);
const CCPoint gAnchorBottom = ccp(0.5, 1);

#define HBRegistCCBLoader(pClassName, pCCNodeLoader) CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary()->registerCCNodeLoader(pClassName, pCCNodeLoader)
CCScene* HBSceneLoader(const char* name, CCNodeLoader* loader);
CCLayer* HBLayerLoader(const char* name, CCNodeLoader* loader);

CCPoint HBgetPositionByPercent(float x, float y);

CCSprite* HBcreateImageWithFrameName(const char* name, float x, float y, CCNode* parent, bool usePercent = true);
CCLabelTTF* HBcreateLabel(const char* label, const char* fontName, float fontSize, const CCPoint& anchor, const ccColor3B& color, float x, float y, CCNode* parent, bool usePercent = true);
CCLabelAtlas* HBcreateLabelAtlas(const char* label, const char* fontName, int width, int height, char startChar, float x, float y, const CCPoint& anchor, CCNode* parent, bool usePercent = true);

extern char HBDeviceLanguage[50];

class HBLocalize : public HBSingleton<HBLocalize>
{
public:
    const char* GetDeviceLanguage();
    bool readFromFile();
    bool readFromFile(const char* name);
    const char* getString(const char* name);
    
private:
    map<string, string> mDict;
};

#define HBLocaShared HBLocalize::shared()
#define gls(id) HBLocaShared->getString(id)

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

class HBPurchase : public HBSingleton<HBPurchase>
{
public:
    HBPurchase();
    
    void init(const vector<string>& itemList);

    string getItemCost(int index);
    string getItemCost(const char* itemName);
    void purchaseItem(int index, CCObject* target, SEL_CallFuncO successCall, SEL_CallFuncO failedCall);
    void purchaseItem(const char* itemName, CCObject* target, SEL_CallFuncO selector, SEL_CallFuncO failedCall);
    void purchaseSuccess(const char* itemName);
    void purchaseFailed(int errorCode, const char* failedReason);
    
private:
    bool mInitialized;
    
    CCObject* mTarget;
    SEL_CallFuncO mSuccessCall;
    SEL_CallFuncO mFailedCall;
    bool isPurchasing;
    
    vector<string>  mItemList;
};

extern "C"
{
void gotoReview();
void gotoMoreGame();
void gotoUrl(const char* url);
}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
const char* HBGetDeviceLanguage_iOS();
#endif

void HBExitApplication();

#endif
