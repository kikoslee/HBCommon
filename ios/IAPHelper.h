//
//  IAPHelper.h
//
//  Created by Limin on 12-12-03.
//  Copyright 2011年 HappyBluefin. All rights reserved.
//

#import "StoreKit/StoreKit.h"

#define kProductsLoadedNotification         @"ProductsLoaded"

@interface IAPHelper : NSObject <SKProductsRequestDelegate, SKPaymentTransactionObserver>

@property (nonatomic, retain) NSSet *productIdentifiers;
@property (nonatomic, retain) NSArray *products;
@property (nonatomic, retain) SKProductsRequest *request;

+ (IAPHelper*)shared;

- (void)initWithProductIdentifiers:(NSSet *)productIdentifiers;
- (void)requestProducts;
- (void)buyProductIdentifier:(NSString *)productIdentifier;
- (void)purchaseFailed:(NSError*)error;
- (NSString*)getItemCost:(NSString*)itemName;

@end
