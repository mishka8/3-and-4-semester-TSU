using System;

interface IFigure
{
    double Square();
    double Perimetr();
}

class RightTriangle : IFigure
{
    private double xa, ya;//координаты точки A
    private double a, b;//катеты

    public RightTriangle(double x = 0 , double y = 0, double a = 3, double b = 4)
    {
        this.xa = x;
        this.ya = y;
        this.a = a;
        this.b = b;
    }

    public double get_xa() => xa;
    public void set_xa(double value) => xa = value;

    public double get_ya() => ya;
    public void set_ya(double value) => ya = value;

    public double get_a() => a; 
    public void set_a(double value) => a = value;
    
    public double get_b() => b;
    public void set_b(double value) => b = value;

    public void input()
    {
        Console.Write("Введите xa: ");
        this.xa = Convert.ToDouble(Console.ReadLine());
        Console.Write("Введите ya: ");
        this.ya = Convert.ToDouble(Console.ReadLine());
        Console.Write("Введите a: ");
        this.a = Convert.ToDouble(Console.ReadLine());
        Console.Write("Введите b: ");
        this.b = Convert.ToDouble(Console.ReadLine());
    }

    public void output()
    {
        Console.WriteLine("Точка A: ( " + this.xa + ", " + this.ya + " )");
        Console.WriteLine("Катет a: " + this.a);
        Console.WriteLine("Катет b: " + this.b);
    }

    public double Hypotenuse() => Math.Sqrt(this.a * this.a + this.b * this.b);//гипотенуза

    public double Perimetr() => this.a + this.b + Hypotenuse();//периметр

    public double Square() => (this.a * this.b) / 2;//площадь 
    
    public double Radius() => Hypotenuse() / 2;

    public double Median() => Math.Sqrt((2 * this.a * this.a + 2 * this.b * this.b - Hypotenuse() * Hypotenuse()) / 2);

    public double LargestAngle()
    { 
        double angle1 = Math.Atan(this.b / this.a) * 180 / Math.PI;//перевод из радиан в градусы
        double anglee2 = 90 - angle1;

        return Math.Max(angle1, anglee2);
    }

    public bool ChekRNB()
    {
        //так как числа типа double используем epsilon это наименьшее возможное число типа double
        return Math.Abs(this.a - this.b) < double.Epsilon;
    }

    public void Mul(double value)
    {
        if (value <= 0)
        {
            throw new ArgumentNullException("value < 0 is not correct");//исключение при нулевом значении
        }

        this.a *= value;
        this.b *= value;
    }

    public bool CheckSimilarity (RightTriangle other)
    {
        if(other == null)
        {
            Console.WriteLine("other is not correct");
            return false;
        }

        double ratio1 = this.a / other.a;//отношения катетов 
        double ratio2 = this.b / other.b;

        return Math.Abs(ratio1 - ratio2) < double.Epsilon;//проверка что это отношение равно
    }

    public bool IsIntersectionQuadrilateral(RightTriangle other)
    {
        if (other == null)
        {
            Console.WriteLine("other is not correct");
            return false;
        }

        //первый треугольник
        
        double ya1 = this.ya;
        double xb1 = xa + this.a;
        double yb1 = ya1;
        double xc1 = xa;
        double yc1 = ya1 + this.b;

        //второй треугольник
        double xa2 = other.xa;
        double ya2 = other.ya;
        double xb2 = xa2 + other.a;
        double yb2 = ya2;
        double xc2 = xa2;
        double yc2 = ya2 + other.b;

        //проверка что треугольники не лежат друг в друге
        bool isFirstInsideSecond = xa >= xa2 && ya1 >= ya2 && xb1 <= xb2 && yb1 <= yb2 && xc1 <= xc2 && yc1 <= yc2;

        bool isSecondInsideFirst = xa2 >= xa && ya2 >= ya1 && xb2 <= xb1 && yb2 <= yb1 && xc2 <= xc1 && yc2 <= yc1;

        if (isFirstInsideSecond || isSecondInsideFirst)
        {
            return false;
        }

        //проверка что они пересекаются 
        //проверка что они лежат не правее не левее не ниже не выше
        bool isOverlapping = !(xb1 < xa2 || xb2 < xa || yc1 < ya2 || yc2 < ya1);
        //Треугольники пересекаются
        //никто не лежит полностью внутри другого
        //они не просто касаются

        return isOverlapping;
    }
}

class Program
{
    static void Main(string[] args)
    {
        RightTriangle triangle1 = new RightTriangle(0, 0, 3, 4);//для проверки подобия 


        //RightTriangle triangle1 = new RightTriangle(0, 0, 5, 5 * Math.Sqrt(3));//треугольник с углами в 60 и 3 градусов

        //RightTriangle triangle1 = new RightTriangle(0, 0, 8, 8);//для проверки равнобедренности и  угла в 45 градусов

        RightTriangle triangle2 = new RightTriangle(1, 1, 6, 8);

        triangle1.output();
        Console.WriteLine();

        //Console.WriteLine("Гипотенуза - " + triangle1.Hypotenuse());
        //Console.WriteLine("Периметр - " + triangle1.Perimetr());
        //Console.WriteLine("Площадь - " + triangle1.Square());
        //Console.WriteLine("Радиус описанной окружности - " + triangle1.Radius());
        //Console.WriteLine("Медиана - " + triangle1.Median());
        //Console.WriteLine("Наибольший угол - " + triangle1.LargestAngle());
        //Console.WriteLine("Проверка на равнобедренность - " + triangle1.ChekRNB());

        Console.WriteLine("Умножение - ");
        triangle1.Mul(5);
        triangle1.output();
        
        //Console.WriteLine("ПОдобие с треугольником 2 - " + triangle1.CheckSimilarity(triangle2));
        //Console.WriteLine("Пересечение является четырехугольником: " + triangle1.IsIntersectionQuadrilateral(triangle2));
        Console.WriteLine();

        triangle2.output();
        Console.WriteLine();
    }
}
