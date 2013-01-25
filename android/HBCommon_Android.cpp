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

    vector<string>::iterator itr;
    bool bFound = false;
    for (itr = mItemList.begin(); itr != mItemList.end(); ++itr)
    {
        if (*itr == itemName)
        {
            bFound = true;
            break;
        }
    }
    
    if (!bFound)
    {
        CCLog("%s item [%s] is out of bounds.", __FUNCTION__, itemName);
        return;
    }

    mTarget = target;
    mSuccessCall = successCall;
    mFailedCall = failedCall;

	::purchaseItem(itemName);
}

void HBPurchase::purchaseSuccess(const char* itemName)
{
    if (mTarget && mSuccessCall)
        (mTarget->*mSuccessCall)(CCString::create(itemName));
}

void HBPurchase::purchaseFailed(int errorCode, const char* failedReason)
{
    if (mTarget && mFailedCall)
        (mTarget->*mFailedCall)(CCString::create(failedReason));
}

#endif