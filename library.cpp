#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

const string BOOK_FILE_NAME = "book-lists.txt";

enum enMainMenuOption
{
    eshowBooks = 1,
    eAddBook = 2,
    eFindBook = 3,
    eUpdateBook = 4,
    eDeleteBook = 5,
    eFilterBook = 6,
    eExit = 7
};

struct stBook
{
    string ISBN, Title, Author, Category;
    int PublicationYear;
    bool MarkForDelete = false;
};

void ShowMainMenuScreen();

vector<string> SplitString(string Str, string Delim)
{
    int pos = 0;
    vector<string> vString;
    string sWord = "";

    while ((pos = Str.find(Delim)) != string::npos)
    {
        sWord = Str.substr(0, pos);

        if (sWord != "")
            vString.push_back(sWord);

        Str.erase(0, pos + Delim.length());
    }

    if (Str != "")
        vString.push_back(Str);

    return vString;
}

stBook ConvertDataLineToRecord(string DataLine, string Separator)
{
    stBook Book;

    vector<string> vString = SplitString(DataLine, Separator);

    Book.ISBN = vString[0];
    Book.Title = vString[1];
    Book.Author = vString[2];
    Book.Category = vString[3];
    Book.PublicationYear = stoi(vString[4]);

    return Book;
}

vector<stBook> LoadBooksFromFile(string FileName, string Separator)
{
    fstream MyFile;
    vector<stBook> vBooks;

    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line = "";
        while (getline(MyFile, Line))
        {
            vBooks.push_back(ConvertDataLineToRecord(Line, Separator));
        }

        MyFile.close();
    }

    return vBooks;
}

void PrintBook(stBook Book)
{
    cout << "| " << left << setw(20) << Book.ISBN;
    cout << "| " << left << setw(35) << Book.Title;
    cout << "| " << left << setw(30) << Book.Author;
    cout << "| " << left << setw(14) << Book.Category;
    cout << "| " << left << setw(5) << Book.PublicationYear;
}

void ShowBookList(vector<stBook> vBooks)
{
    cout << "\n\n\t\t\t\t\tTotal Number Of Books is: " << vBooks.size() << "\n\n";
    cout << "____________________________________________________________________________________________________________________\n\n";
    cout << "| " << left << setw(20) << "ISBN";
    cout << "| " << left << setw(35) << "Title";
    cout << "| " << left << setw(30) << "Author";
    cout << "| " << left << setw(14) << "Category";
    cout << "| " << left << setw(5) << "Year";
    cout << "\n____________________________________________________________________________________________________________________\n\n";

    if (vBooks.size() == 0)
        cout << "\n\t\t\t\t\t No Book Available Right Now!\n";
    else
    {
        for (stBook &Book : vBooks)
        {
            PrintBook(Book);
            cout << endl;
        }
    }
    cout << "\n____________________________________________________________________________________________________________________\n";
}

void ShowBookListScreen()
{
    system("cls");

    vector<stBook> vBooks = LoadBooksFromFile(BOOK_FILE_NAME, "||");

    ShowBookList(vBooks);
}

void GoBackToMainMenu()
{
    cout << "\n\nPlease enter any key to go back to main menu...";
    system("pause>0");
    ShowMainMenuScreen();
}

bool isISBNExist(string ISBN, string FileName)
{
    fstream MyFile;
    stBook Book;

    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line = "";

        while (getline(MyFile, Line))
        {
            Book = ConvertDataLineToRecord(Line, "||");

            if (Book.ISBN == ISBN)
            {
                MyFile.close();
                return true;
            }
        }

        MyFile.close();
    }

    return false;
}

stBook ReadBookInfo()
{
    stBook Book;

    cout << "\nPlease enter Book's ISBN: ";
    getline(cin >> ws, Book.ISBN);

    while (isISBNExist(Book.ISBN, BOOK_FILE_NAME))
    {
        cout << "\nBook with ISBN " << Book.ISBN << " Already Found! Please enter another ISBN: ";
        getline(cin, Book.ISBN);
    }

    cout << "\nPlease enter Book's Title: ";
    getline(cin, Book.Title);
    cout << "\nPlease enter Book's Author: ";
    getline(cin, Book.Author);
    cout << "\nPlease enter Book's Category: ";
    getline(cin, Book.Category);
    cout << "\nPlease enter Book's Publication Year: ";
    cin >> Book.PublicationYear;

    return Book;
}

string ConvertRecordToDataLine(stBook Book, string Separator)
{
    string DataLine = "";

    DataLine += Book.ISBN + Separator;
    DataLine += Book.Title + Separator;
    DataLine += Book.Author + Separator;
    DataLine += Book.Category + Separator;
    DataLine += to_string(Book.PublicationYear);

    return DataLine;
}

void AddBookInfoToFile(string DataLine, string FileName)
{
    fstream MyFile;

    MyFile.open(FileName, ios::out | ios::app);

    if (MyFile.is_open())
    {
        MyFile << DataLine << endl;

        MyFile.close();
    }
}

void AddNewBook()
{
    stBook Book = ReadBookInfo();
    AddBookInfoToFile(ConvertRecordToDataLine(Book, "||"), BOOK_FILE_NAME);
}

void AddBooks()
{
    char AddMore = 'y';

    do
    {
        AddNewBook();

        cout << "\nBook Added Successfully! Do you want to add more books? y/n? ";
        cin >> AddMore;

    } while (tolower(AddMore) == 'y');
}

void ShowAddBookScreen()
{
    system("cls");
    cout << "\n ==================================\n";
    cout << "\tAdd Book Screen\n";
    cout << " ==================================\n";

    AddBooks();
}

short ReadMainMenuOption()
{
    short Option;

    do
    {
        cout << "\n Choose what you want to do? [1 to 7]? ";
        cin >> Option;

    } while (Option < 1 || Option > 7);

    return Option;
}

string ReadISBN()
{
    string ISBN;

    cout << "\nPlease enter Book's ISBN: ";
    cin >> ISBN;

    return ISBN;
}

bool FindBookByISBN(vector<stBook> vBooks, stBook &Book, string ISBN)
{
    for (stBook &B : vBooks)
    {
        if (B.ISBN == ISBN)
        {
            Book = B;
            return true;
        }
    }
    return false;
}

void PrintBookCard(stBook Book)
{
    cout << "\n Book Information:\n";
    cout << "\n Book ISBN: " << Book.ISBN << endl;
    cout << "\n Book Title: " << Book.Title << endl;
    cout << "\n Book Author: " << Book.Author << endl;
    cout << "\n Book Category: " << Book.Category << endl;
    cout << "\n Publication Year: " << Book.PublicationYear << endl;
}

void ShowFindBookScreen()
{
    system("cls");
    cout << "\n ==================================\n";
    cout << "\tFind Book Screen\n";
    cout << " ==================================\n";

    vector<stBook> vBooks = LoadBooksFromFile(BOOK_FILE_NAME, "||");
    stBook Book;

    string ISBN = ReadISBN();

    if (FindBookByISBN(vBooks, Book, ISBN))
        PrintBookCard(Book);
    else
        cout << "\nSorry Book with ISBN [" << ISBN << "] Not Found!\n";
}

stBook UpdateBookRecord(string ISBN)
{
    stBook Book;

    Book.ISBN = ISBN;

    cout << "\nPlease enter Book's Title: ";
    getline(cin >> ws, Book.Title);
    cout << "\nPlease enter Book's Author: ";
    getline(cin, Book.Author);
    cout << "\nPlease enter Book's Category: ";
    getline(cin, Book.Category);
    cout << "\nPlease enter Book's Publication Year: ";
    cin >> Book.PublicationYear;

    return Book;
}

void SaveBooksToFile(vector<stBook> vBooks, string FileName)
{
    fstream MyFile;

    MyFile.open(FileName, ios::out);

    if (MyFile.is_open())
    {

        for (stBook &Book : vBooks)
        {
            if (!Book.MarkForDelete)
                MyFile << ConvertRecordToDataLine(Book, "||") << endl;
        }

        MyFile.close();
    }
}

void UpdateBookByISBN(vector<stBook> &vBooks, string ISBN)
{
    stBook Book;
    char Answer = 'n';

    if (FindBookByISBN(vBooks, Book, ISBN))
    {
        PrintBookCard(Book);

        cout << "\n are sure you want to update this Book Information? y/n? ";
        cin >> Answer;

        if (tolower(Answer) == 'y')
        {
            for (stBook &B : vBooks)
            {
                if (B.ISBN == ISBN)
                {
                    B = UpdateBookRecord(ISBN);
                    break;
                }
            }
            SaveBooksToFile(vBooks, BOOK_FILE_NAME);
            cout << "\nBook Has Been Updated Successfully!\n";
        }
    }
    else
        cout << "\nSorry Book with ISBN [" << ISBN << "] Not Found!\n";
}

void ShowUpdateBookScreen()
{
    system("cls");
    cout << "\n ==================================\n";
    cout << "\tUpdate Book Screen\n";
    cout << " ==================================\n";

    vector<stBook> vBooks = LoadBooksFromFile(BOOK_FILE_NAME, "||");
    string ISBN = ReadISBN();
    UpdateBookByISBN(vBooks, ISBN);
}

void MarkBookForDelete(vector<stBook> &vBooks, string ISBN)
{
    for (stBook &Book : vBooks)
    {
        if (Book.ISBN == ISBN)
        {
            Book.MarkForDelete = true;

            break;
        }
    }
}

void DeleteBookByISBN(vector<stBook> &vBooks, string ISBN)
{
    stBook Book;
    char Answer = 'n';

    if (FindBookByISBN(vBooks, Book, ISBN))
    {
        PrintBookCard(Book);

        cout << "\n are sure you want to delete this Book Information? y/n? ";
        cin >> Answer;

        if (tolower(Answer) == 'y')
        {
            MarkBookForDelete(vBooks, ISBN);
            SaveBooksToFile(vBooks, BOOK_FILE_NAME);
            cout << "\nBook Has Been Deleted Successfully!\n";
        }
    }
    else
        cout << "\nSorry Book with ISBN [" << ISBN << "] Not Found!\n";
}

void ShowDeleteBookScreen()
{
    system("cls");
    cout << "\n ==================================\n";
    cout << "\tDelete Book Screen\n";
    cout << " ==================================\n";

    vector<stBook> vBooks = LoadBooksFromFile(BOOK_FILE_NAME, "||");
    string ISBN = ReadISBN();
    DeleteBookByISBN(vBooks, ISBN);
}

string ReadCategory()
{
    string Category;

    cout << "\nPlease enter Book's Category: ";
    cin >> Category;

    return Category;
}

void FilterBookByCategory(vector<stBook> vBooks, string Category)
{
    vector<stBook> cBooks;

    for (stBook &Book : vBooks)
    {
        if (Book.Category == Category)
            cBooks.push_back(Book);
    }
    ShowBookList(cBooks);
}

void ShowFilterBookScreen()
{
    system("cls");
    cout << "\n ==================================\n";
    cout << "\tFilterBook Screen\n";
    cout << " ==================================\n";

    vector<stBook> vBooks = LoadBooksFromFile(BOOK_FILE_NAME, "||");
    string Category = ReadCategory();
    FilterBookByCategory(vBooks, Category);
}

void ShowExitScreen()
{
    system("cls");
    cout << "\n ==================================\n";
    cout << "\tProgram Ends :-)\n";
    cout << " ==================================\n";
}

void PerformMainMenuOption(enMainMenuOption MainMenuOption)
{
    switch (MainMenuOption)
    {
    case enMainMenuOption::eshowBooks:
        ShowBookListScreen();
        GoBackToMainMenu();
        break;
    case enMainMenuOption::eAddBook:
        ShowAddBookScreen();
        GoBackToMainMenu();
        break;
    case enMainMenuOption::eFindBook:
        ShowFindBookScreen();
        GoBackToMainMenu();
        break;
    case enMainMenuOption::eUpdateBook:
        ShowUpdateBookScreen();
        GoBackToMainMenu();
        break;
    case enMainMenuOption::eDeleteBook:
        ShowDeleteBookScreen();
        GoBackToMainMenu();
        break;
    case enMainMenuOption::eFilterBook:
        ShowFilterBookScreen();
        GoBackToMainMenu();
        break;
    case enMainMenuOption::eExit:
        ShowExitScreen();
        break;
    }
}

void ShowMainMenuScreen()
{
    system("cls");
    cout << "\n =========================================\n";
    cout << "\t\tFez Library\n";
    cout << " =========================================\n";

    cout << "\t[1] Show Books List.\n";
    cout << "\t[2] Add New Book.\n";
    cout << "\t[3] Find Book.\n";
    cout << "\t[4] Update Book Info.\n";
    cout << "\t[5] Delete Book.\n";
    cout << "\t[6] Filter Books.\n";
    cout << "\t[7] Exit.\n";
    cout << " =========================================\n";

    PerformMainMenuOption((enMainMenuOption)ReadMainMenuOption());
}

int main()
{
    ShowMainMenuScreen();
    system("pause>0");
    return 0;
}