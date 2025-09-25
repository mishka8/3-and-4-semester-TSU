#include <stdio.h>
#include"array.h"

using namespace std;
template <class T>
class Stack {
private:
	T* arr;
	int n;
	int max;
public:
	Stack(int k = 50) {
		this->arr = new T[max = k];
		this->n = 0;

	}
	Stack(const Stack<T>& object) {
		this->max = object.max;
		this->n = object.n;
		this->arr = new T[max];
		for (int i = 0; i < n; i++) {
			this->arr[i] = object->arr[i];
		}
	}
	bool Empty() {
		return n == 0 ? true : false;

	}
	void Push(T key) {
		if (this->Full()) {
			cout << "stack is overflow";
			return;
		}
		arr[n++] = key;
	}
	bool Full() {
		return n == max ? true : false;
	}
	T Pop() {
		if (n <= 0) {
			cout << "stack is underflow";
			return 0;
		}
		else {
			return arr[--n];
		}

	}
	T Top() {
		if (n < 0) {
			cout << "stack is underflow";
			return 0;
		}
		else {
			return arr[n];
		}

	}
	Stack& operator =  (const Stack& object) {
		if (this == &object) {
			return *this;
		}
		delete[] this->arr;
		this->arr = new T[object.max];
		this->max = object.max;
		this->n = object.n;
		for (int i = 0; i < n; i++) {
			this->arr[i] = object.arr[i];
		}
		return *this;

	}
	friend ostream& operator<<(ostream& out, Stack<T>& object) {
		if (object.Empty()) {
			cout << "stack is underfull";
		}
		for (int i = 0; i < object.n; i++) {
			out << object.arr[i] << " ";
		}
		out << "\n";
		return out;
	}

};

int main() {
	Stack<double> test(10),u(2);
	//if (test.Empty()) {
	//	cout << "yes";
	//}
	test.Push(10);
	test.Push(10.2);
	test.Push(11.3);
	u = test;
	cout << test;
	cout << test.Pop() << endl;
	cout << u;
}
