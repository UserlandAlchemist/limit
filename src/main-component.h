#pragma once

#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "dev-controller.h"
#include "ui-layout.h"

namespace limit {
class MainComponent final : public juce::AudioAppComponent, private juce::MidiInputCallback {
public:
  explicit MainComponent(bool enable_audio = true);
  ~MainComponent() override;
  MainComponent(const MainComponent &) = delete;
  auto operator=(const MainComponent &) -> MainComponent & = delete;
  MainComponent(MainComponent &&) = delete;
  auto operator=(MainComponent &&) -> MainComponent & = delete;

  void prepareToPlay(int samples_per_block_expected, double sample_rate) override;
  void getNextAudioBlock(const juce::AudioSourceChannelInfo &buffer_to_fill) override;
  void releaseResources() override;
  void paint(juce::Graphics &g) override;
  void resized() override;
  void parentHierarchyChanged() override;
  void visibilityChanged() override;
  auto keyPressed(const juce::KeyPress &key) -> bool override;

  auto getLastMidiMessageForTesting() const -> juce::String;
  void processMidiMessageForTesting(const juce::MidiMessage &message);
  auto processKeyCharForTesting(int key_char) -> bool;
  void handleIncomingMidiMessageForTesting(const juce::MidiMessage &message);
  auto processEncoderActionForTesting(int encoder_index, limit::DevEncoderAction action) -> bool;
  auto processPadIndexForTesting(int pad_index) -> bool;
  void setOctaveOffsetForTesting(int offset);

private:
  void handleIncomingMidiMessage(juce::MidiInput *source,
                                 const juce::MidiMessage &message) override;

  auto handleDevControlBankCycle(const juce::KeyPress &key) -> bool;
  auto handleDevPadBankCycle(const juce::KeyPress &key) -> bool;
  auto handleDevUtilityButtons(const juce::KeyPress &key) -> bool;
  auto handleDevEncoder(const juce::KeyPress &key) -> bool;
  auto handleDevPad(const juce::KeyPress &key) -> bool;
  auto mapKeyCharToPadIndex(int key_char) const -> int;
  auto minOctaveOffset() const -> int;
  auto maxOctaveOffset() const -> int;
  void focusIfVisible();
  void processMidiMessage(const juce::MidiMessage &message);
  auto processKeyChar(int key_char) -> bool;

  struct EncoderKeyAction {
    int encoder_index = 0;
    limit::DevEncoderAction action = limit::DevEncoderAction::kReset;
  };

  auto mapKeyToEncoderAction(const juce::KeyPress &key) const
      -> std::optional<EncoderKeyAction>;

  static auto toRectangle(const limit::LayoutRect &rect) -> juce::Rectangle<int>;
  void drawPanel(juce::Graphics &g, juce::Rectangle<int> area) const;

  static constexpr int kMidiMin = 0;
  static constexpr int kMidiMax = 127;
  static constexpr int kSemitone = 12;
  static constexpr int kEncoderIndex0 = 0;
  static constexpr int kEncoderIndex1 = 1;
  static constexpr int kEncoderIndex2 = 2;
  static constexpr int kEncoderIndex3 = 3;
  static constexpr int kEncoderIndex4 = 4;
  static constexpr int kEncoderIndex5 = 5;

  juce::String last_midi_message;
  limit::DevControllerState dev_state{};
  int note_octave_offset = 0;
  bool mod_active = false;
  bool sustain_active = false;
  int pitch_offset = 0;
  static constexpr int kPitchMin = -12;
  static constexpr int kPitchMax = 12;
};
} // namespace limit
