#include <cstdlib>
#include <iostream>
#include <time.h>
#include <algorithm>


using namespace std;


class Array {
private:
    int* array; // Сам массив
    size_t size; // Кол-во эл-тов
public:
    // Конструкторы
    Array(size_t n = 0) {
        array = new int[n];
        size = n;
        // Избавлюсь от мусорных зн-ий
        for (int i = 0; i < size; i++)
            array[i] = 0;
    }
    Array(int* arr, size_t n) : Array(n) {
        for (int i = 0; i < n; i++)
            array[i] = arr[i];

    }
    Array(const Array& arr) : Array(arr.size) {
        for (int i = 0; i < size; i++)
            array[i] = arr.array[i];
    }
    // Деструктор
    ~Array() {
        if (array) delete[] array;
        array = NULL;
        size = 0;
    }

    size_t getSize() { return size; }

    int Search(int key);
    int findMax();
    int findMin();
    void AddElem(int key, size_t index);
    void DeleteElem(size_t index);
    void RandomArray(int a, int b);


    Array& operator =  (const Array&);
    int& operator [] (size_t);
    Array  operator +  (int key);
    Array& operator += (int key);
    Array  operator +  (const Array&);
    Array& operator += (const Array&);
    Array  operator -  (int key);
    Array& operator -= (int key);

    bool operator == (const Array&);
    bool operator != (const Array&);
    
    friend istream& operator >> (istream&, Array&);
    friend ostream& operator << (ostream&, const Array&);
};


int Array::Search(int key) {
    for (int i = 0; i < size; i++)
        if (array[i] == key)
            return i;
    return -1;
}


int Array::findMax() {
    if (size == 0) {
        cout << "Error: Array is empty!";
        exit(0);
    }
    
    int result = array[0];

    for (int i = 1; i < size; i++)
        if (result < array[i])
            result = array[i];

    return Search(result);
}


int Array::findMin() {
    if (size == 0) {
        cout << "Error: Array is empty!\n";
        exit(0);
    }
    
    int result = array[0];

    for (int i = 1; i < size; i++)
        if (result > array[i])
            result = array[i];

    return Search(result);
}


void Array::AddElem(int key, size_t index) {
    // Вставляем только на существующую позицию
    if (index >= size) {
        cout << "Error: Index out of range!\n";
        exit(0);
    }
    
    int *new_arr = new int[size + 1];

    for (int i = 0; i < size; i++)
        new_arr[i] = array[i];
    
    delete[] array;
    array = new_arr;
    
    size++;
    
    array[size - 1] = key;
    
    for (int i = size - 1; i > index; i--)
        swap(array[i], array[i - 1]);
}


void Array::DeleteElem(size_t index) {
    // Удаляем только из существующей позиции
    if (index >= size) {
        cout << "Error: Index out of range!\n";
        exit(0);
    }

    for (int i = index; i < size - 1; i++)
        swap(array[i], array[i + 1]);

    int* new_arr = new int[size - 1];

    for (int i = 0; i < size - 1; i++)
        new_arr[i] = array[i];
    
    delete[] array;
    
    array = new_arr;
    size--;
}


void Array::RandomArray(int a, int b) {
    if (a > b) swap(a, b);

    for (int i = 0; i < size; i++)
        array[i] = a + rand() % (b - a + 1); // [a;b]
}


Array& Array::operator = (const Array& arr) {
    if (this == &arr)
        return *this;

    delete[] array;

    array = new int[arr.size];
    size = arr.size;

    for (int i = 0; i < size; i++)
        array[i] = arr.array[i];

    return *this;
}


int& Array::operator [] (size_t index) {
    if (index >= size) {
        cout << "Error: Index out of range!\n";
        exit(0);
    }

    return array[index];
}


ostream& operator << (ostream& out, const Array& arr) {
    for (int i = 0; i < arr.size; i++)
        out << arr.array[i] << "  ";

    return out;
}


istream& operator >> (istream& in, Array& arr) {
    cout << "Input size: ";
    cin >> arr.size;

    delete[] arr.array;
    arr.array = new int[arr.size];

    cout << "Array: ";
    for (int i = 0; i < arr.size; i++)
        cin >> arr.array[i];
    return in;
}


bool Array::operator == (const Array& arr1) {
    if (arr1.size != size)
        return false;
    
    for (int i = 0; i < arr1.size; i++) {
        if (array[i] != arr1.array[i])
            return false;
    }
    return true;
}


bool Array::operator != (const Array& arr1) {
    return !(*this == arr1);
}


Array Array::operator + (int key) {
    Array res(*this);
    res.AddElem(key, res.size - 1);
    swap(res.array[res.size - 2], res.array[res.size - 1]);
    return res;
}


Array& Array::operator += (int key) {
    *this = *this + key;
    return *this;
}

Array Array::operator + (const Array& arr) {
    Array result(size + arr.size);

    for (int i = 0; i < size; i++)
        result.array[i] = array[i];

    for (int i = size; i < size + arr.size; i++)
        result.array[i] = arr.array[i - size];

    return result;
}


Array& Array::operator += (const Array& arr) {
    *this = *this + arr;
    return *this;
}


Array Array::operator - (int key) {
    Array res(*this);
    int index = res.Search(key);
    if (index < 0) {
        cout << "Elem not found!\n";
        return res;
    }
    res.DeleteElem(index);
    return res;
}


Array& Array::operator -= (int key) {
    *this = *this - key;
    return *this; 
}


int main() {
    srand(time(NULL));

    Array a1(10);
    Array a2(10);
    a1.RandomArray(-10, 10);
    a2.RandomArray(-10, 20);
    cout << a1 << " " << a1.getSize() << endl;
    cout << a2 << " " << a2.getSize() << endl;


    a1 += 1488;
    a2 -= 2;
    cout << a1 << " " << a1.getSize() << endl;
    cout << a2 << " " << a2.getSize() << endl;

    if (a1 != a2) cout << "YES!\n";

    return 0;
}
