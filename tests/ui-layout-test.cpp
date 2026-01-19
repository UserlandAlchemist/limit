#include "ui-layout.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("UI layout splits header, visualization, encoder, and secondary regions") {
  constexpr int kWidth = 1000;
  constexpr int kHeight = 600;
  constexpr int kHeaderHeight = 60;
  constexpr float kVisualizationRatio = 0.55f;
  constexpr float kEncoderRatio = 0.3f;
  constexpr int kBodyHeight = kHeight - kHeaderHeight;
  constexpr int kExpectedVisualizationHeight =
      static_cast<int>(static_cast<float>(kBodyHeight) * kVisualizationRatio);
  constexpr int kExpectedBottomHeight = kBodyHeight - kExpectedVisualizationHeight;
  constexpr int kExpectedEncoderWidth =
      static_cast<int>(static_cast<float>(kWidth) * kEncoderRatio);
  constexpr int kExpectedSecondaryWidth = kWidth - kExpectedEncoderWidth;

  const auto layout = limit::computeUiLayout({.width = kWidth,
                                              .height = kHeight,
                                              .header_height = kHeaderHeight,
                                              .visualization_ratio = kVisualizationRatio,
                                              .encoder_ratio = kEncoderRatio});

  // NOLINTBEGIN(cppcoreguidelines-avoid-do-while)
  REQUIRE(layout.header.x == 0);
  REQUIRE(layout.header.y == 0);
  REQUIRE(layout.header.width == kWidth);
  REQUIRE(layout.header.height == kHeaderHeight);

  REQUIRE(layout.visualization.x == 0);
  REQUIRE(layout.visualization.y == kHeaderHeight);
  REQUIRE(layout.visualization.width == kWidth);
  REQUIRE(layout.visualization.height == kExpectedVisualizationHeight);

  REQUIRE(layout.encoder.x == 0);
  REQUIRE(layout.encoder.y == kHeaderHeight + layout.visualization.height);
  REQUIRE(layout.encoder.width == kExpectedEncoderWidth);
  REQUIRE(layout.encoder.height == kExpectedBottomHeight);

  REQUIRE(layout.secondary.x == layout.encoder.width);
  REQUIRE(layout.secondary.y == layout.encoder.y);
  REQUIRE(layout.secondary.width == kExpectedSecondaryWidth);
  REQUIRE(layout.secondary.height == layout.encoder.height);
  // NOLINTEND(cppcoreguidelines-avoid-do-while)
}
