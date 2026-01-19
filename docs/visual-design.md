# Limit Visual Design Specification

## Color Palette

### Base Workbench colors

- Background: `#202020`
- Panel: `#242424`
- Border: `#303030`
- Foreground text: `#E6E6E6`
- Highlight text: `#FFFFFF`
- Dimmed/inactive: `#606060`

### Accent colors (Workbench-inspired)

- Blue: `#5078FF`
- Orange: `#FF8800`
- Red: `#FF4040` (hardware LED color)

### Six encoder colors (matching physical stickers on MPD218)

1. Blue: `#5078FF`
1. White: `#FFFFFF`
1. Red: `#FF4040` (matches MPD218 hardware LEDs)
1. Orange: `#FF8800`
1. Grey: `#808080`
1. Dark Grey: `#303030`

### State indicators

- Recording: Red `#FF4040` (matches MPD218 pad LEDs and encoder 3)
- Bank LEDs: Red `#FF4040` (matches hardware LED indicators)

## Typography

**Font:** Topaz Plus NF Mono (or Topaz family), sized appropriately for readability.

- All caps for labels: `CUTOFF`, `ATTACK`, `PADS`
- Monospaced values with consistent formatting
- No bold, no italics, no text effects
- DPI-aware scaling via JUCE

## Logo

**Design:** Mathematical limit notation.

```
LIMIT
♫→∞
```

**Color scheme:**

- LIMIT text: Blue `#5078FF` (Topaz font, 32pt, letter-spacing: 1)
- ♫ (musical note): Orange `#FF8800` (16pt)
- → (arrow): Grey `#808080` (dark backgrounds) or `#303030` (light backgrounds),
  18pt bold
- ∞ (infinity): Orange `#FF8800` (16pt)

**Layout:**

- LIMIT centered at x=100, y=45
- Symbols aligned under letters (under first I): ♫ at x=81.5, y=55
- Symbols aligned under letters (under M): → at x=100, y=55
- Symbols aligned under letters (under second I): ∞ at x=116.5, y=55
- Tight vertical spacing between text and symbols (10 units)
- Letter spacing: 1

**Usage:**

- Application icon (all sizes)
- Splash screen
- About dialog
- Not used in the main UI header (header uses plain `LIMIT` text)

**Variants:**

- Dark background version (Workbench `#202020`)
- Light background version (white `#FFFFFF`)
- Transparent version (no background)

**Files:**

- `assets/limit-logo-final-a.svg` - Dark background
- `assets/limit-logo-final-a-light.svg` - Light background
- `assets/limit-logo-final-a-transparent.svg` - Transparent
- Requires `TopazPlus_a500_v1_0.ttf` available locally (place in `assets/fonts/`
  if bundling)

## Layout Structure

```
┌─────────────────────────────────────────────────────┐
│ MODE :: CONTEXT       LIMIT         ADDITIONAL INFO │ ← Header
├─────────────────────────────────────────────────────┤
│                                                     │
│                                                     │
│              [Full-width visualization]             │ ← Main content
│                                                     │
│                                                     │
├──────────────┬──────────────┬───────────────────────┤
│ PARAM 1      │ PARAM 2      │                       │
│ ████         │ ████         │                       │
│ VALUE        │ VALUE        │                       │
├──────────────┼──────────────┤   [Secondary display] │
│ PARAM 3      │ PARAM 4      │        area           │ ← Encoder grid
│ ▒▒▒▒         │ ████         │  (optional context)   │   (2 cols × 3 rows)
│ VALUE        │ VALUE        │                       │
├──────────────┼──────────────┤                       │
│ PARAM 5      │ PARAM 6      │                       │
│ ▒▒▒▒         │ ████         │                       │
│ VALUE        │ VALUE        │ PADS:[●][○] A:NAME    │ ← Bank indicators
│              │              │ CTRL:[○][●] B:NAME    │
└──────────────┴──────────────┴───────────────────────┘
```

### Layout principles

- Header: Single line, full width, with "LIMIT" text centered
- Visualization: Maximum available space, expandable
- Encoder grid: 2 columns × 3 rows, left side (~30% total width)
- Secondary display area: ~70% width on right side
- Bank indicators: Bottom-right corner of secondary display area, stacked vertically
- All proportions relative to window size

## Encoder Color and Pattern Layout

### Fixed layout matching MPD218 physical positions

```
┌──────────────┬──────────────┐
│ 1: BLUE      │ 2: WHITE     │
│    Solid     │    Solid     │
├──────────────┼──────────────┤
│ 3: RED       │ 4: ORANGE    │
│    Dithered  │    Solid     │
├──────────────┼──────────────┤
│ 5: GREY      │ 6: DARK GREY │
│    Dithered  │    Solid     │
└──────────────┴──────────────┘
```

### Physical sticker placement on MPD218

- Encoder 1 (top-left): Blue sticker
- Encoder 2 (top-right): White sticker
- Encoder 3 (mid-left): Red sticker
- Encoder 4 (mid-right): Orange sticker
- Encoder 5 (bottom-left): Grey sticker
- Encoder 6 (bottom-right): Dark Grey sticker

### Pattern specifications

- **Solid:** Full color fill `████`
- **Dithered:** 50% checkerboard pattern `▒▒▒▒` (classic Amiga stipple)

### Accessibility benefits

- Every adjacent encoder pair has pattern contrast (solid vs dithered)
- Red and Orange (potentially confused by colorblind users) distinguished by pattern
- White and Grey separated by row + pattern variation
- Patterns provide distinction independent of color perception

## Bank Indicators

### Visual format (mimics hardware LED displays)

```
PADS:[●][○] A:NAME
CTRL:[○][●] B:NAME
```

### LED states

- Left lit `[●][○]` = Bank A
- Right lit `[○][●]` = Bank B
- Both lit `[●][●]` = Bank C

### LED colors

- Active (lit): Red `#FF4040` (matches MPD218/nanoKEY2 hardware LEDs)
- Inactive: Dark grey `#303030` or `#202020` (barely visible)

### Bank name format

- Always shown for maximum information
- Format: `A:DESCRIPTION` where description is context-dependent
- Monospaced, all caps
- Bank names are implementation-specific and mode-dependent

### Position

- Bottom-right corner of secondary display area
- Two lines, vertically stacked
- Right-aligned within the area

## Visual Elements

### Borders and panels

- Workbench-style bevels use single-tone edge strokes (no gradients):
  `rgba(255,255,255,0.10-0.15)` for top/left, `rgba(0,0,0,0.3-0.5)` for
  bottom/right
- Thin borders using `#303030`
- Panel backgrounds at `#242424`
- Main background at `#202020`

### Encoder parameter cells

```
┌──────────────┐
│ CUTOFF       │  ← Label (all caps, white #E6E6E6)
│ ████         │  ← Color bar (solid or dithered pattern)
│ 2400         │  ← Value (monospaced, white #E6E6E6)
└──────────────┘
```

Each cell has subtle Workbench bevel borders.

### Value formatting standards

- Frequencies: `2400` or `2.4K` (Hz implied)
- Time: `450` or `0:450` (ms implied)
- Percentages: `64` (0-100 range, no % symbol)
- Enums: `SAW`, `FAST`, `ON`, `OFF` (all caps)
- Notes: `C`, `C#`, `D` etc.
- Positions: `01:23` (minutes:seconds)
- Phrase numbers: `001-016` (zero-padded, range format)
- All numbers right-aligned within their space

## Secondary Display Area

### Potential usage (mode-dependent)

- Additional context (tempo, CPU usage, voice count)
- Complementary visualizations
- Browsers and selectors
- Routing diagrams
- Bank indicators (always present in bottom-right)
- Or remain mostly empty for visual breathing room
- Not required to be filled - available if needed

## Visual Language Principles

1. **Rectangular panels** - Workbench gadget aesthetic with bevels
1. **Grid alignment** - everything aligns to implicit grid structure
1. **Monospaced data** - all values use fixed-width font, align in columns
1. **High contrast** - white/light grey text on dark backgrounds
1. **Color = function** - encoder colors show controller mapping only
1. **Pattern = distinction** - Amiga-style dithering for accessibility and visual
   clarity
1. **No semantic color** - modes, warnings, states use text/icons, not color (except
   recording/LED red)
1. **No gradients** - flat colors only, solid fills or dither patterns
1. **Minimal decoration** - functional only, no embellishment or skeuomorphism
1. **Information density** - use space efficiently without clutter (OctaMED tracker
   density)
1. **Breathing room** - secondary display area can remain empty when not needed
1. **Hardware mirroring** - on-screen elements reflect physical controller state
   where possible

## Accessibility

### Colorblind-friendly design

- Red and Orange differentiated by pattern (dithered vs solid) as well as color
- Adjacent encoders always have pattern contrast
- Pattern distinction works in greyscale
- Parameter labels provide primary information
- Physical encoder position is consistent

### Visual hierarchy

- Color + pattern + position = triple redundancy for identification
- Classic Amiga dithering approach (50% checkerboard stipple)
- Patterns subtle enough not to create visual noise
- High contrast maintained throughout

## What This Is NOT

- Not using OP-1 Field's modern sans-serif font (we use Topaz bitmap)
- Not using OP-1 Field's pure black OLED background (we use Workbench `#202020`)
- Not organically positioning elements (we use structured grid layout)
- Not DAW-style (no timeline, no unlimited tracks, no region blocks, no automation
  lanes)
- Not using color to indicate modes or categories (only for controller mapping,
  recording, and hardware LED states)
- Not clickable or mouse-driven (display only, controller-operated)

## Hardware Aesthetic Integration

### MPD218

- Black body with red LED accents
- Recording state uses matching red `#FF4040`
- Bank switching reflected on screen with red LED indicators
- Bank labels match hardware: "PADS" and "CTRL" (not "ENCODERS")
- 2×3 encoder layout mirrored exactly on screen
- 4×4 pad layout mirrored when relevant
- Red encoder color matches hardware LEDs

### nanoKEY2

- Black finish
- Red/orange LED indicators on control buttons
- Minimal visual presence in interface (used for playing, not on-screen
  representation)
- Performance controller - state doesn't require screen representation

### Desktop environment

- Matches "dark mode Workbench" aesthetic
- Topaz font consistency with system theme
- Blue/orange/red Amiga color palette
- Beveled borders matching Waybar style
- Dithering patterns authentic to Amiga GUI design

## Inspiration Sources

### Primary

- Amiga Workbench 1.3 (beveled gadgets, blue/orange/red palette, dither patterns,
  functional UI)
- OctaMED tracker (structured data display, grid layout, information density)

### Secondary

- OP-1 Field (controller-to-screen parameter mapping philosophy)
- ProTracker (monospaced precision, rectangular panels, pattern-based distinction)

### Hardware tie-in

- MPD218 black/red aesthetic, LED indicators, and physical layout
- nanoKEY2 black finish and red LED indicators
- Physical controller layout dictates screen layout

---

This is the complete visual design specification for Limit.
Mode-specific content and implementation details are to be determined separately.
