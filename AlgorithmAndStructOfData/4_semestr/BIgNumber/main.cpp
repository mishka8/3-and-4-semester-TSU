#include <iostream>
#include <vector>
#include <string>
#include <bitset>//библиотека булевых векторов
#include <algorithm>



//typedef unsigned char base;//2^8 система счисления
//typedef unsigned short dbase;//2^16 double base

//typedef unsigned short BASE;//2^16 система счисления
//typedef unsigned int DBASE;//2^32 double BASE

typedef unsigned int BASE;//2^32 система счисления
typedef unsigned long long int DBASE;//2 ^ 32 double BASE

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

    //BigNumber operator/(const BigNumber&);//
    //BigNumber& operator/=(const BigNumber&);//

    BASE operator%(const BASE&);
    //BigNumber operator%(const BigNumber);//

    //void test();//

    void inputHex()
    {
        cout << "Input hex str - ";
        string str;
        cin >> str;

        // Очищаем текущие данные
        coefs.clear();
        len = 0;

        // Проверка на пустую строку
        if (str.empty()) 
        {
            coefs.push_back(0);
            len = 1;
            return;
        }

        bitset<BASE_SIZE> res(0);
        int k = 0;

        // Обрабатываем строку с конца к началу
        for (int i = str.size() - 1; i >= 0; --i)
        {
            unsigned int int_from_hex = 0;

            if (str[i] >= '0' && str[i] <= '9') 
            {
                int_from_hex = str[i] - '0';
            }
            else if (str[i] >= 'a' && str[i] <= 'f') 
            {
                int_from_hex = str[i] - 'a' + 10;
            }
            else if (str[i] >= 'A' && str[i] <= 'F') 
            {
                int_from_hex = str[i] - 'A' + 10;
            }
            else 
            {
                cerr << "ERROR: simbol is not correct" << endl;
                coefs.clear();
                coefs.push_back(0);
                len = 1;
                return;
            }

            // Добавляем 4 бита текущей цифры
            res |= bitset<BASE_SIZE>(int_from_hex) << k;
            k += 4;

            // Если накопили полное BASE_SIZE бит
            if (k >= BASE_SIZE) 
            {
                coefs.push_back((BASE)res.to_ulong());
                res.reset();
                k = 0;
            }
        }

        // Добавляем оставшиеся биты
        if (k > 0) 
        {
            coefs.push_back((BASE)res.to_ulong());
        }

        len = coefs.size();

        // Удаляем ведущие нули
        while (len > 1 && coefs.back() == 0) 
        {
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
                int tmp = (*ix >> k) & (15);
                if (tmp >= 0 && tmp <= 9)
                {
                    str += (char)(tmp + '0');
                }
                if (tmp >= 10 && tmp <= 15)
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
        //обработка нуля
        if (other.len == 1 && other.coefs[0] == 0) 
        {
            out << "0";
            return out;
        }

        string result;
        BigNumber u = other;//копия числа

        // Константа 10 в виде BigNumber для деления
        BASE ten = 10;

        while (!(u.len == 1 && u.coefs[0] == 0))
        {
            BASE t = u % 10;  // Получаем последнюю цифру
            result += (t + '0');
            u /= 10;  // Убираем последнюю цифру
        }

        // Выводим цифры в обратном порядке (старшие разряды сначала)
        for (int i = result.size() - 1; i >= 0; --i)
        {
            out << result[i];
        }

        return out;
    }

    friend istream& operator>>(istream& in, BigNumber& other)
    {
        string tmp;
        cin >> tmp;

        for (char c : tmp) 
        {
            if (!(isdigit(c))) 
            {
                cout << "Input error!\n";
                exit(-4);
            }
        }

        other = BigNumber(1, true);

        int i = 0;
        while (i < tmp.size()) 
        {
            int t = tmp[i] - 48;
            other = other * (BASE)10 + (BASE)t;
            ++i;
        }

        int len = other.coefs.size() - 1;

        while (other.coefs[len] == 0 && other.coefs.size() > 1) 
        {
            len--;
            other.coefs.pop_back();
        }

        return in;
    }
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
        coefs.resize(lenght);
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

    while (coefs[len - 1] == 0 && len > 1)
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
    while (j < n)//сложение длины которая есть и там и там
    {
        tmp = (DBASE)coefs[j] + k;//прибавляем число
        sum.coefs[j] = (BASE)tmp;

        k = (BASE)(tmp >> BASE_SIZE);//смотрим сдвиг если переходим в новый разяряд

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
        tmp = ((DBASE)1 << BASE_SIZE) | coefs[j];//искусственно увеличиваем число
        tmp = tmp - other.coefs[j] - k;
        res.coefs[j] = (BASE)tmp;

        k = !(tmp >> BASE_SIZE);//проверка если мы занимали у следующего разряда
        j++;
    }

    while (j < n)//если осталось убираем перенос который мог остаться
    {
        tmp = ((DBASE)1 << BASE_SIZE) | coefs[j];
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



    tmp = ((DBASE)1 << BASE_SIZE) | coefs[j];
    tmp = tmp - num_base - k;

    res.coefs[j] = (BASE)tmp;
    k = !(tmp >> BASE_SIZE);

    ++j;

    while (j < n)
    {
        tmp = ((DBASE)1 << BASE_SIZE) | coefs[j];
        tmp = tmp - k;

        res.coefs[j] = (BASE)tmp;

        k = !(tmp >> BASE_SIZE);
        ++j;
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

    DBASE tmp;
    BigNumber res(n + m, true);

    for (int j = 0; j < m; ++j)
    {
        if (other.coefs[j] == 0)
        {
            continue;
        }

        DBASE k = 0;
        for (int i = 0; i < n; ++i)
        {
            tmp = (DBASE)coefs[i] * (DBASE)other.coefs[j] + k + (DBASE)res.coefs[i + j];//i + j значение на данный момент

            res.coefs[i + j] = (BASE)tmp;

            k = (BASE)(tmp >> BASE_SIZE);
        }

        res.coefs[n + j] = k;//последний перенос
    }

    while (res.len > 1 && res.coefs[res.len - 1] == 0)
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
    BigNumber res;


    DBASE k = 0;
    DBASE tmp = 0;

    for (int j = 0; j < n; ++j)
    {
        tmp = (DBASE)coefs[j] * (DBASE)num_base + (DBASE)k;//возможно надо каждую переменную приводить явно к DBASE
        res.coefs.push_back(tmp);

        k = (BASE)(tmp >> BASE_SIZE);
    }

    if (k != 0)
    {
        res.coefs.push_back(k);
    }

    while (res.coefs[res.len - 1] == 0 && res.len > 1)
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

BigNumber BigNumber::operator/(const BASE& num)
{
    if (num == 0)
    {
        cout << "ERRRRRRRRROR" << endl;
        exit(-2);
    }

    int n = len;

    BigNumber div(n, true);
    div.len = n;
    BASE r = 0;
    DBASE tmp;


    for (int j = n - 1; j >= 0; --j)//работает по принципу деления столбиком
    {
        tmp = (((DBASE)r << BASE_SIZE) + (DBASE)coefs[j]);//  tmp = r * b +  ceofs[j]
        div.coefs[j] = (BASE)(tmp / num);
        r = (BASE)(tmp % num);//остаток
    }

    //Нормализация: удаление ведущих нулей

    while (div.len > 1 && div.coefs.back() == 0) 
    {
        div.coefs.pop_back();
        div.len--;
    }

    return div;
}

BigNumber& BigNumber::operator/=(const BASE& num)
{
    *this = *this / num;
    return *this;
}

BASE BigNumber::operator% (const BASE& num)
{
    if (num == 0)
    {
        cout << "ERRROR" << endl;
        exit(-1);
    }

    int n = len;
    BASE r = 0;
    DBASE tmp;

    int j = 0;

    while (j < n)
    {
        tmp = (((DBASE)r << BASE_SIZE) + (DBASE)coefs[n - 1 - j]);// tmp = r * b +  ceofs[n - 1 - j]

        r = (BASE)(tmp % num);
        j++;
    }

    return r;
}

int main()
{
    //+ += - -= * *=
    //BigNumber num1(3); // 5
    //BigNumber num2(5); // 15
    //cout << num1 << endl;
    //cout << num2 << endl;

    BigNumber num_chek;
    cin >> num_chek;
    cout << num_chek << endl;

    //BigNumber num3(5); // случайное 5-разрядное число

    //num3.outputHex();
    //cout << num3;

    cout << sizeof(int) << endl;
    cout << sizeof(long) << endl;
    cout << sizeof(long long int) << endl;

    //num2 += num1;
    //cout << num2 << endl;// Должно вывести "1f" (15 + 16 = 31)

    // Тест вычитания
    //num2 -= num1;
    //cout << num2 << endl; // Должно вывести "ffffffffffffff1"

    //BigNumber num4(0);
    
    //num4 = num1 + num2 + num2 + num1 - num1 - num2;
    //num4.outputHex();

    //BigNumber num5(0);

    //num5 = num1 * num3;
    //num5.outputHex();

    //num5 *= num1;
    //num5.outputHex();

    return 0;
}
