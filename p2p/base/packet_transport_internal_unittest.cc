/*
 *  Copyright 2024 The WebRTC Project Authors. All rights reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "p2p/base/packet_transport_internal.h"

#include "p2p/base/fake_packet_transport.h"
#include "rtc_base/gunit.h"
#include "rtc_base/network/received_packet.h"
#include "rtc_base/third_party/sigslot/sigslot.h"
#include "test/gmock.h"

namespace {

using ::testing::MockFunction;

class SigslotPacketReceiver : public sigslot::has_slots<> {
 public:
  bool packet_received() const { return packet_received_; }

  void OnPacketReceived(rtc::PacketTransportInternal*,
                        const char*,
                        size_t,
                        const int64_t&,
                        int flags) {
    packet_received_ = true;
    flags_ = flags;
  }

  bool packet_received_ = false;
  int flags_ = -1;
};

TEST(PacketTransportInternal,
     PacketFlagsCorrectWithUnDecryptedPacketUsingSigslot) {
  rtc::FakePacketTransport packet_transport("test");
  SigslotPacketReceiver receiver;
  packet_transport.SignalReadPacket.connect(
      &receiver, &SigslotPacketReceiver::OnPacketReceived);

  packet_transport.NotifyPacketReceived(
      rtc::ReceivedPacket({}, rtc::SocketAddress(), absl::nullopt,
                          rtc::ReceivedPacket::kNotDecrypted));
  ASSERT_TRUE(receiver.packet_received_);
  EXPECT_EQ(receiver.flags_, 0);
}

TEST(PacketTransportInternal, PacketFlagsCorrectWithSrtpPacketUsingSigslot) {
  rtc::FakePacketTransport packet_transport("test");
  SigslotPacketReceiver receiver;
  packet_transport.SignalReadPacket.connect(
      &receiver, &SigslotPacketReceiver::OnPacketReceived);

  packet_transport.NotifyPacketReceived(
      rtc::ReceivedPacket({}, rtc::SocketAddress(), absl::nullopt,
                          rtc::ReceivedPacket::kSrtpEncrypted));
  ASSERT_TRUE(receiver.packet_received_);
  EXPECT_EQ(receiver.flags_, 1);
}

TEST(PacketTransportInternal, PacketFlagsCorrectWithDtlsDecryptedUsingSigslot) {
  rtc::FakePacketTransport packet_transport("test");
  SigslotPacketReceiver receiver;
  packet_transport.SignalReadPacket.connect(
      &receiver, &SigslotPacketReceiver::OnPacketReceived);

  packet_transport.NotifyPacketReceived(
      rtc::ReceivedPacket({}, rtc::SocketAddress(), absl::nullopt,
                          rtc::ReceivedPacket::kDtlsDecrypted));
  ASSERT_TRUE(receiver.packet_received_);
  EXPECT_EQ(receiver.flags_, 0);
}

TEST(PacketTransportInternal,
     NotifyPacketReceivedPassThrougPacketToRegisterListener) {
  rtc::FakePacketTransport packet_transport("test");
  MockFunction<void(rtc::PacketTransportInternal*, const rtc::ReceivedPacket&)>
      receiver;

  packet_transport.RegisterReceivedPacketCallback(&receiver,
                                                  receiver.AsStdFunction());
  EXPECT_CALL(receiver, Call)
      .WillOnce(
          [](rtc::PacketTransportInternal*, const rtc::ReceivedPacket& packet) {
            EXPECT_EQ(packet.decryption_info(),
                      rtc::ReceivedPacket::kDtlsDecrypted);
          });
  packet_transport.NotifyPacketReceived(
      rtc::ReceivedPacket({}, rtc::SocketAddress(), absl::nullopt,
                          rtc::ReceivedPacket::kDtlsDecrypted));

  packet_transport.DeregisterReceivedPacketCallback(&receiver);
}

}  // namespace
