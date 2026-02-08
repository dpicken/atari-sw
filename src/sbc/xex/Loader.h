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

  ::sbc::sio::FileSystemReadXexSegmentEntry m_readXexSegmentEntry;
};

} } // namespace sbc::xex

#endif // ifndef sbc_xex_Loader_h
