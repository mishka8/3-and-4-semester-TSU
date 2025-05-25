#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include <limits>
#include <algorithm>
#include <cmath>
#include <sstream>

using namespace std;

int get_index(const int& word, const vector<int>& wlist)
{
	for (int i = 0; i < wlist.size(); ++i)
	{
		if (word == wlist[i])
			return i;

	}
	return  -1;
}


void compress(const string& file_name)
{
	ifstream ifs(file_name, ios::binary);
	if (!ifs.is_open())
	{
		cerr << "File if not open - " << file_name << endl;
		return;
	}

	map<int, int> wlist;//символ частота

	int sim;
	while ((sim = ifs.get()) != EOF)//читаем посимвольно
	{
		wlist[sim]++;

	}

	ifs.close();

	int wcount = 0;//количество символов
	for (const auto& pair : wlist)
	{
		wcount += pair.second;
	}


	vector<int> Vwlist;//уникальные символы

	for (const auto& pair : wlist)
	{
		Vwlist.push_back(pair.first);
	}

	ifs.open(file_name, ios::binary);
	if (!ifs.is_open())
	{
		cerr << "File if not open - " << file_name << endl;
		return;
	}


	//верхняя и нижняя граница
	unsigned int up = numeric_limits<unsigned int>::max();//4 294 967 295
	unsigned int down = 0;


	vector<int> txt;//символы из файла для обработки
	while ((sim = ifs.get()) != EOF)
	{
		txt.push_back(sim);
	}

	ifs.close();

	ofstream compressed_file(file_name + ".compressed", ios::binary);

	if (!compressed_file.is_open())
	{
		cerr << "File is not open " << file_name << ".compressed" << endl;
		return;
	}

	// Записываем таблицу частот символов

	//преобразовываем к типу unsigned short
	unsigned short len = static_cast<unsigned short>(wlist.size());

	//перобразовываем указатель потому что write надо const char*
	//и размер того что мы преобразовываем в данном случае 2 байта будет
	compressed_file.write(reinterpret_cast<const char*>(&len), sizeof(len));


	for (const auto& pair : wlist)
	{
		//перобразовываем указатель потому что write надо const char*
		compressed_file.write(reinterpret_cast<const char*>(&pair.first), sizeof(pair.first));

		unsigned short freq = static_cast<unsigned short>(pair.second);
		compressed_file.write(reinterpret_cast<const char*>(&freq), sizeof(freq));
	}

	for (int word : txt)
	{
		int ix = get_index(word, Vwlist);

		if (ix == -1)
		{
			cerr << "Error word is not found" << endl;
			return;
		}

		unsigned int range = up - down + 1;

		unsigned int cum_count = 0;
		for (int i = 0; i < ix; ++i)
		{
			cum_count += wlist[Vwlist[i]];
		}

		up = down + (range * (cum_count + wlist[word])) / wcount - 1;
		down = down + (range * cum_count) / wcount;

	}

	compressed_file.write(reinterpret_cast<const char*>(&up), sizeof(up));

	compressed_file.close();


	cout << "File compressed in " + file_name + ".compressed" << endl;

}

int main()
{

	string file_name = "input.txt";

	compress(file_name);



	return 0;
}
