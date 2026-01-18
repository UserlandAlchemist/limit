# Project Limit: Design Specification

## Purpose

Learning to make music with software typically means learning a DAW — tools
like Ableton Live, Logic Pro, or Ardour. These are powerful, but their power
comes with complexity: dozens of track types, hundreds of plugins, infinite
routing possibilities, and interfaces designed for mouse-driven precision.

A learner faces two problems simultaneously: learning *music* (rhythm,
harmony, arrangement, sound design) and learning *production* (the DAW's
interface, signal flow, mixing). The DAW's vast surface area makes it easy
to get lost in options rather than making progress.

Limit is a constrained instrument for learning music-making, not a simplified
DAW. The constraints are permanent and deliberate — when you outgrow them,
you graduate to a full DAW, carrying the musical understanding that Limit
helped develop.

### The OP-1 Field Inspiration

Teenage Engineering's OP-1 Field is an all-in-one instrument with strong
constraints, hands-on control, and commitment to audio. It combines
synthesizers, samplers, a drum machine, effects, sequencers, and a four-track
tape recorder into a single portable device. Its limitations (four tracks,
six minutes of tape, one effect at a time) are features, not compromises.

Limit adapts the OP-1's philosophy to software, using controllers the user
already owns (Korg nanoKEY2 for melodic input, Akai MPD218 for pads and
encoders), while adding a phrase-based workflow that acknowledges confident
timing takes time to develop.

## Core Philosophy

**Constraint is the feature.** Limit is not a stripped-down DAW awaiting
expansion. The constraints are permanent, deliberate, and valuable.

**Audio-first with a safety net.** The tape recorder is the destination for
all work. Phrases provide a place to develop ideas, but everything must
eventually be printed to tape as audio.

**Stereo throughout.** The entire signal chain is stereo: synthesis engines,
effects, sampling, and tape tracks.

**Controller-operated.** Every function is accessible from the MPD218 and
nanoKEY2. The GUI exists for visual feedback, not interaction. Keyboard
emulation provides equivalent access for development.

**Mode clarity.** You are always in exactly one mode, doing exactly one
thing. Mode changes are deliberate actions.

**Commitment over perfectionism.** Features that encourage endless tweaking
without progress are deliberately excluded.

## What Limit Is Not

- DAW-style arrangement views, comping, or unlimited undo
- Unlimited tracks, devices, or routing
- Plugin hosting (no VST, LV2, AU)
- Mouse-centric workflows
- Cloud services or online dependencies
- External AI/ML services

## Modes

Limit has four modes: **Sound**, **Sequence**, **Tape**, and **Mix**.

### Sound Mode

Design and play instruments. Synth and Drum are both accessible — toggle
*focus* between them to control which instrument's parameters the encoders
affect.

The OP-1 Field separates Synth and Drum into distinct modes because the same
keys must behave differently. Limit uses separate controllers (nanoKEY2 for
melodic, MPD218 for drums), so a single Sound mode works.

Sub-views:

- **Engine**: Synthesis parameters
- **Envelope**: Amplitude envelope and performance settings
- **Effect**: Insert effect
- **LFO**: Modulation routing

The synth has 8 preset slots; the drum kit has 8 kit slots.

### Sequence Mode

Build and edit phrases. The pads become a step grid for programming patterns.

This mode is central to Limit — the MPD218's 4×4 grid makes step sequencing
a natural primary interaction.

Sub-views:

- **Steps**: Step grid for note entry
- **Settings**: Phrase length, quantisation, swing
- **Sequencer**: Sequencer type selection
- **Phrase**: Phrase slot management

### Tape Mode

Work with the multitrack audio recorder. All finished work lives here.

Operations:

- Track selection and arming
- Transport (play, stop, record, rewind, fast-forward)
- Tape tricks (brake, reverse, chop, loop, speed)
- Editing (lift, drop, cut, split)
- Print phrase to tape

### Mix Mode

Adjust levels, panning, sends, and master output.

Sub-views:

- **Levels**: Track faders (8 tracks)
- **Pan**: Track panning
- **Sends**: Per-track send amounts to Send 1 and Send 2
- **Master**: Master EQ, master effect, send effect parameters

## Phrases

A phrase is a complete musical idea containing:

- **Step/MIDI data**: Notes, timing, velocity, duration
- **Sound settings**: Engine choice and all parameters

When you recall a phrase, you get both the pattern AND the sound. You can
return to an idea weeks later and it sounds exactly as you left it.

### Storage

- Up to **512 phrases** per project (shared between synth and drum)
- Persist with the project across sessions
- Numbered slots, no naming or folders

**Why 512?** The constraint comes from the tape (8 tracks, 10 minutes), not
phrase scarcity. Phrases are ingredients; the tape is the dish. Generous
phrase storage doesn't encourage accumulation because phrases don't become
music until printed to tape.

### Printing to Tape

**Print** renders a phrase to tape as audio:

- Renders through the instrument and effect chain
- Records to the armed tape track at the current position
- Phrase remains in storage (not deleted)

**Why print?**

1. **Layering**: Only one synth and one drum phrase play at a time. Layers
   require separate tape tracks.
1. **Arrangement**: Phrases loop. Tape is linear. Verse → chorus → bridge
   requires printing at specific positions.
1. **Performance**: Printing captures a specific performance with any knob
   tweaks or expression.
1. **Tape tricks**: Audio can be reversed, chopped, speed-shifted.
1. **The mix is audio**: Send effects and master processing operate on tape
   tracks.

## Sequencer Types

### Pattern

Primary step sequencer. Pads = step grid. Enter notes by tapping. This is
where most phrase creation happens. Creates phrases directly.

### Capture

Live performance recording. Play freely, then grab the last N bars as a
phrase. Creates phrases directly.

### Arp

Arpeggiator. Hold notes, they play one at a time in a pattern (up, down,
up-down, random). Can freeze output to create a phrase.

### Euclidean

Distributes N hits across M steps mathematically — produces musically useful
rhythms (e.g., 5 hits across 16 steps = clave pattern). Especially useful for
drums. Can freeze output to create a phrase.

### Hold

Drone/sustain mode. Held notes continue indefinitely. Performance modifier
only — does not create phrases.

## Sound Engines

### Synth Engines (5)

**Analog** (Subtractive): Oscillators → filter → amplifier. The foundational
synthesis method. Basses, leads, pads, brass.

**FM** (Frequency Modulation): One oscillator modulates another's frequency,
creating complex harmonics. Electric pianos, bells, metallic sounds. Famous
from the Yamaha DX7.

**Wavetable**: Scans through a collection of waveforms, morphing timbre.
Evolving pads, aggressive basses, modern electronic sounds.

**Karplus-Strong** (Physical Modelling): Simulates plucked strings via
filtered delay. Guitars, harps, plucked bass, mallets. Acoustic character
without samples.

**Sampler** (Melodic Mode): Plays samples chromatically. Supports
multisampling (different samples for different pitch ranges). Covers any
sound not achievable through synthesis.

### Drum Engines (2)

**Sampler** (Kit Mode): Each pad triggers an independent sample with its own
settings. Up to 16 sounds per kit.

**Drum Synth**: Synthesises percussion from scratch — kicks (sine + pitch
envelope), snares (noise + tone), hats (filtered noise). 808/909-style
sounds without samples.

### Factory Content

Limit ships with samples for immediate use:

- **Drums (~100)**: Acoustic kit, 808, 909, percussion
- **Synth (~20)**: Piano, strings, basic waveforms

Enough to make complete music without external content.

## Effects (16)

### Positions

1. **Synth Insert**: One effect on synth, pre-tape
1. **Drum Insert**: One effect on drums, pre-tape
1. **Send 1**: Shared effect for tape tracks (typically reverb)
1. **Send 2**: Shared effect for tape tracks (typically delay)
1. **Master**: Final output processing

Send effects let multiple tracks share reverb/delay, creating cohesive space.

### Effect List

#### Tone Shaping

- **Filter**: Multi-mode (LP/HP/BP) with resonance
- **EQ**: Three-band parametric

#### Distortion

- **Drive**: Saturation to overdrive
- **Lo-fi**: Bitcrush, sample rate reduction

#### Time-Based

- **Delay**: Tempo-synced echo
- **Reverb**: Room to hall

#### Modulation

- **Chorus**: Thickening/doubling
- **Phaser**: Sweeping comb filter
- **Flanger**: Metallic jet-sweep
- **Tremolo**: Rhythmic volume modulation

#### Dynamics

- **Compressor**: Punch, sustain, glue
- **Limiter**: Brick wall, prevent clipping
- **Gate**: Tighten tails, rhythmic chop
- **Ducker**: Sidechain-style pumping (triggered by drums)

#### Pitch

- **Pitch**: Shift up/down, octaver

#### Imaging

- **Stereo**: Width control

## Tape Recorder

### Specifications

- **8 stereo tracks**
- **10 minutes** maximum length per track
- **32-bit float** internal processing
- **WAV** file storage

**Why 8 tracks?** The OP-1 has 4, likely due to hardware constraints. 4
forces early bouncing; 8 is generous while still being "small and fixed."
Building for 8 from the start avoids rework if expanding later.

**Why 10 minutes?** Memory: 8 tracks × 10 min = ~1.8GB RAM, manageable on
laptops. 10 minutes covers any realistic song. A project = a song, not an
album.

### Tape Tricks

Momentary performance effects:

- **Brake**: Slowdown effect
- **Reverse**: Backwards playback
- **Chop**: Stutter/beat-repeat
- **Loop**: Quick loop control
- **Speed**: Pitch via speed change

### Editing

Simple, destructive operations:

- **Lift**: Copy region
- **Drop**: Paste at position
- **Cut**: Delete region
- **Split**: Divide at playhead

Limited undo (one level at most).

### Signal Flow

```
Instrument (Synth or Drum)
    ↓
Insert Effect
    ↓
Tape Track (recording)

Tape Playback (8 tracks)
    ├──→ Level/Pan ──→ Master Bus
    ├──→ Send 1 ──→ Send 1 Effect ──→ Master Bus
    └──→ Send 2 ──→ Send 2 Effect ──→ Master Bus
                          ↓
                    Master EQ
                          ↓
                    Master Effect
                          ↓
                       Output
```

## Controllers

### Target Hardware

**Korg nanoKEY2**: 25 velocity-sensitive keys. Octave shift, pitch, mod,
sustain buttons.

**Akai MPD218**: 16 pads (4×4) across 3 banks. 6 encoders across 3 banks.

### Keyboard Emulation

Full mapping in `docs/control-scheme.md`:

- Pads: 1234 / QWER / ASDF / ZXCV
- Notes: G–\ (white), Y–[ (black)
- Encoders: Numpad + nav cluster
- Function keys: Bank switching, nanoKEY2 buttons

## Technical Requirements

- Sample rate: 48kHz
- Bit depth: 32-bit float internal
- Latency target: Under 10ms
- DSP: JUCE native facilities (not FAUST)

## Persistence

Automatic save — no manual save command. Project contains:

- Tape tracks (audio)
- All phrases (up to 512)
- 8 synth presets, 8 drum kits
- Mixer settings, effects, tempo

## Success Criteria

Limit succeeds if:

- You can create phrases and print them to tape using only controllers
- You can build multi-track pieces through layering
- You learn synthesis, rhythm, and arrangement through constrained exploration
- The constraints feel liberating rather than frustrating
- Someone can make complete music with only Limit and factory content

Limit fails if:

- You spend more time navigating than making music
- You need the mouse for basic tasks
- You wish for features that would make it a DAW

## Future Considerations (Deferred)

- MIDI sync/output
- Audio input recording
- Additional engines/effects
- Export/mixdown

Add only in response to demonstrated need.

## Related Documents

- `docs/control-scheme.md` — Controller and keyboard mapping

## Implementation Notes

- Update README to reflect JUCE DSP (not FAUST)
- Replace existing control-scheme.md with updated version
- Build for 8 tracks from the start
- Prioritise Pattern sequencer and phrase storage early

## Decision Rationale

Key decisions and why:

- **Track count: 8.** 4 forces early bouncing; 8 stays generous but fixed.
- **Tape length: 10 min.** ~1.8GB RAM; covers a song; enforces project = song.
- **Phrase count: 512.** Constraint from tape, not phrase scarcity.
- **Sound mode: combined.** Separate controllers already split synth/drum.
- **Tombola → Euclidean.** Tombola too OP-1 specific; Euclidean is established.
- **DSP: JUCE native.** FAUST adds toolchain; benefit is low for this scope.
- **Effects: 16.** Covers essentials for the "only tool" use case.
- **Engines: 7.** Analog, FM, Wavetable, Karplus, Sampler (Melodic),
  Sampler (Kit), Drum Synth.
