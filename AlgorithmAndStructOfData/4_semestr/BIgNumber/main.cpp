#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <bitset>

typedef unsigned char BASE;//2^8 система счисления
typedef unsigned short DBASE;//2^16 double base

//typedef unsigned short BASE;//2^16 система счисления
//typedef unsigned int DBASE;//2^32 double BASE

//typedef unsigned int BASE;//2^32 система счисления
//typedef unsigned long long int DBASE;//2 ^ 32 double BASE

#define BASE_SIZE (sizeof(BASE) * 8)   // 2 * 8 == 16 (бит)
#define DBASE_SIZE (sizeof(DBASE) * 8) // 4 * 8 == 32 (бит)
#define BASE_NUM_SHORT 10000;// 15000;
#define BASENUM ((DBASE)1 << BASE_SIZE)

using namespace std;

class BigNumber
{
    vector<BASE> coefs;
public:
    BigNumber(unsigned int len = 0, int c = 1); // 1 2
    BigNumber(string&);
    BigNumber(const BigNumber&);
    ~BigNumber() = default;

    bool operator== (const BigNumber&) const;
    bool operator!= (const BigNumber&) const;
    bool operator>  (const BigNumber&) const;
    bool operator<  (const BigNumber&) const;
    bool operator>= (const BigNumber&) const;
    bool operator<= (const BigNumber&) const;

    BigNumber& operator=  (const BigNumber&);

    BigNumber  operator+  (const BASE&);
    BigNumber& operator+= (const BASE&);

    BigNumber  operator+  (const BigNumber&);
    BigNumber& operator+= (const BigNumber&);

    BigNumber  operator-  (const BASE&);
    BigNumber& operator-= (const BASE&);

    BigNumber  operator-  (const BigNumber&);
    BigNumber& operator-= (const BigNumber&);

    BigNumber  operator*  (const BASE&);
    BigNumber& operator*= (const BASE&);

    BigNumber  operator*  (const BigNumber&);
    BigNumber& operator*= (const BigNumber&);

    BigNumber  operator/  (const BASE&);
    BigNumber& operator/= (const BASE&);

    BigNumber  operator/  (const BigNumber&);
    BigNumber& operator/= (const BigNumber&);

    BigNumber  operator%  (const BASE&);

    BigNumber  operator%  (const BigNumber&);
    BigNumber& operator%= (const BigNumber&);

    unsigned int getLength() const
    {
        return coefs.size();
    }

    void OutputHex();
    void InputHex();

    BigNumber Shift(int bits)
    {
        BigNumber res(1, 2);

        if (bits > 0)
        {
            res.coefs.reserve(coefs.size() + bits);// Резервируем память
            res.coefs.resize(bits, 0);// Заполняем старшие разряды нулями
            res.coefs.insert(res.coefs.end(), coefs.begin(), coefs.end());//перекопируем то что было
        }
        else if (bits < 0)
        {
            int shift_size = static_cast<int>(coefs.size()) + bits;//явно преобразуем тип
            if (shift_size <= 0)
            {
                res.coefs.reserve(1);
                res.coefs.push_back(0);
            }
            else
            {
                res.coefs.reserve(shift_size);
                res.coefs.insert(res.coefs.end(), coefs.begin() - bits, coefs.end());// Копируем часть коэффициентов
            }
        }
        else
        {
            res.coefs.reserve(coefs.size());
            res.coefs = coefs;// Просто копируем исходное число
        }

        int k = coefs.size() - 1;

        while (coefs[k] == 0 && coefs.size() > 1)//удаляем нули из начала 
        {
            coefs.pop_back();
            k--;
        }
        return res;
    }


    friend ostream& operator<< (ostream&, const BigNumber&);
    friend istream& operator>> (istream&, BigNumber&);

};

BigNumber::BigNumber(unsigned int len, int choice)
{
    if (!len)
        return;

    if (choice == 1)
    {
        for (int i = 0; i < len; i++)
        {
            BASE coef = rand();

            // Для ситуации, когда последний коэффицент равен 0 (без ведущих нулей)
            if (i == len - 1 && !coef)
                while (!coef) coef = rand();

            coefs.push_back(coef);
        }
    }
    else
    {
        coefs.push_back(0);
    }
}

BigNumber::BigNumber(string& numInHex)
{
    if (numInHex.empty())
    {
        coefs.push_back(0);
        return;
    }

    bitset<BASE_SIZE> coeff(0);

    int n = 0;

    int i = numInHex.size() - 1;
    while (i >= 0)
    {
        int tmpHex = 0;
        if (numInHex[i] >= '0' && numInHex[i] <= '9')
            tmpHex = numInHex[i] - '0';

        else if (numInHex[i] >= 'a' && numInHex[i] <= 'f')
            tmpHex = numInHex[i] - 'a' + 10;

        else if (numInHex[i] >= 'A' && numInHex[i] <= 'F')
            tmpHex = numInHex[i] - 'A' + 10;

        else
        {
            cout << "Wrong symbol!!!\n";
            exit(-1);
        }

        bitset<BASE_SIZE> tmp(tmpHex);
        coeff |= tmp << n;

        n += 4;
        if (n >= BASE_SIZE)
        {
            n = 0;
            coefs.push_back(coeff.to_ulong());
            coeff.reset();
        }
        i--;
    }
    if (n > 0)
        coefs.push_back(coeff.to_ulong());

    int len = coefs.size();

    while (coefs[len - 1] == 0 && len > 1)
    {
        len--;
        coefs.pop_back();
    }
}

BigNumber::BigNumber(const BigNumber& other)
{
    for (int i = 0; i < other.coefs.size(); i++)
        coefs.push_back(other.coefs[i]);
}

bool BigNumber::operator== (const BigNumber& other) const
{
    int len_l = coefs.size();
    int len_r = other.coefs.size();

    if (len_l != len_r)
        return false;

    for (int i = 0; i < len_l; i++)
        if (this->coefs[i] != other.coefs[i])
            return false;

    return true;
}

bool BigNumber::operator!= (const BigNumber& other) const
{
    return !(*this == other);
}

bool BigNumber::operator> (const BigNumber& other) const
{
    int len_l = coefs.size();
    int len_r = other.coefs.size();

    if (len_l > len_r)
        return true;

    if (len_l < len_r)
        return false;

    for (int i = len_l - 1; i >= 0; i--)
    {
        if (coefs[i] < other.coefs[i])
            return false;
        if (coefs[i] > other.coefs[i])
            return true;
    }

    return false;
}

bool BigNumber::operator< (const BigNumber& other) const
{
    int len_l = coefs.size();
    int len_r = other.coefs.size();

    if (len_l < len_r)
        return true;

    if (len_l > len_r)
        return false;

    for (int i = len_l - 1; i >= 0; i--)
    {
        if (coefs[i] > other.coefs[i])
            return false;
        if (coefs[i] < other.coefs[i])
            return true;
    }

    return false;
}

bool BigNumber::operator<= (const BigNumber& other) const
{
    return !(*this > other);
}

bool BigNumber::operator>= (const BigNumber& other) const
{
    return !(*this < other);
}

BigNumber& BigNumber::operator= (const BigNumber& other)
{
    if (this != &other)
    {
        coefs.clear();
        for (int i = 0; i < other.coefs.size(); i++)
            coefs.push_back(other.coefs[i]);
    }
    return *this;
}

BigNumber BigNumber::operator+ (const BASE& num)
{
    DBASE tmp;
    int len = this->coefs.size();

    if (len == 0) return BigNumber(1, 2);

    BASE k = 0;
    BigNumber res;

    // Сложение
    tmp = DBASE(coefs[0]) + DBASE(num);
    res.coefs.push_back(BASE(tmp));
    k = BASE(tmp >> BASE_SIZE);

    // Перенос
    int i = 1;

    while (i < len)
    {
        tmp = DBASE(coefs[i]) + DBASE(k);
        res.coefs.push_back(BASE(tmp));
        k = BASE(tmp >> BASE_SIZE);
        i++;
    }

    if (k != 0) res.coefs.push_back(k);

    return res;
}

BigNumber& BigNumber::operator+= (const BASE& num)
{
    *this = *this + num;
    return *this;
}

BigNumber BigNumber::operator- (const BASE& num)
{
    int len = coefs.size();

    if (len == 0) return BigNumber(1, 2);

    if ((len == 1) && (coefs[0] < num))
    {
        cout << "Error: other_1 < other_2 (in operator -)\n";
        exit(-2);
    }

    BigNumber res;
    DBASE tmp;

    int k = 0, i = 1;

    tmp = (DBASE(1) << BASE_SIZE) | DBASE(coefs[0]);
    tmp -= DBASE(num);
    res.coefs.push_back(BASE(tmp));
    k = !(tmp >> BASE_SIZE);

    while (i < len)
    {
        tmp = (DBASE(1) << BASE_SIZE) | DBASE(coefs[i]);
        tmp -= DBASE(k);
        res.coefs.push_back(BASE(tmp));
        k = !(tmp >> BASE_SIZE);
        i++;
    }

    i--;

    while (res.coefs[i] == 0 && res.coefs.size() > 1)
    {
        res.coefs.pop_back();
        i--;
    }

    return res;
}

BigNumber& BigNumber::operator-= (const BASE& num)
{
    *this = *this - num;
    return *this;
}

BigNumber BigNumber::operator+ (const BigNumber& other)
{
    int len_l = coefs.size();
    int len_r = other.coefs.size();
    int t = min(len_l, len_r);

    if (len_l == 0 || len_r == 0) return BigNumber(1, 2);

    BASE k = 0;
    DBASE tmp = 0;

    BigNumber res;

    int i = 0;

    while (i < t)
    {
        tmp = DBASE(coefs[i]) + DBASE(other.coefs[i]) + DBASE(k);
        res.coefs.push_back(BASE(tmp));
        k = BASE(tmp >> BASE_SIZE);
        i++;
    }
    while (i < len_l)
    {
        tmp = DBASE(coefs[i]) + DBASE(k);
        res.coefs.push_back(BASE(tmp));
        k = BASE(tmp >> BASE_SIZE);
        i++;
    }
    while (i < len_r)
    {
        tmp = DBASE(other.coefs[i]) + DBASE(k);
        res.coefs.push_back(BASE(tmp));
        k = BASE(tmp >> BASE_SIZE);
        i++;
    }

    if (k != 0)
        res.coefs.push_back(k);

    return res;
}

BigNumber& BigNumber::operator+= (const BigNumber& other)
{
    *this = *this + other;
    return *this;
}

BigNumber BigNumber::operator- (const BigNumber& other)
{
    if (*this < other)
    {
        cout << "Error: other_1 < other_2 (in operator -)\n";
        exit(-2);
    }

    int len_l = coefs.size();
    int len_r = other.coefs.size();

    if (len_l == 0 || len_r == 0) return BigNumber(1, 2);

    BigNumber res;
    DBASE tmp;


    BASE k = 0;
    int i = 0;

    while (i < len_r)
    {
        tmp = (DBASE(1) << BASE_SIZE) | DBASE(coefs[i]);
        tmp = tmp - DBASE(other.coefs[i]) - DBASE(k);
        res.coefs.push_back(BASE(tmp));
        k = !(tmp >> BASE_SIZE);
        i++;
    }

    while (i < len_l)
    {
        tmp = (DBASE(1) << BASE_SIZE) | DBASE(coefs[i]);
        tmp -= DBASE(k);
        res.coefs.push_back(BASE(tmp));
        k = !(tmp >> BASE_SIZE);
        i++;
    }

    i--;

    while (res.coefs[i] == 0 && res.coefs.size() > 1)
    {
        res.coefs.pop_back();
        i--;
    }

    return res;
}

BigNumber& BigNumber::operator-= (const BigNumber& other)
{
    *this = *this - other;
    return *this;
}

BigNumber BigNumber::operator* (const BASE& num)
{
    int len = coefs.size();

    if (len == 0) return BigNumber(1, 2);

    BigNumber res;

    DBASE tmp = 0;
    DBASE k = 0;

    int i = 0;
    while (i < len)
    {
        tmp = (DBASE(coefs[i]) * DBASE(num)) + DBASE(k);
        res.coefs.push_back(BASE(tmp));
        k = BASE(tmp >> BASE_SIZE);
        i++;
    }

    if (k != 0)
        res.coefs.push_back(k);

    return res;
}

BigNumber& BigNumber::operator*= (const BASE& num)
{
    *this = *this * num;
    return *this;
}

BigNumber BigNumber::operator* (const BigNumber& other)
{
    int len_l = coefs.size();
    int len_r = other.coefs.size();

    if (len_l == 0 || len_r == 0) return BigNumber(1, 2);

    int res_len = len_l + len_r;

    BigNumber res(1, 2);
    res.coefs.pop_back();

    for (int i = 0; i < res_len; i++)
        res.coefs.push_back(0);

    DBASE tmp;
    BASE k = 0;

    int j = 0, i = 0;
    while (j < len_r)
    {
        if (other.coefs[j] != 0)
        {
            int i = 0;
            k = 0;

            while (i < len_l)
            {
                tmp = (DBASE(coefs[i]) * DBASE(other.coefs[j])) + DBASE(res.coefs[i + j]) + DBASE(k);
                res.coefs[i + j] = BASE(tmp);
                k = BASE(tmp >> BASE_SIZE);
                i++;
            }
            res.coefs[j + len_l] = BASE(k);
        }
        j++;
    }

    i = len_l + len_r - 1;

    while (res.coefs[i] == 0 && res.coefs.size() > 1)
    {
        res.coefs.pop_back();
        i--;
    }


    return res;
}

BigNumber& BigNumber::operator*= (const BigNumber& other)
{
    *this = *this * other;
    return *this;
}

BigNumber BigNumber::operator/ (const BASE& num)
{
    if (!num)
    {
        cout << "Error other == 0 !!!!\n";
        exit(-3);
    }

    int len = coefs.size();

    if (len == 0)
        return BigNumber(1, 2);

    BigNumber res;


    for (int i = 0; i < len; i++)
        res.coefs.push_back(0);

    DBASE tmp = 0;
    BASE r = 0;

    int j = 0;
    while (j < len)
    {
        tmp = ((DBASE(r) << BASE_SIZE) + (DBASE)coefs[len - 1 - j]);
        res.coefs[len - 1 - j] = ((BASE)(tmp / DBASE(num)));
        r = (BASE)(tmp % DBASE(num));
        j++;
    }


    int i = len - 1;
    while (res.coefs[i] == 0 && (res.coefs.size() > 1))
    {
        res.coefs.pop_back();
        i--;
    }

    return res;
}

BigNumber& BigNumber::operator/= (const BASE& num)
{
    *this = *this / num;
    return *this;
}

BigNumber BigNumber::operator/ (const BigNumber& other)
{
    BigNumber zeroNum(1, 2);// Нулевое число (Для проверки на ноль у делителя)
    BigNumber u(*this);// Делимое
    BigNumber v(other);// Делитель


    //длины чисел
    int sizeL = this->coefs.size();
    int sizeR = other.coefs.size();

    if (sizeL == 0 || sizeR == 0)//ПРОВЕРКА НА НОЛЬ
        return zeroNum;

    int m = sizeL - sizeR;// Длина частного

    BASE d = 0;// Для нормализации
    DBASE b = ((DBASE)1 << (sizeof(BASE) * 8));// Основание

    if (zeroNum == other)
    {
        cout << "Error: in operator/ (other / 0)!\n";
        exit(-3);
    }
    if (sizeR == 1)
        return *this / other.coefs[0];//ДЕЛИМ НА ОДНУ ЦИФРУ

    if (*this == other)//1 ЕСЛИ ЧИСЛА РАВНЫ 
        return zeroNum + 1;

    if (*this < other)
        return zeroNum;


    // Нормализация.
    // На этом шаге u и v становится длиннее на один разряд.
    // Частное от этого не изменится.
    //q′ никогда не будет меньше q
    // 
    // 
    //для быстрой коррекции q
    //так как по второй теореме мы можем выполнить коррекцию не больше двух раз
    //старший разряд делителя был >= b/2 чтобы нормально делить 
    d = BASE(b / (DBASE(other.coefs.back()) + DBASE(1)));//старшая цифра делителя v(n − 1)
    //коэффициент, который масштабирует делимое и делитель
     
    
    //старшая цифра делителя будет больше половины основания системы это важно для теорем которые позволяют быстро находить q
    //и сокрашения коррекции 
    u = u * d;
    v = v * d;

    // Если u не увеличилось в длине, добавляем ноль в старший разряд
    if (u.coefs.size() == sizeL) u.coefs.push_back(0);



    int i = m;// Начинаем со старших разрядов


    BigNumber res(1, 2);

    res.coefs.resize(m + 1, 0);


    while (i >= 0)
    {
        if (i + sizeR >= u.coefs.size())//Проверка на добавление нулей для доступа к разрядам
        {
            while (i + sizeR >= u.coefs.size())
                u.coefs.push_back(0);
        }

        //q - частное, r - остаток
        //берём две старшие цифры делимого и делим на старшую цифру делителя
        DBASE q = ((DBASE(u.coefs[i + sizeR]) << BASE_SIZE) + DBASE(u.coefs[i + sizeR - 1])) / (DBASE(v.coefs[sizeR - 1]));
        DBASE r = ((DBASE(u.coefs[i + sizeR]) << BASE_SIZE) + DBASE(u.coefs[i + sizeR - 1])) % (DBASE(v.coefs[sizeR - 1]));

        // Коррекция q, если оно слишком большое
        if (q == b || (DBASE(q) * DBASE(v.coefs[sizeR - 2])) > ((DBASE(r) << BASE_SIZE) + DBASE(u.coefs[i + sizeR - 2])))
        {
            q--;
            r = DBASE(r) + DBASE(v.coefs[sizeR - 1]);

        }
        if (r < b)
        {
            if (q == b || (DBASE(q) * DBASE(v.coefs[sizeR - 2])) > ((DBASE(r) << BASE_SIZE) + DBASE(u.coefs[i + sizeR - 2])))
            {
                q--;
                r += v.coefs[other.getLength() - 1];
            }
        }

        //Коррекция q
        //Умножаем делитель на пробное q и вычитаем из делимого
        BigNumber t = v * (BASE)q;
        t = t.Shift(i);

        if (u < t)// Если результат вычитания отрицательный, уменьшаем q
        {
            q--;
            t = v * (BASE)q;
            t = t.Shift(i);// Умножаем на b^i (сдвиг влево)
        }

        //Вычитаем из делимого
        u = u - t;

        //записываем цифру в res
        res.coefs[i] = (BASE)q;

        // Переход к следующему разряду.
        i--;
    }

    //удаляем нули из res
    int k = res.getLength() - 1;

    while (res.coefs[k] == 0 && res.getLength() > 1)
    {
        res.coefs.pop_back();
        k--;
    }

    return res;
}

BigNumber BigNumber::operator% (const BigNumber& other)//работает так же как деление но возвращает остаток.
{
    BigNumber zeroNum(1, 2);
    if (other == zeroNum)
    {
        cout << "Error: in operator% (*this % 0)!" << endl;
        exit(-4);
    }
    if (*this < other)
    {
        return *this;
    }
    if (*this == other)
    {
        return zeroNum;
    }
    if (other.coefs.size() == 1)
    {
        return *this % other.coefs[0];
    }

    int n = other.coefs.size();
    int m = coefs.size() - n;

    // Нормализация.
    // На этом шаге u и v становится длиннее на один разряд.
    // Частное от этого не изменится.
    DBASE b = BASENUM;
    BASE d = BASE(b / (DBASE(other.coefs.back()) + DBASE(1)));

    BigNumber u = *this * d;
    BigNumber v = other;

    v *= d;

    //Добавляем нулевую старшую цифру, если число не стало длиннее
    if (coefs.size() == u.coefs.size())
    {
        u.coefs.push_back(0);
    }

    //две старшие цифры делимого и делим на старшую цифру делителя
    for (int j = m; j >= 0; --j)
    {
        if (j + n >= u.coefs.size())
        {
            while (j + n >= u.coefs.size()) u.coefs.push_back(0);
        }


        DBASE q = ((DBASE(u.coefs[j + n]) << BASE_SIZE) + DBASE(u.coefs[j + n - 1])) / (DBASE(v.coefs[n - 1]));
        DBASE r = ((DBASE(u.coefs[j + n]) << BASE_SIZE) + DBASE(u.coefs[j + n - 1])) % (DBASE(v.coefs[n - 1]));

        if ((BASENUM == q) || (q * v.coefs[n - 2] > ((r << BASE_SIZE) + u.coefs[j + n - 2])))
        {
            q--;
            r += v.coefs[n - 1];
        }
        if (r < BASENUM)
        {
            // Коррекция q
            if ((q == BASENUM) || (q * v.coefs[n - 2] > ((r << BASE_SIZE) + u.coefs[j + n - 2])))
            {
                q--;
                r += v.coefs[n - 1];
            }
        }

        BigNumber temp = v * BASE(q);
        temp = temp.Shift(j);

        if (u < temp) 
        {
            q--;
            temp = v * BASE(q);
            temp = temp.Shift(j);//умножаем на сдвигн влево 
        }

        u = u - temp;// Вычитаем из делимого
    }

    u = u / d;
    int k = u.coefs.size() - 1;

    //удаляем нули 
    while (u.coefs[k] == 0 && u.getLength() > 1)
    {
        u.coefs.pop_back();
        k--;
    }

    return u;

}

BigNumber& BigNumber::operator%= (const BigNumber& other)
{
    *this = *this % other;
    return *this;
}

BigNumber& BigNumber::operator/= (const BigNumber& other)
{
    *this = *this / other;
    return *this;
}

BigNumber BigNumber::operator% (const BASE& num)
{
    if (!num)
    {
        cout << "Error: in operator% (other % 0)!\n";
        exit(-4);
    }

    int len = coefs.size();

    DBASE tmp = 0;
    BASE r = 0;

    int i = 0;
    while (i < len)
    {
        tmp = ((DBASE(r) << BASE_SIZE) + DBASE(coefs[len - 1 - i]));
        r = (BASE)(tmp % DBASE(num));
        i++;
    }

    BigNumber res(1, 2);
    res.coefs[0] = r;

    return res;
}

void BigNumber::OutputHex()
{
    int k = BASE_SIZE - 4;
    string res;
    int len = coefs.size();

    for (int j = len - 1; j >= 0; j--)
    {
        while (k >= 0) {
            int tmp = (coefs[j] >> k) & (0xf);

            if (tmp >= 0 && tmp <= 9)
                res += (char)(tmp + '0');
            if (tmp >= 10 && tmp <= 15)
                res += (char)(tmp - 10 + 'A');

            k -= 4;
        }

        res += " | ";
        k = BASE_SIZE - 4;
    }

    cout << res << endl;
}

void BigNumber::InputHex()
{
    cout << "Input (in HEX format): ";
    string numInHex;
    cin >> numInHex;
    BigNumber tmp(numInHex);

    *this = tmp;
}

ostream& operator<< (ostream& out, const BigNumber& other)
{
    string output;
    int c = 10; // Основание СС для вывода в строку

    BigNumber number(other);
    BigNumber zeroNum(1, 2);
    //zeroNum.OutputHex();

    while (number != zeroNum)
    {
        BASE tmp = (number % (BASE)c).coefs[0];
        output += (tmp + '0'); // Преобразование в символ
        number /= (BASE)c;
    }

    if (!output.size()) output = "0";

    reverse(output.begin(), output.end());

    cout << output << endl;

    return out;
}

istream& operator>> (istream& in, BigNumber& other)
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

    other = BigNumber(1, 2);

    int i = 0;
    while (i < tmp.size())
    {
        int t = tmp[i] - 48; // Код символа нуля в ASCII
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

void Test()
{
    int M = 1000, T = 1000;

    int len1 = 0;
    int len2 = 0;
    BigNumber A, B, C, D;

    do
    {
        cout << "T: " << T << endl;
        cout << "Len1 and Len2: " << len1 << " " << len2 << endl;
        len1 = rand() % M + 1;
        len2 = rand() % M + 1;
        BigNumber a(len1, 1), b(len2, 1);
        A = a; B = b;
        C = A / B;
        D = A % B;
    } while (A == C * B + D && A - D == C * B && D < B && --T);
}

int main()
{
    srand(0);  
    
    Test();

    return 0;
}
