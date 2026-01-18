#include <array>
#include <catch2/catch_test_macros.hpp>

#include "dev-controller.h"
#include "keymap.h"

TEST_CASE("smoke", "[limit]") {
  // NOLINTBEGIN(cppcoreguidelines-avoid-do-while)
  REQUIRE(true);
  // NOLINTEND(cppcoreguidelines-avoid-do-while)
}

TEST_CASE("key map", "[limit]") {
  // NOLINTBEGIN(cppcoreguidelines-avoid-do-while)
  struct KeyCase {
    char key;
    int note;
  };
  const std::array<KeyCase, 13> cases = {{
      {.key = 'g', .note = 60},
      {.key = 'y', .note = 61},
      {.key = 'h', .note = 62},
      {.key = 'u', .note = 63},
      {.key = 'j', .note = 64},
      {.key = 'k', .note = 65},
      {.key = 'o', .note = 66},
      {.key = 'l', .note = 67},
      {.key = 'p', .note = 68},
      {.key = ';', .note = 69},
      {.key = '[', .note = 70},
      {.key = '\'', .note = 71},
      {.key = '\\', .note = 72},
  }};

  for (const auto &entry : cases) {
    REQUIRE(limit::mapKeyToMidiNote(entry.key) == entry.note);
  }

  REQUIRE(limit::mapKeyToMidiNote('z') == -1);
  // NOLINTEND(cppcoreguidelines-avoid-do-while)
}

TEST_CASE("dev controller encoder mapping", "[limit]") {
  // NOLINTBEGIN(cppcoreguidelines-avoid-do-while)
  limit::DevControllerState state;
  REQUIRE(state.encoder_bank == 0);
  REQUIRE(state.pad_bank == 0);

  REQUIRE(limit::setDevEncoderBank(1, state));
  REQUIRE(state.encoder_bank == 1);
  REQUIRE_FALSE(limit::setDevEncoderBank(3, state));
  REQUIRE(state.encoder_bank == 1);

  auto event = limit::handleDevEncoderAction(0, limit::DevEncoderAction::kIncrease, state);
  const auto first_event = event.value_or(limit::DevEncoderEvent{});
  REQUIRE(event.has_value());
  REQUIRE(first_event.bank == 1);
  REQUIRE(first_event.encoder_index == 0);
  REQUIRE(first_event.value == 1);
  REQUIRE(first_event.cc == 6);

  event = limit::handleDevEncoderAction(0, limit::DevEncoderAction::kReset, state);
  const auto second_event = event.value_or(limit::DevEncoderEvent{});
  REQUIRE(event.has_value());
  REQUIRE(second_event.value == 64);

  event = limit::handleDevEncoderAction(3, limit::DevEncoderAction::kDecrease, state);
  const auto third_event = event.value_or(limit::DevEncoderEvent{});
  REQUIRE(event.has_value());
  REQUIRE(third_event.encoder_index == 3);

  event = limit::handleDevEncoderAction(-1, limit::DevEncoderAction::kIncrease, state);
  REQUIRE_FALSE(event.has_value());
  // NOLINTEND(cppcoreguidelines-avoid-do-while)
}

TEST_CASE("dev controller pad mapping", "[limit]") {
  // NOLINTBEGIN(cppcoreguidelines-avoid-do-while)
  limit::DevControllerState state;
  REQUIRE(state.pad_bank == 0);
  REQUIRE(limit::setDevPadBank(2, state));
  REQUIRE(state.pad_bank == 2);
  REQUIRE_FALSE(limit::setDevPadBank(-1, state));
  REQUIRE(state.pad_bank == 2);

  auto event = limit::handleDevPadPress(0, state);
  const auto first_pad = event.value_or(limit::DevPadEvent{});
  REQUIRE(event.has_value());
  REQUIRE(first_pad.bank == 2);
  REQUIRE(first_pad.pad_index == 0);

  event = limit::handleDevPadPress(limit::kDevPadCount - 1, state);
  const auto last_pad = event.value_or(limit::DevPadEvent{});
  REQUIRE(event.has_value());
  REQUIRE(last_pad.pad_index == limit::kDevPadCount - 1);

  event = limit::handleDevPadPress(limit::kDevPadCount, state);
  REQUIRE_FALSE(event.has_value());
  // NOLINTEND(cppcoreguidelines-avoid-do-while)
}
