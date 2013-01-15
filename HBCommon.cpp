#include "HBCommon.h"

char HBDeviceLanguage[50] = "";

const char* HBCommaPrint(int n)
{
	static int comma = ',';
	static char retbuf[30];
	
	char *p = &retbuf[sizeof(retbuf)-1];
	int i = 0;
	
	*p = '\0';
	int m = abs(n);
	do
	{
		if( i % 3 == 0 && i != 0)
			*--p = comma;
		*--p = '0' + m % 10;
		m /= 10;
		i++;
	} while(m != 0);
	
	if(n < 0)
		*--p = '-';
	
	return p;
}

CCScene* HBSceneLoader(const char* name, CCNodeLoader* loader)
{
    CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
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
    CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
    ccNodeLoaderLibrary->registerCCNodeLoader(name, loader);
    
    cocos2d::extension::CCBReader* ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
    CCLayer* node = (CCLayer*)ccbReader->readNodeGraphFromFile(fcs("%s.ccbi", name), NULL);
    ccbReader->release();
    
    return node;
}

CCPoint HBgetPositionByPercent(float x, float y)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    return ccp(winSize.width * x / 100.f, winSize.height * y / 100.f);
}

CCSprite* HBcreateImageWithFrameName(const char* name, float x, float y, CCNode* parent, bool usePercent)
{
    CCSprite* sprite = CCSprite::createWithSpriteFrameName(name);
    if (usePercent)
        sprite->setPosition(HBgetPositionByPercent(x, y));
    else
        sprite->setPosition(ccp(x, y));
    parent->addChild(sprite);
    return sprite;
}

CCLabelTTF* HBcreateLabel(const char* label, const char* fontName, float fontSize, const CCPoint& anchor, const ccColor3B& color, float x, float y, CCNode* parent, bool usePercent)
{
    CCLabelTTF* labelTTF = CCLabelTTF::create(label, fontName, fontSize);
    if (usePercent)
        labelTTF->setPosition(HBgetPositionByPercent(x, y));
    else
        labelTTF->setPosition(ccp(x, y));
    labelTTF->setAnchorPoint(anchor);
    labelTTF->setColor(color);
    parent->addChild(labelTTF);
    return labelTTF;
}

CCLabelAtlas* HBcreateLabelAtlas(const char* label, const char* fontName, int width, int height, char startChar, float x, float y, const CCPoint& anchor, CCNode* parent, bool usePercent)
{
    CCLabelAtlas* labelAtlas = CCLabelAtlas::create(label, fontName, width, height, startChar);
    if (usePercent)
        labelAtlas->setPosition(HBgetPositionByPercent(x, y));
    else
        labelAtlas->setPosition(ccp(x, y));
    labelAtlas->setAnchorPoint(anchor);
    parent->addChild(labelAtlas);
    return labelAtlas;
}

#include <libxml/parser.h>
#include <libxml/tree.h>

bool HBLocalize::readFromFile()
{
    if (!readFromFile(fcs("localstring.%s.xml", GetDeviceLanguage())))
        return readFromFile("localstring.en.xml");
    
    return true;
}

bool HBLocalize::readFromFile(const char* name)
{
    assert(name);

	const char *pszPath = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(name);
    CCLog("localization file: [%s]", pszPath);

    unsigned long size;
    unsigned char* pBytes = CCFileUtils::sharedFileUtils()->getFileData(pszPath, "rb", &size);

	xmlDocPtr doc = xmlReadMemory((const char*)pBytes, size, NULL, "utf-8", XML_PARSE_RECOVER);
//    xmlReadFile(pszPath, "utf-8", XML_PARSE_RECOVER);
	
	xmlNodePtr rootNode;
	xmlNodePtr curNode = NULL;
	
	rootNode = xmlDocGetRootElement(doc);
	if (NULL == rootNode)
	{
		CCLOG("localization file [%s] read error", name);
		return false;
	}
    
    mDict.clear();
    
	curNode = rootNode;
	while (NULL != curNode)
	{
		if (!xmlStrcmp(curNode->name, BAD_CAST "strings"))
		{
			xmlNodePtr elem = curNode->children;
			while (NULL != elem)
			{
				if (!xmlStrcmp(elem->name, BAD_CAST "obj"))
				{
					string id = string((char*)xmlGetProp(elem, BAD_CAST "id"));
                    string value = string((char*)xmlGetProp(elem, BAD_CAST "value"));
                    
                    mDict[id] = value;
				}
				elem = elem->next;
			}
		}
		curNode = curNode->next;
	}
	
	xmlFreeDoc(doc);
    
    return true;
}

const char* HBLocalize::getString(const char* name)
{
    map<string, string>::iterator itr = mDict.find(string(name));
    if(itr == mDict.end())
        return name;
    
    return itr->second.c_str();
}

const char* HBLocalize::GetDeviceLanguage()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    strcpy(HBDeviceLanguage, HBGetDeviceLanguage_iOS());
#endif
    
    if(strcmp(HBDeviceLanguage,  "") == 0       ||
       strcmp(HBDeviceLanguage, "zh") == 0      ||
       strcmp(HBDeviceLanguage, "zh_CN") == 0   ||
       strcmp(HBDeviceLanguage, "zh-Hans") == 0)
    {
        return "zh_CN";
    }
    else if(strcmp(HBDeviceLanguage, "zh_TW") == 0      ||
            strcmp(HBDeviceLanguage, "zh_HK") == 0      ||
            strcmp(HBDeviceLanguage, "zh-Hant") == 0)
    {
        return "zh_TW";
    }
    else if(strcmp(HBDeviceLanguage, "ja") == 0      ||
            strcmp(HBDeviceLanguage, "ja_JP") == 0)
    {
        return "ja";
    }
    else
    {
        return HBDeviceLanguage;
    }
}

void HBExitApplication()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCDirector::sharedDirector()->end();
    exit(0);
#else
#endif
}
