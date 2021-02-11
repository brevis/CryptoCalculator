#include <string>

struct Rate {
    std::wstring currency;
    float rate;
    bool base;
};

class DataProvider {
    struct Rate rates[100];
public:
    Rate* GetRates()
    {
        rates[0].currency = L"USD";
        rates[0].rate = 1;
        rates[0].base = true;

        rates[1].currency = L"BTC";
        rates[1].rate = 47230.11;

        rates[2].currency = L"ETH";
        rates[2].rate = 1768.59;

        rates[3].currency = L"BCH";
        rates[3].rate = 517.6;

        rates[4].currency = L"TON";
        rates[4].rate = 0.4103;

        return rates;
    }
};