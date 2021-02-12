#include <codecvt>
#include "ftxui/component/container.hpp"
#include "ftxui/component/input.hpp"
#include "ftxui/component/menu.hpp"
#include "ftxui/component/radiobox.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "CurrencyConverter.cpp"

using namespace ftxui;

class Ui : public Component {
    Container container = Container::Horizontal();
    RadioBox from;
    RadioBox to;
    Container subcontainer = Container::Vertical();
    Container input_container = Container::Horizontal();
    Input input_add;
    Menu input;

    CurrencyConverter *converter;
    float entered_amount = 0;
    float result_amount = 0;
    std::wstring selected_from_currency = L"UAH";
    std::wstring selected_to_currency = L"UAH";

public:
    Ui(CurrencyConverter *c) {
        converter = c;
        Add(&container);

        container.Add(&subcontainer);

        subcontainer.Add(&input_container);

        input_add.on_change = [this] {
            try {
                entered_amount = stof(input_add.content);
            } catch (std::exception e) {
                entered_amount = 0;
            }
            if (entered_amount < 0) {
                entered_amount = 0;
            }
            Calculate();
        };
        input_container.Add(&input_add);
        input_container.Add(&input);

        Rate *rates = converter->GetAvailableCurrenciesRates();
        for (int i = 0; i < 100; i++)
        {
            Rate rate = rates[i];
            if (rate.currency.empty()) break;
            from.entries.push_back(rate.currency);
            to.entries.push_back(rate.currency);
        }

        from.on_change = [this] {
            selected_from_currency = from.entries[from.selected];
            Calculate();
        };

        to.on_change = [this] {
            selected_to_currency = to.entries[to.selected];
            Calculate();
        };

        container.Add(&from);
        container.Add(&to);
    }

    void Calculate() {
        result_amount = converter->convert(entered_amount, selected_from_currency, selected_to_currency);
    }

    Element Render() {
        auto from_win = window(text(L" Из "), from.Render() | frame) | color(Color::BlueLight);
        auto to_win = window(text(L" В "), to.Render() | frame) | color(Color::GreenLight);

        auto input_win = window(text(L" Количество "),
        hbox({
            vbox({
                hbox({
                    input_add.Render(),
                }) | size(WIDTH, EQUAL, 20) | size(HEIGHT, EQUAL, 1),
            })
        }));

        focus(input_win);

        char entered_buffer[100];
        sprintf(entered_buffer, "%.4f", entered_amount);

        char result_buffer[100];
        sprintf(result_buffer, "%.4f", result_amount);

        return vbox({
            vbox({
                input_win | size(WIDTH, EQUAL, 60) | size(HEIGHT, LESS_THAN, 4),
            }),
            hbox({
                from_win | size(WIDTH, EQUAL, 16) | size(HEIGHT, LESS_THAN, 20),
                to_win | size(WIDTH, EQUAL, 16) | size(HEIGHT, LESS_THAN, 20),
            }),
            window(text(L" Результат "),
            hbox({
                    vbox({
                        hbox({
                            text(stringToWstring(entered_buffer)) |
                            color(Color::BlueLight),
                            text(L" " + selected_from_currency) |
                            color(Color::BlueLight),
                            text(L" = "),
                            text(stringToWstring(result_buffer)) | color(Color::GreenLight),
                            text(L" " + selected_to_currency) |
                            color(Color::GreenLight),
                        }) | size(WIDTH, EQUAL, 60) | size(HEIGHT, EQUAL, 1),
                    }),
                })
            ) | color(Color::RedLight),
            window(text(L" *** РЕКЛАМА *** "),
        vbox({
                    hbox({
                        text(L"Зарабатывай на бирже:"),
                    }) | size(WIDTH, EQUAL, 60) | size(HEIGHT, EQUAL, 1),
                    hbox({
                        text(L"https://kuna.io/?r=kunaid-duob8z1cegq5") |
                        color(Color::Blue),
                    }) | size(WIDTH, EQUAL, 60) | size(HEIGHT, EQUAL, 1)
                })
            ) | color(Color::Yellow)
        }) | flex_grow | border;
    }

    std::wstring stringToWstring(const std::string &t_str) {
        typedef std::codecvt_utf8<wchar_t> convert_type;
        std::wstring_convert<convert_type, wchar_t> converter;
        return converter.from_bytes(t_str);
    }
};
