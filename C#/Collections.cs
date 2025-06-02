using System;
using System.Collections.Generic;

public class Stack<type>
{
    private readonly List<type> st;

    public Stack(int size = 10)
    {
        st = new List<type>(size);
    }

    public int Count => st.Count;//размерность
    public bool IsEmpty => Count == 0;//првоерка на пустоту

    public void Push(type value)//положить
    {
        st.Add(value);
    }

    public type Pop()//вытащить
    {
        if (IsEmpty)
            throw new InvalidOperationException("is empty");

        int lastIndex = st.Count - 1;
        type item = st[lastIndex];
        st.RemoveAt(lastIndex);//
        return item;
    }

    public type Peek()//что сверху
    {
        if (IsEmpty)
            throw new InvalidOperationException("Стек пуст!");

        return st[st.Count - 1];
    }

    public void Clear()//очистка удаление элементов
    {
        st.Clear();
    }

    public void PrintStack()//вывод
    {
        Console.WriteLine("элементов - " + Count);
        for (int i = st.Count - 1; i >= 0; i--)
        {
            Console.WriteLine("[" + i + "]" + " - " + st[i]);
        }
        Console.WriteLine();
    }
}

class Program
{
    static void Main(string[] args)
    {
        var stack = new Stack<int>();

        
        stack.Push(10);
        stack.Push(15);
        stack.Push(4);
        Console.WriteLine("наш стек");
        stack.PrintStack();
        

        Console.WriteLine("Пример работы со стеком");
        int a = stack.Pop();
        int b = stack.Pop();
        Console.WriteLine("1 - " + a);
        Console.WriteLine("2 -  " + b);


        int sum = a + b;
        Console.WriteLine("Сумма - " + sum);
        Console.WriteLine();

        stack.Push(sum);
        Console.WriteLine("наш стек");
        stack.PrintStack();

        Console.WriteLine("\nДобавляем 4 и 5:");
        stack.Push(4);
        stack.Push(5);
        stack.PrintStack();
        Console.WriteLine();

        Console.WriteLine("Очистка стека:");
        stack.Clear();
        stack.PrintStack();
    }
}
