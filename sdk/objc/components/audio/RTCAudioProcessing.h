//
//  RTCAudioProcessing.h
//
//  Created by Arthur Hayrapetyan on 26.01.23.
//  Copyright © 2023 Krisp Technologies. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "RTCMacros.h"

NS_ASSUME_NONNULL_BEGIN

RTC_OBJC_EXPORT
@interface RTC_OBJC_TYPE (RTCAudioProcessing) : NSObject

- (instancetype)initWithModule:(unsigned long)processor;

@property(nonatomic, assign) unsigned long audioProcessing;

@end

NS_ASSUME_NONNULL_END
