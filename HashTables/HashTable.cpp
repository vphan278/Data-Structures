#include <iostream>
#include <vector>
#include <string>
#include <cmath>            // for PI if you want, optional
//#include "ConsoleColor.h"
#ifndef CONSOLECOLOR_H
#define CONSOLECOLOR_H

#include <ostream>

// Minimal color tokens so code compiles with: cout << yellow << "text";
inline std::ostream& yellow(std::ostream& os)   { return os; }
inline std::ostream& cyan(std::ostream& os)     { return os; }
inline std::ostream& white(std::ostream& os)    { return os; }
inline std::ostream& lavender(std::ostream& os) { return os; }
inline std::ostream& green(std::ostream& os)    { return os; }
inline std::ostream& red(std::ostream& os)      { return os; }
inline std::ostream& grey(std::ostream& os)     { return os; }
inline std::ostream& orange(std::ostream& os)   { return os; }

#endif


using namespace std;

const double PI = 3.14159;
const int TABLE_SIZE = 100;

vector<string> elements =
{
    "Aluminium", "Americium", "Argon", "Arsenic", "Astatine",
    "Barium", "Berkelium", "Beryllium", "Bismuth", "Bohrium",
    "Boron", "Bromine", "Cadmium", "Caesium", "Calcium",
    "Californium", "Carbon", "Cerium", "Chlorine", "Chromium",
    "Cobalt", "Copernicium", "Copper", "Curium", "Darmstadtium",
    "Dubnium", "Dysprosium", "Einsteinium", "Erbium", "Europium",
    "Fermium", "Flerovium", "Fluorine", "Francium", "Gadolinium",
    "Gallium", "Germanium", "Gold", "Hafnium", "Hassium",
    "Helium", "Holmium", "Hydrogen", "Indium", "Iodine",
    "Iridium", "Iron", "Krypton", "Lanthanum", "Lawrencium",
    "Lead", "Lithium", "Livermorium", "Lutetium", "Magnesium",
    "Manganese", "Meitnerium", "Mendelevium", "Mercury", "Molybdenum",
    "Moscovium", "Neodymium", "Neon", "Neptunium", "Nickel",
    "Nihonium", "Niobium", "Nitrogen", "Nobelium", "Oganesson",
    "Osmium", "Oxygen", "Palladium", "Phosphorus", "Platinum",
    "Plutonium", "Polonium", "Potassium", "Praseodymium", "Promethium",
    "Protactinium", "Radium", "Radon", "Rhenium", "Rhodium",
    "Roentgenium", "Rubidium", "Ruthenium", "Rutherfordium", "Samarium",
    "Scandium", "Seaborgium", "Selenium", "Silicon", "Silver",
    "Sodium", "Strontium", "Sulfur", "Tantalum", "Technetium",
    "Tellurium", "Tennessine", "Terbium", "Thallium", "Thorium",
    "Thulium", "Tin", "Titanium", "Tungsten", "Uranium",
    "Vanadium", "Xenon", "Ytterbium", "Yttrium", "Zinc", "Zirconium"
};

// sample hashing function
int hashFunction(const string& skey)
{
    int hash = 0;
    for (char c : skey)
        hash += static_cast<int>(c * PI);   // keep it int

    return hash % TABLE_SIZE;
}

class HashTableEntry
{
    int hkey;
    string value;

public:
    explicit HashTableEntry(const string& str)
        : hkey(hashFunction(str)), value(str)
    {}

    int getkey() const { return hkey; }
    const string& getvalue() const { return value; }

    friend ostream& operator<<(ostream& os, const HashTableEntry& hentry)
    {
        os << hentry.value;
        return os;
    }
};

class HashTable
{
    vector<vector<HashTableEntry>> table;
    int collisions;
    int unused;

public:
    HashTable()
        : table(TABLE_SIZE), collisions(0), unused(0)
    {}

    void insert(const string& str)
    {
        int hash = hashFunction(str);
        table[hash].emplace_back(str);
    }

    // FIXED: compare actual value, not just hash
    string search(const string& str) const
    {
        int index = hashFunction(str);

        for (const auto& entry : table[index])
        {
            if (entry.getvalue() == str)
                return entry.getvalue();
        }
        return "---";
    }

    void output()
    {
        // reset counters each time output runs
        collisions = 0;
        unused = 0;

        for (int i = 0; i < (int)table.size(); i++)
        {
            cout << white << i << ".\t";

            if (!table[i].empty())
            {
                // FIXED collision counting: bucket_size - 1
                if (table[i].size() > 1)
                    collisions += (int)table[i].size() - 1;

                for (int j = 0; j < (int)table[i].size(); j++)
                {
                    if (table[i].size() > 1)
                        cout << red << table[i][j] << "\t" << white;
                    else
                        cout << green << table[i][j] << " " << white;
                }
            }
            else
            {
                unused++;
                cout << yellow << "---" << white;
            }

            cout << endl;
        }
    }

    void statistics() const
    {
        cout << white;
        cout << "\nStatistics\n";
        cout << "Elements Size:\t\t\t" << elements.size() << endl;
        cout << "HashTable Size:\t\t\t" << TABLE_SIZE << endl;
        cout << "Collisions:\t\t\t" << collisions << endl;
        cout << "Unused Positions:\t\t" << unused << endl;

        // "Efficiency" label is a bit misleading; this is collision rate %
        cout << "Collision Rate:\t\t\t"
             << int((float)collisions / (float)TABLE_SIZE * 100) << "%\n";
    }
};

int main()
{
    HashTable hashTable;

    for (int i = 0; i < (int)elements.size(); i++)
        hashTable.insert(elements[i]);

    hashTable.output();
    hashTable.statistics();

    return 0;
}