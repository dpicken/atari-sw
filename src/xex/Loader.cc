#include "Loader.h"

#include "sbc/sio/AtariControl.h"
#include "util/String.h"

#include <atari.h>

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
    onLoadError("entry", index);
    return false;
  }

  if (m_readXexSegmentEntry.data.eos()) {
    return false;
  }

  if (!::sbc::sio::FileSystemReadXexSegmentData::execute(
      reinterpret_cast<void*>(m_readXexSegmentEntry.data.segmentAddressBegin()),
      m_readXexSegmentEntry.data.segmentDataSize())) {
    onLoadError("data", index);
    return false;
  }

  return true;
}

template<typename FunctionPointerType>
bool xex::Loader::testExec(FunctionPointerType* fn) {
  if (m_readXexSegmentEntry.data.eos()) {
    return false;
  }

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

void xex::Loader::onLoadError(const char* const message, std::uint16_t index) {
  util::String<9>{"xex error"}.print();
  util::String<2>{": "}.print();
  util::String<5>{message}.print();
  util::String<2>{": "}.print();
  util::String<1>{static_cast<char>('0' + index)}.print();
  for (;;) {}
}
