#ifndef SDK_OBJC_NATIVE_API_AUDIO_PROCESSING_MODULE_H_
#define SDK_OBJC_NATIVE_API_AUDIO_PROCESSING_MODULE_H_

#import "components/audio//RTCAudioProcessing.h"
#include "modules/audio_processing/include/audio_processing.h"

namespace webrtc {
  rtc::scoped_refptr<AudioProcessing> ObjCToNativeAudioProcessingModule(RTCAudioProcessing* objc_audio_processing_module);
}  // namespace webrtc

#endif  // SDK_OBJC_NATIVE_API_AUDIO_PROCESSING_MODULE_H_
