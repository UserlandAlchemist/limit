#include <array>

namespace limit {
constexpr int kBaseNote = 60;
constexpr std::array<char, 13> kKeyMap = {'a', 'w', 's', 'e', 'd', 'f', 't',
                                          'g', 'y', 'h', 'u', 'j', 'k'};

auto mapKeyToMidiNote(int key_char) -> int {
  int index = 0;
  for (const auto mapped_key : kKeyMap) {
    if (key_char == static_cast<int>(mapped_key)) {
      return kBaseNote + index;
    }
    ++index;
  }
  return -1;
}
} // namespace limit
