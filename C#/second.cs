using System;

class Matrix
{
    static int[,] create_rand_mx(int rows, int cols, int min, int max)
    {
        Random rand = new Random();
        int[,] matrix = new int[rows, cols];

        for(int i = 0; i < rows; i++)
        {
            for(int j = 0; j < cols; j++)
            {
                matrix[i, j] = rand.Next(min, max + 1);
            }
        }
        return matrix;
    }

    static (int MinRows, int MaxRows) FindMinMaxSumRows(int[,] mx, int rows, int cols)//можно создовать методы которые возвращают несколько значений
    {
        int MinRows = 0;
        int MaxRows = 0;
        int MinSum = 1000000;
        int MaxSum = -1;

        for(int i = 0; i < rows; i++)
        {
            int sum_in_rows = 0;
            for(int j = 0; j < cols; j++)
            {
                sum_in_rows += mx[i, j];
            }

            if(sum_in_rows < MinSum)
            {
                MinSum = sum_in_rows;
                MinRows = i;
            }
            
            if(sum_in_rows > MaxSum)
            {
                MaxSum = sum_in_rows;
                MaxRows = i;
            }
        }
        return (MinRows, MaxRows);
    }

    static void SwapRows(int[,] mx, int cols, int row1, int row2)
    {
        if(row1 == row2)
            return;
        
        for(int j = 0; j < cols; j++)
        {
            int tmp = mx[row1, j];
            mx[row1, j] = mx[row2, j];
            mx[row2, j] = tmp;
        }
    }

    static void PrintMx(int[,] mx, int rows, int cols)
    {
        for(int i = 0; i < rows; i++)
        {
            for(int j = 0; j < cols; j++)
            {
                Console.Write(mx[i, j] + " ");
            }
            Console.WriteLine();
        }
    }

    static void Main()
    {
        int rows = 4;
        int cols = 5;
        int min = 5;
        int max = 20;
        
        int[,] mx = create_rand_mx(rows, cols, min, max);

        Console.WriteLine("Матрица");
        PrintMx(mx, rows, cols);
        Console.WriteLine();
        
        var (MinRow, MaxRow) = FindMinMaxSumRows(mx, rows, cols);
        //Конструкция var в C# предназначена для объявления переменных без явного указания их типа данных
        //Кортеж в C# — это структура данных, которая позволяет объединить несколько значений разного типа в одну логическую единицу.
        Console.WriteLine("Min - " + MinRow + " Max - " + MaxRow);
        Console.WriteLine();

        SwapRows(mx, cols, MinRow, MaxRow);

        Console.WriteLine("Новая матрица");
        PrintMx(mx, rows, cols);

    }
}
