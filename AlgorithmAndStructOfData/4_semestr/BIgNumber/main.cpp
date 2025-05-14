#include <iostream>
#include <vector>
#include <string>
#include <bitset>//библиотека булевых векторов
#include <algorithm>



//typedef unsigned char BASE;//2^8 система счисления
//typedef unsigned short DBASE;2^16 double BASE

//typedef unsigned short BASE;//2^16 система счисления
//tepedef unsigned int DBASE;2^32 double BASE

typedef unsigned int BASE;//2^32 система счисления
typedef unsigned long DBASE;//2 ^ 32 double BASE

#define BASE_SIZE (sizeof(BASE) * 8)

using namespace std;

class BigNumber
{
    vector<BASE> coefs;
    int len;

public:
    BigNumber(const string& str);
    BigNumber(int l = 0, bool fl = false);
    BigNumber(const BigNumber&) = default;//так как для вектора копирование числа определено
    ~BigNumber() = default;//так как для вектора деструктор определен\

    BigNumber operator=(const BigNumber&);

    bool operator==(const BigNumber&) const;
    bool operator > (const BigNumber&) const;
    bool operator < (const BigNumber&) const;
    bool operator != (const BigNumber&) const;
    bool operator >= (const BigNumber&) const;
    bool operator <= (const BigNumber&) const;

    BigNumber operator+(const BigNumber&);
    BigNumber& operator+=(const BigNumber&);//coefs + other.coefs 

    BigNumber operator+(const BASE&);
    BigNumber& operator+=(const BASE&);//coefs + BASE(одно число)
    
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

    BigNumber operator/(const BigNumber&);//
    BigNumber& operator/=(const BigNumber&);

    BASE operator%(const BASE&);

    void inputHex();//
    string outputHex();//

    friend ostream& operator<<(ostream& os, const BigNumber& other);//
    friend istream& operator>>(istream& os, const BigNumber& other);//
};

BigNumber::BigNumber(int lenght, bool fl)
{
    if (lenght <= 0)
    {
        coefs.push_back(0);
        len = 1;
        return;
    }

    if (fl == true)//если флаг true то будет большое число из нулей сделано для удобства операций
    {
        len = lenght;
        for (int i = 0; i < lenght; ++i)
        {
            coefs[i] = 0;
        }
        return;
    }
    //заполняем рандомными коэфицентами вектор
    for (int i = 0; i < lenght; i++)
    {
        BASE coef = rand();

        //rand создаёт число длины 16 для того чтобы заполнить uint надо 32
        if (BASE_SIZE > 16)
        {
            coef = coef << 16;
            coef += rand();
        }
        coefs.push_back(coef);
    }

    while (coefs[lenght - 1] == 0)//если последний коэф 0 генерируем заново
    {
        BASE coef = rand();
        if (BASE_SIZE > 16)
        {
            coef = coef << 16;
            coef += rand();
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

    bitset<BASE_SIZE> res(0);//нулевой булев вектор
    len = 0;
    int  k = 0;

    for (int i = str.size() - 1; i >= 0; i--)//цикл по строке
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

        bitset<BASE_SIZE> tmp(from_str_to_int);//двоичный вектор из 0 и 1//можно убрать
        res = res | (tmp << k);

        k = k + 4;
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

    len = coefs.size();

    while (coefs[len - 1] == 0 && len != 1)
    {
        coefs.pop_back();
        len--;
    }
}

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
        return false;

    if (len < other.len)
        return false;

    for (int i = len - 1; i < 0; ++i)
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
    return other > *this;//воспользуемся уже существующим оператором
}

bool BigNumber::operator!=(const BigNumber& other) const
{
    return !(*this == other);
}

bool BigNumber::operator>=(const BigNumber& other) const
{
    //>= по сути комбинация > и =
    return !(*this < other);
}

bool BigNumber::operator<=(const BigNumber& other) const
{
    //<= по сути комбинация < и =
    return !(*this > other);
}

BigNumber BigNumber::operator+(const BigNumber& other)
{
    int n = this->len;//длина число u
    int m = other.len;//длина числа v

    int l = 0;//длина результата числа w
    l = max(n, m) + 1;//может добавиться новый разряд
    int t = 0;
    t = min(n, m);

    BigNumber sum(l, true);

    DBASE tmp;
    int j = 0;//по разрядам
    int k = 0;//переход

    while (j < t)//сложение длины которая есть и там и там
    {
        //скаладываем оба вектора + перенос на след разряд
        tmp = this->coefs[j] + other.coefs[j] + k;

        //преобразуем в BASE и заносим то что осталось
        sum.coefs[j] = (BASE)tmp;

        k = (BASE)(tmp >> BASE_SIZE);// (>> BASE_SIZE) = /2^32
        // в k остается перенос который мы добавляем в следующий разряд

        j++;
    }

    while (j < n)// Если this длиннее
    {
        tmp = this->coefs[j] + k;
        sum.coefs[j] = (BASE)tmp;

        k = (BASE)(tmp >> BASE_SIZE);

        j++;
    }

    while (j < m)// Если other длиннее
    {
        tmp = other.coefs[j] + k;
        sum.coefs[j] = (BASE)tmp;

        k = (BASE)(tmp >> BASE_SIZE);

        j++;
    }

    //заполняем последнего переноса
    sum.coefs[j] = k;

    //удаление если перенос равен нулю
    if (sum.coefs[j] == 0)
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
    while (j < n)//сложение длины которая есть и там и там
    {
        tmp = (DBASE)coefs[j] + k;//прибавляем число
        sum.coefs[j] = (BASE)tmp;

        k = (BASE)(tmp >> BASE_SIZE);//смотрим сдвиг если переходим в новый разяряд

        j++;
    }

    sum.coefs[j] = k;
    if (sum.coefs[j] == 0)
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
    if (*this < other)//если вычитаемое меньше
    {
        cerr << "Error wiht size" << endl;
        exit(-2);
    }

    int n = len;
    int m = other.len;

    BigNumber res(n, true);

    int j = 0;
    int k = 0;
    DBASE tmp;

    while (j < m)//смотрим по длине other
    {
        tmp = (1 << BASE_SIZE) | coefs[j];//искусственно увеличиваем число
        tmp = tmp - other.coefs[j] - k;
        res.coefs[j] = (BASE)tmp;

        k = !(tmp >> BASE_SIZE);//проверка если мы занимали у следующего разряда
        j++;
    }

    while (j < n)//если осталось убираем перенос который мог остаться
    {
        tmp = (1 << BASE_SIZE) | coefs[j];
        tmp = tmp - k;//отнимаем перенос

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
    int n = len;//тут понимаем что число this > num_base
    BigNumber res(n, true);//делаем большое число из нулей

    DBASE tmp;

    int j = 0;//разряды
    BASE k = 0;//перенос

    

    tmp = (1 << BASE_SIZE) | coefs[j];
    tmp = tmp - num_base - k;

    res.coefs[j] = (BASE)tmp;
    k = !(tmp >> BASE_SIZE);

    while (j < n)
    {
        tmp = (1 << BASE_SIZE) | coefs[j];
        tmp = tmp - k;

        res.coefs[j] = (BASE)tmp;

        k = !(tmp >> BASE_SIZE);
        j++;
    }

    while (res.coefs[len - 1] == 0 && res.len > 1)
    {
        res.len--;
        res.coefs.pop_back();
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

    int j = 0;
    int i = 0;
    DBASE tmp;
    DBASE k = 0;

    BigNumber res(n + m, true);

    while (j < m)
    {
        if (other.coefs[j] == 0)
        {
            j++;
            continue;
        }

        k = 0;
        i = 0;
        while (i < n)
        {
            tmp = coefs[i] * other.coefs[j];  +k + res.coefs[i + j];//i + j значение на данный момент

            res.coefs[i + j] = (BASE)tmp;

            k = (BASE)(tmp >> BASE_SIZE);

            i++;
        }

        res.coefs[n + j] = k;//последний перенос

        j++;
    }

    while (res.len > 1 && res.coefs.back())
    {
        res.len--;
        res.coefs.pop_back();
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

    int j = 0;
    DBASE k = 0;
    DBASE tmp;

    while (j < n)
    {
        tmp = coefs[j] * num_base + k;//возможно надо каждую переменную приводить явно к DBASE
        res.coefs[j] = (BASE)tmp;

        k = (BASE)(tmp >> BASE_SIZE);

        j++;
    }

    res.coefs[j] = k;
    while(res.coefs[res.len - 1] == 0 && res.len > 1)
    {
        res.len--;
        res.coefs.pop_back();
    }


    return res;
}

BigNumber& BigNumber::operator*=(const BASE& num_base)
{
    *this = *this * num_base;
    return *this;
}

BigNumber BigNumber::operator/(const BASE& num_base)//првоерить
{
    if (num_base == 0)
    {
        cerr << "del from 0 IN /";
        exit(-1);
    }

    int n = len;

    BigNumber res(n, true);

    BASE r = 0;
    DBASE tmp;
    int j = n - 1;
    
    while (j >= 0)
    {
        tmp = (r << BASE_SIZE) + coefs[j];

        res.coefs[j] = (BASE)(tmp / num_base);
        r = (BASE)(tmp % num_base);

        j--;
    }

    while (res.coefs.back() == 0 && res.len > 1)
    {
        res.len--;
        res.coefs.pop_back();
    }

    return res;
}

BigNumber& BigNumber::operator/=(const BASE& num_base)//проверить
{
    *this = *this / num_base;
    return *this;
}

BASE BigNumber::operator%(const BASE& num_base)//проверить
{
    if (num_base == 0)
    {
        cerr << "del from 0 IN %";
        exit(-1);
    }

    BASE res = 0;
    int n = len;
    int j = n - 1;
    DBASE tmp;

    while (j >= 0)
    {
        tmp = (res << BASE_SIZE) + coefs[j];

        res = (BASE)(tmp & num_base);

        j--;
    }

    return res;
}


void BigNumber::inputHex()
{
    cout << "Hex string - ";
    string str;
    cin >> str;

    int k = 0;
    bitset<BASE_SIZE> res(0);

    for (int i = str.size() - 1; i >= 0; --i)
    {
        int str_from_hex = 0;
        if (str[i] >= '0' && str[i] <= '9')
            str_from_hex = str[i] - '0';

        else if (str[i] >= 'a' && str[i] <= 'f')
            str_from_hex = str[i] - 'a' + 10;

        else if (str[i] >= 'A' && str[i] <= 'F')
            str_from_hex = str[i] - 'A' + 10;

        else
        {
            cerr << "Error simbol not correct" << endl;
            return;
        }

        bitset<BASE_SIZE> tmp(str_from_hex);
        res = res | (tmp << k);

        k = k + 4;
        if (k >= BASE_SIZE)
        {
            k = 0;
            coefs.push_back((BASE)res.to_ulong());
            res.reset();

        }
    }

    if (k > 0)
    {
        coefs.push_back((BASE)res.to_ulong());
    }
}

string BigNumber::outputHex()
{
    int k = BASE_SIZE - 4;
    string str;

    for (int i = len - 1; i >= 0; --i)
    {
        while (k >= 0)
        {
            int tmp = (coefs[i] >> k) & (00010101);//15 
            if (tmp >= 0 && tmp <= 9)
                str += (char)(tmp + '0');

            if(tmp >= 10 && tmp <= 15)
                str += (char)(tmp - 10 + 'a');

            k = k - 4;

        }

        str += " ";
        k = BASE_SIZE - 4;
    }

    cout << str << endl;
}

ostream& operator<<(ostream& out, BigNumber& other)
{
    string res;
    BigNumber num(other);
    BigNumber null(1, true);

    while (num != null)
    {
        BASE tmp = num % (BASE)10;

        res = res + (char)(tmp + '0');
        num = num / 10;
    }

    if (res.empty())
    {
        out << "0";
    }

    else
    {
        for (int i = res.size() -1; i >= 0; --i)
        {
            out << res[i];
        }
    }

    return out;
}

istream& operator>>(istream& out, BigNumber& other)
{

}

//BigNumber BigNumber::operator/(const BigNumber& other)
//{
//
//}
//
//BigNumber& BigNumber::operator/=(const BigNumber& other)
//{
//    *this = *this / other;
//    return *this;
//}


void test()
{

}


int main()
{



    return 0;
}
