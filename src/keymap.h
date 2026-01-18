#pragma once

namespace limit {
constexpr int kKeymapMinNote = 60;
constexpr int kKeymapMaxNote = 72;
auto mapKeyToMidiNote(int key_char) -> int;
}
