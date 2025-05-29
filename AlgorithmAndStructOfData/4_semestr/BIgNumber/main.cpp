#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <bitset>

typedef unsigned char BASE;       // [0; 2^16]
typedef unsigned short DBASE;         // [0; 2^32]
// DBASE нужен для промежуточных вычислений

// В качестве основания выбирать: 2^8, 2^16, 2^32
#define BASE_SIZE (sizeof(BASE) * 8)   // 2 * 8 == 16 (бит)
#define DBASE_SIZE (sizeof(DBASE) * 8) // 4 * 8 == 32 (бит)
#define BASE_NUM_SHORT 10000;// 15000;
#define BASENUM ((DBASE)1 << BASE_SIZE)

using namespace std;


// Представление числа: X = a_0 + a_1*b + a_2*(b^2) + ... + a_(n-1)*(b^(n-1))
// Массив коэффицентов: {a_0, a_1, a_2, ..., a_(n-1)}


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
        BigNumber result(1, 2);

        if (bits > 0)
        {
            result.coefs.reserve(coefs.size() + bits);
            result.coefs.resize(bits, 0);
            result.coefs.insert(result.coefs.end(), coefs.begin(), coefs.end());
        }
        else if (bits < 0)
        {
            int shift_size = static_cast<int>(coefs.size()) + bits;
            if (shift_size <= 0)
            {
                result.coefs.reserve(1);
                result.coefs.push_back(0);
            }
            else
            {
                result.coefs.reserve(shift_size);
                result.coefs.insert(result.coefs.end(), coefs.begin() - bits, coefs.end());
            }
        }
        else
        {
            result.coefs.reserve(coefs.size());
            result.coefs = coefs;
        }

        int k = coefs.size() - 1;

        while (coefs[k] == 0 && coefs.size() > 1)
        {
            coefs.pop_back();
            k--;
        }
        return result;
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
            // Инициализация генератора случайных чисел
            srand(time(0));

            BASE coefficent = rand(); // Генерирует случайное число

            // Для ситуации, когда последний коэффицент равен 0 (без ведущих нулей)
            if (i == len - 1 && !coefficent)
                while (!coefficent) coefficent = rand();

            coefs.push_back(coefficent);
        }
    }
    else
    {
        coefs.push_back(0);
    }
}

// Конструктор по строке
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

// Конструтор копирования
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
    BigNumber result;

    // Сложение
    tmp = DBASE(coefs[0]) + DBASE(num);
    result.coefs.push_back(BASE(tmp));
    k = BASE(tmp >> BASE_SIZE);

    // Перенос
    int i = 1;

    while (i < len)
    {
        tmp = DBASE(coefs[i]) + DBASE(k);
        result.coefs.push_back(BASE(tmp));
        k = BASE(tmp >> BASE_SIZE);
        i++;
    }

    if (k != 0) result.coefs.push_back(k);

    return result;
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

    BigNumber result;
    DBASE tmp;

    int k = 0, i = 1;

    tmp = (DBASE(1) << BASE_SIZE) | DBASE(coefs[0]);
    tmp -= DBASE(num);
    result.coefs.push_back(BASE(tmp));
    k = !(tmp >> BASE_SIZE);

    while (i < len)
    {
        tmp = (DBASE(1) << BASE_SIZE) | DBASE(coefs[i]);
        tmp -= DBASE(k);
        result.coefs.push_back(BASE(tmp));
        k = !(tmp >> BASE_SIZE);
        i++;
    }

    i--;

    while (result.coefs[i] == 0 && result.coefs.size() > 1)
    {
        result.coefs.pop_back();
        i--;
    }

    return result;
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

    BigNumber result;

    int i = 0;

    while (i < t)
    {
        tmp = DBASE(coefs[i]) + DBASE(other.coefs[i]) + DBASE(k);
        result.coefs.push_back(BASE(tmp));
        k = BASE(tmp >> BASE_SIZE);
        i++;
    }
    while (i < len_l)
    {
        tmp = DBASE(coefs[i]) + DBASE(k);
        result.coefs.push_back(BASE(tmp));
        k = BASE(tmp >> BASE_SIZE);
        i++;
    }
    while (i < len_r)
    {
        tmp = DBASE(other.coefs[i]) + DBASE(k);
        result.coefs.push_back(BASE(tmp));
        k = BASE(tmp >> BASE_SIZE);
        i++;
    }

    if (k != 0)
        result.coefs.push_back(k);

    return result;
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

    BigNumber result;
    DBASE tmp;


    BASE k = 0;
    int i = 0;

    while (i < len_r)
    {
        tmp = (DBASE(1) << BASE_SIZE) | DBASE(coefs[i]);
        tmp = tmp - DBASE(other.coefs[i]) - DBASE(k);
        result.coefs.push_back(BASE(tmp));
        k = !(tmp >> BASE_SIZE);
        i++;
    }

    while (i < len_l)
    {
        tmp = (DBASE(1) << BASE_SIZE) | DBASE(coefs[i]);
        tmp -= DBASE(k);
        result.coefs.push_back(BASE(tmp));
        k = !(tmp >> BASE_SIZE);
        i++;
    }

    i--;

    while (result.coefs[i] == 0 && result.coefs.size() > 1)
    {
        result.coefs.pop_back();
        i--;
    }

    return result;
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

    BigNumber result;

    DBASE tmp = 0;
    DBASE k = 0;

    int i = 0;
    while (i < len)
    {
        tmp = (DBASE(coefs[i]) * DBASE(num)) + DBASE(k);
        result.coefs.push_back(BASE(tmp));
        k = BASE(tmp >> BASE_SIZE);
        i++;
    }

    if (k != 0)
        result.coefs.push_back(k);

    return result;
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

    int result_len = len_l + len_r;

    BigNumber result(1, 2);
    result.coefs.pop_back();

    for (int i = 0; i < result_len; i++)
        result.coefs.push_back(0);

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
                tmp = (DBASE(coefs[i]) * DBASE(other.coefs[j])) + DBASE(result.coefs[i + j]) + DBASE(k);
                result.coefs[i + j] = BASE(tmp);
                k = BASE(tmp >> BASE_SIZE);
                i++;
            }
            result.coefs[j + len_l] = BASE(k);
        }
        j++;
    }

    i = len_l + len_r - 1;

    while (result.coefs[i] == 0 && result.coefs.size() > 1)
    {
        result.coefs.pop_back();
        i--;
    }


    return result;
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
        cout << "Error: in operator/ (other / 0)!\n";
        exit(-3);
    }

    int len = coefs.size();

    if (len == 0) return BigNumber(1, 2);

    BigNumber result;


    for (int i = 0; i < len; i++)
        result.coefs.push_back(0);

    DBASE tmp = 0;
    BASE r = 0;

    int j = 0;
    while (j < len)
    {
        tmp = ((DBASE(r) << BASE_SIZE) + (DBASE)coefs[len - 1 - j]);
        result.coefs[len - 1 - j] = ((BASE)(tmp / DBASE(num)));
        r = (BASE)(tmp % DBASE(num));
        j++;
    }


    int i = len - 1;
    while (result.coefs[i] == 0 && (result.coefs.size() > 1))
    {
        result.coefs.pop_back();
        i--;
    }

    return result;
}

BigNumber& BigNumber::operator/= (const BASE& num)
{
    *this = *this / num;
    return *this;
}

BigNumber BigNumber::operator/ (const BigNumber& other)
{
    BigNumber zeroNum(1, 2); // Нулевое число (Для проверки на ноль у делителя)
    BigNumber u(*this); // Делимое
    BigNumber v(other);   // Делитель

    int sizeL = this->coefs.size(); // Длина делимого
    int sizeR = other.coefs.size();   // Длина делителя

    if (sizeL == 0 || sizeR == 0) return zeroNum;

    int m = sizeL - sizeR; // Длина частного

    BASE d = 0;  // Для нормализации
    DBASE b = BASENUM; // Основание

    if (zeroNum == other)
    {
        cout << "Error: in operator/ (other / 0)!\n";
        exit(-3);
    }
    if (sizeR == 1)
        return *this / other.coefs[0];

    if (*this == other)
        return zeroNum + 1;

    if (*this < other)
        return zeroNum;


    // Нормализация.
    // На этом шаге делимое(u) и делитель(v) становится длиннее на один разряд.
    // Частное от этого не изменится.
    // d = [b / (v_(n-1) + 1)]
    // Обеспечивает, условия v_n-1 >= [b/2]. Частное в этом случае будет более приблеженное 
    d = BASE(b / (DBASE(other.coefs.back()) + DBASE(1)));
    u = u * d;

    // Если u не стало длиннее, то необходимо присвоить u_n+m зн-ие 0.
    if (u.coefs.size() == sizeL) u.coefs.push_back(0);
    v = v * d;

    // Начальная установка
    int i = m;


    BigNumber result(1, 2);

    result.coefs.resize(m + 1, 0);


    while (i >= 0) {
        if (i + sizeR >= u.coefs.size())
        {
            while (i + sizeR >= u.coefs.size()) u.coefs.push_back(0);
        }

        // Вычисляем q и r по модифицированным формулам. Они гарантируют что q` = q или q` - 1 = q
        // q - частное, r - остаток

        DBASE q = ((DBASE(u.coefs[i + sizeR]) << BASE_SIZE) + DBASE(u.coefs[i + sizeR - 1])) / (DBASE(v.coefs[sizeR - 1]));
        DBASE r = ((DBASE(u.coefs[i + sizeR]) << BASE_SIZE) + DBASE(u.coefs[i + sizeR - 1])) % (DBASE(v.coefs[sizeR - 1]));

        // Проверка выполнения условий q == b или q * v_n-2 > b * r + u_j+n-2
        // Если выполняется, то q--, r = r + v_n-1 и повторяем проверку, если r < b

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

        // Коррекция q.
        BigNumber t = v * (BASE)q;
        t = t.Shift(i);

        if (u < t)
        {
            q--;
            t = v * (BASE)q;
            t = t.Shift(i);
        }

        // Вычитаем из tmp произведение делителя на частное.
        u = u - t;

        // Найденное частное идет в результат.
        result.coefs[i] = (BASE)q;

        // Переход к следующему разряду.
        i--;
    }

    // Избавляемся от ведущих нулей.
    int k = result.getLength() - 1;

    while (result.coefs[k] == 0 && result.getLength() > 1)
    {
        result.coefs.pop_back();
        k--;
    }

    return result;
}

BigNumber BigNumber::operator% (const BigNumber& other) {
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

    DBASE b = BASENUM;
    BASE d = BASE(b / (DBASE(other.coefs.back()) + DBASE(1)));

    BigNumber u = *this * d;
    BigNumber v = other;

    v *= d;

    if (coefs.size() == u.coefs.size())
    {
        u.coefs.push_back(0);
    }

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
            if ((q == BASENUM) || (q * v.coefs[n - 2] > ((r << BASE_SIZE) + u.coefs[j + n - 2])))
            {
                q--;
                r += v.coefs[n - 1];
            }
        }

        BigNumber temp = v * BASE(q);
        temp = temp.Shift(j);

        if (u < temp) {
            q--;
            temp = v * BASE(q);
            temp = temp.Shift(j);
        }

        u = u - temp;
    }

    u = u / d;
    int k = u.coefs.size() - 1;

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

    BigNumber result(1, 2);
    result.coefs[0] = r;

    return result;
}

void BigNumber::OutputHex()
{
    int k = BASE_SIZE - 4;
    string result;
    int len = coefs.size();

    for (int j = len - 1; j >= 0; j--)
    {
        while (k >= 0) {
            int tmp = (coefs[j] >> k) & (0xf);

            if (tmp >= 0 && tmp <= 9)
                result += (char)(tmp + '0');
            if (tmp >= 10 && tmp <= 15)
                result += (char)(tmp - 10 + 'A');

            k -= 4;
        }

        result += " | ";
        k = BASE_SIZE - 4;
    }

    cout << result << endl;
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
    // Инициализация генератора случайных чисел
    srand(time(0));

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
    Test();

    return 0;
}
