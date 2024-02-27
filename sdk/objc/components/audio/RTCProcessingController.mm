//
//  RTCProcessingController.mm
//
//  Created by Arthur Hayrapetyan on 26.01.23.
//  Copyright © 2023 Krisp Technologies. All rights reserved.
//

#include "RTCProcessingController.h"
#include "RTCAudioProcessing.h"

#import "RTCAudioProcessing+Native.h"

@implementation RTC_OBJC_TYPE (RTCProcessingController) {
  std::unique_ptr<CustomProcessingImpl> _customProcessing;
  id<RTCAudioProcessorDelegate> _audioProcessorDelegate;

}

- (instancetype)initWithDelegate:(id<RTCAudioProcessorDelegate>)processorDelegate
{
  self = [super init];
  if (self != nil) {
      _audioProcessorDelegate = processorDelegate;

      ProcInitCallback initCallback = [self]() {
          if (_audioProcessorDelegate ) { [_audioProcessorDelegate initializeProcessor]; }
      };

      ProcSessionInitializeCallback sessionInitializeCallback = [self](const int sampleRateHz, const int numChannels) {
          if (_audioProcessorDelegate ) { [_audioProcessorDelegate initializeSession: sampleRateHz numChannels: numChannels]; }
      };

      ProcDestroyCallback destroyCallback = [self]() {
          if (_audioProcessorDelegate ) { [_audioProcessorDelegate destroyed]; }
      };

      ProcResetCallback resetCallback = [self]() {
          if (_audioProcessorDelegate ) { [_audioProcessorDelegate reset]; }
      };

      AudioFrameProcessCallback frameProcessCallback = [self](const size_t numChannel, const size_t numBands, const size_t bufferSize, float * _Nonnull buffer) {
          if (_audioProcessorDelegate ) { [_audioProcessorDelegate frameProcess: numChannel numBands:numBands  bufferSize: bufferSize  buffer: buffer]; }
      };

      _customProcessing = std::make_unique<CustomProcessingImpl>(initCallback, sessionInitializeCallback,
            destroyCallback, resetCallback, frameProcessCallback);
  }
  return self;
}

- (RTCAudioProcessing*)getProcessor
{
    auto audioProcessModule = webrtc::AudioProcessingBuilder()
         .SetCapturePostProcessing(std::move(_customProcessing))
         .Create();

    webrtc::AudioProcessing::Config config;
    config.echo_canceller.enabled = false;
    config.echo_canceller.mobile_mode = true;
    audioProcessModule->ApplyConfig(config);
    auto apm_ptr = audioProcessModule.release();

    RTCAudioProcessing* audioProcessing = [[RTCAudioProcessing alloc ] initWithModule: (unsigned long)apm_ptr];
    return audioProcessing;
}

@end
