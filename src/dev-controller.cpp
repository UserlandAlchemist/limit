#include "dev-controller.h"

#include <algorithm>

namespace {
struct EncoderKeyBinding {
  char key;
  int encoder_index;
  int delta;
};

constexpr int kEncoderIndex0 = 0;
constexpr int kEncoderIndex1 = 1;
constexpr int kEncoderIndex2 = 2;
constexpr int kEncoderIndex3 = 3;
constexpr int kEncoderIndex4 = 4;
constexpr int kEncoderIndex5 = 5;
constexpr int kDeltaDown = -1;
constexpr int kDeltaUp = 1;
constexpr int kMidiMin = 0;
constexpr int kMidiMax = 127;

constexpr std::array<EncoderKeyBinding, 12> kEncoderKeyBindings = {{
    {.key = '1', .encoder_index = kEncoderIndex0, .delta = kDeltaDown},
    {.key = '2', .encoder_index = kEncoderIndex0, .delta = kDeltaUp},
    {.key = '3', .encoder_index = kEncoderIndex1, .delta = kDeltaDown},
    {.key = '4', .encoder_index = kEncoderIndex1, .delta = kDeltaUp},
    {.key = '5', .encoder_index = kEncoderIndex2, .delta = kDeltaDown},
    {.key = '6', .encoder_index = kEncoderIndex2, .delta = kDeltaUp},
    {.key = '7', .encoder_index = kEncoderIndex3, .delta = kDeltaDown},
    {.key = '8', .encoder_index = kEncoderIndex3, .delta = kDeltaUp},
    {.key = '9', .encoder_index = kEncoderIndex4, .delta = kDeltaDown},
    {.key = '0', .encoder_index = kEncoderIndex4, .delta = kDeltaUp},
    {.key = '-', .encoder_index = kEncoderIndex5, .delta = kDeltaDown},
    {.key = '=', .encoder_index = kEncoderIndex5, .delta = kDeltaUp},
}};
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

auto handleDevEncoderKey(int key_char, DevControllerState &state)
    -> std::optional<DevEncoderEvent> {
  int encoder_index = -1;
  int delta = 0;
  for (const auto &binding : kEncoderKeyBindings) {
    if (binding.key == static_cast<char>(key_char)) {
      encoder_index = binding.encoder_index;
      delta = binding.delta;
      break;
    }
  }
  if (encoder_index < 0) {
    return std::nullopt;
  }

  const auto encoder_slot = static_cast<std::size_t>(encoder_index);
  auto &value = state.encoders.at(encoder_slot);
  value = std::clamp(value + delta, kMidiMin, kMidiMax);

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
