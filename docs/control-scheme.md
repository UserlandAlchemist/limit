# Control Scheme (Dev Keyboard + Target Controllers)

This document describes the control scheme for Limit and its keyboard emulation.
Keep this file in sync with the input mapping code.

## Target Controllers

### Korg nanoKEY2

25 velocity-sensitive keys for melodic input, plus:

- Octave - button
- Octave + button
- Pitch - button
- Pitch + button
- Mod button
- Sustain button

### Akai MPD218

16 velocity- and pressure-sensitive pads arranged in a 4×4 grid, across 3 banks
(A, B, C) accessed via the Pad Bank button.

6 continuous rotary encoders across 3 banks (1, 2, 3) accessed via the Control
Bank button.

Prog Select button for additional functionality.

## Dev Keyboard Emulation

Use this layout to exercise the full control scheme without the hardware
controllers. The mapping emulates both controllers simultaneously, allowing
two-handed operation.

### Pads (4×4 grid, left hand)

```
[1][2][3][4]  = Pads 1-4
[Q][W][E][R]  = Pads 5-8
[A][S][D][F]  = Pads 9-12
[Z][X][C][V]  = Pads 13-16
```

### Notes (one octave, right hand)

White keys (C D E F G A B C):

```
[G][H][J][K][L][;]['][\]
```

Black keys (C# D# F# G# A#):

```
[Y][U]   [O][P][[]
```

The black keys sit on the row above the white keys, mirroring piano geometry.
Use Octave +/- to shift the range.

### Encoders (numpad + nav cluster)

Each encoder uses three keys: decrease / reset to center / increase.

```
Encoder 1: [Num7] [Num8] [Num9]
Encoder 2: [Num4] [Num5] [Num6]
Encoder 3: [Num1] [Num2] [Num3]
Encoder 4: [Ins]  [Home] [PgUp]
Encoder 5: [Del]  [End]  [PgDn]
Encoder 6: [←]    [↓]    [→]
```

### Bank and Utility Buttons

```
[F1] = Pad Bank cycle (A → B → C → A)
[F2] = Prog Select
[↑]  = Control Bank cycle (1 → 2 → 3 → 1)
```

### NanoKey2 Buttons

```
[F3] = Mod
[F4] = Sustain
[F5] = Octave -
[F6] = Octave +
[F7] = Pitch -
[F8] = Pitch +
```

## Summary Table

| Controller | Element | Keyboard |
|------------|---------|----------|
| **MPD218** | | |
| | Pads 1-4 | 1 2 3 4 |
| | Pads 5-8 | Q W E R |
| | Pads 9-12 | A S D F |
| | Pads 13-16 | Z X C V |
| | Pad Bank | F1 |
| | Prog Select | F2 |
| | Control Bank | ↑ |
| | Encoder 1 (dec/reset/inc) | Num7 / Num8 / Num9 |
| | Encoder 2 | Num4 / Num5 / Num6 |
| | Encoder 3 | Num1 / Num2 / Num3 |
| | Encoder 4 | Ins / Home / PgUp |
| | Encoder 5 | Del / End / PgDn |
| | Encoder 6 | ← / ↓ / → |
| **NanoKey2** | | |
| | Notes (white): C D E F G A B C | G H J K L ; ' \ |
| | Notes (black): C# D# F# G# A# | Y U O P [ |
| | Mod | F3 |
| | Sustain | F4 |
| | Octave - | F5 |
| | Octave + | F6 |
| | Pitch - | F7 |
| | Pitch + | F8 |

## Unused Keys

The following keys are available for future assignment:

- Number row: 5, 6, 7, 8, 9, 0, -, =
- Letters: T, I, ], B, N, M, ,, ., /
- Modifiers: Tab, Caps Lock, Left Shift, Left Ctrl, Left Alt, Space, Right Alt,
  Right Ctrl
- Function keys: F9, F10, F11, F12
- Numpad: Num0, Num., Num+, Num-, Num*, Num/, NumEnter, NumLock
- Other: Backspace, Enter

## Keeping This Up To Date

When the input mapping changes in code, update this document in the same commit
so the dev keyboard layout stays accurate.
