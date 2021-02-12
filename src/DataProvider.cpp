#include <iostream>
#include <string>
#include "curl/curl.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

struct Rate {
    std::wstring currency;
    float rate;
    bool base;
};

size_t WriteCallback(char *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

class DataProvider {
    struct Rate rates[100];
public:
    DataProvider() {
        std::string readBuffer;

        curl_global_init(CURL_GLOBAL_ALL);
        CURL* curl = curl_easy_init();
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.kuna.io/v3/exchange-rates");
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        json resp = json::parse(readBuffer);

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

    Rate* GetRates()
    {
        return rates;
    }
};