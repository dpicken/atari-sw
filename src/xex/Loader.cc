#include "Loader.h"

#include "sbc/sio/AtariControl.h"

#include <atari.h>

xex::Loader* xex::Loader::instance() {
  return nullptr;
}

void xex::Loader::run() {
  auto execRunAddress = false;
  for (std::uint16_t index = 0; loadSegment(index); ++index) {
    if (testExec(&OS.initad)) {
      OS.initad();
    }
    execRunAddress = execRunAddress || testExec(&OS.runad);
  }
  if (execRunAddress) {
    OS.runad();
  }
  ::sbc::sio::AtariControlReset::execute();
}

bool xex::Loader::loadSegment(std::uint16_t index) {
  if (!m_readXexSegmentEntry.execute(index)) {
    return false;
  }

  if (!::sbc::sio::FileSystemReadXexSegmentData::execute(
      reinterpret_cast<void*>(m_readXexSegmentEntry.data.segmentAddressBegin()),
      m_readXexSegmentEntry.data.segmentDataSize(),
      m_readXexSegmentEntry.data.index())) {
    return false;
  }

  return true;
}

template<typename FunctionPointerType>
bool xex::Loader::testExec(FunctionPointerType* fn) {
  auto fnAddressBegin = reinterpret_cast<std::uint16_t>(fn);
  if (fnAddressBegin > m_readXexSegmentEntry.data.segmentAddressLast()) {
    return false;
  }

  auto fnAddressEnd = reinterpret_cast<std::uint16_t>(fn + 1);
  if (fnAddressEnd <= m_readXexSegmentEntry.data.segmentAddressBegin()) {
    return false;
  }

  return true;
}
