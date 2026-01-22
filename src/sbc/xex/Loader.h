#ifndef sbc_xex_Loader_h
#define sbc_xex_Loader_h

#include "sbc/sio/FileSystem.h"

namespace sbc { namespace xex {

class Loader {
public:
  void run();

private:
  bool loadSegment(std::uint16_t index);

  template<std::uint16_t AddressV>
  bool segmentContainsAddress();

  template<std::uint16_t address>
  void callFnWithUndefinedCallingConvention();

  void onLoadStatus(const char* const message, std::uint16_t index);
  void onLoadError(const char* const message, std::uint16_t index);

  ::sbc::sio::FileSystemReadXexSegmentEntry m_readXexSegmentEntry;
};

} } // namespace sbc::xex

#endif // ifndef sbc_xex_Loader_h
