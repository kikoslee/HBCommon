#include "HBCommon.h"
#import "HBGameCenter.h"

const char* getLocalizationString(const char* str)
{
	NSString* strName = [NSString stringWithFormat:@"%s", str];
	strName = NSLocalizedString(strName, nil);
	return [strName cStringUsingEncoding:NSUTF8StringEncoding];
}

void gotoReview()
{
    NSString *str = [NSString stringWithFormat:@"itms-apps://ax.itunes.apple.com/WebObjects/MZStore.woa/wa/viewContentsUserReviews?type=Purple+Software&id=%s", kAppIDiOS];
	[[UIApplication sharedApplication] openURL:[NSURL URLWithString:str]];
}

void gotoUrl(const char* url)
{
    NSString* u = [NSString stringWithFormat:@"%s", url];
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:u]];
}


//////////////////////////////////////////////////////////
// HBScore
//////////////////////////////////////////////////////////

void HBScore::initScore()
{
    [HBGameCenter shared];
}

void HBScore::showBoard(const char* boardName)
{
    assert(boardName);
    assert(strlen(boardName) > 0);
    
    [[HBGameCenter shared] showLeaderboard:[NSString stringWithFormat:@"%s", boardName]];
}

void HBScore::reportScore(const char* boardName, int score)
{
    assert(boardName);
    assert(strlen(boardName) > 0);
    
    [[HBGameCenter shared] reportScoreTo:[NSString stringWithFormat:@"%s", boardName] withScore:score];
}


//////////////////////////////////////////////////////////
// HBScore
//////////////////////////////////////////////////////////

#import "MobClick.h"

#if DEBUG
#define UMENG_CHANNEL "Test Mode"
#else
#if HBCHANNEL == 1
#define UMENG_CHANNEL "AppStore"
#elif HBCHANNEL == 2
#define UMENG_CHANNEL "91Store"
#else
#define UMENG_CHANNEL "AppStore"
#endif
#endif

void HBUmeng::startup()
{
    NSString* key = [NSString stringWithFormat:@"%s", UMENG_KEY];
    NSString* channel = [NSString stringWithFormat:@"%s", UMENG_CHANNEL];
    [MobClick startWithAppkey:key reportPolicy:REALTIME channelId:channel];
    [MobClick checkUpdate];
}

void HBUmeng::event(const char* name, const char* value)
{
#if DEBUG
#else
    NSString* key = [NSString stringWithFormat:@"%s", name];
    
    if (value)
    {
        NSString* v = [NSString stringWithFormat:@"%s", value];
        [MobClick event:key label:v];
    }
    else
        [MobClick event:key];
#endif
}

void HBUmeng::updateConfig()
{
    [MobClick updateOnlineConfig];
}

int HBUmeng::getParamValue(const char* name)
{
    NSString* key = [NSString stringWithFormat:@"%s", name];
    NSString* value = [MobClick getConfigParams:key];
    
    if (value)
        return [value intValue];
    else
        return -1;
}
