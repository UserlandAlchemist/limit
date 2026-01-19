#include <JuceHeader.h>

#include "main-component.h"

namespace {
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
        : juce::DocumentWindow(name, kWindowBackground, juce::DocumentWindow::allButtons),
          main_component(std::make_unique<limit::MainComponent>()) {
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
    static inline const juce::Colour kWindowBackground{0xff202020};

    std::unique_ptr<limit::MainComponent> main_component;
  };

  std::unique_ptr<MainWindow> main_window;
};
} // namespace

// NOLINTBEGIN(cppcoreguidelines-owning-memory,modernize-use-trailing-return-type)
START_JUCE_APPLICATION(LimitApplication)
// NOLINTEND(cppcoreguidelines-owning-memory,modernize-use-trailing-return-type)
