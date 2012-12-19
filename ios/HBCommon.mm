#include "HBCommon.h"

const char* getLocalizationString(const char* str)
{
	NSString* strName = [NSString stringWithFormat:@"%s", str];
	strName = NSLocalizedString(strName, nil);
	return [strName cStringUsingEncoding:NSUTF8StringEncoding];
}