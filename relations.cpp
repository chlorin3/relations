#include <iostream>
#include <fstream>
#include <limits>

using namespace std;

/*
    w pliku relacje.txt każda linia tekstu zawiera imię pierwszej osoby, 
    imię drugiej osoby i lata jak długo się znają
*/
struct relations
{
    string first_person;
    string second_person;
    int T; 
} relation;

void append(string name) // funkcja wpisująca slowo do pliku txt
{
    ofstream output;
    output.open("names.txt", ios::out | ios::app);
    if (!output)
        cerr << "Nieudane otwarcie pliku names.txt z funkcji append";
    else
    {
        output << name << endl;
        output.close();
    }
}

bool found(string name) // funkcja szukająca duplikatów słów
{
    ifstream input;
    input.open("names.txt", ios::in);
    if (!input)
        cerr << "Nieudane otwarcie pliku names.txt z funkcji bool";
    else
    {
        string line;
        while (input >> line)
        {
            if (line == name)
            {
                input.close(); 
                return true; // znaleziono duplikat
            }
        }
        input.close();
        return false; // nie ma duplikatów
    }
}

/*
funkcja ktorą tworzmy plik only_letters.txt do którego wpisujemy tylko litery
z innego pliku tekstowego
po napotkaniu spacji przechodzi do nowej linii
*/
void only_letters(ifstream &in, int &lines) 
{
    fstream fs;
    fs.open("only_letters.txt", ios::out);
    if (!fs)
        cerr << "Nieudane otwarcie pliku only_letters";
    else
    {
        char c;
        while (in.get(c))
        {
            if (isdigit(c))
                in.ignore(numeric_limits<streamsize>::max(), '\n');
            else
            {
                if (!isspace(c))
                    fs << c;
                else
                {
                    fs << "\n";
                    lines++;
                }
            }
        }
        fs.close();
    }
    in.close();
}

void removing_duplicates(int &lines) //
{
    ifstream in;
    in.open("only_letters.txt", ios::in);
    if (!in)
        cerr << "Nieudane otwarcie pliku only_letters.txt";
    else
    {
        string name, name2;
        int i = 0;

        while (in >> name && i <= lines)
        {
            if (i == 0)
                append(name);
            else if (!found(name))
                  append(name);
            i++;
        }
        in.close();
    }
}

int main()
{
    ifstream input;
    ofstream output;
    fstream fsfile;

    input.open("relacje.txt", ios::in);
    if (!input)
    {
        cerr << "Nieudane otwarcie pliku relacje!";
        return (1);
    }

    int lines = 0;
    only_letters(input, lines); //powstanie plik tekstowy tylko z imionami w oddzielnych liniach
    removing_duplicates(lines); //powstanie plik z imionami, ale bez duplikatów imion
    /* 
       z pliku names.txt będą kolejno pobierane imiona 
       oraz poszukiwane będą relacje między osobami 
                                                                */
    output.open("people_and_their_friends.txt", ios::out | ios::app);

    fsfile.open("names.txt", ios::in);
    if (!fsfile)
    {
        cerr << "Nieudane otwarcie pliku names.txt 2 czesc!";
        return (1);
    }

    string name;

    while (fsfile >> name) //pobieranie imion z pliku names.txt
    {
        output << name << " "; //pierwsze imie osoby dla ktorej szukamy relacji wpisujemy do pliku people_and_their_friends.txt
        input.open("relacje.txt", ios::in);
        if (!input)
        {
            cerr << "Nieudane otwarcie pliku relacje.txt";
            return (1);
        }
        /*
            przeglądanie całego pliku relacje.txt w celu znalezienia wszystkich znajomych
            dla imienia pobranego z pliku names.txt
         */
        while (input >> relation.first_person >> relation.second_person >> relation.T)
        {
            if (name == relation.first_person)
                output << relation.second_person << " ";
            else if (name == relation.second_person)
                output << relation.first_person << " ";
        }
        output << "\n";
        input.close();
    }
    output.close();
    fsfile.close();
    return 0;
}
