/*
Lukasz Tkacz
*/


#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <cstdlib>
#include <windows.h>
using namespace std;


/*
* Klasy
*/
class Contener;
class Address;
class Date;
class Database;
class Worker;
class View;
class ViewConsole;
class ViewHTML;


/*
* Funkcje
*/
int SetRecordID();
void PrintMessage(string, int);
void SearchAction(char);
void SearchOptions();


/*
* Komunikaty
*/
enum {MSG_ERROR = -1, MSG_CONFIRM = 0, MSG_SUCCESS = 1};


/*
* Zmienne
*/
extern Database DB;
HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);;

/******************************************************/



class Address
{
	protected:
		char street[255];
		char city[255];
		char zip_code[20];	
		
	public:	
		char const * getStreet() const
		{
			return street;
		}
		
		char const * getCity() const
		{
			return city;
		}
		
		char const * getZipCode() const
		{
			return zip_code;
		}
		
		void setAddress();
		void setFromFile(fstream &);
};


void Address::setFromFile(fstream & File)
{
	File >> street;
	File >> city;
	File >> zip_code;
}


void Address::setAddress()
{
	char input[255];

    do
    {
        cout << "Adres (ulica): "; 
        cin >> street;
        cin.sync();
    }
    while (strlen(street) == 0);
    
    do
    {
        cout << "Adres (miasto): "; 
        cin.getline(city, 255);
        cin.sync();
    }
    while (strlen(city) == 0);
    
    while (1)
    {
        cout << "Kod pocztowy: "; 
        cin >> zip_code;
        cin.sync();

		if (strlen(zip_code) == 6 
			&& isdigit(zip_code[0])
			&& isdigit(zip_code[1])
			&& isdigit(zip_code[3])
			&& isdigit(zip_code[4])
			&& isdigit(zip_code[5])
			&& zip_code[2] == '-')
		{	
			break;	
		}
		PrintMessage("Podany kod pocztowy ma nieprawidlowy format!", MSG_ERROR);	
    }		
}
/******************************************************/



class Date
{
	protected:
		int day;
		int month;
		int year;	
	
	protected:
		bool ValidateDate() const;
		
	public:	
		int getDay() const
		{
			return day;
		}
		
		int getMonth() const
		{
			return month;
		}
		
		int getYear() const
		{
			return year;
		}
		
		void setDate();
		void setFromFile(fstream &);
};


bool Date::ValidateDate() const
{
	bool leap_year = false;
  
	if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
	{
		leap_year = true;
	}

	if ((month == 1 && day < 32)
		|| (month == 2 && (day < 29 && !leap_year) || (day < 30 && leap_year))
		|| (month == 3 && day < 32)
		|| (month == 4 && day < 31)
		|| (month == 5 && day < 32)
		|| (month == 6 && day < 31)
		|| (month == 7 && day < 32)
		|| (month == 8 && day < 32)
		|| (month == 9 && day < 31)
		|| (month == 10 && day < 32)
		|| (month == 11 && day < 31)
		|| (month == 12 && day < 32))
	{
		return true;
	}

	return false;
}


void Date::setDate()
{
	char input[255];

	while(1)
	{
		cout << "Rok urodzenia: "; 
		cin >> input;
		year = atoi(input);
		cin.sync();

		
		if (year >= 1950 && year <= 1995)
		{
			break;
		}
		PrintMessage("Podaj rok z zakresu 1950-1995!", MSG_ERROR);
	}
  
	while(1)
	{
		cout << "Miesiac urodzenia: "; 
		cin >> input;
		month = atoi(input);
		cin.sync();
		
		if (month >= 1 && month <= 12)
		{
			break;
		}
		PrintMessage("Podaj miesiac z zakresu 1-12!", MSG_ERROR);
	}
	
	while(1)
	{
		cout << "Dzien urodzenia: "; 
		cin >> input;
		day = atoi(input);
		cin.sync();
		
		if (day >= 1 && day <= 31 && ValidateDate())
		{
			break;
		}
		PrintMessage("Podany dzien nie jest w zakresie 1-31, lub jest nieodpowiedni dla tego roku i miesiaca!", MSG_ERROR);
	}			
}


void Date::setFromFile(fstream & File)
{
	File >> day;
	File >> month;
	File >> year;
}
/******************************************************/



class Worker
{
	protected:
		char name[255];
		char surname[255];
		Address address;
		Date birth_date;
		double salary;	
		
	public:	
		char const * getName() const
		{
			return name;
		}
		
		char const * getSurname() const
		{
			return surname;
		}
		
		Address getAddress() const
		{
			return address;
		}
		
		Date getBirthDate() const
		{
			return birth_date;
		}
		
		double getSalary() const
		{
			return salary;
		}
		
		void setName();
		void setSurname();
		void setSalary();	

		void setBirthDate(Date new_birth_date)
		{
			birth_date = new_birth_date;	
		}
		
		void setAddress(Address new_address)
		{
			address = new_address;	
		}
		
		void setFromFile(fstream &);
};


void Worker::setName()
{
	int i;
    
    cout << "Imie: "; 
    cin.sync();
    cin.getline(name, sizeof(name));

    name[0] = toupper(name[0]);
    for (i = 1; name[i] != 0; i++)
    {
        name[i] = tolower(name[i]);
    }
	
	cin.sync();			
}


void Worker::setSurname()
{
	int i;
    
    cout << "Nazwisko: "; 
    cin.sync();
    cin.getline(surname, sizeof(surname));

    surname[0] = toupper(surname[0]);
    for (i = 1; surname[i] != 0; i++)
    {
        surname[i] = tolower(surname[i]);
    }
    
	cin.sync();		
}


void Worker::setSalary()
{
	char input[255];
	
    while (1)
    {
        cout << "Wynagrodzenie (z zakresu 1000-4000): "; 
        cin.sync();
        cin >> input;
        salary = atoi(input);
    	cin.sync();
        
        if (salary >= 1000 && salary <= 4000)
        {
            break;    
        }
        PrintMessage("Podaj wynagrodzenie z zakresu 1000-4000!", MSG_ERROR);
    }	
}	


void Worker::setFromFile(fstream & File)
{
	File >> name;
	File >> surname;
	birth_date.setFromFile(File);
	address.setFromFile(File);
	File >> salary;	
}

/******************************************************/



class Contener
{
	protected:
		int data_size;
		int data_size_max;
		Worker * data;
		
	protected:
		bool reallocate();
    
	public:
		void deleteRecord();
		bool addRecord(Worker &);
		
		~Contener()
		{
			if(data != NULL) delete [] data;
		}
		
		Worker & operator[] (int id)
		{
			if (data != NULL && id >= 0 && id < data_size)
			{
				return data[id];
			}
			else
			{
				PrintMessage("Wystapil nieznany blad!", MSG_ERROR);
				exit(1);
			}
		}
		
		int getDataSize()
		{
			return data_size;
		}
};


bool Contener::reallocate()
{
	data_size_max += 5;
	Worker * DBTemp = new Worker[data_size_max];
	
	if (DBTemp == NULL)
	{
		return false;	
	}
	
	for (int i = 0; i < data_size; i++)
	{
		DBTemp[i] = data[i];
	}
	delete [] data;
    
	data = DBTemp;
	return true;
}	


void Contener::deleteRecord()
{
	int index_id;
    
    if (data_size < 1)
    {
        PrintMessage("DB jest pusta, nie ma co usuwac!", MSG_ERROR); 
        return;
    }
    
    for (index_id = SetRecordID(); index_id < data_size; index_id++)
    {
        data[index_id] = data[index_id + 1];
    }

    data_size--;
    PrintMessage("Wskazany rekord zostal pomyslnie usuniety!", MSG_SUCCESS);
}


bool Contener::addRecord(Worker & new_record)
{
    int index_new = 0, index = 0, i = 0;

	// Realokacja bazy jezeli potrzeba miejsca
	if (data_size == data_size_max)
	{
		if(!reallocate()) return false;
	}

	// Szukanie miejsca dla nowego rekordu (alfabetycznie)
	index_new = data_size;
	for (i = 0; i < data_size; i++)
	{
		if (strcmp(new_record.getSurname(), data[i].getSurname()) < 0)
		{
			index_new = i;
			break;
		} 
	} 

	// Zmiana kolejnosci rekordow
    for (index = data_size - 1; index >= index_new; index--)
    {
        data[index + 1] = data[index];
    }
    data[index_new] = new_record; 
    data_size++; 
    
    return true; 
}
/******************************************************/



class Database : public Contener
{
	protected:
		char filename[255];
		
	protected:
		void setRecord(Worker &);
		void setData();  
    
	public:
		void getData();
		void setFile(const char []);
		void addRecord();
		
		Database(const char new_file[] = "BAZA.TXT")
		{
			setFile(new_file);	
			getData();	
		}
		
		~Database()
		{
			setData();
		}
};


void Database::getData()
{
	fstream File;
	
	File.open(filename, ios::in);
	if (!File.good())
	{
		PrintMessage("BLAD! Odczyt bazy z pliku niemozliwy!", MSG_ERROR); 
		return;
	}     
   
	File >> data_size;
	data_size_max = data_size + 5;
	data = new Worker[data_size_max];
	
	for (int i = 0; i < data_size; i++)
	{
		data[i].setFromFile(File);
	}
  
	File.close();
}


void Database::setData()
{
	fstream File;
	
	File.open(filename, ios::out);
	if (!File.good())
	{
		PrintMessage("BLAD! Zapis do pliku niemozliwy!", MSG_ERROR);
		return;
	} 
	
	File << data_size << endl;
	for (int i = 0; i < data_size; i++)
	{
		File << data[i].getName() << endl; 
		File << data[i].getSurname() << endl; 
		File << data[i].getBirthDate().getDay() << endl; 
		File << data[i].getBirthDate().getMonth() << endl; 
		File << data[i].getBirthDate().getYear() << endl; 
		File << data[i].getAddress().getStreet() << endl; 
		File << data[i].getAddress().getCity() << endl; 
		File << data[i].getAddress().getZipCode() << endl; 
		File << data[i].getSalary() << endl; 
	}   
	
	File.close();
}


void Database::setFile(const char new_file[])
{
	strcpy(filename, new_file);
}


void Database::addRecord()
{
    char command = 't';
    Worker Record;

	do
	{
		Record.setName();
		Record.setSurname();
	
	    Date RecordBirthDate;
	    RecordBirthDate.setDate();
	    Record.setBirthDate(RecordBirthDate);
	    
	    Address RecordAddress;
	    RecordAddress.setAddress();
	    Record.setAddress(RecordAddress);
	    Record.setSalary();
	    
	    // Add to database
		if (Contener::addRecord(Record))
		{
			PrintMessage("Nowy rekord zostal pomyslnie dodany!", MSG_SUCCESS); 	
		}
		else
		{
			PrintMessage("Wystapil nieoczekiwany blad!", MSG_ERROR);	
		}

		// Commands   
        do 
        {
            PrintMessage("Czy chcesz dodac kolejny rekord? [T/N]: ", MSG_CONFIRM);
            cin >> command;
        
            if (command == 'n' || command == 't')
            {
                break; 
            }
            PrintMessage("Niepoprawne polecenie", MSG_ERROR);
        } 
        while(1); 

    }
    while (command != 'n'); 
}
/******************************************************/



class ViewConsole
{
	public:
		void printRecord(int &, bool) const;
		void printTable() const;
		void printTable(int [], int) const;
		
	protected:
		void printTableHeader(int) const;
};


void ViewConsole::printRecord(int & id, bool table_format = true) const
{
	const Worker & Record = DB[id];
	
	if (table_format)
	{
        cout << "| " << setw(2) << id << " | ";
        cout << setw(10) << Record.getName() <<    " | ";
        cout << setw(10) << Record.getSurname() <<    " | ";
        cout << setw(4) << Record.getBirthDate().getYear();
                
        if (Record.getBirthDate().getMonth() < 10)
        {
            cout << "-" << "0" << Record.getBirthDate().getMonth();        
        }
        else
        {
            cout << "-" << setw(2) << Record.getBirthDate().getMonth(); 
        }
        if (Record.getBirthDate().getDay() < 10)
        {
            cout << "-" << "0" << Record.getBirthDate().getDay() << " |";        
        }
        else
        {
            cout << "-" << setw(2) << Record.getBirthDate().getDay() << " |"; 
        }

        cout << setw(12) << Record.getAddress().getStreet() <<    " | "; 
        cout << setw(10) << Record.getAddress().getCity() <<    " | ";
        cout << setw(10) << Record.getAddress().getZipCode() << " | ";
        cout << setw(5) << Record.getSalary() << " |\n"; 
	}
	else
	{
		cout << "Dane z indeksu: " << id << "\n";
		cout << "Imie: " << Record.getName() << "\n";
		cout << "Nazwisko: " << Record.getSurname() << "\n";
		cout << "Data urodzenia: " << Record.getBirthDate().getYear();    
		cout << "-" << Record.getBirthDate().getMonth();
		cout << "-" << Record.getBirthDate().getDay() << "\n";
		cout << "Adres: " << Record.getAddress().getStreet(); 
		cout << ", " << Record.getAddress().getZipCode(); 
		cout << " " << Record.getAddress().getCity() << "\n";
		cout << "Wynagrodzenie: " << Record.getSalary() << "\n"; 	
	}	
}


void ViewConsole::printTableHeader(int num_records = DB.getDataSize()) const
{
	cout << "\nZnazlezionych rekordow: " << num_records << "\n"; 
	
    SetConsoleTextAttribute(hStdout, 7 | FOREGROUND_INTENSITY);
    cout << "| " << setw(2) << "ID";
    cout << " | " << setw(10) << "Imie"; 
    cout << " | " << setw(10) << "Nazwisko"; 
    cout << " | " << setw(10) << "Data ur.";
    cout << " | " << setw(11) << "Ulica";
    cout << " | " << setw(10) << "Miasto";
    cout << " | " << setw(10) << "Kod";
    cout << " | " << setw(5) << "Placa |\n";
    cout << "*********************************************************************************************\n";	
    SetConsoleTextAttribute(hStdout, 7);
}


void ViewConsole::printTable() const
{
	printTableHeader();
     
    for (int i = 0; i < DB.getDataSize(); i++)
    {
        if (i % 2 == 1)
        {
            SetConsoleTextAttribute(hStdout, 2 | FOREGROUND_INTENSITY);
        }
        else
        {
            SetConsoleTextAttribute(hStdout, 3 | FOREGROUND_INTENSITY);
        }
        printRecord(i);    
    }
    SetConsoleTextAttribute(hStdout, 7);
}


void ViewConsole::printTable(int records[], int num_records) const
{
	printTableHeader();
     
    for (int i = 0; i < num_records; i++)
    {
        if (i % 2 == 1)
        {
            SetConsoleTextAttribute(hStdout, 2 | FOREGROUND_INTENSITY);
        }
        else
        {
            SetConsoleTextAttribute(hStdout, 3 | FOREGROUND_INTENSITY);
        }
        printRecord(records[i]);    
    }
    SetConsoleTextAttribute(hStdout, 7);
}
/******************************************************/



class ViewHTML
{
	public:
		void printRecord(int &, bool);
		void printTable(string description);
		void printTable(int [], int);
		void htmlHeader();
		void outputHTML();
	
		ViewHTML(const char new_file[] = "Raport.html")
		{
			setFile(new_file);	
		    File.open(filename, ios::out);
		    if (!File.good())
		    {
		        PrintMessage("BLAD! Zapis do pliku niemozliwy!", MSG_ERROR);
		        return;
		    } 	
		}
		
		~ViewHTML()
		{
		}

	protected:
		char filename[255];
		fstream File;

	protected:
		void setFile(const char []);
		void printTableHeader(int);	
};


void ViewHTML::printRecord(int & id, bool table_format = true)
{
	const Worker & Record = DB[id];
        
	if (table_format)
	{
	    File << "\t\t<td>" << Record.getName() << "</td>\n";
        File << "\t\t<td>" << Record.getSurname() << "</td>\n";
        File << "\t\t<td>" << Record.getBirthDate().getDay();
        File << "-" << Record.getBirthDate().getMonth();
        File << "-" << Record.getBirthDate().getYear() << "</td>\n";
        File << "\t\t<td>" << Record.getAddress().getStreet();
        File << "<br />" << Record.getAddress().getZipCode();
        File << ", " << Record.getAddress().getCity() << "</td>\n";
        File << "<td>" << Record.getSalary() << "</td>\n";
	}
	else 
	{
		File << "<ul><li><strong style=\"text-shadow: #000 1px 1px 5px\">Dane dla rekordu nr " << id << ":</strong><ul>\n";
		File << "<li>Imie: " << Record.getName() << "</li>\n";
		File << "<li>Nazwisko: " << Record.getSurname() << "</li>\n";
		File << "<li>Data urodzenia: " << Record.getBirthDate().getDay();
		File << "-" << Record.getBirthDate().getMonth();
		File << "-" << Record.getBirthDate().getYear() << "</li>\n";
		File << "<li>Adres zamieszkania: " << Record.getAddress().getStreet();
		File << "<br />" << Record.getAddress().getZipCode();
		File << "," << Record.getAddress().getCity() << "</li>\n";  
		File << "<li>Wynagrodzenie: " << Record.getSalary() << "</li>\n";
		File << "</ul></li></ul><br />\n";	
	}
}


void ViewHTML::printTableHeader(int num_records = DB.getDataSize())
{
	File << "<h3>Liczba rekordow: " << num_records << "</h3>\n";
    File << "<table>\n";
    File << "<tr>\n";
    File << "<th>Imie</th>\n";
    File << "<th>Nazwisko</th>\n";
    File << "<th>Data urodzenia</th>\n";
    File << "<th>Adres</th>\n";
    File << "<th>Placa</th>\n";
    File << "</tr>\n";
}


void ViewHTML::printTable(string description = "")
{
    File << "<h1>Spis rekordow z bazy danych</h1>\n";
    
    if (!description.empty())
    {
        File << "<h2>" << description << "</h2>\n"; 
    }
    
    printTableHeader();
    for (int i = 0; i < DB.getDataSize(); i++)
    {
		File << "<tr class=\"" << ((i % 2) ? "row1" : "row2") << "\">\n";
		printRecord(i);
		File << "</tr>\n";
    }

    File << "</table>\n";	
}


void ViewHTML::printTable(int records[], int num_records)
{
    File << "<h1>Spis rekordow z bazy danych</h1>\n";

    printTableHeader(num_records);
    for (int i = 0; i < num_records; i++)
    {
		File << "<tr class=\"" << ((i % 2) ? "row1" : "row2") << "\">\n";
		printRecord(records[i]);
		File << "</tr>\n";
    }

    File << "</table>\n";	
}


void ViewHTML::setFile(const char new_file[])
{
	strcpy(filename, new_file);
}


void ViewHTML::htmlHeader()
{
    File << "<!doctype html>\n";
    File << "\t\t<html lang=\"pl\">\n";
    File << "<head>\n";
    File << "<meta charset=\"UTF-8\">\n";
    File << "<title></title>\n";
    File << "<link rel=\"stylesheet\" type=\"text/css\" href=\"style.css\" />\n";
    File << "</head>\n";
    File << "<body>\n";
}

void ViewHTML::outputHTML()
{
    File << "</body>\n";
    File << "</html>";
    
	File.close();
	system(filename);		
}
/******************************************************/



class View
{
	private:
		View() {};

	public:
		void printRecord(int) const;
		void printTable() const;
		void printTable(int [], int) const;
		void printMenu(string) const;
		void printSearchOptions() const;
		
        static View & getInstance()
        {
          static View instance;
          return instance;
        }
};


void View::printRecord(int index_id = -1) const
{
	index_id = (index_id >= 0) ? index_id : SetRecordID();

	const ViewConsole Console;
	ViewHTML HTML;
	
	Console.printRecord(index_id, false);
	
	HTML.htmlHeader();
	HTML.printRecord(index_id, false);
	HTML.outputHTML();
}


void View::printTable() const
{
	const ViewConsole Console;
	ViewHTML HTML;
	
	Console.printTable();
	
	HTML.htmlHeader();
	HTML.printTable("Wszystkie rekordy");
	HTML.outputHTML();
}


void View::printTable(int records[], int num_records = 0) const
{
	if (num_records == 0)
	{
		PrintMessage("Nic nie znaleziono!", MSG_ERROR);	
	}


	ViewConsole Console;
	ViewHTML HTML;
	
	Console.printTable(records, num_records);
	
	HTML.htmlHeader();
	HTML.printTable(records, num_records);
	HTML.outputHTML();
}


void View::printMenu(string mode = "") const
{
	if (mode == "")
	{
	    cout << "Spis dostepnych polecen:\n";       
	    cout << "d - Drukowanie tablicy z rekordami\n"; 
	    cout << "n - Dodawanie nowego rekordu do bazy\n";
	    cout << "p - Informacje z zadanego rekordu\n";
	    cout << "u - Usuwanie zadanego rekordu\n";
	    cout << "w - Opcje wyszukiwania w bazie\n";
	    cout << "i - Ilosc rekordow w bazie\n";
	    cout << "h - Lista polecen\n";
	    cout << "x - Zakonczenie dzialania\n";

	}
	else if (mode == "search")
	{
	    cout << "Spis dostepnych form wyszukiwania:\n";        
	    cout << "n - Zadana litere nazwiska\n"; 
	    cout << "d - Zadany zakres daty urodzenia\n"; 
	    cout << "w - Minimalne i maksymalne wynagrodzenie\n";
	    cout << "b - Powrot do poprzedniego menu\n";	
	}
}

void View::printSearchOptions() const
{	
    if (DB.getDataSize() < 1)
    {
        PrintMessage("Nie mozna wyszukiwac jezeli w bazie nie ma zadnych rekordow!", MSG_ERROR);
        return;
    }
    
    char command;
    
    do
    {
    	printMenu("search");
			
        cout << "\nPodaj polecenie: ";
		cin.sync();
		cin >> command;
    
        switch (command)
        {
            case 'n':
            case 'd':
            case 'w':
                SearchAction(command);
            break;

            case 'b':
                system("cls");
                printMenu();
                return;
            break;
            
            default:
                PrintMessage("Nieznane polecenie!", MSG_ERROR);
            break;
        }
    }
    while (1);
}


/******************************************************/



/*
* DB variables
*/
Database DB;


/*
* Drukowanie komunikatu
*
*/
void PrintMessage(string Txt, int type)
{
	if (type == MSG_ERROR)
	{
		SetConsoleTextAttribute(hStdout, FOREGROUND_RED | FOREGROUND_INTENSITY);
	}
	else if (type == MSG_SUCCESS)
	{
		SetConsoleTextAttribute(hStdout, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	}
	else if (type == MSG_CONFIRM)
	{
		SetConsoleTextAttribute(hStdout, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	}
	cout << "\n" << Txt << "\a\n";
	SetConsoleTextAttribute(hStdout, 7);
}


/*
* Wczytywanie numeru indeksu
*
*/
int SetRecordID()
{
	int index_id;
	
	while(1)
	{
		cout << "Numer indeksu: "; 
		cin >> index_id;
		
		if (index_id >= 0 && index_id < DB.getDataSize())
		{
			break;
		}
		PrintMessage("Nieprawidlowy numer indeksu - rekord o takim indeksie nie istnieje", MSG_ERROR);
	}
	
	return index_id; 
}  


/*
* Akcje wyszukiwania
*
*/
void SearchAction(char mode)
{
	const View Template = View::getInstance();
	
    switch (mode)
    {
        // Wyszukiwanie po pierwszej literze nazwiska
        case 'n':
        {
            char letter;
            int i = 0, num_records = 0;
            
            int *records;
            records = new int[DB.getDataSize()];
        
            while(1) 
            {
                cin.sync();             
                cout << "Podaj litere: "; 
                cin >> letter;
                
                letter = toupper(letter);
                if (letter >= 'A' && letter <= 'Z')
                {
                    break; 
                }
                PrintMessage("Podaj poprawna litere alfabetu!", MSG_ERROR);
            }
            
            for (; i < DB.getDataSize(); i++)
            {
                if (DB[i].getSurname()[0] == letter)
                {
                    records[num_records] = i; 
                    num_records++;
                }
            }
            
            if (num_records > 0)
            { 
                Template.printTable(records, num_records);
            }
            else
            {
                PrintMessage("Niestety nie znaleziono zadnych nazwisk na ta litere!", MSG_ERROR);
            }
            
            delete [] records;
        }
        break;
        
        // Szukanie z okreslonego przedzialu daty urodzenia
        case 'd':
        {
            int i = 0, num_records = 0;
            Date dataMin, dataMax;
            bool hit;
            
            int *records;
            records = new int[DB.getDataSize()];

            cout << "Dolny zakres daty urodzenia:\n"; 
            dataMin.setDate();
            
            cout << "\nGorny zakres daty urodzenia:\n";
            dataMax.setDate();
            
            
            for (; i < DB.getDataSize(); i++)
            {
                hit = false;
                
                // Rok urodzenia wewnatrz zakresu, na pewno ok
                if (DB[i].getBirthDate().getYear() > dataMin.getYear() && DB[i].getBirthDate().getYear() < dataMax.getYear())
                {
                    hit = true;    
                } 
                // Rok urodzenia minimalny
                else if (DB[i].getBirthDate().getYear() == dataMin.getYear())
                {
                    // Jezeli miesiac wiekszy niz minimalny, na pewno ok
                    if (DB[i].getBirthDate().getMonth() > dataMin.getMonth())
                    {
                        hit = true;
                    }
                    // Jezeli miesiac ten sam, sprawdzamy czy dzien rowny/wiekszy 
                    else if (DB[i].getBirthDate().getMonth() == dataMin.getMonth() && DB[i].getBirthDate().getDay() >= dataMin.getDay())
                    {
                        hit = true;
                    }
                }
                // Rok urodzenia maksymalny
                else if (DB[i].getBirthDate().getYear() == dataMax.getYear())
                {
                    // Jezeli miesiac mniejszy niz maksymalny, na pewno ok
                    if (DB[i].getBirthDate().getMonth() < dataMax.getMonth())
                    {
                        hit = true;
                    }
                    // Jezeli miesiac ten sam, sprawdzamy czy dzien rowny/mniejszy
                    else if (DB[i].getBirthDate().getMonth() == dataMax.getMonth() && DB[i].getBirthDate().getDay() <= dataMax.getDay())
                    {
                        hit = true;
                    }
                }
                
                if (hit)
                {
                    records[num_records] = i; 
                    num_records++;
                }
            }
            
            if (num_records > 0)
            { 
                Template.printTable(records, num_records);
            }
            else
            {
                PrintMessage("Niestety nie znaleziono zadnych osob z podanego przedzialu urodzenia!", MSG_ERROR);
            }
            
            delete [] records;
        }
        break;

        // Szukanie maksymalnej i minimalnej placy
        case 'w':
        {
            int i = 0, minID = 0, maxID = 0;
            double min, max;
            
            min = DB[i].getSalary();
            max = DB[i].getSalary();   
            
            for (; i < DB.getDataSize(); i++)
            {
				if (min > DB[i].getSalary())
                {
                    minID = i;
                    min = DB[i].getSalary(); 
                }
                if (max < DB[i].getSalary())
                {
                    maxID = i;
                    max = DB[i].getSalary(); 
                }
            }
            
            cout << "\nNajmniejsze wynagrodzenie: " << min << "\n";
            Template.printRecord(minID);
            
            cout << "\nNajwieksze wynagrodzenie: " << max << "\n";
            Template.printRecord(maxID);
        }
        break;
    }
    cout << endl;                         
}


int main()
{
	char command;
	const View Template = View::getInstance();

    Template.printMenu();

    while (1)
    {
        cout << "\nPodaj polecenie: ";
        cin.sync();
        cin >> command;
        
        switch (command)
        {
            case 'd':
                Template.printTable();
            break;
            
            case 'n':
				DB.addRecord();
            break;
            
            case 'i':
                cout << "\nIlosc rekordow w bazie: " << DB.getDataSize() << endl;
            break;
            
            case 'p':   
				Template.printRecord();
            break;
            
            case 'u':
                DB.deleteRecord();
            break;
            
            case 'h':
                Template.printMenu();
            break;
            
            case 'w':
                Template.printSearchOptions();
            break;
            
            case 'x':
                return 0;
            break;
            
            default:
                PrintMessage("Nieznane polecenie!", MSG_ERROR);
            break;
        }
    } 
    system("pause");
}
