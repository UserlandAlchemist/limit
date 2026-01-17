#include <array>
#include <catch2/catch_test_macros.hpp>

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
      {.key = 'a', .note = 60},
      {.key = 'w', .note = 61},
      {.key = 's', .note = 62},
      {.key = 'e', .note = 63},
      {.key = 'd', .note = 64},
      {.key = 'f', .note = 65},
      {.key = 't', .note = 66},
      {.key = 'g', .note = 67},
      {.key = 'y', .note = 68},
      {.key = 'h', .note = 69},
      {.key = 'u', .note = 70},
      {.key = 'j', .note = 71},
      {.key = 'k', .note = 72},
  }};

  for (const auto &entry : cases) {
    REQUIRE(limit::mapKeyToMidiNote(entry.key) == entry.note);
  }

  REQUIRE(limit::mapKeyToMidiNote('z') == -1);
  // NOLINTEND(cppcoreguidelines-avoid-do-while)
}
