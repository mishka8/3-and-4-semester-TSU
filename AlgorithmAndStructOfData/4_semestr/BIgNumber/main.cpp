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
    BigNumber(int l = 0);
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
    BigNumber& operator+=(const BigNumber&);
    //coefs + other.coefs


    BigNumber operator+(const BASE&);
    BigNumber& operator+=(const BASE&);
    //coefs + BASE(одно число)

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

    BigNumber operator%(const BASE&);

    void inputHex(const string& num)
    {
        BigNumber res(num);
        *this = res;

    }
    string outputHex()
    {
        
    }


    friend ostream& operator<<(ostream& os, const BigNumber& other)
    {

    }

    friend istream& operator>>(istream& os, const BigNumber& other)
    {

    }

};

BigNumber::BigNumber(int lenght)
{
    if (lenght <= 0) {
        coefs.push_back(0);
        len = 1;
        return;
    }

    //заполняем рандомными коэфицентами вектор
    for (int i = 0; i < lenght; i++)
    {
        BASE coef = rand();

        //rand создаёт число длины 16 для того чтобы заполнить uint надо 32
        if (BASE_SIZE > 16)
        {
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
}

BigNumber::BigNumber(const string& str)
{
    if (str.empty())
    {
        len = 0;
        return;
    }


    bitset<BASE_SIZE> res(0);//нулевой булев вектор
    len = 0;
    int  k = 0;

    for (int i = str.size() - 1; i >= 0; i--)//цикл по строке
    {
        int from_str_to_int = 0;

        if (str[i] >= '0' && str[i] <= '9')
            from_str_to_int = str[i] - '0';

        else if (str[i] >= 'a' && str[i] <= 'f')
            from_str_to_int = str[i] - 'a' + 10;

        else if (str[i] >= 'A' && str[i] <= 'F')
            from_str_to_int = str[i] - 'A' + 10;

        else
        {
            cout << "WRONG" << endl;
            return;
        }

        bitset<BASE_SIZE> tmp(from_str_to_int);//двоичный вектор из 0 и 1
        res = res | (tmp << k);
        //добавляем в res сдвигаем добавленное число на 4
        //так как максимальное число в 16-ти системе
        //это 15 = 1111 в двоичной системе

        //в пустой вектор добавляем 15 и сдвигаем на 4 чтобы добалять новые числа
        //0000 0000 0000 0000
        //0000 0000 0000 1111
        //0000 0000 1111 0000

        //в вектор длобавляем 8 и сдвигаем на 8 чтобы добавить новые число и не испортить старые
        //0000 0000 1111 0111
        //0000 1111 0111 0000

        //добавляем 5 и сдвигаем на 12  чтобы добавить новые числа и не испортить старые
        //0000 1111 0111 0101
        //1111 0111 0101 0000

        k = k + 4;
        if (k >= BASE_SIZE)
        {
            k = 0;
            coefs.push_back((BASE)res.to_ulong());
            //bitset нельзя просто так перевести в BASE
            //в cppreferns bitset можно перевести в ulong, ullong, string
            //мы переводим его в ulong потом переводим в BASE
            res = 0;
        }
    }

    if (k > 0)
    {
        coefs.push_back((BASE)res.to_ulong());//добавляем в вектор то что осталось

    }

    len = coefs.size();

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
    /*if (len < other.len)
        return false;

    if (len > other.len)
        return false;

    for (int i = ken - 1; i < 0; ++i)
    {
        if (coefs[i] < other.coefs[i])
            return true;
        if (coefs[i] > other.coefs[i])
            return false;
    }

    return false;*/
    return other > *this;//воспользуемся уже существующим оператором
}

bool BigNumber::operator!=(const BigNumber& other) const
{
    //(*this == other)
    //так как они равны
    //то !(*this == other) false
    //если не равны
    //то !(*this == other) будет true

    return !(*this == other);
}

bool BigNumber::operator>=(const BigNumber& other) const
{
    //>= по сути комбинация > и =
    return (*this > other) || (*this == other);
}

bool BigNumber::operator<=(const BigNumber& other) const
{
    //<= по сути комбинация < и =
    return (*this < other) || (*this == other);
}

BigNumber BigNumber::operator+(const BigNumber& other)
{
    int n = this->len;//длина число u
    int m = other.len;//длина числа v

    int l = 0;//длина результата числа w
    l = max(n, m) + 1;//может добавиться новый разряд
    int t = 0;
    t = min(n, m);

    BigNumber sum(l);

    DBASE tmp;
    int j = 0;//по разрядам
    int k = 0;//переход

    while (j < t)//сложение длины которая есть и там и там
    {
        //скаладываем оба вектора + перенос на след разряд
        tmp = this->coefs[j] + other.coefs[j] + k;

        //преобразуем в BASE и заносим то что осталось
        sum.coefs[j] = (BASE)tmp;

        k = (BASE) (tmp >> BASE_SIZE);
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
    if(sum.coefs[j] == 0)
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
    BigNumber sum(n + 1);

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
    if(*this < other)//если вычитаемое меньше
    {
        cerr << "Error wiht size" << endl;
        exit(-2);
    }

    int n = len;
    int m = other.len;

    BigNumber res(n);

    int j = 0;
    int k = 0;
    DBASE tmp;

    while(j < m)//смотрим по длине other
    {
        tmp = (1 << BASE_SIZE) | coefs[j];//искусственно увеличиваем число
        tmp = tmp - other.coefs[j] - k;
        res.coefs[j] = (BASE)tmp;

        k = !(tmp >> BASE_SIZE);//проверка если мы занимали у следующего разряда
        j++;
    }

    while(j < n)//если осталось убираем перенос который мог остаться
    {
        tmp = (1 << BASE_SIZE) | coefs[j];
        tmp = tmp - k;//отнимаем перенос

        k = (tmp >> BASE_SIZE);
        j++;
    }

    while(res.coefs.size() > 1 && res.coefs.back() == 0)//
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

//BigNumber BigNumber::operator-(const BASE& num_base)
//{

//}

int main()
{

    return 0;
}
