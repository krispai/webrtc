//
//  RTCAudioProcessing.mm
//
//  Created by Arthur Hayrapetyan on 26.01.23.
//  Copyright © 2023 Krisp Technologies. All rights reserved.
//

#include "RTCAudioProcessing.h"

@implementation RTC_OBJC_TYPE (RTCAudioProcessing) {

}

@synthesize audioProcessing = _audioProcessing;

- (instancetype)initWithModule:(unsigned long)processor {
  _audioProcessing = processor;

  return self;
}

@end
