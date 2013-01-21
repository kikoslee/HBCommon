#include "HBCommon.h"
#import "HBGameCenter.h"
#import "IAPHelper.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

void gotoReview()
{
    NSString *str = [NSString stringWithFormat:@"itms-apps://ax.itunes.apple.com/WebObjects/MZStore.woa/wa/viewContentsUserReviews?type=Purple+Software&id=%s", kAppIDiOS];
	[[UIApplication sharedApplication] openURL:[NSURL URLWithString:str]];
}

void gotoMoreGame()
{
    gotoUrl("itms-apps://itunes.com/apps/limin");
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
// HBPurchase
//////////////////////////////////////////////////////////

HBPurchase::HBPurchase()
: mInitialized(false)
, mTarget(NULL)
, isPurchasing(false)
, mSuccessCall(NULL)
, mFailedCall(NULL)
{
    
}

void HBPurchase::init(const vector<string>& itemList)
{
    mItemList = itemList;
    NSMutableSet* p = [[NSMutableSet alloc] init];
    
    vector<string>::const_iterator itr;
    for (itr = itemList.begin(); itr != itemList.end(); ++itr)
        [p addObject:[NSString stringWithFormat:@"%s", (*itr).c_str()]];

    [[IAPHelper shared] initWithProductIdentifiers:p];
    [p release];
    mInitialized = true;
}

string HBPurchase::getItemCost(int index)
{
    CCAssert(mInitialized, "HBPurchase must be initialize first.");
    if (index >= mItemList.size())
    {
        CCLog("%s item index[%d] is out of bounds.", __FUNCTION__, index);
        return "";
    }

    return getItemCost(mItemList[index].c_str());
}

string HBPurchase::getItemCost(const char* itemName)
{
    CCAssert(mInitialized, "HBPurchase must be initialize first.");
    NSString* cost = [[IAPHelper shared] getItemCost:[NSString stringWithFormat:@"%s", itemName]];
    if (cost)
    {
        string str = [cost cStringUsingEncoding:NSUTF8StringEncoding];
        return str;
    }
    
    return "";
}

void HBPurchase::purchaseItem(int index, CCObject* target, SEL_CallFuncO successCall, SEL_CallFuncO failedCall)
{
    CCAssert(mInitialized, "HBPurchase must be initialize first.");
    if (index >= mItemList.size())
    {
        CCLog("%s item index[%d] is out of bounds.", __FUNCTION__, index);
        return;
    }
    
    purchaseItem(mItemList[index].c_str(), target, successCall, failedCall);
}

void HBPurchase::purchaseItem(const char* itemName, CCObject* target, SEL_CallFuncO successCall, SEL_CallFuncO failedCall)
{
    CCAssert(mInitialized, "HBPurchase must be initialize first.");
    if (find(mItemList.begin(), mItemList.end(), itemName) == mItemList.end())
    {
        CCLog("%s item [%s] is out of bounds.", __FUNCTION__, itemName);
        return;
    }
    
    mTarget = target;
    mSuccessCall = successCall;
    mFailedCall = failedCall;

    [[IAPHelper shared] buyProductIdentifier:[NSString stringWithFormat:@"%s", itemName]];
}

void HBPurchase::purchaseSuccess(const char* itemName)
{
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Success" message:@"Purchase Success" delegate:nil cancelButtonTitle:nil otherButtonTitles:@"Close", nil];
    [alert show];
    [alert release];
    
    if (mTarget && mSuccessCall)
        (mTarget->*mSuccessCall)(CCString::create(itemName));
}

void HBPurchase::purchaseFailed(int errorCode, const char* failedReason)
{
//	if (errorCode != SKErrorPaymentCancelled)
//    {
//		UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Error!" message:[NSString stringWithFormat:@"%s", failedReason] delegate:nil cancelButtonTitle:nil otherButtonTitles:@"Close", nil];
//		[alert show];
//        [alert release];
//	}

    if (mTarget && mFailedCall)
        (mTarget->*mFailedCall)(CCString::create(failedReason));
}


//////////////////////////////////////////////////////////
// HBUmeng
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

const char* HBGetDeviceLanguage_iOS()
{
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    NSArray *languages = [defaults objectForKey:@"AppleLanguages"];
    NSString *currentLanguage = [languages objectAtIndex:0];
    
    return [currentLanguage UTF8String];
}

#endif