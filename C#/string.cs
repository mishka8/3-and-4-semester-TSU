//Вывести на экран 3 исходных строки не менее, чем из 5 слов.
//Создать новую строку из двух на выбор,
//затем расположить их в алфавитном порядке.
//Ход программы отобразить пояснениями на экране.
using System;
namespace String1
{
    internal class Program
    {
        static void Main(string[] args)
        {
            String[] orig_str = ["Мы видели маленькую рыженькую белочку",
                                 "Два щенка, щека к щеке, щиплют щёку в уголке",
                                 "Расскажите про покупки! — Про какие про покупки?",
                                 "И прыгают скороговорки, как караси на сковородке" ];

            foreach(string str in orig_str)
            {
                Console.WriteLine(str);
            }
            Console.WriteLine();

            Console.Write("Введите два числа ");
            int num1 = 0;
            int num2 = 0;

            string input1 = Console.ReadLine();
            string input2 = Console.ReadLine();

            int.TryParse(input1, out num1);//метод преобразовываем строку в указанный тип
            int.TryParse(input2, out num2);
            
            if(num1 > 0 && num2 > 0 && num1 < 4 && num2 < 4)
            {
                string str_res = orig_str[num1 - 1] + " " + orig_str[num2 - 1];

                Console.WriteLine("Результат\n" + str_res);
                Console.WriteLine();

                //теперь строку которую получили отсортируем по алфавиту 
                //разделим на слова и массив слов отсортируем и выведем
                //и уберем знаки препинания

                string[] words = str_res.Replace("!", "").Replace("?", "").Replace(",", "")
                    .Replace("—", "").Replace(".", "").Replace("  ", " ").Split(' ');


                foreach (string word in words)
                {
                    Console.Write(word + ' ');
                }
                Console.WriteLine();
                Console.WriteLine();

                Array.Sort(words);
                //сортировка по возрастанию в случае строк
                //сортировка в алфавитном порядке

                foreach (string word in words)
                {
                    Console.Write(word + ' ');
                }
                Console.WriteLine();
            }
            else
            {
                Console.WriteLine("Нет строк с такими номерами");
            }
        }
    }
}
