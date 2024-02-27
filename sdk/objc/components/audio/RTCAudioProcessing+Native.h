 //
 //  RTCAudioProcessing+native.h
 //
 //  Created by Arthur Hayrapetyan on 26.01.23.
 //  Copyright © 2023 Krisp Technologies. All rights reserved.
 //
#import <Foundation/Foundation.h>
#import "RTCMacros.h"

#include "modules/audio_processing/include/audio_processing.h"
#include "modules/audio_processing/audio_processing_impl.h"
#include "modules/audio_processing/audio_buffer.h"

typedef std::function<void()> ProcInitCallback;
typedef std::function<void(const int sampleRateHz, const int numChannels)> ProcSessionInitializeCallback;
typedef std::function<void()> ProcDestroyCallback;
typedef std::function<void()> ProcResetCallback;
typedef std::function<void(const size_t numChannel, const size_t numBands, const size_t bufferSize, float* _Nonnull buffer)> AudioFrameProcessCallback;

class CustomProcessingImpl : public webrtc::CustomProcessing {

  ProcInitCallback  initCB_;
  ProcSessionInitializeCallback  sessionInitCB_;
  ProcDestroyCallback destroyCB_;
  ProcResetCallback resetCB_;
  AudioFrameProcessCallback audioFrameProcessCB_;

  public:

    CustomProcessingImpl(ProcInitCallback initCB, ProcSessionInitializeCallback sessionInitCB,
                         ProcDestroyCallback  destroyCB, ProcResetCallback resetCB, AudioFrameProcessCallback audioFrameProcessCB)
        : initCB_(initCB), sessionInitCB_(sessionInitCB), destroyCB_(destroyCB), resetCB_(resetCB), audioFrameProcessCB_(audioFrameProcessCB)
  	{
        initCB_();
  	}

    ~CustomProcessingImpl()
    {
        destroyCB_();
    }

    void Reset()
    {
        resetCB_();
    }

    void Initialize(int sample_rate_hz, int num_channels) override
    {
        sessionInitCB_(sample_rate_hz, num_channels);
    }

    void Process(webrtc::AudioBuffer* __nullable audio) override {
        audioFrameProcessCB_(audio->num_channels(), audio->num_bands(), audio->num_frames(), audio->channels()[0]);
    }

    std::string ToString() const override {
  	  return "CustomProcessingImpl";
    }

    void SetRuntimeSetting(webrtc::AudioProcessing::RuntimeSetting setting) override {

    }
};
