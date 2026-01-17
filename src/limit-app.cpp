#include <JuceHeader.h>

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
    const auto note = limit::mapKeyToMidiNote(static_cast<int>(key.getTextCharacter()));
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
