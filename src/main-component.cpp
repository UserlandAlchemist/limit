#include "main-component.h"

#include <algorithm>
#include <cstddef>

#include "BinaryData.h"
#include "dev-controller.h"
#include "keymap.h"
#include "ui-layout.h"

namespace limit {
namespace {
constexpr const char *kTopazFallbackName = "Topaz Plus NF Mono";

auto getTopazTypeface() -> juce::Typeface::Ptr {
  int size = 0;
  auto *data = BinaryData::getNamedResource("TopazPlusNFMono-Regular.ttf", size);
  if (data == nullptr || size == 0) {
    return nullptr;
  }
  return juce::Typeface::createSystemTypefaceFor(data, static_cast<std::size_t>(size));
}

auto makeTopazFontOptions(float height) -> juce::FontOptions {
  auto options = juce::FontOptions{}.withHeight(height);
  if (auto typeface = getTopazTypeface()) {
    return options.withTypeface(typeface);
  }
  return options.withName(kTopazFallbackName);
}
} // namespace

MainComponent::MainComponent(bool enable_audio) {
  setSize(kWindowWidth, kWindowHeight);
  setWantsKeyboardFocus(true);

  auto *device_manager = &deviceManager;
  const auto midi_inputs = juce::MidiInput::getAvailableDevices();
  for (const auto &device : midi_inputs) {
    device_manager->setMidiInputDeviceEnabled(device.identifier, true);
    device_manager->addMidiInputDeviceCallback(device.identifier, this);
  }

  if (enable_audio) {
    setAudioChannels(2, 2);
  }
}

MainComponent::~MainComponent() { shutdownAudio(); }

void MainComponent::prepareToPlay(int /*samples_per_block_expected*/,
                                  double /*sample_rate*/) {
  last_midi_message = "";
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo &buffer_to_fill) {
  buffer_to_fill.clearActiveBufferRegion();
}

void MainComponent::releaseResources() {}

void MainComponent::paint(juce::Graphics &g) {
  g.fillAll(kColorBackground);

  const auto bounds = getLocalBounds();
  const auto layout = limit::computeUiLayout(
      {.width = bounds.getWidth(),
       .height = bounds.getHeight(),
       .header_height = kHeaderHeight,
       .visualization_ratio = kVisualizationRatio,
       .encoder_ratio = kEncoderGridRatio});
  const auto header_area = toRectangle(layout.header);
  const auto visualization_area = toRectangle(layout.visualization);
  const auto encoder_area = toRectangle(layout.encoder);
  const auto secondary_area = toRectangle(layout.secondary);

  drawPanel(g, header_area);
  drawPanel(g, visualization_area);
  drawPanel(g, encoder_area);
  drawPanel(g, secondary_area);

  g.setColour(kColorAccentBlue);
  g.setFont(juce::Font(makeTopazFontOptions(kTitleFontSize)));
  g.drawText("LIMIT", header_area, juce::Justification::centred);

  g.setColour(kColorText);
  g.setFont(juce::Font(makeTopazFontOptions(kBodyFontSize)));
  const auto midi_text = last_midi_message.isEmpty() ? "NONE" : last_midi_message;
  const auto midi_label = juce::String("MIDI: ") + midi_text;
  g.drawText(midi_label, secondary_area.reduced(kPadding), juce::Justification::topLeft);
}

void MainComponent::resized() {}
void MainComponent::parentHierarchyChanged() { focusIfVisible(); }
void MainComponent::visibilityChanged() { focusIfVisible(); }

auto MainComponent::keyPressed(const juce::KeyPress &key) -> bool {
  if (handleDevPadBankCycle(key) || handleDevControlBankCycle(key) ||
      handleDevUtilityButtons(key) || handleDevEncoder(key) || handleDevPad(key)) {
    return true;
  }
  const auto key_char = static_cast<unsigned char>(key.getTextCharacter());
  return processKeyChar(static_cast<int>(key_char));
}

auto MainComponent::getLastMidiMessageForTesting() const -> juce::String {
  return last_midi_message;
}

void MainComponent::processMidiMessageForTesting(const juce::MidiMessage &message) {
  processMidiMessage(message);
}

auto MainComponent::processKeyCharForTesting(int key_char) -> bool {
  return processKeyChar(key_char);
}

void MainComponent::handleIncomingMidiMessageForTesting(const juce::MidiMessage &message) {
  handleIncomingMidiMessage(nullptr, message);
}

auto MainComponent::processEncoderActionForTesting(int encoder_index,
                                                   limit::DevEncoderAction action) -> bool {
  const auto event = limit::handleDevEncoderAction(encoder_index, action, dev_state);
  if (!event) {
    return false;
  }
  last_midi_message = "dev cc " + juce::String(event->cc) + " = " +
                      juce::String(event->value) + " (bank " +
                      juce::String(event->bank + 1) + ")";
  repaint();
  return true;
}

auto MainComponent::processPadIndexForTesting(int pad_index) -> bool {
  const auto event = limit::handleDevPadPress(pad_index, dev_state);
  if (!event) {
    return false;
  }
  last_midi_message = "dev pad " + juce::String(event->pad_index + 1) + " (bank " +
                      juce::String(event->bank + 1) + ")";
  repaint();
  return true;
}

void MainComponent::setOctaveOffsetForTesting(int offset) { note_octave_offset = offset; }

void MainComponent::handleIncomingMidiMessage(juce::MidiInput * /*source*/,
                                              const juce::MidiMessage &message) {
  processMidiMessage(message);
}

auto MainComponent::handleDevControlBankCycle(const juce::KeyPress &key) -> bool {
  const auto key_code = key.getKeyCode();
  if (key_code != juce::KeyPress::upKey) {
    return false;
  }
  const auto next_bank = (dev_state.encoder_bank + 1) % limit::kDevBankCount;
  limit::setDevEncoderBank(next_bank, dev_state);
  last_midi_message = "dev control bank " + juce::String(dev_state.encoder_bank + 1);
  repaint();
  return true;
}

auto MainComponent::handleDevPadBankCycle(const juce::KeyPress &key) -> bool {
  const auto key_code = key.getKeyCode();
  if (key_code != juce::KeyPress::F1Key) {
    return false;
  }
  const auto next_bank = (dev_state.pad_bank + 1) % limit::kDevBankCount;
  limit::setDevPadBank(next_bank, dev_state);
  const auto bank_label =
      juce::String::charToString(static_cast<juce::juce_wchar>('A' + dev_state.pad_bank));
  last_midi_message = "dev pad bank " + bank_label;
  repaint();
  return true;
}

auto MainComponent::handleDevUtilityButtons(const juce::KeyPress &key) -> bool {
  const auto key_code = key.getKeyCode();
  if (key_code == juce::KeyPress::F2Key) {
    last_midi_message = "dev prog select";
  } else if (key_code == juce::KeyPress::F3Key) {
    mod_active = !mod_active;
    last_midi_message = mod_active ? "dev mod on" : "dev mod off";
  } else if (key_code == juce::KeyPress::F4Key) {
    sustain_active = !sustain_active;
    last_midi_message = sustain_active ? "dev sustain on" : "dev sustain off";
  } else if (key_code == juce::KeyPress::F5Key) {
    note_octave_offset = std::max(note_octave_offset - 1, minOctaveOffset());
    last_midi_message = "dev octave " + juce::String(note_octave_offset);
  } else if (key_code == juce::KeyPress::F6Key) {
    note_octave_offset = std::min(note_octave_offset + 1, maxOctaveOffset());
    last_midi_message = "dev octave " + juce::String(note_octave_offset);
  } else if (key_code == juce::KeyPress::F7Key) {
    pitch_offset = std::max(pitch_offset - 1, kPitchMin);
    last_midi_message = "dev pitch " + juce::String(pitch_offset);
  } else if (key_code == juce::KeyPress::F8Key) {
    pitch_offset = std::min(pitch_offset + 1, kPitchMax);
    last_midi_message = "dev pitch " + juce::String(pitch_offset);
  } else {
    return false;
  }
  repaint();
  return true;
}

auto MainComponent::mapKeyToEncoderAction(const juce::KeyPress &key) const
    -> std::optional<EncoderKeyAction> {
  using Key = juce::KeyPress;
  const auto key_code = key.getKeyCode();
  if (key_code == Key::numberPad7) {
    return EncoderKeyAction{.encoder_index = kEncoderIndex0,
                            .action = limit::DevEncoderAction::kDecrease};
  }
  if (key_code == Key::numberPad8) {
    return EncoderKeyAction{.encoder_index = kEncoderIndex0,
                            .action = limit::DevEncoderAction::kReset};
  }
  if (key_code == Key::numberPad9) {
    return EncoderKeyAction{.encoder_index = kEncoderIndex0,
                            .action = limit::DevEncoderAction::kIncrease};
  }
  if (key_code == Key::numberPad4) {
    return EncoderKeyAction{.encoder_index = kEncoderIndex1,
                            .action = limit::DevEncoderAction::kDecrease};
  }
  if (key_code == Key::numberPad5) {
    return EncoderKeyAction{.encoder_index = kEncoderIndex1,
                            .action = limit::DevEncoderAction::kReset};
  }
  if (key_code == Key::numberPad6) {
    return EncoderKeyAction{.encoder_index = kEncoderIndex1,
                            .action = limit::DevEncoderAction::kIncrease};
  }
  if (key_code == Key::numberPad1) {
    return EncoderKeyAction{.encoder_index = kEncoderIndex2,
                            .action = limit::DevEncoderAction::kDecrease};
  }
  if (key_code == Key::numberPad2) {
    return EncoderKeyAction{.encoder_index = kEncoderIndex2,
                            .action = limit::DevEncoderAction::kReset};
  }
  if (key_code == Key::numberPad3) {
    return EncoderKeyAction{.encoder_index = kEncoderIndex2,
                            .action = limit::DevEncoderAction::kIncrease};
  }
  if (key_code == Key::insertKey) {
    return EncoderKeyAction{.encoder_index = kEncoderIndex3,
                            .action = limit::DevEncoderAction::kDecrease};
  }
  if (key_code == Key::homeKey) {
    return EncoderKeyAction{.encoder_index = kEncoderIndex3,
                            .action = limit::DevEncoderAction::kReset};
  }
  if (key_code == Key::pageUpKey) {
    return EncoderKeyAction{.encoder_index = kEncoderIndex3,
                            .action = limit::DevEncoderAction::kIncrease};
  }
  if (key_code == Key::deleteKey) {
    return EncoderKeyAction{.encoder_index = kEncoderIndex4,
                            .action = limit::DevEncoderAction::kDecrease};
  }
  if (key_code == Key::endKey) {
    return EncoderKeyAction{.encoder_index = kEncoderIndex4,
                            .action = limit::DevEncoderAction::kReset};
  }
  if (key_code == Key::pageDownKey) {
    return EncoderKeyAction{.encoder_index = kEncoderIndex4,
                            .action = limit::DevEncoderAction::kIncrease};
  }
  if (key_code == Key::leftKey) {
    return EncoderKeyAction{.encoder_index = kEncoderIndex5,
                            .action = limit::DevEncoderAction::kDecrease};
  }
  if (key_code == Key::downKey) {
    return EncoderKeyAction{.encoder_index = kEncoderIndex5,
                            .action = limit::DevEncoderAction::kReset};
  }
  if (key_code == Key::rightKey) {
    return EncoderKeyAction{.encoder_index = kEncoderIndex5,
                            .action = limit::DevEncoderAction::kIncrease};
  }
  return std::nullopt;
}

auto MainComponent::handleDevEncoder(const juce::KeyPress &key) -> bool {
  const auto action = mapKeyToEncoderAction(key);
  if (!action) {
    return false;
  }

  const auto event =
      limit::handleDevEncoderAction(action->encoder_index, action->action, dev_state);
  if (!event) {
    return false;
  }

  last_midi_message = "dev cc " + juce::String(event->cc) + " = " +
                      juce::String(event->value) + " (bank " +
                      juce::String(event->bank + 1) + ")";
  repaint();
  return true;
}

auto MainComponent::handleDevPad(const juce::KeyPress &key) -> bool {
  const auto key_char = static_cast<unsigned char>(key.getTextCharacter());
  const auto pad_index = mapKeyCharToPadIndex(static_cast<int>(key_char));
  if (pad_index < 0) {
    return false;
  }

  const auto event = limit::handleDevPadPress(pad_index, dev_state);
  if (!event) {
    return false;
  }

  last_midi_message = "dev pad " + juce::String(event->pad_index + 1) + " (bank " +
                      juce::String(event->bank + 1) + ")";
  repaint();
  return true;
}

auto MainComponent::mapKeyCharToPadIndex(int key_char) const -> int {
  static const std::array<char, limit::kDevPadCount> kPadKeys = {
      '1', '2', '3', '4', 'q', 'w', 'e', 'r',
      'a', 's', 'd', 'f', 'z', 'x', 'c', 'v'};

  const auto normalized = static_cast<char>(std::tolower(key_char));
  for (std::size_t index = 0; index < kPadKeys.size(); ++index) {
    if (normalized == kPadKeys.at(index)) {
      return static_cast<int>(index);
    }
  }
  return -1;
}

auto MainComponent::minOctaveOffset() const -> int {
  return (kMidiMin - limit::kKeymapMinNote) / kSemitone;
}

auto MainComponent::maxOctaveOffset() const -> int {
  return (kMidiMax - limit::kKeymapMaxNote) / kSemitone;
}

void MainComponent::focusIfVisible() {
  if (isShowing()) {
    grabKeyboardFocus();
  }
}

void MainComponent::processMidiMessage(const juce::MidiMessage &message) {
  if (message.isNoteOn()) {
    last_midi_message =
        "note-on " + juce::MidiMessage::getMidiNoteName(message.getNoteNumber(), true, true, 3);
  } else if (message.isNoteOff()) {
    last_midi_message =
        "note-off " + juce::MidiMessage::getMidiNoteName(message.getNoteNumber(), true, true, 3);
  } else if (message.isController()) {
    last_midi_message = "cc " + juce::String(message.getControllerNumber()) + " = " +
                        juce::String(message.getControllerValue());
  } else {
    last_midi_message = "message";
  }

  repaint();
}

auto MainComponent::processKeyChar(int key_char) -> bool {
  const auto normalized = static_cast<unsigned char>(std::tolower(key_char));
  const auto note = limit::mapKeyToMidiNote(static_cast<int>(normalized));
  if (note < 0) {
    return false;
  }
  const auto octave_shift = note_octave_offset * kSemitone;
  const auto shifted_note = note + octave_shift;
  if (shifted_note < kMidiMin || shifted_note > kMidiMax) {
    return false;
  }
  last_midi_message =
      "note-on " + juce::MidiMessage::getMidiNoteName(shifted_note, true, true, 3);
  repaint();
  return true;
}

auto MainComponent::toRectangle(const limit::LayoutRect &rect) -> juce::Rectangle<int> {
  return {rect.x, rect.y, rect.width, rect.height};
}

void MainComponent::drawPanel(juce::Graphics &g, juce::Rectangle<int> area) const {
  g.setColour(kColorPanel);
  g.fillRect(area);
  g.setColour(kColorBorder);
  g.drawRect(area);
  g.setColour(kColorBevelLight);
  g.drawLine(static_cast<float>(area.getX()), static_cast<float>(area.getY()),
             static_cast<float>(area.getRight()), static_cast<float>(area.getY()));
  g.drawLine(static_cast<float>(area.getX()), static_cast<float>(area.getY()),
             static_cast<float>(area.getX()), static_cast<float>(area.getBottom()));
  g.setColour(kColorBevelDark);
  g.drawLine(static_cast<float>(area.getX()), static_cast<float>(area.getBottom()),
             static_cast<float>(area.getRight()), static_cast<float>(area.getBottom()));
  g.drawLine(static_cast<float>(area.getRight()), static_cast<float>(area.getY()),
             static_cast<float>(area.getRight()), static_cast<float>(area.getBottom()));
}
} // namespace limit
