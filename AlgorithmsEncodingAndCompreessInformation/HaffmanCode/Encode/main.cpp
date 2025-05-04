#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;



void read_file(const string& filename, vector<int>& freq)
{
	ifstream ifs(filename, ifstream::binary);

	if (!ifs)
	{
		cerr << "ERROR File is not open";
	}

	vector<int> freq(256, 0);//подсчёт количества каждого символа


	while (!ifs.eof())
	{
		char ch;
		ifs.read(&ch, 1);

		//читаем по одному байту

		freq[static_cast<unsigned char>(ch)]++;
		//cout << static_cast<unsigned char>(ch) << ", ";

		//обычный char может быть отрицательный
		//размер вектора не может быть отрицательным
		//поэтому приводим с помощьлю stasic_cast к usigned char

	}

	for (int i = 0; i < 256; ++i)
	{
		if (freq[i] != 0)
		{
			cout << "[" << i << "] = " << freq[i] << ", ";
		}
	}
	cout << endl;
}



int main()
{
	vector<int> freq(256, 0);
	read_file("primer.txt", freq);

	

}
