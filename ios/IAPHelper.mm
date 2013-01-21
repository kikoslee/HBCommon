//
//  IAPHelper.m
//  Lance
//
//  Created by Limin on 12-12-03.
//  Copyright 2011年 HappyBluefin. All rights reserved.
//

#import "IAPHelper.h"
#include "HBCommon.h"

@implementation IAPHelper

@synthesize productIdentifiers = _productIdentifiers;
@synthesize products = _products;
@synthesize request = _request;

static IAPHelper* _sharedIAPHelper;

+ (IAPHelper*)shared
{
	if (_sharedIAPHelper == nil) {
		_sharedIAPHelper = [[IAPHelper alloc] init];
	}
	return _sharedIAPHelper;
}

- (void)dealloc
{
	[_productIdentifiers release];
	[_products release];
	[_request release];
	[super dealloc];
}

- (void)initWithProductIdentifiers:(NSSet*)productIdentifiers
{
    self.productIdentifiers = productIdentifiers;
    [self requestProducts];
    [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
}

- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response
{
	self.products = response.products;
	self.request = nil;
    
    NSLog(@"[IAP - products Result]");
    int i = 1;
    for (SKProduct* p in response.products)
    {
        NSLog(@"[IAP - %d]", i);
        NSLog(@"  [Identifier]:%@", p.productIdentifier);
        NSNumberFormatter *numberFormatter = [[NSNumberFormatter alloc] init];
        [numberFormatter setFormatterBehavior:NSNumberFormatterBehavior10_4];
        [numberFormatter setNumberStyle:NSNumberFormatterCurrencyStyle];
        [numberFormatter setLocale:p.priceLocale];
        NSString *formattedString = [numberFormatter stringFromNumber:p.price];
        [numberFormatter release];
        NSLog(@"  [Price]:%@", formattedString);
        NSLog(@"  [LocalizedTitle]:%@", p.localizedTitle);
        NSLog(@"  [LocalizedDescription]:%@", p.localizedDescription);
        i++;
    }
    for (NSString* p in response.invalidProductIdentifiers)
    {
        NSLog(@"[IAP Invalid - %d]", i);
        NSLog(@"  [Identifier]:%@", p);
        i++;
    }
	
    [[NSNotificationCenter defaultCenter] postNotificationName:kProductsLoadedNotification object:_products userInfo:nil];
}

- (void)requestProducts
{
    NSLog(@"[IAP - productsRequest %@]", _productIdentifiers);        
	SKProductsRequest* pr = [[SKProductsRequest alloc] initWithProductIdentifiers:_productIdentifiers];
	self.request = pr;
	[pr release];
	_request.delegate = self;
	[_request start];
}

- (NSString*)getItemCost:(NSString*)itemName
{
	for(SKProduct* product in self.products)
	{
        if ([itemName isEqualToString:product.productIdentifier])
        {
            NSNumberFormatter *numberFormatter = [[NSNumberFormatter alloc] init];
            [numberFormatter setFormatterBehavior:NSNumberFormatterBehavior10_4];
            [numberFormatter setNumberStyle:NSNumberFormatterCurrencyStyle];
            [numberFormatter setLocale:product.priceLocale];
            NSString *formattedString = [numberFormatter stringFromNumber:product.price];
            [numberFormatter release];
            return formattedString;
        }
	}
	return nil;
}

- (void)recordTransaction:(SKPaymentTransaction *)transaction
{
	// Optional: Record the transaction on the server side...    
}

- (void)provideContent:(NSString *)productIdentifier myData:(NSData*)productData
{
    HBPurchase::shared()->purchaseSuccess([productIdentifier cStringUsingEncoding:NSASCIIStringEncoding]);
}

- (void)completeTransaction:(SKPaymentTransaction *)transaction
{
	[self recordTransaction: transaction];
	[self provideContent:transaction.payment.productIdentifier myData:transaction.transactionReceipt];
	[[SKPaymentQueue defaultQueue] finishTransaction: transaction];	
}

- (void)restoreTransaction:(SKPaymentTransaction *)transaction
{
	[self recordTransaction: transaction];
	[self provideContent: transaction.originalTransaction.payment.productIdentifier myData:transaction.transactionReceipt];
	[[SKPaymentQueue defaultQueue] finishTransaction: transaction];	
}

-(void)purchaseFailed:(NSError*)error
{
//    NSLog(@"purchaseFailed:");
//    NSLog(@"   localizedDescription [%@]", error.localizedDescription);
//    NSLog(@"   localizedFailureReason [%@]", error.localizedFailureReason);
//    NSLog(@"   localizedRecoveryOptions [%@]", error.localizedRecoveryOptions);
//    NSLog(@"   localizedRecoverySuggestion [%@]", error.localizedRecoverySuggestion);
    if (error.localizedDescription)
        HBPurchase::shared()->purchaseFailed(error.code, [error.localizedDescription cStringUsingEncoding:NSUTF8StringEncoding]);
    else
        HBPurchase::shared()->purchaseFailed(error.code, "Purchase Failed");
}

- (void)failedTransaction:(SKPaymentTransaction *)transaction
{
	[self purchaseFailed:transaction.error];
	[[SKPaymentQueue defaultQueue] finishTransaction: transaction];
}

- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions
{
	for (SKPaymentTransaction *transaction in transactions)
	{
		switch (transaction.transactionState)
		{
			case SKPaymentTransactionStatePurchased:
				[self completeTransaction:transaction];
				break;
			case SKPaymentTransactionStateFailed:
				[self failedTransaction:transaction];
				break;
			case SKPaymentTransactionStateRestored:
				[self restoreTransaction:transaction];
			default:
				break;
		}
	}
}

- (void)buyProductIdentifier:(NSString *)productIdentifier
{
	SKPayment *payment = [SKPayment paymentWithProductIdentifier:productIdentifier];
	[[SKPaymentQueue defaultQueue] addPayment:payment];    
}

@end
