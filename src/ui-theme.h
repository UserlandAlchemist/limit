#pragma once

#include <juce_graphics/juce_graphics.h>

namespace limit {
struct UiTheme {
  static constexpr const char *kFontName = "Topaz Plus NF Mono";
  static constexpr float kTitleFontSize = 24.0f;
  static constexpr float kBodyFontSize = 14.0f;
  static constexpr int kWindowWidth = 720;
  static constexpr int kWindowHeight = 420;
  static constexpr int kHeaderHeight = 60;
  static constexpr int kPadding = 16;
  static constexpr int kSecondaryTextGap = 6;
  static constexpr int kStatusBoxPadding = 8;
  static constexpr float kStatusBoxLineCount = 2.0f;
  static constexpr float kVisualizationRatio = 0.55f;
  static constexpr float kEncoderRatio = 0.3f;
  static constexpr juce::uint32 kColorBackground = 0xff202020;
  static constexpr juce::uint32 kColorPanel = 0xff242424;
  static constexpr juce::uint32 kColorBorder = 0xff303030;
  static constexpr juce::uint32 kColorText = 0xffe6e6e6;
  static constexpr juce::uint32 kColorAccentBlue = 0xff5078ff;
  static constexpr juce::uint32 kColorBevelLight = 0x26ffffff;
  static constexpr juce::uint32 kColorBevelDark = 0x66000000;

  const char *font_name = kFontName;
  float title_font_size = kTitleFontSize;
  float body_font_size = kBodyFontSize;
  int window_width = kWindowWidth;
  int window_height = kWindowHeight;
  int header_height = kHeaderHeight;
  int padding = kPadding;
  int secondary_text_gap = kSecondaryTextGap;
  int status_box_padding = kStatusBoxPadding;
  float status_box_line_count = kStatusBoxLineCount;
  float visualization_ratio = kVisualizationRatio;
  float encoder_ratio = kEncoderRatio;
  juce::Colour background{kColorBackground};
  juce::Colour panel{kColorPanel};
  juce::Colour border{kColorBorder};
  juce::Colour text{kColorText};
  juce::Colour accent_blue{kColorAccentBlue};
  juce::Colour bevel_light{kColorBevelLight};
  juce::Colour bevel_dark{kColorBevelDark};
};

auto getUiTheme() -> const UiTheme &;
} // namespace limit
