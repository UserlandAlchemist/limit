#include <JuceHeader.h>

#include <array>

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
    if (handleDevEncoderBankSwitch(key) || handleDevPadBankSwitch(key) ||
        handleDevEncoder(key) || handleDevPad(key)) {
      return true;
    }
    const auto key_char = static_cast<unsigned char>(key.getTextCharacter());
    const auto note = limit::mapKeyToMidiNote(static_cast<int>(key_char));
    if (note >= 0) {
      last_midi_message = "note-on " + juce::MidiMessage::getMidiNoteName(note, true, true, 3);
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

  auto handleDevEncoderBankSwitch(const juce::KeyPress &key) -> bool {
    const auto key_code = key.getKeyCode();
    if (key_code == juce::KeyPress::F1Key) {
      limit::setDevEncoderBank(0, dev_state);
    } else if (key_code == juce::KeyPress::F2Key) {
      limit::setDevEncoderBank(1, dev_state);
    } else if (key_code == juce::KeyPress::F3Key) {
      limit::setDevEncoderBank(2, dev_state);
    } else {
      return false;
    }

    last_midi_message = "dev enc bank " + juce::String(dev_state.encoder_bank + 1);
    repaint();
    return true;
  }

  auto handleDevPadBankSwitch(const juce::KeyPress &key) -> bool {
    const auto key_code = key.getKeyCode();
    if (key_code == juce::KeyPress::F5Key) {
      limit::setDevPadBank(0, dev_state);
    } else if (key_code == juce::KeyPress::F6Key) {
      limit::setDevPadBank(1, dev_state);
    } else if (key_code == juce::KeyPress::F7Key) {
      limit::setDevPadBank(2, dev_state);
    } else {
      return false;
    }

    last_midi_message = "dev pad bank " + juce::String(dev_state.pad_bank + 1);
    repaint();
    return true;
  }

  auto handleDevEncoder(const juce::KeyPress &key) -> bool {
    const auto key_char = static_cast<int>(static_cast<unsigned char>(key.getTextCharacter()));
    const auto event = limit::handleDevEncoderKey(key_char, dev_state);
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
    const auto key_code = key.getKeyCode();
    const auto pad_index = mapKeyCodeToPadIndex(key_code);
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

  auto mapKeyCodeToPadIndex(int key_code) const -> int {
    using Key = juce::KeyPress;
    static const std::array<int, limit::kDevPadCount> kPadKeys = {
        Key::numberPad7,       Key::numberPad8,        Key::numberPad9,
        Key::numberPadDivide,  Key::numberPad4,        Key::numberPad5,
        Key::numberPad6,       Key::numberPadMultiply, Key::numberPad1,
        Key::numberPad2,       Key::numberPad3,        Key::numberPadSubtract,
        Key::numberPad0,       Key::numberPadDecimalPoint,
        Key::numberPadEquals,  Key::numberPadAdd};

    for (std::size_t index = 0; index < kPadKeys.size(); ++index) {
      if (key_code == kPadKeys.at(index)) {
        return static_cast<int>(index);
      }
    }
    return -1;
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
