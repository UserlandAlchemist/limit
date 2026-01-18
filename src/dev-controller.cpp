#include "dev-controller.h"

#include <algorithm>

namespace {
constexpr int kMidiMin = 0;
constexpr int kMidiMax = 127;
constexpr int kMidiCenter = 64;
constexpr int kDeltaDown = -1;
constexpr int kDeltaUp = 1;
} // namespace

namespace limit {
auto setDevEncoderBank(int bank_index, DevControllerState &state) -> bool {
  if (bank_index < 0 || bank_index >= kDevBankCount) {
    return false;
  }
  state.encoder_bank = bank_index;
  return true;
}

auto setDevPadBank(int bank_index, DevControllerState &state) -> bool {
  if (bank_index < 0 || bank_index >= kDevBankCount) {
    return false;
  }
  state.pad_bank = bank_index;
  return true;
}

auto handleDevEncoderAction(int encoder_index, DevEncoderAction action,
                            DevControllerState &state)
    -> std::optional<DevEncoderEvent> {
  if (encoder_index < 0 || encoder_index >= kDevEncoderCount) {
    return std::nullopt;
  }

  const auto encoder_slot = static_cast<std::size_t>(encoder_index);
  auto &value = state.encoders.at(encoder_slot);
  switch (action) {
  case DevEncoderAction::kDecrease:
    value = std::clamp(value + kDeltaDown, kMidiMin, kMidiMax);
    break;
  case DevEncoderAction::kIncrease:
    value = std::clamp(value + kDeltaUp, kMidiMin, kMidiMax);
    break;
  case DevEncoderAction::kReset:
    value = kMidiCenter;
    break;
  }

  DevEncoderEvent event;
  event.bank = state.encoder_bank;
  event.encoder_index = encoder_index;
  event.value = value;
  event.cc = state.encoder_bank * kDevEncoderCount + encoder_index;
  return event;
}

auto handleDevPadPress(int pad_index, DevControllerState &state)
    -> std::optional<DevPadEvent> {
  if (pad_index < 0 || pad_index >= kDevPadCount) {
    return std::nullopt;
  }

  DevPadEvent event;
  event.bank = state.pad_bank;
  event.pad_index = pad_index;
  return event;
}
} // namespace limit
