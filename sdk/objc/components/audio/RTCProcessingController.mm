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

      ProcessorInitCallback initCallback = [self]() {
          if (_audioProcessorDelegate ) { [_audioProcessorDelegate initializeProcessor]; }
      };

      ProcessorInitializeSessionCallback sessionInitializeCallback = [self](int sample_rate_hz, int num_channels) {
          if (_audioProcessorDelegate ) { [_audioProcessorDelegate initializeSession: sample_rate_hz numChannels: num_channels]; }
      };

      ProcessorDestroyCallback destroyCallback = [self]() {
          if (_audioProcessorDelegate ) { [_audioProcessorDelegate destroyed]; }
      };

      ProcessorResetCallback resetCallback = [self]() {
          if (_audioProcessorDelegate ) { [_audioProcessorDelegate reset]; }
      };

      AudioFrameProcessCallback frameProcessCallback = [self](const size_t channelNumber, const size_t num_bands, const size_t bufferSize, float * _Nonnull buffer) {
          if (_audioProcessorDelegate ) { [_audioProcessorDelegate frameProcess: channelNumber numBands:num_bands  bufferSize: bufferSize  buffer: buffer]; }
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
