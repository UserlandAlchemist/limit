#pragma once

namespace limit {
struct LayoutRect {
  int x = 0;
  int y = 0;
  int width = 0;
  int height = 0;
};

struct UiLayout {
  LayoutRect header;
  LayoutRect visualization;
  LayoutRect encoder;
  LayoutRect secondary;
};

struct UiLayoutParams {
  int width = 0;
  int height = 0;
  int header_height = 0;
  float visualization_ratio = 0.0f;
  float encoder_ratio = 0.0f;
};

auto computeUiLayout(UiLayoutParams params) -> UiLayout;
} // namespace limit
