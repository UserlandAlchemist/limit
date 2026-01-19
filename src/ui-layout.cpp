#include "ui-layout.h"

#include <algorithm>

namespace limit {
auto computeUiLayout(UiLayoutParams params) -> UiLayout {
  UiLayout layout{};
  layout.header = LayoutRect{.x = 0, .y = 0, .width = params.width, .height = params.header_height};

  const auto clamped_height = std::max(params.height - params.header_height, 0);
  const auto visualization_height =
      static_cast<int>(static_cast<float>(clamped_height) * params.visualization_ratio);
  const auto bottom_height = std::max(clamped_height - visualization_height, 0);
  const auto encoder_width =
      static_cast<int>(static_cast<float>(params.width) * params.encoder_ratio);
  const auto secondary_width = std::max(params.width - encoder_width, 0);

  layout.visualization = LayoutRect{.x = 0,
                                    .y = params.header_height,
                                    .width = params.width,
                                    .height = visualization_height};
  layout.encoder = LayoutRect{.x = 0,
                              .y = params.header_height + visualization_height,
                              .width = encoder_width,
                              .height = bottom_height};
  layout.secondary = LayoutRect{.x = encoder_width,
                                .y = params.header_height + visualization_height,
                                .width = secondary_width,
                                .height = bottom_height};

  return layout;
}
} // namespace limit
