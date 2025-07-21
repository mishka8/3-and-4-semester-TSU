#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <cstdint>
#include <limits>

using namespace std;

using u32 = uint32_t;
using u64 = uint64_t;

constexpr int SYMBOLS = 257; // 256 + EOF
constexpr u32 MAX_CODE = 0xFFFFFFFF;
constexpr u32 ONE_HALF = (MAX_CODE >> 1) + 1;
constexpr u32 ONE_FOURTH = ONE_HALF >> 1;
constexpr u32 THREE_FOURTHS = ONE_HALF + ONE_FOURTH;

// Таблица частот
struct FreqTable 
{
    array<u32, SYMBOLS> freq {};
    array<u32, SYMBOLS + 1> cum {};

    void build(const vector<int>& data) 
    {
        for (int c : data)
            freq[c]++;

        const u32 MAX_TOTAL = 32768;
        for (int i = 0; i < SYMBOLS; ++i)
        {
            if (freq[i] == 0)
                freq[i] = 1;
        }

        u64 sum = 0;

        for (int i = 0; i < SYMBOLS; ++i) 
            sum += freq[i];

        if (sum > MAX_TOTAL) 
        {
            for (int i = 0; i < SYMBOLS; ++i) 
            {
                freq[i] = (freq[i] * MAX_TOTAL) / sum;

                if (freq[i] == 0) 
                    freq[i] = 1;
            }
        }

        cum[0] = 0;
        for (int i = 1; i <= SYMBOLS; ++i)
            cum[i] = cum[i - 1] + freq[i - 1];
    }

    void write(ofstream& out) const 
    {
        for (int i = 0; i < SYMBOLS; ++i)
            out.write(reinterpret_cast<const char*>(&freq[i]), sizeof(u32));
    }

    void read(ifstream& in) 
    {
        for (int i = 0; i < SYMBOLS; ++i)
            in.read(reinterpret_cast<char*>(&freq[i]), sizeof(u32));
        cum[0] = 0;
        for (int i = 1; i <= SYMBOLS; ++i)
            cum[i] = cum[i - 1] + freq[i - 1];
    }

    u32 total() const 
    { 
        return cum[SYMBOLS]; 
    }

    void get_range(int sym, u32& low, u32& high) const 
    {
        low = cum[sym];
        high = cum[sym + 1];
    }

    int get_symbol(u32 value, u32& low, u32& high) const 
    {
        for (int i = 0; i < SYMBOLS; ++i) 
        {
            if (cum[i + 1] > value) 
            {
                low = cum[i];
                high = cum[i + 1];
                return i;
            }
        }
        return -1;
    }
};

int read_bit(ifstream& in, int& buffer, int& bits_left) 
{
    if (bits_left == 0) 
    {
        buffer = in.get();

        if (buffer == EOF) 
            return 0;

        bits_left = 8;
    }

    int bit = (buffer >> 7) & 1;
    buffer <<= 1;
    bits_left--;

    return bit;
}

void write_bit(ofstream& out, int& buffer, int& bits_filled, int bit) 
{
    buffer = (buffer << 1) | bit;
    bits_filled++;
    if (bits_filled == 8) 
    {
        out.put(static_cast<char>(buffer));
        buffer = 0;
        bits_filled = 0;
    }
}

void flush_bit_buffer(ofstream& out, int buffer, int bits_filled) 
{
    if (bits_filled > 0) 
    {
        buffer <<= (8 - bits_filled);
        out.put(static_cast<char>(buffer));
    }
}

void compress(const string& in_file, const string& out_file) 
{
    ifstream in(in_file, ios::binary);
    ofstream out(out_file, ios::binary);
    vector<int> data;
    char ch;
    while (in.get(ch)) 
        data.push_back(static_cast<unsigned char>(ch));

    data.push_back(256); // EOF

    FreqTable table;
    table.build(data);
    table.write(out);

    u32 low = 0, high = MAX_CODE;
    int bits_to_follow = 0, buffer = 0, bits_filled = 0;

    auto output_bit = [&](int bit) 
    {
    write_bit(out, buffer, bits_filled, bit);
    while (bits_to_follow-- > 0)
        write_bit(out, buffer, bits_filled, !bit);
    bits_to_follow = 0;
    };

    for (int sym : data) 
    {
        u32 sym_low, sym_high;
        table.get_range(sym, sym_low, sym_high);
        u64 range = (u64)high - low + 1;

        high = low + (range * sym_high) / table.total() - 1;
        low = low + (range * sym_low) / table.total();

        while (true) 
        {
            if (high < ONE_HALF) 
            {
                output_bit(0);
                low <<= 1; high = (high << 1) + 1;
            }
            else if (low >= ONE_HALF) 
            {
                output_bit(1);
                low = (low - ONE_HALF) << 1;
                high = ((high - ONE_HALF) << 1) + 1;
            }
            else if (low >= ONE_FOURTH && high < THREE_FOURTHS) 
            {
                bits_to_follow++;
                low = (low - ONE_FOURTH) << 1;
                high = ((high - ONE_FOURTH) << 1) + 1;
            }
            else 
                break;
        }
    }
    bits_to_follow++;
    output_bit(low < ONE_FOURTH ? 0 : 1);
    flush_bit_buffer(out, buffer, bits_filled);
}

void decompress(const string& in_file, const string& out_file) 
{
    ifstream in(in_file, ios::binary);
    ofstream out(out_file, ios::binary);

    FreqTable table;
    table.read(in);

    u32 low = 0, high = MAX_CODE, value = 0;
    int buffer = 0, bits_left = 0;

    for (int i = 0; i < 32; ++i)
        value = (value << 1) | read_bit(in, buffer, bits_left);

    while (true) 
    {
        u64 range = (u64)high - low + 1;
        u32 scaled = ((u64)(value - low + 1) * table.total() - 1) / range;

        u32 sym_low, sym_high;
        int sym = table.get_symbol(scaled, sym_low, sym_high);
        if (sym == 256) break;
        out.put(static_cast<char>(sym));

        high = low + (range * sym_high) / table.total() - 1;
        low = low + (range * sym_low) / table.total();

        while (true) 
        {
            if (high < ONE_HALF) {}

            else if (low >= ONE_HALF) 
            {
                value -= ONE_HALF; low -= ONE_HALF; high -= ONE_HALF;
            }
            else if (low >= ONE_FOURTH && high < THREE_FOURTHS) 
            {
                value -= ONE_FOURTH; low -= ONE_FOURTH; high -= ONE_FOURTH;
            }
            else break;
            low <<= 1; high = (high << 1) + 1;
            value = (value << 1) | read_bit(in, buffer, bits_left);
        }
    }
}

int main() 
{
    string input = "input.txt";
    compress(input, "compressed.bin");
    decompress("compressed.bin", "input_decompress.txt");
    return 0;
}
