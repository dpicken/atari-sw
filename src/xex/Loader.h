#ifndef xex_loader_h
#define xex_loader_h

#include "sbc/sio/FileSystem.h"

namespace xex {

class Loader {
public:
  static Loader* instance();

  void run();

private:
  bool loadSegment(std::uint16_t index);

  template<typename FunctionPointerType>
  bool testExec(FunctionPointerType* fn);

  ::sbc::sio::FileSystemReadXexSegmentEntry m_readXexSegmentEntry;
};

} // namespace xex

#endif // ifndef xex_loader_h
