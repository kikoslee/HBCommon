#include "HBCommon.h"

CCScene* HBSceneLoader(const char* name, CCNodeLoader* loader)
{
    CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    ccNodeLoaderLibrary->registerCCNodeLoader(name, loader);
    
    CCScene* scene = CCScene::create();
    
    cocos2d::extension::CCBReader* ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
    CCNode* node = ccbReader->readNodeGraphFromFile(fcs("%s.ccbi", name), scene);
    ccbReader->release();
    
    if(node != NULL)
        scene->addChild(node);
    
    return scene;
}

CCLayer* HBLayerLoader(const char* name, CCNodeLoader* loader)
{
    CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    ccNodeLoaderLibrary->registerCCNodeLoader(name, loader);
    
    cocos2d::extension::CCBReader* ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
    CCLayer* node = (CCLayer*)ccbReader->readNodeGraphFromFile(fcs("%s.ccbi", name), NULL);
    ccbReader->release();
    
    return node;
}

CCPoint getPositionByPercent(const CCPoint& p)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    return ccp(winSize.width * p.x, winSize.height * p.y);
}


CCSprite* createImageWithFrameName(const char* name, float x, float y, CCNode* parent)
{
    CCSprite* sprite = CCSprite::createWithSpriteFrameName(name);
    sprite->setPosition(getPositionByPercent(ccp(x,y)));
    parent->addChild(sprite);
    return sprite;
}
