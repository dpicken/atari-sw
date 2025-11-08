#include "Loader.h"

#include "sbc/sio/AtariControl.h"
#include "util/Debug.h"
#include "util/String.h"

#include <atari.h>

void xex::Loader::run() {
  constexpr auto initad_v = offsetof(__os, initad);
  constexpr auto runad_v = offsetof(__os, runad);

  auto callRunAddress = false;
  for (std::uint16_t index = 0; loadSegment(index); ++index) {
    DEBUG_ONLY(onLoadStatus("loaded", index));
    if (segmentContainsAddress<initad_v>()) {
      DEBUG_ONLY(onLoadStatus("initad", index));
      callFnWithUndefinedCallingConvention<initad_v>();
    }
    callRunAddress = callRunAddress || segmentContainsAddress<runad_v>();
  }
  if (callRunAddress) {
    DEBUG_ONLY(onLoadStatus("runad", 0));
    callFnWithUndefinedCallingConvention<runad_v>();
  }
  DEBUG_ONLY(onLoadStatus("reset", 0));
  ::sbc::sio::AtariControlReset::execute();
}

bool xex::Loader::loadSegment(std::uint16_t index) {
  if (!m_readXexSegmentEntry.execute(index)) {
    DEBUG_ONLY(onLoadError("e-err", index));
    return false;
  }

  if (m_readXexSegmentEntry.data.eos()) {
    return false;
  }

  if (!::sbc::sio::FileSystemReadXexSegmentData::execute(
      reinterpret_cast<void*>(m_readXexSegmentEntry.data.segmentAddressBegin()),
      m_readXexSegmentEntry.data.segmentDataSize())) {
    DEBUG_ONLY(onLoadError("d-err", index));
    return false;
  }

  return true;
}

template<std::uint16_t AddressV>
bool xex::Loader::segmentContainsAddress() {
  if (AddressV > m_readXexSegmentEntry.data.segmentAddressLast()) {
    return false;
  }

  constexpr auto addressLast = AddressV + 1;
  if (addressLast < m_readXexSegmentEntry.data.segmentAddressBegin()) {
    return false;
  }

  return true;
}

template<std::uint16_t AddressV>
void xex::Loader::Loader::callFnWithUndefinedCallingConvention() {
  asm volatile(
    // TODO: Save a snapshot of llvm-mos's zero page environment.
    // TODO: Restore any snapshot of the application's zero page environment?
    "LDX %[AddressV]\n"
    "STX __rc18\n"
    "LDX %[AddressV] + 1\n"
    "STX __rc19\n"
    "JSR __call_indir\n"
    // TODO: Save a snapshot of the application's zero page environment?
    // TODO: Restore snapshot of llvm-mos's zero page environment.
    /*output*/:
    /*input*/: [AddressV] "i" (AddressV)
    /*clobber*/: "a", "x", "y", "rs0", "rs10", "rs11", "rs12", "rs13", "rs14", "rs15"
  );
}

void xex::Loader::onLoadStatus(const char* const message, std::uint16_t index) {
  util::String<6>{message}.print();
  util::String<2>{": "}.print();
  util::String<1>{static_cast<char>('0' + index)}.printLine();
}

void xex::Loader::onLoadError(const char* const message, std::uint16_t index) {
  onLoadError(message, index);
  for (;;) {}
}
