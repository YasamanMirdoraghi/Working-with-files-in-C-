#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <string.h>
#include <windows.h>

using namespace std;

void Check_Enter();
void Menu();
string CheckCode(string check);
void Reaplace(string codei);
bool Check_h(string name);
void readTeacherInfo(string &name, string &lastname, string &TenderNumber);
bool Check_e(string TenderNumber);
bool Search(string TenderNumber);
void writeTeacherInfo(string name, string lastname, string TenderNumber);
void CheckChange(string name, bool b);
string CheckChange1(string name);
void Enter(string code);
void Exit(string code);
void Report();
void CurrentTeacher();
void allTeachers();
void checkEnter(string code, bool b);
bool TimeTable(int hour1, int min1, int sec1, string khoroj, string month1, string day1, string year1, string month2, string day2, string year2);
void ListMaster();
void pak();
void Listed();
void MONTH();
void listExtra(string code, string action);
void behtarin(bool check);
void itsok2(string enter, string exit, string code, string year1, string year2, string month1, string month2, string day1, string day2, string action);
void Checking(string code, string enter);
void bihodeh();
HANDLE hconsol = GetStdHandle(STD_OUTPUT_HANDLE);

int main()
{
    Check_Enter(); // Check kardan inke enter mandeh dar roz bad nabashad :)
    Menu();        // Menu
}

// Check kardan inke enter mandeh dar roz bad nabashad :)
void Check_Enter()
{
    string exit, code1;
    bool count;
    string action[100], Code[100], h[100], m[100], s[100], mah[100], day[100], sal[100], tim[100];
    int c = 0, month1, day1, year1;
    ifstream file("Enter.txt");
    while (getline(file, action[c], '\t'))
    {
        getline(file, Code[c], '\t');
        getline(file, h[c], ':');
        getline(file, m[c], ':');
        getline(file, s[c], '\t');
        // getline(file, tim[c], '\t');
        getline(file, mah[c], '/');
        getline(file, day[c], '/');
        getline(file, sal[c], '\n');
        c++;
    }
    file.close();

    int sec = 0, min = 0, hour = 0, day22 = 0, month = 0, year = 0;
    time_t now = time(0);
    tm *local = localtime(&now);

    sec = local->tm_sec;          // seconds (0-61)
    min = local->tm_min;          // minutes (0-59)
    hour = local->tm_hour;        // hours (0-23)
    day22 = local->tm_mday;       // day of the month (1-31)
    month = local->tm_mon + 1;    // month (1-12)
    year = local->tm_year + 1900; // year since 1900

    string save[100];
    bool hasExited = false;
    ofstream b("Enter.txt");
    for (int i = 0; i < c; i++)
    {
        if (action[i] == "Enter:")
        {
            // in karbar vared shode
            //  barresi in ke aya karbar exit dashteh ya na
            hasExited = false;
            for (int j = i + 1; j < c; j++)
            {
                if (Code[i] == Code[j] && action[j] == "Exit:")
                {
                    // in karbar exit shode
                    hasExited = true;
                    break;
                }
            }
            if (!hasExited)
            {
                // in karbar exit nashode
                ifstream v("teachers.txt");
                string name;
                string Duplicate;
                code1 = Code[i];
                while (getline(v >> ws, name, '\n'))
                {
                    v >> Duplicate;
                    if (code1 == Duplicate)
                    {
                        save[i] = Duplicate;
                        break;
                    }
                }
                day1 = stoi(day[i]);
                if ((day1 != day22))
                {
                    ofstream p("extra.txt", ios::app);
                    p << action[i] << "\t" << save[i] << "\t" << h[i] << ":" << m[i] << ":" << s[i] << '\t' << mah[i] << "/" << day[i] << "/" << sal[i] << "\n";
                    p.close();
                }
                v.close();
            }
        }
        if (Code[i] != save[i])
        {
            b << action[i]<< "\t"  << Code[i] << "\t" << h[i] << ":" << m[i] << ":" << s[i] << '\t' << mah[i] << "/" << day[i] << "/" << sal[i] << "\n";
        }
    }
    b.close();

}
// Menu
void Menu()
{
    while (true)
    {
        int number2;
        string TenderNumber;
        SetConsoleTextAttribute(hconsol, FOREGROUND_BLUE);
        cout << "\n****************** Welcome to The Teacher's Attendance Registration System ******************\n";
        cout << " What can I do for you?\n";
        cout << "1. Editing information\t2. Enter Registration\n";
        cout << "3. Exit Registration\t4.Reports\n5.Exit the program\n6.RegisterMaster\n";
        cin >> number2;
        cin.ignore();
        bool b = true; // taeen konandeh vorod khoroj baray karbar
        SetConsoleTextAttribute(hconsol, FOREGROUND_INTENSITY);
        if (number2 == 1)
        {
            string check;
            cout << "Enter  Name Or Code :";
            cin >> check;
            cin.ignore();
            string name1 = CheckCode(check); // tatabog dadan option ba code use
            cin.ignore();
            if (name1 != "")
                Reaplace(name1); // avaz kardan name and last name...
        }
        else if (number2 == 2)
        {
            string Name;
            int option;
            cout << "Enter Your Name Or Code :";
            cin >> Name;
            CheckChange(Name, b); // find user with search...
        }
        else if (number2 == 3)
        {
            b = false;
            string name;
            cout << "Enter Your Name Or Code :";
            cin >> name;
            CheckChange(name, b); // find user with search...
        }
        else if (number2 == 4)
        {
            Report(); // gozareshat...
        }
        else if (number2 == 5)
        {
            cout << "Bay :) !";
            exit(-1);
        }
        else if (number2 == 6)
        {
            string name, lastname, TenderNumber;
            readTeacherInfo(name, lastname, TenderNumber);          // read information..
            SetConsoleTextAttribute(hconsol, FOREGROUND_INTENSITY); // color
            writeTeacherInfo(name, lastname, TenderNumber);
            cout << " You are registered ..! \n";
        }
    }
}

// readTeacherInfo // khandan etelaat az karbar *******
void readTeacherInfo(string &name, string &lastname, string &TenderNumber)
{
    cout << "Enter the teacher's name : ";
    getline(cin, name, '\n');
    int a = Check_h(name); // check name with out number..
    while (a == false)
    {
        cout << "Enter your first name without numbers..!\n";
        cout << "Enter the teacher's name :";
        getline(cin, name, '\n');
        a = Check_h(name);
    }
    cout << "Enter the teacher's lastname: ";
    getline(cin, lastname, '\n');
    int c = Check_h(lastname); // check lastname with out number..
    while (c == false)
    {
        cout << "Enter your last name without numbers..!\n";
        cout << "Enter the teacher's lastname:";
        getline(cin, lastname, '\n');
        c = Check_h(lastname);
    }
    cout << "Enter the teacher's TenderNumber: ";
    getline(cin, TenderNumber, '\n');
    int b = Check_e(TenderNumber);     // check code with out letter..
    bool value = Search(TenderNumber); // check same code...
    while (TenderNumber.length() != 6 || b == false || value == true)
    {
        if (TenderNumber.length() != 6 || b == false)
        {
            cout << "Your code must be six digits without letters..!\n";
            cout << "Enter the teacher's TenderNumber:";
        }
        else if (value == true)
        {
            cout << "You have already registered with the number Or\n"
                 << "this password has already been used by someone else.. " << endl;
            cout << "Enter the teacher's TenderNumber:";
        }

        getline(cin, TenderNumber, '\n');
        b = Check_e(TenderNumber);
        value = Search(TenderNumber);
    }
}

// check harf dar code...
bool Check_e(string TenderNumber)
{
    bool isNumber = true;
    for (int i = 0; i < TenderNumber.length(); i++)
    {
        if (!isdigit(TenderNumber[i]))
        {
            isNumber = false;
            break;
        }
    }
    return isNumber;
}

// check number dar name..
bool Check_h(string name)
{
    bool isNumber = true;
    for (int i = 0; i < name.length(); i++)
    {
        if (isdigit(name[i]))
        {
            isNumber = false;
            break;
        }
    }
    return isNumber;
}

// Search.. codey ke ghablan vared shodeh dige nasheh..
bool Search(string TenderNumber)
{
    ifstream file("teachers.txt");
    string name;
    string Duplicate;
    while (getline(file >> ws, name, '\n'))
    {
        file >> Duplicate;
        if (Duplicate == TenderNumber)
            return true;
    }
    return false;
    file.close();
}

// writeTeacherInfo
void writeTeacherInfo(string name, string lastname, string TenderNumber)
{
    ofstream file("teachers.txt", ios::app);
    file << name << " " << lastname << "\n"
         << TenderNumber << endl;
    file.close();
}

// Reaplace // taghir name and lastname...

void Reaplace(string code22)
{
    bool q = true;
    string numi, newName, Number, num;
    ifstream infile("teachers.txt");
    ofstream tempfile("temp.txt");

    while (getline(infile >> ws, newName, '\n'))
    {
        infile >> Number;
        if (code22 == Number)
        {
            num = newName;
        }
        if (num == newName)
        {
            q = false;
            cout << "Enter your new first and last name: ";
            getline(cin, newName, '\n');
            bool K = Check_h(newName);
            if (K == true)
            {
                tempfile << newName << '\n'
                         << Number << endl;
            }
            else
            {
                while (K == false)
                {
                    cout << "Your name and lastname do not contain numbers!\n";
                    cout << "Enter your new first and last name:";
                    getline(cin, newName, '\n');
                    K = Check_h(newName);
                }
            }
        }
        else
        {
            tempfile << newName << '\n'
                     << Number << endl;
        }
    }
    if (q == false)
        cout << "The edit was successful!" << endl;
    else
        cout << "The edit was not successful" << endl;

    infile.close();
    tempfile.close();
    remove("teachers.txt");
    rename("temp.txt", "teachers.txt");
}

string CheckCode(string check)
{
    int c = 0, count = 0;
    int option, index[100], index1[100];
    string first[100], Code[100], first1[100], last1[100], Code1[100];
    ifstream file("teachers.txt");
    if (!file)
    {
        cout << "can not open!";
        exit(-1);
    }

    while (getline(file >> ws, first[c], '\n'))
    {
        getline(file >> ws, Code[c], '\n');
        c++;
    }

    // jostejo dar halgheh for baray aray first..
    for (int i = 0; i < c; i++)
    {
        if ((first[i].find(check) != string ::npos) || (Code[i].find(check) != string ::npos))
        {
            index1[count] = i;
            index[count] = count;
            count++;
            cout << count << ". " << first[i] << '\t' << Code[i] << endl;
        }
        continue;
    }
    if (count == 0)
    {
        cout << "The desired teachers was not found..!\n";
        return "";
    }
    else
    {
        cout << count << " Number of teachers found " << endl;
        cout << " Which option do you want?\nnone:-1" << endl;
        cin >> option;
        for (int i = 0; i < count; i++)
        {
            if ((index[i] + 1) == option)
                return Code[index1[i]];
        }
        if (option == -1)
            Menu();
    }
    file.close();
}
// end sabt edit

// Check name  // peyda kardan search tekrari...
void CheckChange(string name, bool b)
{
    int c = 0, count = 0;
    int option, index[100], index1[100];
    string first[100], Code[100], first1[100], last1[100], Code1[100];
    ifstream file("teachers.txt");
    if (!file)
    {
        cout << "can not open!";
        exit(-1);
    }

    while (getline(file >> ws, first[c], '\n'))
    {
        getline(file >> ws, Code[c], '\n');
        c++;
    }

    // jostejo dar halgheh for baray aray first..
    for (int i = 0; i < c; i++)
    {
        if ((first[i].find(name) != string ::npos) || (Code[i].find(name) != string ::npos))
        {
            index1[count] = i;
            index[count] = count;
            count++;
            cout << count << ". " << first[i] << '\t' << Code[i] << endl;
        }
        continue;
    }
    if (count == 0)
    {
        cout << "The desired teachers was not found..!\n";
        Menu();
    }
    else
    {
        cout << count << " Number of teachers found " << endl;
        cout << " Which option do you want?\nnone:-1" << endl;
        cin >> option;
        for (int i = 0; i < count; i++)
        {
            if ((index[i] + 1) == option)
                checkEnter(Code[index1[i]], b);
        }
        if (option == -1)
            return;
    }
    file.close();
}

// check vorod khoroj*************
void checkEnter(string code, bool b)
{
    string exit;
    int count = 0;
    string name[100], Code[100], mah[100], qnam, day[100], sal[100], month1, day1, year1, tim[100], enter, month2, day2, year2;
    int c = 0;
    ifstream file("Enter.txt");
    while (getline(file, name[c], '\t'))
    {
        getline(file, Code[c], '\t');
        getline(file, tim[c], '\t');
        getline(file, mah[c], '/');
        getline(file, day[c], '/');
        getline(file, sal[c], '\n');

        for (int i = 0; i <= c; i++)
        {
            if (name[i].find("Enter:") != string::npos)
            {
                if (Code[i] == code)
                {
                    count++;
                    enter = tim[i];
                    month1 = mah[i];
                    day1 = day[i];
                    year1 = sal[i];
                }
            }
            break;
        }

        for (int i = 0; i <= c; i++)
        {
            if (name[i].find("Exit:") != string::npos)
            {
                if (Code[i] == code)
                {
                    count--;
                    exit = tim[i];
                    month2 = mah[i];
                    day2 = day[i];
                    year2 = sal[i];
                }
            }
            break;
        }
    }
    file.close();
    if (b == true)
    {
        if (count == 1)
            Checking(code, enter); // check enter2
        else
        {
            Enter(code); // enter
            cout << " You are in..";
        }
    }
    else
    {
        string action = "Exit:";
        itsok2(enter, exit, code, year1, year2, month1, month2, day1, day2, action); // khoroj
    }
}

// enter dovom check
void Checking(string code, string enter)
{
    string name[100], Code[100], mah[100], tim[100], day[100], sal[100];
    int c = 0;
    string Center, Ccode, Cmah, Ctim, Cday, Csal;
    ifstream f("Enter.txt");
    while (getline(f, name[c], '\t'))
    {
        getline(f, Code[c], '\t');
        getline(f, tim[c], '\t');
        getline(f, mah[c], '/');
        getline(f, day[c], '/');
        getline(f, sal[c], '\n');
        c++;
    }

    for (int i = 0; i < c; i++)
    {
        if (code == Code[i] && name[i] == "Enter:")
        {
            Center = name[i];
            Ccode = Code[i];
            Ctim = tim[i];
            Cmah = mah[i];
            Cday = day[i];
            Csal = sal[i];
            cout << " You were already entered in today..!\n";
        }
    }

    // gharar dadan vorod aval dar sorat vojod
    ofstream h("extra.txt", ios::app);
    h << Center << "\t" << Ccode << "\t" << Ctim << '\t' << Cmah << "/" << Cday << "/" << Csal << "\n";
    h.close();

    for (int i = 0; i < c; i++)
    {
        if (code == Code[i] && tim[i] == Ctim)
        {
            int sec = 0, min = 0, hour = 0, day = 0, month = 0, year = 0;
            time_t now = time(0);
            tm *local = localtime(&now);

            sec = local->tm_sec;          // seconds (0-61)
            min = local->tm_min;          // minutes (0-59)
            hour = local->tm_hour;        // hours (0-23)
            day = local->tm_mday;         // day of the month (1-31)
            month = local->tm_mon + 1;    // month (1-12)
            year = local->tm_year + 1900; // year since 1900
            string tim2 = to_string(hour) + ":" + to_string(min) + ":" + to_string(sec);
            tim[i] = tim2;
        }
    }
    // beroz resani file enter.txt
    ofstream b("Enter.txt");
    for (int i = 0; i < c; i++)
        b << name[i] << "\t" << Code[i] << "\t" << tim[i] << '\t' << mah[i] << "/" << day[i] << "/" << sal[i] << "\n";
    b.close();
}
// Enter
void Enter(string code)
{
    ofstream file;
    if (!file)
    {
        cout << "Unable to open file";
        exit(-1);
    }
    int sec = 0, min = 0, hour = 0, day = 0, month = 0, year = 0;
    // gereftan zaman az system...
    time_t now = time(0);
    tm *local = localtime(&now);

    sec = local->tm_sec;          // seconds (0-61)
    min = local->tm_min;          // minutes (0-59)
    hour = local->tm_hour;        // hours (0-23)
    day = local->tm_mday;         // day of the month (1-31)
    month = local->tm_mon + 1;    // month (1-12)
    year = local->tm_year + 1900; // year since 1900

    file.open("Enter.txt", ios::app);
    file << "Enter:"
         << "\t" << code << "\t" << hour << ":" << min << ":" << sec << "\t" << month << "/" << day << "/" << year << "\n";
    file.close();
}

void itsok2(string enter, string exit, string code, string year1, string year2, string month1, string month2, string day1, string day2, string action)
{
    bool b;
    if (enter.empty())
    {
        listExtra(code, action);
        cout << "You are not entered in before..!";
        // agar bar aval khoroj kard vorod nakard dobareh khoroj khast bokone nazareh....
    }
    else if (exit.empty())
    {
        cout << "You are out..";
        Exit(code);
    }
    else
    {
        char *token = strtok(const_cast<char *>(enter.c_str()), ":");
        int hour1 = stoi(token);

        token = strtok(NULL, ":");
        int min1 = stoi(token);

        token = strtok(NULL, ":");
        int sec1 = stoi(token);

        b = TimeTable(hour1, min1, sec1, exit, month1, day1, year1, month2, day2, year2);

        if (b == false)
        {
            cout << "You are out..";
            Exit(code);
        }
        else
        {
            cout << " You were already exited in today..!\n";
            listExtra(code, action);
        }
    }
}

// mohasebeh zaman vorod khoroj....

bool TimeTable(int hour1, int min1, int sec1, string khoroj, string month1, string day1, string year1, string month2, string day2, string year2)
{
    char *token = strtok(const_cast<char *>(khoroj.c_str()), ":");

    int hour2 = stoi(token);

    token = strtok(NULL, ":");
    int min2 = stoi(token);

    token = strtok(NULL, ":");
    int sec2 = stoi(token);

    // tabdil mah sal roz be int..
    int mah1 = stoi(month1);
    int Day1 = stoi(day1);
    int Year1 = stoi(year1);

    int mah2 = stoi(month2);
    int Day2 = stoi(day2);
    int Year2 = stoi(year2);

    if ((Year2 > Year1) || (Year2 == Year1 && mah2 > mah1) || (Year2 == Year1 && mah2 == mah1 && Day2 > Day1) ||
        (Year2 == Year1 && mah2 == mah1 && Day2 == Day1 && hour2 > hour1) ||
        (Year2 == Year1 && mah2 == mah1 && Day2 == Day1 && hour2 == hour1 && min2 > min1) ||
        (Year2 == Year1 && mah2 == mah1 && Day2 == Day1 && hour2 == hour1 && min2 == min1 && sec2 > sec1))
        return true;
    else
        return false;
}

// Exit
void Exit(string Code)
{
    int sec = 0, min = 0, hour = 0, day = 0, month = 0, year = 0;
    time_t now = time(0);
    tm *local = localtime(&now);

    sec = local->tm_sec;          // seconds (0-61)
    min = local->tm_min;          // minutes (0-59)
    hour = local->tm_hour;        // hours (0-23)
    day = local->tm_mday;         // day of the month (1-31)
    month = local->tm_mon + 1;    // month (1-12)
    year = local->tm_year + 1900; // year since 1900

    ofstream file;
    file.open("Enter.txt", ios::app);

    file << "Exit:"
         << "\t" << Code << "\t" << hour << ":" << min << ":" << sec << '\t' << month << "/" << day << "/" << year << "\n";

    file.close();

    if (!file)
    {
        cout << "Unable to open file";
        exit(-1);
    }
}

// Report
void Report()
{
    while (1)
    {

        SetConsoleTextAttribute(hconsol, FOREGROUND_GREEN);
        int Choose;
        cout << "1.Who is the 5 best teacher? \n 2.Who is the 5 worst teacher? \n";
        cout << "3.How many teachers are currently in the university? \n ";
        cout << "4.List of  people available in the university? \n";
        cout << "5.List of people currently in the university? \n 6.How many teachers are there in the university?\n";
        cout << "7.What is the monthly attendance of teachers in a month? \n8.How many entries and exits have been made in vain?!\n9.Back!\n";
        cout << "\nChoose an option:\n";
        cin >> Choose;
        cin.ignore();
        // moshsakhas kardan behtarin badtarin
        bool check = true;
        switch (Choose)
        {
        case 1:
            behtarin(check);
            break;
        case 2:
            check = false;
            behtarin(check);
            break;
        case 3:
            CurrentTeacher();
            break;
        case 4:
            Listed();
            break;
        case 5:
            pak();
            break;
        case 6:
            allTeachers();
            break;
        case 7:
            MONTH();
            break;
        case 8:
            bihodeh();
            break;
        case 9:
            return;
            break;
        }
    }
}
////////////////////////////////////////////////////// behtarin ///////////////////////////////////////////////
void behtarin(bool check)
{
    SetConsoleTextAttribute(hconsol, FOREGROUND_INTENSITY);
    int numTeachers = 0;
    string teacherCodes[100], teacherNames[100];
    ifstream teacherFile("teachers.txt");
    while (getline(teacherFile >> ws, teacherNames[numTeachers], '\n'))
    {
        getline(teacherFile, teacherCodes[numTeachers], '\n');
        numTeachers++;
    }
    teacherFile.close();

    int total[100] = {0};
    ifstream enterFile("Enter.txt");
    int c = 0, b = 0;
    int h1, m1;
    string action, teacherCode, TCode[100], h, m, s[100], date, action1[100];
    int teacherIndex = -2;
    while (getline(enterFile, action, '\t'))
    {
        getline(enterFile, teacherCode, '\t');
        getline(enterFile, h, ':');
        getline(enterFile, m, ':');
        getline(enterFile, s[b], '\t');
        getline(enterFile, date, '\n');
        h1 = stoi(h);
        m1 = stoi(m);
        for (int i = 0; i < numTeachers; i++)
        {
            if (teacherCode == teacherCodes[i])
            {
                teacherIndex = i;
                break;
            }
        }
        if (teacherIndex != -2)
        {
            int hour, minute, totalMinutes;
            hour = h1;
            minute = m1;
            totalMinutes = hour * 60 + minute;

            if (action == "Enter:")
            {
                total[teacherIndex] -= totalMinutes;
  
            }
            else
            {
                total[teacherIndex] += totalMinutes;
            }
        }
    }
    enterFile.close();

    // moratab sazi arayeh********
    for (int i = 0; i < numTeachers - 1; i++)
    {
        for (int j = i + 1; j < numTeachers; j++)
        {
            if (total[i] < total[j])
            {
                swap(total[i], total[j]);
                swap(teacherNames[i], teacherNames[j]);
            }
        }
    }

    if (check == true)
    {
        // print top 5 teachers
        cout << "Top 5 teachers:\n";
        for (int i = 0; i < 5 && i < numTeachers; i++)
        {
            cout << i + 1 << ". " << teacherNames[i] << " _ " << total[i] << " minutes\n";
        }
    }
    else
    {
        // print botton 5 teachers
        cout << "Bottom 5 teachers:\n";
        for (int i = numTeachers - 1; i >= 0 && i >= numTeachers - 5; i--)
        {
            cout << numTeachers - i << ". " << teacherNames[i] << " _ " << total[i] << " minutes\n";
        }
    }
}

/////////////////////////////////////// list asatid hazer/////////////////////////////
void pak()
{
    SetConsoleTextAttribute(hconsol, FOREGROUND_INTENSITY);
    string exit, code1;
    bool count;
    string action[100], Code[100], mah[100], day[100], sal[100], tim[100];
    int c = 0, month1, day1, year1;
    ifstream file("Enter.txt");
    while (getline(file, action[c], '\t'))
    {
        getline(file, Code[c], '\t');
        getline(file, tim[c], '\t');
        getline(file, mah[c], '/');
        getline(file, day[c], '/');
        getline(file, sal[c], '\n');
        c++;
    }
    int sec = 0, min = 0, hour = 0, day22 = 0, month = 0, year = 0;
    time_t now = time(0);
    tm *local = localtime(&now);

    sec = local->tm_sec;          // seconds (0-61)
    min = local->tm_min;          // minutes (0-59)
    hour = local->tm_hour;        // hours (0-23)
    day22 = local->tm_mday;       // day of the month (1-31)
    month = local->tm_mon + 1;    // month (1-12)
    year = local->tm_year + 1900; // year since 1900

    bool hasExited = false;
    for (int i = 0; i < c; i++)
    {
        if (action[i] == "Enter:")
        {
            // in karbar vared shode
            //  barresi in ke aya karbar exit dashteh ya na
            hasExited = false;
            for (int j = i + 1; j < c; j++)
            {
                if (Code[i] == Code[j] && action[j] == "Exit:")
                {
                    // in karbar exit shode
                    hasExited = true;
                    break;
                }
            }
            if (!hasExited)
            {
                // in karbar exit nashode
                ifstream v("teachers.txt");
                string name;
                string Duplicate;
                code1 = Code[i];
                while (getline(v >> ws, name, '\n'))
                {
                    v >> Duplicate;
                    if (code1 == Duplicate)
                    {
                        cout << name << " ";
                        break;
                    }
                }
                month1 = stoi(mah[i]);
                day1 = stoi(day[i]);
                year1 = stoi(sal[i]);
                if ((day1 == day22) && (month == month1) && (year1 == year))
                {
                    cout << " with code " << code1 << " has entered but not exited." << endl;
                }
                v.close();
            }
        }
    }
    if (hasExited)
    {
        cout << "No one is in college..\n";
    }

    file.close();
}

// CurrentTeacher... Tedad asatid hazer dar daneshgah
void CurrentTeacher()
{
    SetConsoleTextAttribute(hconsol, FOREGROUND_INTENSITY);
    cout << "The number of teachers currently in the university : ";
    ifstream file("Enter.txt");
    int counter = 0;
    string name;
    while (file >> name)
    {
        if (name == "Enter:")
        {
            counter += 1;
        }
        if (name == "")
        {
            break;
        }
        if (name == "Exit:")
        {
            counter -= 1;
        }
    }
    cout << counter << endl;
}

// allTeachers////////////////////////////////////////////////////////
void allTeachers()
{
    SetConsoleTextAttribute(hconsol, FOREGROUND_INTENSITY);

    string name;
    int counter = 0;
    int Code;
    ifstream file("teachers.txt");
    while (getline(file >> ws, name, '\n'))
    {
        file >> Code;
        counter++;
    }
    file.close();
    cout << "The number of teachers in the university : " << counter << endl;
}

/////////////////////////////////// list mojod ///////////////////////////////////////////////////////

void Listed()
{
    SetConsoleTextAttribute(hconsol, FOREGROUND_INTENSITY);

    string name, code;
    int count = 0;
    ifstream file("teachers.txt");
    while (getline(file >> ws, name, '\n'))
    {
        getline(file, code, '\n');
        count++;
        cout << count << ". " << name << "\n";
    }

    cout << "good like " << endl;

    file.close();
}

///////////////////////////////////////////////// list extra ////////////////////////////////////////////////////

void listExtra(string code, string action)
{
    ofstream Y("extra.txt", ios::app);
    if (!Y)
    {
        cout << "Unable to open file";
        exit(-1);
    }
    int sec = 0, min = 0, hour = 0, day = 0, month = 0, year = 0;
    time_t now = time(0);
    tm *local = localtime(&now);

    sec = local->tm_sec;          // seconds (0-61)
    min = local->tm_min;          // minutes (0-59)
    hour = local->tm_hour;        // hours (0-23)
    day = local->tm_mday;         // day of the month (1-31)
    month = local->tm_mon + 1;    // month (1-12)
    year = local->tm_year + 1900; // year since 1900
    Y << action
      << "\t" << code << "\t" << hour << ":" << min << ":" << sec << "\t" << month << "/" << day << "/" << year << "\n";
    Y.close();
}

string CheckChange1(string name)
{
    int c = 0, count = 0;
    int option, index[100], index1[100];
    string first[100], last[100], Code[100], TCode[100], first1[100], last1[100], Code1[100];
    ;
    ifstream file("teachers.txt");
    if (!file)
    {
        cout << "can not open!";
        exit(-1);
    }

    while (getline(file >> ws, first[c], '\n'))
    {
        getline(file, Code[c], '\n');
        c++;
        // jostejo dar halgheh for baray aray first..
    }

    for (int i = 0; i < c; i++)
    {
        if ((first[i].find(name) != string ::npos) || (Code[i].find(name) != string ::npos))
        {
            TCode[i] = Code[i];
            index1[count] = i;
            index[count] = count;
            count++;
            cout << count << ". " << first[i] << " : " << Code[i] << endl;
        }
        continue;
    }
    if (count == 0)
    {
        cout << "The desired teachers was not found..!\n";
        Report();
    }
    else
    {
        cout << count << " Number of teachers found " << endl;
        cout << " Which option do you want?\nnone:-1" << endl;
        cin >> option;
        for (int i = 0; i < count; i++)
        {
            if ((index[i] + 1) == option)
                return TCode[index1[i]];
        }
        if (option == -1)
            Menu();
    }
    file.close();
}

////////////////////////////////                    HOZOR MAHANEH              ///////////////////////////////////////////////

void MONTH()
{
    SetConsoleTextAttribute(hconsol, FOREGROUND_INTENSITY);
    string name, lastname, code;
    int cinmah;
    bool check = true, b;
    string last, Code, first;
    do
    {
        // readTeacherInfo(name, lastname, code);
        cout << "Enter Your name or code :";
        cin >> name;
        code = CheckChange1(name);
        cout << "Enter your month :\n";
        cin >> cinmah;
        cin.ignore();
        ifstream file("teachers.txt");
        while (getline(file >> ws, first, ' '))
        {
            getline(file >> ws, last, '\n');
            getline(file >> ws, Code, '\n');
            if (code == Code)
            {
                check = false;
                break;
            }
        }
        if (check == true)
            cout << "User is not found..!\n";
    } while (check);

    const int DAYS_IN_MONTH = 31;
    int totalend[DAYS_IN_MONTH] = {0};
    int c = 0;
    string Tcode2[100], action[100], Tcode[100], h[100], m[100], s[100], mah[100], day[100], sal[100];
    int hour[100], min[100], sec[100], mah1[100];
    ifstream enter("Enter.txt");
    while (getline(enter, action[c], '\t'))
    {
        getline(enter, Tcode[c], '\t');
        getline(enter, h[c], ':');
        getline(enter, m[c], ':');
        getline(enter, s[c], '\t');
        getline(enter, mah[c], '/');
        getline(enter, day[c], '/');
        getline(enter, sal[c], '\n');
        hour[c] = stoi(h[c]);
        min[c] = stoi(m[c]);
        sec[c] = stoi(s[c]);
        mah1[c] = stoi(mah[c]);
        Tcode2[c] = Tcode[c];
        c++;
    }

    for (int i = 0; i < c; i++)
    {
        int dayIndex = stoi(day[i]) - 1;

        if (Tcode2[i] == code && mah1[i] == cinmah)
        {
            if (action[i] == "Enter:")
            {
                // in karbar vared shode
                totalend[dayIndex] -= hour[i] * 60 + min[i];

                // barresi inke in karbar kharej shode ya na
                bool hasExited = false;
                for (int j = i + 1; j < c; j++)
                {
                    if (Tcode2[i] == Tcode2[j] && action[j] == "Exit:")
                    {
                        hasExited = true;
                        break;
                    }
                }

                // agar karbar khoroj nadashte bashad
                if (!hasExited)
                {
                    totalend[dayIndex] += hour[i] * 60 + min[i];
                }
            }
            else
            {
                totalend[dayIndex] += hour[i] * 60 + min[i];
            }
        }
    }
    int all = 0;
    for (int i = 0; i < DAYS_IN_MONTH; i++)
    {
        if (totalend[i] == 0)
        {
            all += totalend[i];
            cout << "User: " << first << " " << last << " WithCode " << code << " in the month of " << cinmah << " in the day of " << i + 1 << " min: " << totalend[i] << "has been present\n";
        }
    }
    if (all != 0)
    {
        cout << "This teachers has been present for a total of " << all << " minutes this month\n";
    }
    else
    {
        cout << "This teachers was not present this month\n";
    }
}

void bihodeh()
{
    SetConsoleTextAttribute(hconsol, FOREGROUND_INTENSITY);
    string check;
    cout << "Enter  Name Or Code :";
    cin >> check;
    cin.ignore();
    string name1 = CheckCode(check);
    int countenter = 0, countexit = 0;
    string name[100], Code[100], mah[100], day[100], sal[100], tim[100];
    int c = 0;
    ifstream file("extra.txt");
    while (getline(file, name[c], '\t'))
    {
        getline(file, Code[c], '\t');
        getline(file, tim[c], '\t');
        getline(file, mah[c], '/');
        getline(file, day[c], '/');
        getline(file, sal[c], '\n');

        for (int i = 0; i <= c; i++)
        {
            if (name[i].find("Enter:") != string::npos)
            {
                if (Code[i] == name1)
                {
                    countenter++;
                }
            }
            break;
        }
        for (int i = 0; i <= c; i++)
        {
            if (name[i].find("Exit:") != string::npos)
            {
                if (Code[i] == name1)
                {
                    countexit++;
                }
            }
            break;
        }
    }
    int numTeachers = 0;
    string teacherCodes[100], teacherNames[100];
    ifstream teacherFile("teachers.txt");
    while (getline(teacherFile >> ws, teacherNames[numTeachers], '\n'))
    {
        getline(teacherFile, teacherCodes[numTeachers], '\n');
        numTeachers++;
    }
    teacherFile.close();
    for (int i = 0; i < numTeachers; i++)
    {
        if (teacherCodes[i] == name1)
        {
            cout << "User: " << teacherNames[i] << " with code " << teacherCodes[i] << " had " << countenter << " enter without exit  and " << countexit << " exit without  enter\n";
        }
    }
    file.close();
} // the end :)!