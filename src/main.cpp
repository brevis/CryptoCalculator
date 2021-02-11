#include <thread>
#include "Ui.cpp"

int main(int argc, const char* argv[]) {
    auto screen = ScreenInteractive::FitComponent();

    std::thread update([&screen]() {
        while (true) {
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(0.05s);
            screen.PostEvent(Event::Custom);
        }
    });

    DataProvider dp;
    auto cc = new CurrencyConverter(dp);
    auto *ui = new Ui(cc);
    screen.Loop(ui);

    return 0;
}