#ifndef sio_Operation_h
#define sio_Operation_h

#include "util/type_traits.h"

#include <atari.h>

#include <cstdint>
#include <cstdio>
#include <utility>

extern "C" std::uint8_t sys_sio();

namespace sio {

static constexpr std::uint8_t DefaultUnitNumber = 1;
static constexpr std::uint8_t DefaultTimeoutSecondsCount = 2;

enum class DataTransfer : std::uint8_t {
  None  = 0x00,
  Read  = 0x40,
  Write = 0x80
};

template<std::uint8_t DeviceBusIdV, std::uint8_t CommandV, DataTransfer DataTransferV, std::uint8_t TimeoutSecondsCountV>
struct OperationBase {
  static const char* makeStatusMessage(const char* prefix) {
    static char messageBuffer[36];
    snprintf(messageBuffer, sizeof(messageBuffer), "%s: %hhu", prefix, OS.dcb.dstats);
    return messageBuffer;
  }

protected:
  static constexpr std::uint8_t status_success_v = 0x01;

  static bool execute(std::uint16_t aux, std::uint8_t unitNumber) {
    OS.dcb.ddevic = DeviceBusIdV;
    OS.dcb.dunit = unitNumber;
    OS.dcb.dcomnd = CommandV;
    OS.dcb.dstats = util::to_underlying(DataTransferV);
    OS.dcb.dtimlo = TimeoutSecondsCountV;
    OS.dcb.daux = aux;
    sys_sio();
    return OS.dcb.dstats == status_success_v;
  }
};

template<std::uint8_t DeviceBusIdV, std::uint8_t CommandV, std::uint8_t TimeoutSecondsCountV = DefaultTimeoutSecondsCount>
struct Operation : OperationBase<DeviceBusIdV, CommandV, DataTransfer::None, TimeoutSecondsCountV> {
  using base_type = OperationBase<DeviceBusIdV, CommandV, DataTransfer::None, TimeoutSecondsCountV>;

  static bool execute(std::uint16_t aux = 0, std::uint8_t unitNumber = DefaultUnitNumber) {
    return base_type::execute(aux, unitNumber);
  }
};

template<std::uint8_t DeviceBusIdV, std::uint8_t CommandV, DataTransfer DataTransferV, typename BufferT, std::uint8_t TimeoutSecondsCountV = DefaultTimeoutSecondsCount>
struct OperationWithDataTransfer : OperationBase<DeviceBusIdV, CommandV, DataTransferV, TimeoutSecondsCountV> {
  using base_type = OperationBase<DeviceBusIdV, CommandV, DataTransferV, TimeoutSecondsCountV>;
  using buffer_type = BufferT;

  buffer_type data;

  bool execute(std::uint16_t aux = 0, std::uint8_t unitNumber = DefaultUnitNumber) {
    static_assert (DataTransferV != DataTransfer::None);
    OS.dcb.dbuf = &data;
    OS.dcb.dbyt = sizeof(data);
    return base_type::execute(aux, unitNumber);
  }
};

} // namespace sio

#endif // ifndef sio_Operation_h
