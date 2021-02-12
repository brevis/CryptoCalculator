#include <iostream>
#include <string>
#include <fstream>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

struct Rate {
    std::wstring currency;
    float rate;
    double_t base;
};

class DataProvider {
    struct Rate rates[100];
public:
    DataProvider() {
        std::string output = exec("curl --silent https://api.kuna.io/v3/exchange-rates");
        json resp = json::parse(output);
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

        int i = 0;
        for (json item : resp) {
            auto currency = item["currency"].get<std::string>();
            rates[i].currency = converter.from_bytes(currency);
            transform(
                    rates[i].currency.begin(), rates[i].currency.end(),
                    rates[i].currency.begin(),
                    toupper);
            rates[i].rate = item["usd"];
            if (rates[i].currency == L"USD") {
                rates[i].base = true;
            }
            i++;
        }
    }

    Rate *GetRates() {
        return rates;
    }

    std::string exec(const std::string &command) {
        system((command + " > temp.txt").c_str());
        std::ifstream ifs("temp.txt");
        std::string ret{std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>()};
        ifs.close();
        if (std::remove("temp.txt") != 0) {
            perror("Error deleting temporary file");
        }
        return ret;
    }
};