using System;
using System.Collections.Generic;

class Program
{
    static void Main()
    {
        Stack<int> numbers = new Stack<int>();

        //добавляем числа в стек
        numbers.Push(5);
        numbers.Push(10);
        numbers.Push(15);

        Console.WriteLine("что в стеке:");
        foreach (int num in numbers)
        {
            Console.Write(num + " ");
        }
        Console.WriteLine();
        Console.WriteLine();

        // Извлекаем два верхних числа
        int a = numbers.Pop();
        int b = numbers.Pop();
        Console.WriteLine("достали - " + a + " " + b);
        Console.WriteLine();

        // Складываем их
        int sum = a + b;
        Console.WriteLine("Сумма - " + sum);
        Console.WriteLine();

        //обратно
        numbers.Push(sum);

        Console.WriteLine("что осталось в стеке");
        foreach (int num in numbers)
        {
            Console.Write(num + " ");
        }
        Console.WriteLine();
        Console.WriteLine();

        a = numbers.Pop();
        b = numbers.Pop();

        Console.WriteLine("достали - " + a + " " + b);
        Console.WriteLine();

        sum = a + b;

        Console.WriteLine("Сумма - " + sum);
        Console.WriteLine();


        numbers.Push(sum);
        Console.WriteLine("что осталось");

        foreach (int num in numbers)
        {
            Console.WriteLine(num);
        }


    }
}
