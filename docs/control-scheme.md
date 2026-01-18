# Control Scheme (Dev Keyboard + Target Controllers)

This document describes the current control scheme for Limit and its keyboard
emulation. Keep this file in sync with the input mapping code in
`src/limit-app.cpp` and `src/dev-controller.cpp`.

## Target Controllers (Planned)

- Korg nanoKEY2: 25 keys for melodic input.
- Akai MPD218: 6 knobs × 3 banks, 4×4 pads × 3 banks.

## Dev Keyboard Emulation (Current)

Use this layout to exercise the control scheme without the hardware.

### Encoder Banks

- F1 = encoder bank 1
- F2 = encoder bank 2
- F3 = encoder bank 3

### Pad Banks

- F5 = pad bank 1
- F6 = pad bank 2
- F7 = pad bank 3

### Encoders (6)

Each encoder uses a pair of keys (decrease/increase):

- Enc 1: `1` / `2`
- Enc 2: `3` / `4`
- Enc 3: `5` / `6`
- Enc 4: `7` / `8`
- Enc 5: `9` / `0`
- Enc 6: `-` / `=`

### Pads (4×4)

Pads map to the numeric keypad:

- Row 1: Num7, Num8, Num9, Num/
- Row 2: Num4, Num5, Num6, Num*
- Row 3: Num1, Num2, Num3, Num-
- Row 4: Num0, Num., Num=, Num+

### Notes (Keyboard)

Notes use the existing keymap in `src/keymap.cpp`:

`a w s e d f t g y h u j k`

## Keeping This Up To Date

When the input mapping changes, update this document in the same change as the
code update so the dev keyboard layout stays accurate. This is the
human-readable source of truth for the emulation layer.
