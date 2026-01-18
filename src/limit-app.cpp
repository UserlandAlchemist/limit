#include <JuceHeader.h>

#include <algorithm>
#include <array>
#include <cctype>

#include "dev-controller.h"
#include "keymap.h"

namespace {
class MainComponent final : public juce::AudioAppComponent, private juce::MidiInputCallback {
public:
  MainComponent() {
    setSize(kWindowWidth, kWindowHeight);
    setWantsKeyboardFocus(true);

    auto *device_manager = &deviceManager;
    const auto midi_inputs = juce::MidiInput::getAvailableDevices();
    for (const auto &device : midi_inputs) {
      device_manager->setMidiInputDeviceEnabled(device.identifier, true);
      device_manager->addMidiInputDeviceCallback(device.identifier, this);
    }

    setAudioChannels(2, 2);
  }

  ~MainComponent() override { shutdownAudio(); }
  MainComponent(const MainComponent &) = delete;
  auto operator=(const MainComponent &) -> MainComponent & = delete;
  MainComponent(MainComponent &&) = delete;
  auto operator=(MainComponent &&) -> MainComponent & = delete;

  void prepareToPlay(int /*samples_per_block_expected*/, double /*sample_rate*/) override {
    last_midi_message = "";
  }

  void getNextAudioBlock(const juce::AudioSourceChannelInfo &buffer_to_fill) override {
    buffer_to_fill.clearActiveBufferRegion();
  }

  void releaseResources() override {}

  void paint(juce::Graphics &g) override {
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
    g.setFont(juce::Font(juce::FontOptions{}.withHeight(kTitleFontSize).withStyle("bold")));
    g.drawText("Limit", getLocalBounds().removeFromTop(kTitleHeight), juce::Justification::centred);

    g.setFont(juce::Font(juce::FontOptions{}.withHeight(kBodyFontSize)));
    const auto text = last_midi_message.isEmpty() ? "MIDI: none" : "MIDI: " + last_midi_message;
    g.drawText(text, getLocalBounds().reduced(kPadding), juce::Justification::bottomLeft);
  }

  void resized() override {}
  void parentHierarchyChanged() override { focusIfVisible(); }
  void visibilityChanged() override { focusIfVisible(); }
  auto keyPressed(const juce::KeyPress &key) -> bool override {
    if (handleDevPadBankCycle(key) || handleDevControlBankCycle(key) ||
        handleDevUtilityButtons(key) || handleDevEncoder(key) || handleDevPad(key)) {
      return true;
    }
    const auto key_char = static_cast<unsigned char>(key.getTextCharacter());
    const auto normalized = static_cast<unsigned char>(std::tolower(key_char));
    const auto note = limit::mapKeyToMidiNote(static_cast<int>(normalized));
    if (note >= 0) {
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
    return false;
  }

private:
  static constexpr int kWindowWidth = 720;
  static constexpr int kWindowHeight = 420;
  static constexpr float kTitleFontSize = 24.0f;
  static constexpr float kBodyFontSize = 14.0f;
  static constexpr int kTitleHeight = 60;
  static constexpr int kPadding = 24;
  static constexpr int kMidiMin = 0;
  static constexpr int kMidiMax = 127;
  static constexpr int kSemitone = 12;
  static constexpr int kEncoderIndex0 = 0;
  static constexpr int kEncoderIndex1 = 1;
  static constexpr int kEncoderIndex2 = 2;
  static constexpr int kEncoderIndex3 = 3;
  static constexpr int kEncoderIndex4 = 4;
  static constexpr int kEncoderIndex5 = 5;

  auto handleDevControlBankCycle(const juce::KeyPress &key) -> bool {
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

  auto handleDevPadBankCycle(const juce::KeyPress &key) -> bool {
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

  auto handleDevUtilityButtons(const juce::KeyPress &key) -> bool {
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

  struct EncoderKeyAction {
    int encoder_index = 0;
    limit::DevEncoderAction action = limit::DevEncoderAction::kReset;
  };

  auto mapKeyToEncoderAction(const juce::KeyPress &key) const
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

  auto handleDevEncoder(const juce::KeyPress &key) -> bool {
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

  auto handleDevPad(const juce::KeyPress &key) -> bool {
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

  auto mapKeyCharToPadIndex(int key_char) const -> int {
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

  auto minOctaveOffset() const -> int {
    return (kMidiMin - limit::kKeymapMinNote) / kSemitone;
  }

  auto maxOctaveOffset() const -> int {
    return (kMidiMax - limit::kKeymapMaxNote) / kSemitone;
  }

  void focusIfVisible() {
    if (isShowing()) {
      grabKeyboardFocus();
    }
  }

  void handleIncomingMidiMessage(juce::MidiInput * /*source*/,
                                 const juce::MidiMessage &message) override {
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

  juce::String last_midi_message;
  limit::DevControllerState dev_state{};
  int note_octave_offset = 0;
  bool mod_active = false;
  bool sustain_active = false;
  int pitch_offset = 0;
  static constexpr int kPitchMin = -12;
  static constexpr int kPitchMax = 12;
};

class LimitApplication final : public juce::JUCEApplication {
public:
  auto getApplicationName() -> const juce::String override { return "Limit"; }
  auto getApplicationVersion() -> const juce::String override { return ProjectInfo::versionString; }
  auto moreThanOneInstanceAllowed() -> bool override { return true; }

  void initialise(const juce::String & /*command_line*/) override {
    main_window = std::make_unique<MainWindow>(getApplicationName());
  }

  void shutdown() override { main_window.reset(); }

  void systemRequestedQuit() override { quit(); }

  void anotherInstanceStarted(const juce::String & /*command_line*/) override {}

private:
  class MainWindow final : public juce::DocumentWindow {
  public:
    explicit MainWindow(const juce::String &name)
        : juce::DocumentWindow(name, juce::Colours::black, juce::DocumentWindow::allButtons),
          main_component(std::make_unique<MainComponent>()) {
      setUsingNativeTitleBar(true);
      setResizable(true, true);
      setContentOwned(main_component.get(), false);
      centreWithSize(getWidth(), getHeight());
      setVisible(true);
    }

    void closeButtonPressed() override {
      juce::JUCEApplication::getInstance()->systemRequestedQuit();
    }

  private:
    std::unique_ptr<MainComponent> main_component;
  };

  std::unique_ptr<MainWindow> main_window;
};
} // namespace

// NOLINTBEGIN(cppcoreguidelines-owning-memory,modernize-use-trailing-return-type)
START_JUCE_APPLICATION(LimitApplication)
// NOLINTEND(cppcoreguidelines-owning-memory,modernize-use-trailing-return-type)
