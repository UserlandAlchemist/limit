#include <array>

namespace limit {
struct KeyNoteBinding {
  char key;
  int note;
};

constexpr std::array<KeyNoteBinding, 13> kKeyMap = {{
    {.key = 'g', .note = 60},  // C
    {.key = 'y', .note = 61},  // C#
    {.key = 'h', .note = 62},  // D
    {.key = 'u', .note = 63},  // D#
    {.key = 'j', .note = 64},  // E
    {.key = 'k', .note = 65},  // F
    {.key = 'o', .note = 66},  // F#
    {.key = 'l', .note = 67},  // G
    {.key = 'p', .note = 68},  // G#
    {.key = ';', .note = 69},  // A
    {.key = '[', .note = 70},  // A#
    {.key = '\'', .note = 71}, // B
    {.key = '\\', .note = 72}, // C
}};

auto mapKeyToMidiNote(int key_char) -> int {
  for (const auto &binding : kKeyMap) {
    if (key_char == static_cast<int>(binding.key)) {
      return binding.note;
    }
  }
  return -1;
}
} // namespace limit
