#ifndef sbc_sio_FileSystem_h
#define sbc_sio_FileSystem_h

#include "sio/Operation.h"
#include "sio/sdr/FileSystem.h"

namespace sbc { namespace sio {

static constexpr std::uint8_t FileSystemDeviceBusId = 0x81;

using FileSystemGetCurrentDir   = ::sio::OperationWithDataTransfer<FileSystemDeviceBusId, 0x00, ::sio::DataTransfer::Read, ::sio::sdr::Path>;
using FileSystemReadDir         = ::sio::OperationWithDataTransfer<FileSystemDeviceBusId, 0x01, ::sio::DataTransfer::Read, ::sio::sdr::DirEntryPage>;
using FileSystemSelectDirEntry  = ::sio::Operation<FileSystemDeviceBusId, 0x02>;

} } // namespace sbc::sio

#endif // ifndef sbc_sio_FileSystem_h
