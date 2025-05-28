
#include <iostream>
#include <vector>
#include <string>
#include <bitset>
#include <algorithm>
#include <cstdlib> // для rand()

typedef unsigned int BASE;               // 2^32 система счисления
typedef unsigned long long int DBASE;   // 2^64 double BASE

#define BASE_SIZE (sizeof(BASE) * 8)

using namespace std;

class BigNumber
{
    vector<BASE> coefs;
    int len;

public:
    BigNumber(const string& str);
    BigNumber(int l = 0, bool fl = false);
    BigNumber(const BigNumber&) = default;
    ~BigNumber() = default;

    BigNumber operator=(const BigNumber&);

    bool operator==(const BigNumber&) const;
    bool operator > (const BigNumber&) const;
    bool operator < (const BigNumber&) const;
    bool operator != (const BigNumber&) const;
    bool operator >= (const BigNumber&) const;
    bool operator <= (const BigNumber&) const;

    BigNumber operator+(const BigNumber&);
    BigNumber& operator+=(const BigNumber&);

    BigNumber operator+(const BASE&);
    BigNumber& operator+=(const BASE&);

    BigNumber operator-(const BigNumber&);
    BigNumber& operator-=(const BigNumber&);

    BigNumber operator-(const BASE&);
    BigNumber& operator-=(const BASE&);

    BigNumber operator*(const BigNumber&);
    BigNumber& operator*=(const BigNumber&);

    BigNumber operator*(const BASE&);
    BigNumber& operator*=(const BASE&);

    BigNumber operator/(const BASE&);
    BigNumber& operator /= (const BASE&);

    // Деление и остаток для BigNumber на BigNumber не реализованы
    BigNumber operator/(const BigNumber&) = delete;
    BigNumber& operator/=(const BigNumber&) = delete;

    BigNumber operator%(const BASE&);
    // Остаток для BigNumber на BigNumber не реализован
    BigNumber operator%(const BigNumber&) = delete;
    BigNumber& operator%=(const BigNumber&) = delete;

    void inputHex()
    {
        cout << "Input hex string - ";
        string str;
        cin >> str;

        coefs.clear();
        len = 0;

        if (str.empty()) {
            coefs.push_back(0);
            len = 1;
            return;
        }

        bitset<BASE_SIZE> res(0);
        int k = 0;

        for (int i = (int)str.size() - 1; i >= 0; --i)
        {
            unsigned int int_from_hex = 0;

            if (str[i] >= '0' && str[i] <= '9') {
                int_from_hex = str[i] - '0';
            }
            else if (str[i] >= 'a' && str[i] <= 'f') {
                int_from_hex = str[i] - 'a' + 10;
            }
            else if (str[i] >= 'A' && str[i] <= 'F') {
                int_from_hex = str[i] - 'A' + 10;
            }
            else {
                cerr << "ERROR: Invalid hex character '" << str[i] << "'" << endl;
                coefs.clear();
                coefs.push_back(0);
                len = 1;
                return;
            }

            res |= bitset<BASE_SIZE>(int_from_hex) << k;
            k += 4;

            if (k >= BASE_SIZE) {
                coefs.push_back((BASE)res.to_ulong());
                res.reset();
                k = 0;
            }
        }

        if (k > 0) {
            coefs.push_back((BASE)res.to_ulong());
        }

        len = (int)coefs.size();

        while (len > 1 && coefs.back() == 0) {
            coefs.pop_back();
            len--;
        }
    }

    void outputHex()
    {
        int k = BASE_SIZE - 4;

        string str;
        for (auto ix = coefs.rbegin(); ix != coefs.rend(); ++ix)
        {
            while (k >= 0)
            {
                int tmp = (*ix >> k) & 15;
                if (tmp >= 0 && tmp <= 9)
                {
                    str += (char)(tmp + '0');
                }
                else if (tmp >= 10 && tmp <= 15)
                {
                    str += (char)(tmp - 10 + 'a');
                }

                k -= 4;
            }

            str += " ";
            k = BASE_SIZE - 4;
        }

        cout << str << endl;
    }

    friend ostream& operator<<(ostream& out, const BigNumber& other)
    {
        if (other.len == 1 && other.coefs[0] == 0) {
            out << "0";
            return out;
        }

        string output;
        BigNumber number(other);
        BigNumber zeroNum(1, true);

        while (number != zeroNum) {
            BASE tmp = (number % (BASE)10).coefs[0];
            output += (char)(tmp + '0');
            number /= (BASE)10;
        }

        reverse(output.begin(), output.end());
        out << output;

        return out;
    }

    friend istream& operator>>(istream& in, BigNumber& other)
    {
        string tmp;
        in >> tmp;

        for (char c : tmp) {
            if (!isdigit(c)) {
                cerr << "Input error!\n";
                exit(-4);
            }
        }

        other = BigNumber(1, true);

        for (char c : tmp) {
            int t = c - '0';
            other = other * (BASE)10 + (BASE)t;
        }

        while (other.coefs.size() > 1 && other.coefs.back() == 0) {
            other.coefs.pop_back();
            other.len--;
        }

        return in;
    }
};

// Конструкторы

BigNumber::BigNumber(int lenght, bool fl)
{
    if (lenght <= 0)
    {
        coefs.push_back(0);
        len = 1;
        return;
    }

    if (fl == true) // большое число из нулей
    {
        len = lenght;
        coefs.resize(lenght, 0);
        return;
    }

    for (int i = 0; i < lenght; i++)
    {
        BASE coef = rand();

        if (BASE_SIZE > 16)
        {
            coef = (coef << 16) + rand();
        }
        coefs.push_back(coef);
    }

    while (coefs[lenght - 1] == 0)
    {
        BASE coef = rand();
        if (BASE_SIZE > 16)
        {
            coef = (coef << 16) + rand();
        }
        coefs[lenght - 1] = coef;
    }

    len = lenght;
}

BigNumber::BigNumber(const string& str)
{
    if (str.empty())
    {
        coefs.push_back(0);
        len = 1;
        return;
    }

    bitset<BASE_SIZE> res(0);
    len = 0;
    int  k = 0;

    for (int i = (int)str.size() - 1; i >= 0; i--)
    {
        unsigned int from_str_to_int = 0;

        if (str[i] >= '0' && str[i] <= '9')
            from_str_to_int = str[i] - '0';
        else if (str[i] >= 'a' && str[i] <= 'f')
            from_str_to_int = str[i] - 'a' + 10;
        else if (str[i] >= 'A' && str[i] <= 'F')
            from_str_to_int = str[i] - 'A' + 10;
        else
        {
            cerr << "WRONG" << endl;
            return;
        }

        bitset<BASE_SIZE> tmp(from_str_to_int);
        res = res | (tmp << k);

        k += 4;
        if (k >= BASE_SIZE)
        {
            k = 0;
            coefs.push_back((BASE)res.to_ulong());
            res = 0;
        }
    }

    if (k > 0)
    {
        coefs.push_back((BASE)res.to_ulong());
    }

    len = (int)coefs.size();

    while (len > 1 && coefs[len - 1] == 0)
    {
        coefs.pop_back();
        len--;
    }
}

// Операторы

BigNumber BigNumber::operator=(const BigNumber& other)
{
    this->coefs = other.coefs;
    this->len = other.len;
    return *this;
}

bool BigNumber::operator==(const BigNumber& other) const
{
    if (len != other.len)
        return false;

    for (int i = 0; i < len; ++i)
    {
        if (coefs[i] != other.coefs[i])
            return false;
    }

    return true;
}

bool BigNumber::operator>(const BigNumber& other) const
{
    if (len > other.len)
        return true;

    if (len < other.len)
        return false;

    for (int i = len - 1; i >= 0; --i)
    {
        if (coefs[i] > other.coefs[i])
            return true;
        if (coefs[i] < other.coefs[i])
            return false;
    }

    return false;
}

bool BigNumber::operator<(const BigNumber& other) const
{
    return other > *this;
}

bool BigNumber::operator!=(const BigNumber& other) const
{
    return !(*this == other);
}

bool BigNumber::operator>=(const BigNumber& other) const
{
    return !(*this < other);
}

bool BigNumber::operator<=(const BigNumber& other) const
{
    return !(*this > other);
}

BigNumber BigNumber::operator+(const BigNumber& other)
{
    int n = this->len;
    int m = other.len;

    int l = max(n, m) + 1;
    int t = min(n, m);

    BigNumber sum(l, true);

    DBASE tmp;
    int j = 0;
    int k = 0;

    while (j < t)
    {
        tmp = (DBASE)coefs[j] + other.coefs[j] + k;
        sum.coefs[j] = (BASE)tmp;
        k = (BASE)(tmp >> BASE_SIZE);
        j++;
    }

    while (j < n)
    {
        tmp = (DBASE)coefs[j] + k;
        sum.coefs[j] = (BASE)tmp;
        k = (BASE)(tmp >> BASE_SIZE);
        j++;
    }

    while (j < m)
    {
        tmp = (DBASE)other.coefs[j] + k;
        sum.coefs[j] = (BASE)tmp;
        k = (BASE)(tmp >> BASE_SIZE);
        j++;
    }

    sum.coefs[j] = k;

    if (sum.coefs[j] == 0 && sum.len > 1)
    {
        sum.len--;
        sum.coefs.pop_back();
    }

    return sum;
}

BigNumber& BigNumber::operator+=(const BigNumber& other)
{
    *this = *this + other;
    return *this;
}

BigNumber BigNumber::operator+(const BASE& num_base)
{
    int n = len;
    BigNumber sum(n + 1, true);

    DBASE tmp;
    DBASE k = num_base;

    int j = 0;
    while (j < n)
    {
        tmp = (DBASE)coefs[j] + k;
        sum.coefs[j] = (BASE)tmp;
        k = (BASE)(tmp >> BASE_SIZE);
        j++;
    }

    sum.coefs[j] = (BASE)k;
    if (sum.coefs[j] == 0 && sum.len > 1)
    {
        sum.len--;
        sum.coefs.pop_back();
    }

    return sum;
}

BigNumber& BigNumber::operator+=(const BASE& num_base)
{
    *this = *this + num_base;
    return *this;
}

BigNumber BigNumber::operator-(const BigNumber& other)
{
    if (*this < other)
    {
        cerr << "Error: subtraction result would be negative" << endl;
        exit(-2);
    }

    int n = len;
    int m = other.len;

    BigNumber res(n, true);

    int j = 0;
    int k = 0;
    DBASE tmp;

    while (j < m)
    {
        tmp = ((DBASE)1 << BASE_SIZE) | coefs[j];
        tmp = tmp - other.coefs[j] - k;
        res.coefs[j] = (BASE)tmp;
        k = !(tmp >> BASE_SIZE);
        j++;
    }

    while (j < n)
    {
        tmp = ((DBASE)1 << BASE_SIZE) | coefs[j];
        tmp = tmp - k;
        res.coefs[j] = (BASE)tmp;
        k = !(tmp >> BASE_SIZE);
        j++;
    }

    while (res.coefs.size() > 1 && res.coefs.back() == 0)
    {
        res.coefs.pop_back();
        res.len--;
    }

    return res;
}

BigNumber& BigNumber::operator-=(const BigNumber& other)
{
    *this = *this - other;
    return *this;
}

BigNumber BigNumber::operator-(const BASE& num_base)
{
    if (*this < BigNumber(to_string(num_base))) {
        cerr << "Error: subtraction result would be negative" << endl;
        exit(-2);
    }

    int n = len;
    BigNumber res(n, true);

    DBASE tmp;
    BASE k = 0;
    int j = 0;

    tmp = ((DBASE)1 << BASE_SIZE) | coefs[j];
    tmp = tmp - num_base - k;
    res.coefs[j] = (BASE)tmp;
    k = !(tmp >> BASE_SIZE);
    j++;

    while (j < n)
    {
        tmp = ((DBASE)1 << BASE_SIZE) | coefs[j];
        tmp = tmp - k;
        res.coefs[j] = (BASE)tmp;
        k = !(tmp >> BASE_SIZE);
        j++;
    }

    while (res.coefs.size() > 1 && res.coefs.back() == 0)
    {
        res.coefs.pop_back();
        res.len--;
    }

    return res;
}

BigNumber& BigNumber::operator-=(const BASE& num_base)
{
    *this = *this - num_base;
    return *this;
}

BigNumber BigNumber::operator*(const BigNumber& other)
{
    int n = len;
    int m = other.len;

    DBASE tmp;
    BigNumber res(n + m, true);

    for (int j = 0; j < m; ++j)
    {
        if (other.coefs[j] == 0)
            continue;

        DBASE k = 0;
        for (int i = 0; i < n; ++i)
        {
            tmp = (DBASE)coefs[i] * (DBASE)other.coefs[j] + k + (DBASE)res.coefs[i + j];
            res.coefs[i + j] = (BASE)tmp;
            k = (BASE)(tmp >> BASE_SIZE);
        }

        res.coefs[n + j] = k;
    }

    while (res.len > 1 && res.coefs.back() == 0)
    {
        res.coefs.pop_back();
        res.len--;
    }

    return res;
}

BigNumber& BigNumber::operator*=(const BigNumber& other)
{
    *this = *this * other;
    return *this;
}

BigNumber BigNumber::operator*(const BASE& num_base)
{
    int n = len;
    BigNumber res(n + 1, true);

    DBASE k = 0;
    DBASE tmp;

    for (int j = 0; j < n; ++j)
    {
        tmp = (DBASE)coefs[j] * num_base + k;
        res.coefs[j] = (BASE)tmp;
        k = (BASE)(tmp >> BASE_SIZE);
    }

    res.coefs[n] = k;

    while (res.len > 1 && res.coefs.back() == 0)
    {
        res.coefs.pop_back();
        res.len--;
    }

    return res;
}

BigNumber& BigNumber::operator*=(const BASE& num_base)
{
    *this = *this * num_base;
    return *this;
}

BigNumber BigNumber::operator/(const BASE& num)
{
    if (num == 0)
    {
        cerr << "Error: division by zero" << endl;
        exit(-2);
    }

    int n = (int)coefs.size();
    if (n == 0)
        return BigNumber(1, true);

    BigNumber res(n, true);
    res.len = n;

    DBASE tmp = 0;
    BASE r = 0;

    for (int i = n - 1; i >= 0; --i)
    {
        tmp = ((DBASE)r << BASE_SIZE) + coefs[i];
        res.coefs[i] = (BASE)(tmp / num);
        r = (BASE)(tmp % num);
    }

    while (res.coefs.size() > 1 && res.coefs.back() == 0)
    {
        res.coefs.pop_back();
        res.len--;
    }

    return res;
}

BigNumber& BigNumber::operator/=(const BASE& num)
{
    *this = *this / num;
    return *this;
}

BigNumber BigNumber::operator%(const BASE& num)
{
    if (num == 0)
    {
        cerr << "Error num == 0" << endl;
        exit(-1);
    }

    int n = (int)coefs.size();

    DBASE tmp;
    BASE r = 0;

    for (int i = n - 1; i >= 0; --i)
    {
        tmp = (((DBASE)r << BASE_SIZE) + coefs[i]);
        r = (BASE)(tmp % num);
    }

    BigNumber res(1, true);
    res.coefs[0] = r;

    return res;
}


int main()
{
    BigNumber num1("f");  // 15 decimal
    BigNumber num2("10"); // 16 decimal

    cout << "num1 (hex): ";
    num1.outputHex();

    cout << "num2 (hex): ";
    num2.outputHex();

    BigNumber num3(5); // случайное 5-разрядное число
    cout << "num3 (random hex): ";
    num3.outputHex();

    cout << "num3 (decimal): " << num3 << endl;

    num2 += num1;
    cout << "num2 + num1 (hex): ";
    num2.outputHex(); // должно быть 1f (31 decimal)

    num2 -= num1;
    cout << "num2 - num1 (hex): ";
    num2.outputHex(); // должно быть 10 (16 decimal)

    BigNumber num4(0);

    num4 = num1 + num2 + num2 + num1 - num1 - num2;
    cout << "num4 (hex): ";
    num4.outputHex();

    BigNumber num5(0);

    num5 = num1 * num3;
    cout << "num5 = num1 * num3 (hex): ";
    num5.outputHex();

    num5 *= num1;
    cout << "num5 *= num1 (hex): ";
    num5.outputHex();

    return 0;
}
