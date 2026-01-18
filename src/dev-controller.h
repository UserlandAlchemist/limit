#pragma once

#include <array>
#include <optional>

namespace limit {
constexpr int kDevEncoderCount = 6;
constexpr int kDevBankCount = 3;
constexpr int kDevPadCount = 16;

struct DevControllerState {
  int encoder_bank = 0;
  int pad_bank = 0;
  std::array<int, kDevEncoderCount> encoders{};
};

struct DevEncoderEvent {
  int bank = 0;
  int encoder_index = 0;
  int value = 0;
  int cc = 0;
};

struct DevPadEvent {
  int bank = 0;
  int pad_index = 0;
};

auto setDevEncoderBank(int bank_index, DevControllerState &state) -> bool;
auto setDevPadBank(int bank_index, DevControllerState &state) -> bool;
auto handleDevEncoderKey(int key_char, DevControllerState &state)
    -> std::optional<DevEncoderEvent>;
auto handleDevPadPress(int pad_index, DevControllerState &state)
    -> std::optional<DevPadEvent>;
} // namespace limit
