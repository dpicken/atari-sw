#ifndef sbc_sio_FileSystem_h
#define sbc_sio_FileSystem_h

#include "sio/Operation.h"
#include "sio/sdr/FileSystem.h"

namespace sbc { namespace sio {

static constexpr std::uint8_t FileSystemDeviceBusId = 0x81;

using FileSystemGetCurrentDir       = ::sio::OperationWithTypedDataTransfer<FileSystemDeviceBusId, 0x00, ::sio::DataTransfer::Read, ::sio::sdr::CurrentDirPath>;
using FileSystemSelectParentDir     = ::sio::Operation                     <FileSystemDeviceBusId, 0x01>;
using FileSystemReadDir             = ::sio::OperationWithTypedDataTransfer<FileSystemDeviceBusId, 0x02, ::sio::DataTransfer::Read, ::sio::sdr::DirEntryPage>;
using FileSystemSelectDirEntry      = ::sio::Operation                     <FileSystemDeviceBusId, 0x03>;
using FileSystemReadXexSegmentEntry = ::sio::OperationWithTypedDataTransfer<FileSystemDeviceBusId, 0x04, ::sio::DataTransfer::Read, ::sio::sdr::XexEntry>;
using FileSystemReadXexSegmentData  = ::sio::OperationWithDataTransfer     <FileSystemDeviceBusId, 0x05, ::sio::DataTransfer::Read, 30>;

} } // namespace sbc::sio

#endif // ifndef sbc_sio_FileSystem_h
