#include <JuceHeader.h>

namespace {
class MainComponent final : public juce::AudioAppComponent,
                            private juce::MidiInputCallback {
public:
  MainComponent() {
    setSize(720, 420);
    setWantsKeyboardFocus(true);

    auto* device_manager = &deviceManager;
    const auto midi_inputs = juce::MidiInput::getAvailableDevices();
    for (const auto& device : midi_inputs) {
      device_manager->setMidiInputDeviceEnabled(device.identifier, true);
      device_manager->addMidiInputDeviceCallback(device.identifier, this);
    }

    setAudioChannels(2, 2);
  }

  ~MainComponent() override { shutdownAudio(); }

  void prepareToPlay(int /*samples_per_block_expected*/, double /*sample_rate*/) override {
    last_midi_message = "";
  }

  void getNextAudioBlock(const juce::AudioSourceChannelInfo& buffer_to_fill) override {
    buffer_to_fill.clearActiveBufferRegion();
  }

  void releaseResources() override {}

  void paint(juce::Graphics& g) override {
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
    g.setFont(juce::Font(juce::FontOptions{}.withHeight(24.0f).withStyle("bold")));
    g.drawText("Limit", getLocalBounds().removeFromTop(60), juce::Justification::centred);

    g.setFont(juce::Font(juce::FontOptions{}.withHeight(14.0f)));
    const auto text = last_midi_message.isEmpty() ? "MIDI: none" : "MIDI: " + last_midi_message;
    g.drawText(text, getLocalBounds().reduced(24), juce::Justification::bottomLeft);
  }

  void resized() override {}
  void parentHierarchyChanged() override { focusIfVisible(); }
  void visibilityChanged() override { focusIfVisible(); }
  bool keyPressed(const juce::KeyPress& key) override {
    const auto note = mapKeyToMidiNote(key);
    if (note >= 0) {
      last_midi_message = "note-on " + juce::MidiMessage::getMidiNoteName(note, true, true, 3);
      repaint();
      return true;
    }
    return false;
  }

private:
  int mapKeyToMidiNote(const juce::KeyPress& key) const {
    const auto base_note = 60;
    switch (key.getTextCharacter()) {
      case 'a': return base_note + 0;
      case 'w': return base_note + 1;
      case 's': return base_note + 2;
      case 'e': return base_note + 3;
      case 'd': return base_note + 4;
      case 'f': return base_note + 5;
      case 't': return base_note + 6;
      case 'g': return base_note + 7;
      case 'y': return base_note + 8;
      case 'h': return base_note + 9;
      case 'u': return base_note + 10;
      case 'j': return base_note + 11;
      case 'k': return base_note + 12;
      default: return -1;
    }
  }

  void focusIfVisible() {
    if (isShowing()) {
      grabKeyboardFocus();
    }
  }

  void handleIncomingMidiMessage(juce::MidiInput* /*source*/, const juce::MidiMessage& message) override {
    if (message.isNoteOn()) {
      last_midi_message = "note-on " + juce::MidiMessage::getMidiNoteName(message.getNoteNumber(), true, true, 3);
    } else if (message.isNoteOff()) {
      last_midi_message = "note-off " + juce::MidiMessage::getMidiNoteName(message.getNoteNumber(), true, true, 3);
    } else if (message.isController()) {
      last_midi_message = "cc " + juce::String(message.getControllerNumber()) + " = " + juce::String(message.getControllerValue());
    } else {
      last_midi_message = "message";
    }

    repaint();
  }

  juce::String last_midi_message;
};

class LimitApplication final : public juce::JUCEApplication {
public:
  const juce::String getApplicationName() override { return "Limit"; }
  const juce::String getApplicationVersion() override { return ProjectInfo::versionString; }
  bool moreThanOneInstanceAllowed() override { return true; }

  void initialise(const juce::String& /*command_line*/) override {
    main_window = std::make_unique<MainWindow>(getApplicationName());
  }

  void shutdown() override { main_window.reset(); }

  void systemRequestedQuit() override { quit(); }

  void anotherInstanceStarted(const juce::String& /*command_line*/) override {}

private:
  class MainWindow final : public juce::DocumentWindow {
  public:
    explicit MainWindow(juce::String name)
        : juce::DocumentWindow(std::move(name), juce::Colours::black, juce::DocumentWindow::allButtons) {
      setUsingNativeTitleBar(true);
      setResizable(true, true);
      setContentOwned(new MainComponent(), true);
      centreWithSize(getWidth(), getHeight());
      setVisible(true);
    }

    void closeButtonPressed() override { juce::JUCEApplication::getInstance()->systemRequestedQuit(); }
  };

  std::unique_ptr<MainWindow> main_window;
};
}  // namespace

START_JUCE_APPLICATION(LimitApplication)
