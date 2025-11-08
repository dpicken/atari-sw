#ifndef xex_loader_h
#define xex_loader_h

#include "sbc/sio/FileSystem.h"

namespace xex {

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

} // namespace xex

#endif // ifndef xex_loader_h
