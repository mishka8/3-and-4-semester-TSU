#include <iostream>
#include <exception>
#include <vector>
#define buffer 1000
#define size_tab 256

using namespace std;


class String
{
    char *pStr;
    int len;

    public:
        String(int l = 0)
        {
            pStr = new char[l+1];
            len = l;
            for(int i = 0; i != l+1; ++i)
                pStr[i] = 0;
            pStr[l+1] = '\0';
        }

        String(const char *str)
        {
            int i = 0;
            while(str[i] != '\0')
            {
                ++i;
            }
            pStr = new char[i + 1];
            len = i;
            for(int j = 0; j < len; ++j)
            {
                pStr[j] = str[j];
            }
            pStr[len + 1] = '\0';
        }

        String(String &str)
        {
            len = str.len;
            pStr = new char[len + 1];
            for(int i = 0; i != len; ++i)
            {
                pStr[i] = str.pStr[i];
            }
            pStr[len] = '\0';
        }

//        String(String &&str)//правая ссылка, конструктор перемещения
//        {
//            len = str.len;
//            delete [] pStr;
//            pStr = str.pStr;
//        }


        ~String ()
        {
            if(pStr)
            {
                delete []pStr;
            }
            
            pStr = nullptr;// это ключевое слово, используемое для явного указания на отсутствие значения или нулевого указателя
        }

        int strLength()
        {
            return len;
        }

        String & operator = (const String &str)
        {
            if(this == &str)
            {
                return *this;
            }
            
            delete []pStr;
            len = str.len;
            pStr = new char[len + 1];

            for(int i = 0; i != len; ++i)
            {
                pStr[i] = str.pStr[i];
            }
            pStr[len] = '\0';
            return *this;
        }

        char &operator [](int i)
        {
            if(i < 0 || i >= len)
            {
//                throw out_of_range("index out of range");
                cout << "index out of range" << endl;
                exit(-1);

            }
            else
            {
                return pStr[i];
            }
        }

        bool operator == (String &str)
        {
            if(len != str.len)
                return false;

            for(int i = 0; i != len; ++i)
            {
                if(pStr[i] != str.pStr[i])
                    return false;
            }

            return true;
        }

        bool operator != (String &str)
        {
            if(len != str.len)
                return true;

            for(int i = 0; i != len; ++i)
            {
                if(pStr[i] != str.pStr[i])
                    return true;
            }

            return false;
        }

        String operator + (const String &str)
        {
            int size = this->len + str.len;
            String s_sum(size);

            for(int i = 0; i != len; ++i)
            {
                s_sum.pStr[i] = pStr[i];
            }

            for(int i = 0; i != size; ++i)
            {
                s_sum.pStr[len + i] = str.pStr[i];
            }

            return s_sum;
        }

        String & operator += (const String &str)
        {
            char *pStrTmp = new char [len + str.len + 1];
            
            for(int  i = 0; i != len; ++i)
            {
                pStrTmp[i] = pStr[i];
            }
            for(int i = 0; i != len + str.len; ++i)
            {
                pStrTmp[len + i] = str.pStr[i];
            }

            delete [] pStr;
            pStr = pStrTmp;
            len = len + str.len;
            pStrTmp[len] = '\0';
            
            return *this;
        }



        friend istream & operator >> (istream &in, String &str)
    {
    char pStrTmp[buffer];  // Исправлено на правильный тип
    in >> pStrTmp;
    int size = 0;
    while(pStrTmp[size] != '\0')  // Исправлено на правильную проверку конца строки
    {
        ++size;
    }
    
    delete []str.pStr;
    
    str.pStr = new char[size + 1];
    str.len = size;
    str.pStr[size] = '\0';  // Исправлено на size
    for(int i = 0; i != size; ++i)  // Исправлено на правильную копию
    {
        str.pStr[i] = pStrTmp[i];
    }
    
    return in;
    }


        friend ostream & operator << (ostream &out, String &str)
        {
            return out << str.pStr;
        }

        String operator() (int i, int j)
        {
            if(i > j ||  i < 0 || i > len || j < 0 || j > len)
            {
                cout << "index out of range" << endl;
                exit(-1);
            }

            int new_len = j - i;
            String res(new_len);

            for(int  k = 0; k < new_len; k++)
            {
                res[k] = pStr[i + k];
            }

            return res;
        }

        String firstEntry(String& prototype)
        {
            String res(0);

            int ix = BMSearch(prototype);
            if(ix == -1)
            {
                cout << "NO pattern in str" << endl;
                return res;
            }

            res = (*this)( 0, ix);
            res+= (*this)(ix + prototype.len, len);

            return res;
        }
        
        int BMSearch(String &prototype);

};

int String:: BMSearch(String &prototype)
{
    int len_pr = prototype.len;
    int lenS = len;
    int TAB[size_tab];

    if(len_pr > lenS)
    {
        return -1;
    }

    for(int i = 0; i < size_tab; TAB[i] = len_pr, ++i);//таблица смещений

    for(int i = 0; i < len_pr - 1; TAB[prototype[i]] = len_pr - i - 1, ++i);
    //prototype[i] = a -> (int)prototype[i] = 97 -> TAB[97] = len_pr - i - 1;

    int i = len_pr - 1;//позиция в pStr с которой сравниваем
    int j = i;//позиция в шаблоне
    int k = i;//вспомогатеьная переменная для сравнения

    while(j >= 0 && i <= lenS - 1)//цикл по строке
    {
        j = len_pr - 1;
        k = i;
        while(j >= 0 && pStr[k] == prototype[j])//сравниваем шаблон и строку справа налево
        {
            k--;
            j--;
        }
        
        i += TAB[pStr[i]];//смещяемся по строке если несовпали
    }
    if(k >= lenS - len_pr)
    {
        return -1;
    }
    else
    {
        return k + 1;//возвращаем индех вхождения
    }


}

int main()
{
    String s4("how are too you?");
    String s3("too");

    String s5(0);

    s5 = s4.firstEntry(s3);
    cout << "string - " << s4 << endl;
    cout << "prototype - " << s3 << endl;
    cout << "res - " << s5;

    return 0;
}
