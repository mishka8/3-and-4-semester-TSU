#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;


void print_vec(vector<int> vec)
{
    for(int i = 0;  i < vec.size(); i++)
        cout << vec[i] << " ";
    cout << endl;

    
}

vector<int> deykstra(vector<int> vec)
{
    unsigned int size = vec.size();
    
    for(int i = size - 2; i >= 0; i--)
    {
        if((i < size - 1) && (vec[i] < vec[i + 1]))
        {
            for(int j = size - 1; j > i; j--)
            {
                if(vec[i] < vec[j])
                {
                    swap(vec[i], vec[j]);
                    reverse(vec.begin() + i + 1, vec.end());
                    return vec;
                }
            }
        }
    }
    return vec;
}




int main()
{
    vector<int> vec1 = {1, 2, 3, 4};
    vector<int> vec2 = {1, 2, 3, 4};
    
    reverse(vec2.begin(), vec2.end());
    
    while(vec1 != vec2)
    {
        print_vec(vec1);
        vec1 = deykstra(vec1);
    }
    
    return 0;
}
