#include "ui-layout.h"
#include "ui-theme.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("UI layout splits header, visualization, encoder, and secondary regions") {
  const auto &theme = limit::getUiTheme();
  const int width = theme.window_width;
  const int height = theme.window_height;
  const int header_height = theme.header_height;
  const float visualization_ratio = theme.visualization_ratio;
  const float encoder_ratio = theme.encoder_ratio;
  const int body_height = height - header_height;
  const int expected_visualization_height =
      static_cast<int>(static_cast<float>(body_height) * visualization_ratio);
  const int expected_bottom_height = body_height - expected_visualization_height;
  const int expected_encoder_width =
      static_cast<int>(static_cast<float>(width) * encoder_ratio);
  const int expected_secondary_width = width - expected_encoder_width;

  const auto layout = limit::computeUiLayout({.width = width,
                                              .height = height,
                                              .header_height = header_height,
                                              .visualization_ratio = visualization_ratio,
                                              .encoder_ratio = encoder_ratio});

  // NOLINTBEGIN(cppcoreguidelines-avoid-do-while)
  REQUIRE(layout.header.x == 0);
  REQUIRE(layout.header.y == 0);
  REQUIRE(layout.header.width == width);
  REQUIRE(layout.header.height == header_height);

  REQUIRE(layout.visualization.x == 0);
  REQUIRE(layout.visualization.y == header_height);
  REQUIRE(layout.visualization.width == width);
  REQUIRE(layout.visualization.height == expected_visualization_height);

  REQUIRE(layout.encoder.x == 0);
  REQUIRE(layout.encoder.y == header_height + layout.visualization.height);
  REQUIRE(layout.encoder.width == expected_encoder_width);
  REQUIRE(layout.encoder.height == expected_bottom_height);

  REQUIRE(layout.secondary.x == layout.encoder.width);
  REQUIRE(layout.secondary.y == layout.encoder.y);
  REQUIRE(layout.secondary.width == expected_secondary_width);
  REQUIRE(layout.secondary.height == layout.encoder.height);
  // NOLINTEND(cppcoreguidelines-avoid-do-while)
}

TEST_CASE("UI theme is available for tests") {
  const auto &theme = limit::getUiTheme();

  // NOLINTBEGIN(cppcoreguidelines-avoid-do-while)
  REQUIRE(theme.window_width > 0);
  REQUIRE(theme.window_height > 0);
  REQUIRE(theme.font_name != nullptr);
  // NOLINTEND(cppcoreguidelines-avoid-do-while)
}
