#include <iostream>
#include <fstream>
#include <string>
#include <vector>


using namespace std;


int main()
{
	string filename = "primer.txt";

	ifstream ifs(filename, ifstream::binary);

	if (!ifs)
	{
		cerr << "ERROR File is not open";
	}

	vector<int> freq(256, 0);


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

	/*for (int i = 0; i < 256; ++i)
	{
		cout << "[" << i << "] = " << freq[i] << ", ";
		if (i % 10 == 0 && i != 0)
			cout << endl;
	}
	cout << endl;*/

}
