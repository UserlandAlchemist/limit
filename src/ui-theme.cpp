#include "ui-theme.h"

namespace limit {
auto getUiTheme() -> const UiTheme & {
  static const UiTheme theme{};
  return theme;
}
} // namespace limit
