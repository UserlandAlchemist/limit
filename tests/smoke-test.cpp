#include <catch2/catch_test_macros.hpp>

#include "keymap.h"

TEST_CASE("smoke", "[limit]") {
  // NOLINTBEGIN(cppcoreguidelines-avoid-do-while)
  REQUIRE(true);
  // NOLINTEND(cppcoreguidelines-avoid-do-while)
}

TEST_CASE("key map", "[limit]") {
  // NOLINTBEGIN(cppcoreguidelines-avoid-do-while)
  REQUIRE(limit::mapKeyToMidiNote('a') == 60);
  REQUIRE(limit::mapKeyToMidiNote('w') == 61);
  REQUIRE(limit::mapKeyToMidiNote('s') == 62);
  REQUIRE(limit::mapKeyToMidiNote('e') == 63);
  REQUIRE(limit::mapKeyToMidiNote('d') == 64);
  REQUIRE(limit::mapKeyToMidiNote('f') == 65);
  REQUIRE(limit::mapKeyToMidiNote('t') == 66);
  REQUIRE(limit::mapKeyToMidiNote('g') == 67);
  REQUIRE(limit::mapKeyToMidiNote('y') == 68);
  REQUIRE(limit::mapKeyToMidiNote('h') == 69);
  REQUIRE(limit::mapKeyToMidiNote('u') == 70);
  REQUIRE(limit::mapKeyToMidiNote('j') == 71);
  REQUIRE(limit::mapKeyToMidiNote('k') == 72);
  REQUIRE(limit::mapKeyToMidiNote('z') == -1);
  // NOLINTEND(cppcoreguidelines-avoid-do-while)
}
