#include "audio_processing_module.h"

#include "api/make_ref_counted.h"
#include "rtc_base/logging.h"

namespace webrtc {

  rtc::scoped_refptr<AudioProcessing> ObjCToNativeAudioProcessingModule(RTCAudioProcessing* objc_audio_processing_module) {
    RTC_DLOG(LS_INFO) << __FUNCTION__;
    unsigned long audioProcessor = [objc_audio_processing_module audioProcessing];
    AudioProcessing* audioProcesSmartPointer = reinterpret_cast<AudioProcessing*>(audioProcessor);
    return rtc::scoped_refptr<AudioProcessing>(audioProcesSmartPointer);
  }
}  // namespace webrtc
