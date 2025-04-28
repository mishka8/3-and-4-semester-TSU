#include <iostream>
#include <fstream>

using namespace std;
//кодирование
//Чтение исходного файла
//Подсчет частот символов
//
//Построение дерева Хаффмана
//
//Генерация кодов для каждого символа
//
//Замена символов на соответствующие коды
//
//Сохранение таблицы частот и закодированных данных в файл



//узел дерева хаффмана
struct Haffman_block
{
    char data;//символ
    int period;//частота
    Haffman_block* left;//левый потомок
    Haffman_block* right;//правый потомок

    Haffman_block(char data, int period) 
        : data(data), period(period), left(nullptr), right(nullptr) {}

};

struct compare
{
    bool operator() (Haffman_block* a, Haffman_block* b)
    {
        return a -> period > b -> period;//проверка у кого больше частота
    }
};

//читаем файл
string read_file(const string& filename)
{
    ifstream file(filename);
    if (!file)
    {
        throw runtime_error("файл не открыт " + filename);
    }

    string text;

    char sim;
    while (file.get(sim))
    {
        text += sim;
    }

    return text;
}







int main()
{

    cout << "Hello World!\n";

}
