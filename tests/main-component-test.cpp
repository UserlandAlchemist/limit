#include "main-component.h"
#include "keymap.h"
#include "ui-layout.h"
#include "ui-theme.h"

#include <array>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("MainComponent updates last MIDI message on key press") {
  juce::ScopedJuceInitialiser_GUI gui;
  limit::MainComponent component(false);
  component.prepareToPlay(0, 0.0);

  // NOLINTBEGIN(cppcoreguidelines-avoid-do-while)
  const auto handled = component.processKeyCharForTesting('g');
  REQUIRE(handled);

  const auto expected_note = limit::mapKeyToMidiNote('g');
  REQUIRE(expected_note >= 0);

  const auto expected = juce::String("note-on ") +
                        juce::MidiMessage::getMidiNoteName(expected_note, true, true, 3);
  REQUIRE(component.getLastMidiMessageForTesting() == expected);
  // NOLINTEND(cppcoreguidelines-avoid-do-while)
}

TEST_CASE("MainComponent updates last MIDI message on incoming MIDI") {
  juce::ScopedJuceInitialiser_GUI gui;
  limit::MainComponent component(false);
  component.prepareToPlay(0, 0.0);

  // NOLINTBEGIN(cppcoreguidelines-avoid-do-while)
  const auto message = juce::MidiMessage::noteOn(1, 61, static_cast<juce::uint8>(100));
  component.processMidiMessageForTesting(message);

  const auto expected = juce::String("note-on ") +
                        juce::MidiMessage::getMidiNoteName(61, true, true, 3);
  REQUIRE(component.getLastMidiMessageForTesting() == expected);

  const auto note_off = juce::MidiMessage::noteOff(1, 61);
  component.processMidiMessageForTesting(note_off);
  const auto expected_off = juce::String("note-off ") +
                            juce::MidiMessage::getMidiNoteName(61, true, true, 3);
  REQUIRE(component.getLastMidiMessageForTesting() == expected_off);

  const auto controller = juce::MidiMessage::controllerEvent(1, 7, 99);
  component.processMidiMessageForTesting(controller);
  REQUIRE(component.getLastMidiMessageForTesting() == "cc 7 = 99");

  const auto other = juce::MidiMessage::pitchWheel(1, 0);
  component.processMidiMessageForTesting(other);
  REQUIRE(component.getLastMidiMessageForTesting() == "message");

  const auto incoming = juce::MidiMessage::noteOn(1, 60, static_cast<juce::uint8>(80));
  component.handleIncomingMidiMessageForTesting(incoming);
  const auto expected_incoming = juce::String("note-on ") +
                                 juce::MidiMessage::getMidiNoteName(60, true, true, 3);
  REQUIRE(component.getLastMidiMessageForTesting() == expected_incoming);
  // NOLINTEND(cppcoreguidelines-avoid-do-while)
}

TEST_CASE("MainComponent handles dev keys and pads") {
  juce::ScopedJuceInitialiser_GUI gui;
  limit::MainComponent component(false);
  component.prepareToPlay(0, 0.0);

  // NOLINTBEGIN(cppcoreguidelines-avoid-do-while)
  REQUIRE(component.keyPressed(juce::KeyPress(juce::KeyPress::upKey)));
  REQUIRE(component.getLastMidiMessageForTesting() == "dev control bank 2");

  REQUIRE(component.keyPressed(juce::KeyPress(juce::KeyPress::F1Key)));
  REQUIRE(component.getLastMidiMessageForTesting() == "dev pad bank B");

  REQUIRE(component.keyPressed(juce::KeyPress(juce::KeyPress::F2Key)));
  REQUIRE(component.getLastMidiMessageForTesting() == "dev prog select");

  REQUIRE(component.keyPressed(juce::KeyPress(juce::KeyPress::F3Key)));
  REQUIRE(component.getLastMidiMessageForTesting() == "dev mod on");

  REQUIRE(component.keyPressed(juce::KeyPress(juce::KeyPress::F4Key)));
  REQUIRE(component.getLastMidiMessageForTesting() == "dev sustain on");

  REQUIRE(component.keyPressed(juce::KeyPress(juce::KeyPress::F5Key)));
  REQUIRE(component.getLastMidiMessageForTesting() == "dev octave -1");

  REQUIRE(component.keyPressed(juce::KeyPress(juce::KeyPress::F6Key)));
  REQUIRE(component.getLastMidiMessageForTesting() == "dev octave 0");

  REQUIRE(component.keyPressed(juce::KeyPress(juce::KeyPress::F7Key)));
  REQUIRE(component.getLastMidiMessageForTesting() == "dev pitch -1");

  REQUIRE(component.keyPressed(juce::KeyPress(juce::KeyPress::F8Key)));
  REQUIRE(component.getLastMidiMessageForTesting() == "dev pitch 0");

  REQUIRE(component.keyPressed(juce::KeyPress(juce::KeyPress::F3Key)));
  REQUIRE(component.getLastMidiMessageForTesting() == "dev mod off");

  REQUIRE(component.keyPressed(juce::KeyPress(juce::KeyPress::F4Key)));
  REQUIRE(component.getLastMidiMessageForTesting() == "dev sustain off");

  const std::array<int, 18> encoder_keys = {
      juce::KeyPress::numberPad7, juce::KeyPress::numberPad8, juce::KeyPress::numberPad9,
      juce::KeyPress::numberPad4, juce::KeyPress::numberPad5, juce::KeyPress::numberPad6,
      juce::KeyPress::numberPad1, juce::KeyPress::numberPad2, juce::KeyPress::numberPad3,
      juce::KeyPress::insertKey, juce::KeyPress::homeKey, juce::KeyPress::pageUpKey,
      juce::KeyPress::deleteKey, juce::KeyPress::endKey, juce::KeyPress::pageDownKey,
      juce::KeyPress::leftKey, juce::KeyPress::downKey, juce::KeyPress::rightKey};
  for (const auto key_code : encoder_keys) {
    REQUIRE(component.keyPressed(juce::KeyPress(key_code)));
    REQUIRE(component.getLastMidiMessageForTesting().startsWith("dev cc "));
  }

  REQUIRE(
      component.processEncoderActionForTesting(0, limit::DevEncoderAction::kIncrease));
  REQUIRE_FALSE(
      component.processEncoderActionForTesting(-1, limit::DevEncoderAction::kIncrease));

  const auto pad_key = juce::KeyPress(static_cast<int>('1'), juce::ModifierKeys::noModifiers,
                                      static_cast<juce::juce_wchar>('1'));
  REQUIRE(component.keyPressed(pad_key));
  REQUIRE(component.getLastMidiMessageForTesting().startsWith("dev pad 1"));

  REQUIRE(component.processPadIndexForTesting(0));
  REQUIRE_FALSE(component.processPadIndexForTesting(-1));

  REQUIRE_FALSE(component.processKeyCharForTesting('z'));
  REQUIRE_FALSE(component.keyPressed(juce::KeyPress(juce::KeyPress::spaceKey)));
  // NOLINTEND(cppcoreguidelines-avoid-do-while)
}

TEST_CASE("MainComponent paints into an image") {
  juce::ScopedJuceInitialiser_GUI gui;
  limit::MainComponent component(false);
  const auto &theme = limit::getUiTheme();
  const int canvas_width = theme.window_width;
  const int canvas_height = theme.window_height;
  constexpr int kSampleInset = 5;
  const auto layout = limit::computeUiLayout({.width = canvas_width,
                                              .height = canvas_height,
                                              .header_height = theme.header_height,
                                              .visualization_ratio = theme.visualization_ratio,
                                              .encoder_ratio = theme.encoder_ratio});
  juce::Image canvas(juce::Image::RGB, canvas_width, canvas_height, true);
  juce::Graphics g(canvas);
  component.paint(g);

  const auto panel_color = theme.panel;
  auto sample = [&](const limit::LayoutRect &rect) {
    const auto x = rect.x + rect.width - kSampleInset;
    const auto y = rect.y + rect.height - kSampleInset;
    return canvas.getPixelAt(x, y);
  };

  // NOLINTBEGIN(cppcoreguidelines-avoid-do-while)
  REQUIRE(sample(layout.header).getARGB() == panel_color.getARGB());
  REQUIRE(sample(layout.visualization).getARGB() == panel_color.getARGB());
  REQUIRE(sample(layout.encoder).getARGB() == panel_color.getARGB());
  REQUIRE(sample(layout.secondary).getARGB() == panel_color.getARGB());
  // NOLINTEND(cppcoreguidelines-avoid-do-while)
}

TEST_CASE("MainComponent renders secondary status box styling") {
  juce::ScopedJuceInitialiser_GUI gui;
  limit::MainComponent component(false);
  component.prepareToPlay(0, 0.0);
  constexpr int kMidiChannel = 1;
  constexpr int kTestNote = 60;
  constexpr int kTestVelocity = 100;
  component.processMidiMessageForTesting(juce::MidiMessage::noteOn(
      kMidiChannel, kTestNote, static_cast<juce::uint8>(kTestVelocity)));

  const auto &theme = limit::getUiTheme();
  const int canvas_width = theme.window_width;
  const int canvas_height = theme.window_height;
  const auto layout = limit::computeUiLayout({.width = canvas_width,
                                              .height = canvas_height,
                                              .header_height = theme.header_height,
                                              .visualization_ratio = theme.visualization_ratio,
                                              .encoder_ratio = theme.encoder_ratio});
  auto to_rect = [](const limit::LayoutRect &rect) {
    return juce::Rectangle<int>(rect.x, rect.y, rect.width, rect.height);
  };

  auto secondary_content = to_rect(layout.secondary).reduced(theme.padding);
  secondary_content.removeFromTop(static_cast<int>(theme.body_font_size));
  secondary_content.removeFromTop(theme.secondary_text_gap);
  const auto status_box = secondary_content.removeFromTop(
      static_cast<int>(theme.body_font_size * theme.status_box_line_count));

  juce::Image canvas(juce::Image::RGB, canvas_width, canvas_height, true);
  juce::Graphics g(canvas);
  component.paint(g);

  const auto border_sample_x = status_box.getX() + (status_box.getWidth() / 2);
  const auto border_sample_y = status_box.getY();
  const auto fill_sample_x = status_box.getRight() - theme.status_box_padding - 1;
  const auto fill_sample_y = status_box.getCentreY();

  // NOLINTBEGIN(cppcoreguidelines-avoid-do-while)
  REQUIRE(canvas.getPixelAt(border_sample_x, border_sample_y).getARGB() ==
          theme.border.getARGB());
  REQUIRE(canvas.getPixelAt(fill_sample_x, fill_sample_y).getARGB() ==
          theme.background.getARGB());
  // NOLINTEND(cppcoreguidelines-avoid-do-while)
}

TEST_CASE("MainComponent audio and focus helpers execute") {
  juce::ScopedJuceInitialiser_GUI gui;
  limit::MainComponent component(false);
  constexpr int kAudioChannels = 2;
  constexpr int kAudioSamples = 16;
  juce::AudioBuffer<float> buffer(kAudioChannels, kAudioSamples);
  juce::AudioSourceChannelInfo info(&buffer, 0, buffer.getNumSamples());
  component.getNextAudioBlock(info);
  component.releaseResources();
  component.parentHierarchyChanged();
  component.visibilityChanged();

  constexpr int kOctaveOffset = 5;
  component.setOctaveOffsetForTesting(kOctaveOffset);
  // NOLINTBEGIN(cppcoreguidelines-avoid-do-while)
  REQUIRE_FALSE(component.processKeyCharForTesting('\\'));
  // NOLINTEND(cppcoreguidelines-avoid-do-while)
}
