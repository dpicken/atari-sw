#include "Loader.h"

#include "call_ad.h"

#include "sbc/sio/AtariControl.h"

#include <atari.h>

void sbc::xex::Loader::run() {
  constexpr auto initad_v = offsetof(__os, initad);
  constexpr auto runad_v = offsetof(__os, runad);

  auto callRunAddress = false;
  for (std::uint16_t index = 0; loadSegment(index); ++index) {
    if (segmentContainsAddress<initad_v>()) {
      call_initad();
    }
    callRunAddress = callRunAddress || segmentContainsAddress<runad_v>();
  }
  if (callRunAddress) {
    call_runad();
  }
  ::sbc::sio::AtariControlReset::execute();
}

bool sbc::xex::Loader::loadSegment(std::uint16_t index) {
  if (!m_readXexSegmentEntry.execute(index)) {
    return false;
  }

  if (m_readXexSegmentEntry.data.eos()) {
    return false;
  }

  if (!::sbc::sio::FileSystemReadXexSegmentData::execute(
      reinterpret_cast<void*>(m_readXexSegmentEntry.data.segmentAddressBegin()),
      m_readXexSegmentEntry.data.segmentDataSize())) {
    return false;
  }

  return true;
}

template<std::uint16_t AddressV>
bool sbc::xex::Loader::segmentContainsAddress() {
  if (AddressV > m_readXexSegmentEntry.data.segmentAddressLast()) {
    return false;
  }

  constexpr auto addressLast = AddressV + 1;
  if (addressLast < m_readXexSegmentEntry.data.segmentAddressBegin()) {
    return false;
  }

  return true;
}
