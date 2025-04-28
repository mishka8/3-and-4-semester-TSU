using System;//Заданы два массива X и Y, в каждом из них нет повторяющихся чисел. Верно ли, что все элементы массива X входят в массив Y?
namespace first1
{

    internal class Program
    {
        class Array
        {
            static bool CheckArray(int[] X, int[] Y)
            {
                foreach (int x in X)
                {
                    bool flag = false;
                    foreach (int y in Y)
                    {
                        if (x == y)
                        {
                            flag = true;
                            break;
                        }
                    }

                    if (flag == false)
                        return false;
                }
                return true;
            }
            
            //Поиск индекса числа num в массиве x
            static int IndexOf(int[] x, int size, int num)
            {
                for (int i = 0; i < size; i++)
                {
                    if (x[i] == num) return i;
                }
                return -1;
            }

            
            static int[] RandomArray(int size, int min, int max)
            {
                // Проверка, что диапазон достаточен для уникальных чисел


                Random rand = new Random();
                int[] array = new int[size];
                int check = 0;

                for (int i = 0; i < size; i++)
                {
                    int num;
                    do //генерация случайных чисел которых точно нет в массиве
                    {
                        num = rand.Next(min, max + 1);
                        check = IndexOf(array, size, num);
                    }
                    while (check != -1);

                    array[i] = num;
                }

                return array;
            }

            static void Main()
            {
                int size = 10;
                int min1 = 5;
                int max1 = 15;
                int min2 = 7;
                int max2 = 17;

                int[] Arr1 = RandomArray(size, min1, max1);
                int[] Arr2 = RandomArray(size, min2, max2);

                // int size = 3;
                // int min = 1;
                // int max = 4;

                // int[] Arr1 = RandomArray(size, min, max);
                // int[] Arr2 = RandomArray(size, min, max);

                Console.Write("Массив 1 - ");
                foreach (int i in Arr1)
                {
                    Console.Write(i + " ");
                }

                Console.WriteLine();

                Console.Write("Массив 2 - ");
                foreach (int i in Arr2)
                {
                    Console.Write(i + " ");
                }

                Console.WriteLine();

                bool res = CheckArray(Arr1, Arr2);
                Console.WriteLine(res ? "Верно" : "Не верно");
            }
        }
    }
}
