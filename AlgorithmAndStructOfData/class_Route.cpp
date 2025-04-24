    #include <iostream>

    using namespace std;

    class route
    {
        int *r, n;// r – массив, представляющий маршрут; n – количество городов

    public:
            route(int num = 0)
            {
                n = num;
                r = new int[n];
                for(int i = 0; i < n; ++i)
                {
                    r[i] = i;
                }
            }

            route(const route &rcopy)
            {
                r = new int[rcopy.n];//rcopy.n размер
                for(int i = 0; i < rcopy.n; ++i)
                {
                    r[i] = rcopy.r[i];//rcopy объект обращаемся к его полю r[i]
                }
                n = rcopy.n;
            }


            route & operator=(const route &r2)
            {
                if(this == &r2)
                    return *this;
                delete[] r;//удаляем то что хранит элемент

                r = new int[r2.n];//rcopy.n размер
                for(int i = 0; i < n; i++)
                {
                    r[i] = r2.r[i];//rcopy объект обращаемся к его полю r[i]
                }
                n = r2.n;

                return *this;
            }

            ~route()//вызывается компилятором автоматически когда объект перестаёт существовать
            {
                if(r)
                    delete []r;
                r = NULL;
            }

            int routePrice(int** matrix_cost);

            bool nextRoute();


            //friend ostream & operator<<(ostream&, const route&);
            friend ostream& operator << (ostream& out, const route& routeObj) {
                out << "Route -  ";
                for (int i = 0; i < routeObj.n; i++)
                    out << routeObj.r[i] + 1 << " ";
                out << endl;
                return out;
            }

    };

    int** create_matrix_cost(int n)
    {
        int **matrix = NULL;
        matrix = new int*[n];

        if(matrix== NULL)
            return NULL;
        for(int i = 0; i < n; i++)
        {
            matrix[i] = new int[n];

            if(matrix[i] == NULL)
            {
                delete[] matrix;
                matrix = NULL;
                return matrix;
            }
            for(int j = 0; j < n; j++)
                matrix[i][j] = 0;
        }

        return matrix;
    }

    void Print_matrix_cost(int **matrix, int n)
    {
        for(int i = 0; i < n; i++)
        {
            for(int j = 0; j < n; j++)
                cout << matrix[i][j] << "   ";
            cout << endl;
        }
    }

    int route::routePrice(int** matrix_cost)
    {
        int result = 0;

        if(matrix_cost == NULL)
            return result;

        for (int i = 0; i < n - 1; i++)
            result += matrix_cost[r[i]][r[i+1]];
        result += matrix_cost[r[n-1]][r[0]];

        return result;
    }

    void swap_in_array(int **array, int n, int swap_1, int swap_2)
    {
        if(n < 0 || swap_1 < 0 || swap_2 < 0 || swap_1 >= n || swap_2 >= n)
            return;

        int tmp = (*array)[swap_1];
        (*array)[swap_1] = (*array)[swap_2];
        (*array)[swap_2] = tmp;

    }

    void reverse_array(int** array, int n, int begin_r, int end_r)
    {
        if(n < 0 || begin_r < 0 || begin_r >= n || end_r < 0 || end_r >= n)
            return;
        int center = (begin_r + end_r) / 2;
        for(int i = begin_r; i <= center; i++)
        {
            int tmp = (*array)[begin_r];
            (*array)[begin_r] = (*array)[end_r];
            (*array)[end_r] = tmp;
            begin_r++;
            end_r--;
        }
    }


    bool route::nextRoute()
    {
        int in = -1, jn = -1;//индексы в случае чего так же для проверки ошибки
        for(int i = n - 2; i >= 0; i--)
        {
            if((i < n - 1) && (r[i] < r[i + 1]))
            {
                in = i;
                break;
            }
        }
        if(in == 0)
            return false;

        jn = in;
        for(int j = n - 1; j > in; j--)
        {
            if(r[j] > r[in])
            {
                jn = j;
                break;
            }
        }
        swap_in_array(&r, n, in, jn);
        reverse_array(&r, n, in + 1, n - 1);
        return true;
    }

    int main()
    {
        int **matrix_cost = NULL;
        int n = 0;


        cout << "Input n (number of cities) ";
        cin >> n;

        route r1(n);
        route min_r1(r1);
        

        matrix_cost = create_matrix_cost(n);

        for(int i = 0; i < n; ++i)
        {
            for(int j =0; j < n; ++j)
            {
                if(i == j)
                    continue;
                cout << "[" << i << "]" << "[" << j << "] = ";
                cin >> matrix_cost[i][j];
            }
        }

        Print_matrix_cost(matrix_cost, n);

        do
        {
            cout << "route: " << r1;
            cout << "cost: " << r1.routePrice(matrix_cost) << endl;
            cout  << endl;
            if(r1.routePrice(matrix_cost) < min_r1.routePrice(matrix_cost))
            {
                min_r1 = r1;
            }
        } while (r1.nextRoute());
        
        cout << "min " << min_r1;
        cout << "min cost " << min_r1.routePrice(matrix_cost) << endl;
        

        return 0;
    }
