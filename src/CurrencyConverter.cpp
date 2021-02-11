#include "DataProvider.cpp"

class CurrencyConverter {
private:
    DataProvider dataProvider;
public:
    CurrencyConverter(DataProvider dp)
    {
        dataProvider = dp;
    }

    Rate* GetAvailableCurrenciesRates()
    {
        return dataProvider.GetRates();
    }

    float convert(float amount, std::wstring from, std::wstring to)
    {
        // TODO: optimize this

        Rate *rates = GetAvailableCurrenciesRates();

        // get base currency
        std::wstring base_currency;
        for (int i = 0; i < 100; i++)
        {
            Rate rate = rates[i];
            if (rate.base) base_currency = rate.base;
            if (rate.currency.empty()) break;
        }

        // how much amount is in base currency
        float amount_in_base = 0;
        for (int i = 0; i < 100; i++)
        {
            Rate rate = rates[i];
            if (rate.currency == from) {
                amount_in_base = amount * rate.rate;
                break;
            }
            if (rate.currency.empty()) break;
        }

        // how much amount_in_base in to currency
        float res = 0;
        for (int i = 0; i < 100; i++)
        {
            Rate rate = rates[i];
            if (rate.currency == to) {
                res = amount_in_base * amount / rate.rate;
                break;
            }
            if (rate.currency.empty()) break;
        }

        return res;
    }
};