#include <iostream>
#include <cstring>

using namespace std;

class String
{
    char *line; // строка
    size_t len; // длина строки

public:
    String(const char *line1)
    {
        size_t len = strlen(line1);
        char *line = new char[len + 1]; // выделяем под последний '\0' (стандарт строки в C)
        strcpy(line, line1);
    } // конструктор (classic, я бы даже сказал pleasantly)

    String(size_t L = 80)
    {
        size_t len = L;
        char *line = new char[len + 1];
        line[0] = '\0';
    } // конструктор с указанной длиной строки

    String(const String &other)
    {
        size_t len = other.len;
        char *line = new char[len + 1];
        line = other.line;
    } // конструктор копирования (копирует указанный объект)

    ~String()
    {
        delete[] line;
    } // деструктор (удаляет динамическую память)

    String &operator=(const String &);  // присвоение
    char &operator[](size_t);           // возвращает ссылку
    String operator+(const String &);  // конкатенация
    String &operator+=(const String &); // конкатенация
    bool operator==(const String &);    // сравнение строк

    friend istream &operator>>(istream &, String &); // потоковый ввод
    friend ostream &operator<<(ostream &, const String &); // потоковый вывод
};

String &String::operator=(const String &other)
{
    if (this == &other)
        return *this; // если совпадают ссылка, значит одинаковые

    delete[] line;
    this->len = other.len;
    this->line = new char[len];
    strcpy(this->line, other.line);

    return *this;
}

char &String::operator[](size_t index)
{
    if (index == 0 || index >= this->len)
    {
        cout << "Index out of range!" << endl;
        exit(0);
    } // если index не в промежутке длины строки => соси яйца
    return line[index]; // возвращает ССЫЛКУ (не яйца) на элмент в массиве
}

String &String::operator+=(const String &other)
{
    int oldlen = this->len;
    char *oldline = new char[oldlen];
    strcpy(oldline, line);      // сохраняем наш len, ведь мы его будем изменять
    this->len += other.len - 1; // потому что в обоих объектах + 1 для '\0' => один отнимаем (соси яйца)
    this->line = new char[len];
    for (int i = 0; i < this->len; i++)
    {
        if (i < oldlen)
        { // проходимся по старому слову и заполняем
            line[i] = oldline[i];
        }
        else
            line[i] = other.line[i]; // проходимся по новому
    }
    line[len] = '\0'; // в конце не забываем '\0'

    return *this;
} // яйца

String String::operator+ (const String& other) {
    String result("");
    result.len = (this->len + other.len) - 1; // потому что в обоих объектах + 1 для '\0' => один отнимаем (соси яйца)
    result.line = new char[result.len];
    for (int i = 0; i < result.len; i++)
    {
        if (i < this->len)
        { // проходимся по старому слову и заполняем
            result.line[i] = this->line[i];
        }
        else
            result.line[i] = other.line[i]; // проходимся по новому
    }
    result.line[result.len] = '\0'; // в конце не забываем '\0'

    return result;
}

bool String::operator==(const String &other)
{
    if (this->len != other.len)
        return 0;

    for (int i = 0; i < this->len; i++)
    {
        if(this->line[i] != other.line[i]) return 0;
    }
    return 1;
}

ostream& operator<<(ostream &out, const String& string) {
    for(int i = 0; i < string.len; i++) {
        out << string.line[i];
    }
    return out; 
}

istream &operator>>(istream &in, String& string)
{
    cout << "Input length of string: ";
    cin >> string.len;
    string.len++;
    string.line = new char[string.len];
    for(int i = 0; i < string.len-1; i++)
    {
        cin >> string.line[i];
    }
    string.line[string.len] = '\0';
    return in;
}

int main () {
    String string1("hui");
    String string2("9ica");
    String myaco("chlen");

    myaco = string2;
    
    cout << myaco;

}
