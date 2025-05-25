#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include <limits>
#include <algorithm>
#include <cmath>

using namespace std;
using uint = unsigned int;
using ushort = unsigned short;

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

	char sim;
	while (ifs.get(sim))//читаем посимвольно
	{
		wlist[static_cast<int>(sim)]++;
	}

	ifs.close();

	int wcount = 0;//количество символов
	for (const auto& pair : wlist)
	{
		wcount += pair.second;
	}


	vector<int> wordlist;//уникальные символы

	for (const auto& pair : wlist)
	{
		wordlist.push_back(pair.first);
	}

	ifs.open(file_name, ios::binary);
	if (!ifs.is_open())
	{
		cerr << "File if not open - " << file_name << endl;
		return;
	}


	//верхняя и нижняя граница
	uint up = numeric_limits<uint>::max();//4 294 967 295
	uint down = 0;


	vector<char> txt;//символы из файла для обработки
	while (ifs.get(sim))
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

	//преобразовываем к типу ushirt
	ushort len = static_cast<ushort>(wlist.size());

	//перобразовываем указатель потому что write надо const char*
	//и размер того что мы преобразовываем в данном случае 2 байта будет
	compressed_file.write(reinterpret_cast<const char*>(&len), sizeof(len));

	for (const auto& pair : wlist)
	{
		//перобразовываем указатель потому что write надо const char*
		compressed_file.write(reinterpret_cast<const char*>(&pair.first), sizeof(pair.first));

		ushort freq = static_cast<ushort>(pair.second);
		compressed_file.write(reinterpret_cast<const char*>(&freq), sizeof(freq));
	}

	for (char word : txt)
	{
		int ix = get_index(static_cast<int>(word), wordlist);

		if (ix == -1)
		{
			cerr << "Error word is not found" << endl;
			return;
		}

		uint range = up - down + 1;
		uint cum_count = 0;

		for (int i = 0; i < ix; ++i)
		{
			cum_count += wlist[wordlist[i]];
		}

		up = down + (range * (cum_count + wlist[word])) / wcount - 1;
		down = down + (range * cum_count) / wcount;

	}

	compressed_file.write(reinterpret_cast<const char*>(&up), sizeof(up));

	compressed_file.close();


	cout << "File compressed in " + file_name + ".compressed" << endl;

}

void decompress(const string& file_name)
{
	ifstream compressed_file(file_name + ".compressed", ios::binary);
	if (!compressed_file.is_open())
	{
		cerr << "File is not open - " << file_name << endl;
		return;
	}

	//читаем таблицу частот символ - частота
	ushort len;
	compressed_file.read(reinterpret_cast<char*>(&len), sizeof(len));

	int len_int = static_cast<int>(len);

	map<int, int> wlist;

	for (int i = 0; i < len_int; ++i)
	{
		int word;
		compressed_file.read(reinterpret_cast<char*>(&word), sizeof(word));
		ushort freq;
		compressed_file.read(reinterpret_cast<char*>(&freq), sizeof(freq));

		wlist[word] = static_cast<int>(freq);
	}

	//xbnftv c;fnsq rjl
	uint compressed_code;
	compressed_file.read(reinterpret_cast<char*>(&compressed_code), sizeof(compressed_code));
	compressed_file.close();

	int wcount = 0;
	for (const auto& pair : wlist)
	{
		wcount += pair.second;
	}

	//список символов
	vector<int> wordlist;
	for (const auto& pair : wlist)
	{
		wordlist.push_back(pair.first);
	}

	uint up = numeric_limits<uint>::max();
	uint down = 0;

	string d_txt;

	for (int i = 0; i < wcount; ++i)
	{
		uint range = up - down + 1;
		int sim_found = -1;

		// ищем символ для интервала
		for (int j = 0; j < wordlist.size(); ++j)
		{
			uint cum_count = 0;
			for (int k = 0; k < j; ++k)
			{
				cum_count += wlist[wordlist[k]];
			}

			//границы символа
			uint sim_up = down + (range * (cum_count + wlist[wordlist[j]])) / wcount - 1;
			uint sim_down = down + (range * cum_count) / wcount;

			if (compressed_code >= sim_down && compressed_code <= sim_up)
			{
				sim_found = wordlist[j];
				up = sim_up;
				down = sim_down;
				break;
			}
		}

		if (sim_found == -1)
		{
			cerr << "Error sim is not decoded" << endl;
			return;
		}

		d_txt += static_cast<char>(sim_found);
	}

	string output_name = file_name + ".decompressed";
	ofstream out(output_name);
	if (!out.is_open())
	{
		cerr << "File is not open - " << output_name << endl;
		return;
	}

	out.write(d_txt.c_str(), d_txt.length());
	out.close();

	cout << "File decompressed in " + file_name + ".decompressed" << endl;
}

void compress_decompress(const string& file_name)
{
	compress(file_name);
	decompress(file_name);
}

void ariffmetic(const string& file_name, int num)
{
	if (num == 1)
		compress(file_name);

	else if (num == 2)
		decompress(file_name);

	else if (num == 3)
		compress_decompress(file_name);

	else
	{
		cout << "Error" << endl;
		return;
	}
}

int main()
{
	string file_name = "input.txt";

	int num;

	cout << "Chose what you need - " << endl;
	cout << "1 - compressed\n 2 - decompress\n 3 - compress and decompress " << endl;
	cin >> num;

	ariffmetic(file_name, num);


	return 0;
}
