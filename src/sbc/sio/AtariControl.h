#ifndef sbc_sio_AtariControl_h
#define sbc_sio_AtariControl_h

#include "sio/Operation.h"

namespace sbc { namespace sio {

static constexpr std::uint8_t AtariControlDeviceBusId = 0x80;

using AtariControlPowerOff      = ::sio::Operation<AtariControlDeviceBusId, 0x00>;
using AtariControlPowerCycle    = ::sio::Operation<AtariControlDeviceBusId, 0x01>;
using AtariControlReset         = ::sio::Operation<AtariControlDeviceBusId, 0x02>;

} } // namespace sbc::sio

#endif // ifndef sbc_sio_AtariControl_h
