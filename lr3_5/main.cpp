#include <iostream>
#include <cctype>
#include <cstdlib>
#include <vector>
#include <string>
#include <Windows.h>
#include <stdexcept>

using namespace std;

// =====================================================
// ДОПОМІЖНІ ФУНКЦІЇ ПЕРЕВІРКИ
// =====================================================

bool validDate(const string& date)
{
    if (date.length() != 10 ||
        date[4] != '-' || date[7] != '-')
        return false;

    for (int i = 0; i < 10; i++)
    {
        if (i != 4 && i != 7 && !isdigit(date[i]))
            return false;
    }

    int year = stoi(date.substr(0, 4));
    int month = stoi(date.substr(5, 2));
    int day = stoi(date.substr(8, 2));

    if (year < 1900 || month < 1 || month > 12)
        return false;

    int daysInMonth[] =
    {
        31, 28, 31, 30, 31, 30,
        31, 31, 30, 31, 30, 31
    };

    bool leapYear =
        (year % 400 == 0) ||
        (year % 4 == 0 && year % 100 != 0);

    if (leapYear)
        daysInMonth[1] = 29;

    return day >= 1 && day <= daysInMonth[month - 1];
}

bool validTime(const string& time)
{
    if (time.length() != 5 || time[2] != ':')
        return false;

    if (!isdigit(time[0]) || !isdigit(time[1]) ||
        !isdigit(time[3]) || !isdigit(time[4]))
        return false;

    int hours = stoi(time.substr(0, 2));
    int minutes = stoi(time.substr(3, 2));

    return hours >= 0 && hours <= 23 &&
        minutes >= 0 && minutes <= 59;
}

void errorExit(const string& message)
{
    cerr << "\nПомилка: " << message << endl;
    exit(1);
}

// =====================================================
// ШАБЛОННИЙ КЛАС ARRAY
// =====================================================

template <class T>
class Array
{
protected:
    static const int MAX_SIZE = 100;

    vector<T> values;          // Об'єкти зберігаються в динамічній пам'яті
    int elements[MAX_SIZE];    // 0 або 1 — наявність у множині

    int count;
    int universeCount;

public:

    // Конструктор без параметрів
    Array()
    {
        count = 0;
        universeCount = 0;

        for (int i = 0; i < MAX_SIZE; i++)
            elements[i] = 0;

        values.reserve(MAX_SIZE);
    }

    // Конструктор з параметрами
    Array(int size, const T& value = T())
    {
        if (size < 0 || size > MAX_SIZE)
            errorExit("Некоректний розмір множини!");

        count = 0;
        universeCount = 0;

        for (int i = 0; i < MAX_SIZE; i++)
            elements[i] = 0;

        values.reserve(MAX_SIZE);

        for (int i = 0; i < size; i++)
            AddElement(value);
    }

    // Конструктор копіювання
    Array(const Array& other)
    {
        values = other.values;

        count = other.count;
        universeCount = other.universeCount;

        for (int i = 0; i < MAX_SIZE; i++)
            elements[i] = other.elements[i];
    }

    // Оператор присвоювання
    Array& operator=(const Array& other)
    {
        if (this != &other)
        {
            values = other.values;

            count = other.count;
            universeCount = other.universeCount;

            for (int i = 0; i < MAX_SIZE; i++)
                elements[i] = other.elements[i];
        }

        return *this;
    }

    // Додавання елемента
    bool AddElement(const T& value)
    {
        for (int i = 0; i < universeCount; i++)
        {
            if (values[i] == value)
            {
                if (elements[i] == 0)
                {
                    elements[i] = 1;
                    count++;
                    return true;
                }

                return false;
            }
        }

        if (universeCount < 0 || universeCount >= MAX_SIZE)
        {
            cerr << "Помилка: множина переповнена!" << endl;
            exit(EXIT_FAILURE);
        }

        values.push_back(value);
        elements[universeCount] = 1;

        universeCount++;
        count++;

        return true;
    }

    // Видалення елемента за логічним індексом
    void RemoveElementAt(int index)
    {
        if (!rangeCheck(index))
            errorExit("Некоректний індекс!");

        int current = 0;

        for (int i = 0; i < universeCount; i++)
        {
            if (elements[i] == 1)
            {
                if (current == index)
                {
                    elements[i] = 0;
                    count--;
                    return;
                }

                current++;
            }
        }
    }

    // Перевірка наявності елемента
    bool ContainsElement(const T& value) const
    {
        for (int i = 0; i < universeCount; i++)
        {
            if (elements[i] == 1 && values[i] == value)
                return true;
        }

        return false;
    }

    // Перевірка індексу
    bool rangeCheck(int index) const
    {
        return index >= 0 && index < count;
    }

    // Доступ до елемента за логічним індексом
    T& operator[](int index)
    {
        if (!rangeCheck(index))
            errorExit("Індекс виходить за межі множини!");

        int current = 0;

        for (int i = 0; i < universeCount; i++)
        {
            if (elements[i] == 1)
            {
                if (current == index)
                    return values[i];

                current++;
            }
        }

        errorExit("Елемент не знайдено!");
        return values[0];
    }

    // Константний доступ до елемента
    const T& operator[](int index) const
    {
        if (!rangeCheck(index))
            errorExit("Індекс виходить за межі множини!");

        int current = 0;

        for (int i = 0; i < universeCount; i++)
        {
            if (elements[i] == 1)
            {
                if (current == index)
                    return values[i];

                current++;
            }
        }

        errorExit("Елемент не знайдено!");
        return values[0];
    }

    // Кількість елементів множини
    int GetCount() const
    {
        return count;
    }

    // Виведення масиву 0 та 1
    void DisplayArray() const
    {
        cout << "Масив множини: ";

        for (int i = 0; i < MAX_SIZE; i++)
            cout << elements[i] << " ";

        cout << endl;
    }

    virtual ~Array()
    {
    }
};

// =====================================================
// КЛАС FILEINFO
// =====================================================

class FileInfo
{
private:
    string name;
    string extension;
    string creationDate;
    string creationTime;

    bool readOnly;
    bool hidden;
    bool system;

    long long size;

public:

    // Конструктор без параметрів
    FileInfo()
    {
        SetName("Unknown");
        SetExtension("");
        SetCreationDate("2000-01-01");
        SetCreationTime("00:00");
        SetReadOnly(0);
        SetHidden(0);
        SetSystem(0);
        SetSize(0);
    }

    // Конструктор з параметрами
    FileInfo(string n, string ext, string date,
        string time, int ro, int h, int s, long long sz)
    {
        SetName(n);
        SetExtension(ext);
        SetCreationDate(date);
        SetCreationTime(time);
        SetReadOnly(ro);
        SetHidden(h);
        SetSystem(s);
        SetSize(sz);
    }

    // Методи встановлення значень

    void SetName(const string& value)
    {
        if (value.empty() ||
            value.find_first_of("/\\.") != string::npos)
            errorExit("Некоректна назва файлу!");

        name = value;
    }

    void SetExtension(const string& value)
    {
        if (value.find_first_of("/\\.") != string::npos)
            errorExit("Некоректне розширення файлу!");

        extension = value;
    }

    void SetCreationDate(const string& value)
    {
        if (!validDate(value))
            errorExit("Некоректна дата створення!");

        creationDate = value;
    }

    void SetCreationTime(const string& value)
    {
        if (!validTime(value))
            errorExit("Некоректний час створення!");

        creationTime = value;
    }

    void SetReadOnly(int value)
    {
        if (value != 0 && value != 1)
            errorExit("Атрибут ReadOnly повинен бути 0 або 1!");

        readOnly = (value == 1);
    }

    void SetHidden(int value)
    {
        if (value != 0 && value != 1)
            errorExit("Атрибут Hidden повинен бути 0 або 1!");

        hidden = (value == 1);
    }

    void SetSystem(int value)
    {
        if (value != 0 && value != 1)
            errorExit("Атрибут System повинен бути 0 або 1!");

        system = (value == 1);
    }

    void SetSize(long long value)
    {
        if (value < 0)
            errorExit("Розмір файлу не може бути від'ємним!");

        size = value;
    }

    // Методи отримання значень

    string GetName() const { return name; }
    string GetExtension() const { return extension; }
    string GetCreationDate() const { return creationDate; }
    string GetCreationTime() const { return creationTime; }

    bool GetReadOnly() const { return readOnly; }
    bool GetHidden() const { return hidden; }
    bool GetSystem() const { return system; }

    long long GetSize() const { return size; }

    // Перетворення об'єкта в рядок
    operator string() const
    {
        return name + "." + extension;
    }

    // Порівняння файлів за назвою, розширенням,
    // датою та часом створення
    bool operator==(const FileInfo& other) const
    {
        return name == other.name &&
            extension == other.extension &&
            creationDate == other.creationDate &&
            creationTime == other.creationTime;
    }

    // Операція введення
    friend istream& operator>>(istream& in, FileInfo& file)
    {
        string n, ext, date, time;
        int ro, h, s;
        long long sz;

        cout << "Назва файлу: ";
        in >> n;

        cout << "Розширення: ";
        in >> ext;

        cout << "Дата створення (РРРР-ММ-ДД): ";
        in >> date;

        cout << "Час створення (ГГ:ХХ): ";
        in >> time;

        cout << "Тільки читання (0/1): ";
        in >> ro;

        cout << "Прихований (0/1): ";
        in >> h;

        cout << "Системний (0/1): ";
        in >> s;

        cout << "Розмір файлу: ";
        in >> sz;

        if (!in)
            return in;

        try
        {
            file.SetName(n);
            file.SetExtension(ext);
            file.SetCreationDate(date);
            file.SetCreationTime(time);
            file.SetReadOnly(ro);
            file.SetHidden(h);
            file.SetSystem(s);
            file.SetSize(sz);
        }
        catch (...)
        {
            in.setstate(ios::failbit);
        }

        return in;
    }

    // Операція виведення
    friend ostream& operator<<(ostream& out, const FileInfo& file)
    {
        out << "Назва: " << file.name << "." << file.extension << endl;
        out << "Дата створення: " << file.creationDate << endl;
        out << "Час створення: " << file.creationTime << endl;
        out << "Тільки читання: " << file.readOnly << endl;
        out << "Прихований: " << file.hidden << endl;
        out << "Системний: " << file.system << endl;
        out << "Розмір: " << file.size << " байт" << endl;

        return out;
    }
};

// =====================================================
// КЛАС FILEENTRY
// =====================================================

class FileEntry
{
private:
    FileInfo info;
    string modificationDate;
    bool selected;
    bool deleted;

public:

    // Конструктор без параметрів
    FileEntry()
    {
        SetInfo(FileInfo());
        SetModificationDate("2000-01-01");
        SetSelected(0);
        SetDeleted(0);
    }

    // Конструктор з параметрами
    FileEntry(const FileInfo& f, string date,
        int sel, int del)
    {
        SetInfo(f);
        SetModificationDate(date);
        SetSelected(sel);
        SetDeleted(del);
    }

    // Методи встановлення

    void SetInfo(const FileInfo& value)
    {
        info = value;
    }

    void SetModificationDate(const string& value)
    {
        if (!validDate(value))
            errorExit("Некоректна дата модифікації!");

        modificationDate = value;
    }

    void SetSelected(int value)
    {
        if (value != 0 && value != 1)
            errorExit("Selected повинен бути 0 або 1!");

        selected = (value == 1);
    }

    void SetDeleted(int value)
    {
        if (value != 0 && value != 1)
            errorExit("Deleted повинен бути 0 або 1!");

        deleted = (value == 1);
    }

    // Методи отримання

    FileInfo GetInfo() const 
    { 
        return info; 
    }

    string GetModificationDate() const 
    { 
        return modificationDate; 
    }

    bool GetSelected() const 
    { 
        return selected; 
    }

    bool GetDeleted() const 
    { 
        return deleted; 
    }

    // Перетворення в рядок
    operator string() const
    {
        return string(info) + " | " + modificationDate;
    }

    // Порівняння записів за інформацією про файл
    bool operator==(const FileEntry& other) const
    {
        return info == other.info;
    }

    // Введення
    friend istream& operator>>(istream& in, FileEntry& entry)
    {
        FileInfo temp;

        in >> temp;

        if (!in)
            return in;

        string date;
        int sel, del;

        cout << "Дата модифікації (РРРР-ММ-ДД): ";
        in >> date;

        cout << "Вибраний (0/1): ";
        in >> sel;

        cout << "Видалений (0/1): ";
        in >> del;

        if (!in)
            return in;

        try
        {
            entry.SetInfo(temp);
            entry.SetModificationDate(date);
            entry.SetSelected(sel);
            entry.SetDeleted(del);
        }
        catch (...)
        {
            in.setstate(ios::failbit);
        }

        return in;
    }

    // Виведення
    friend ostream& operator<<(ostream& out, const FileEntry& entry)
    {
        out << entry.info;
        out << "Дата модифікації: "
            << entry.modificationDate << endl;
        out << "Вибраний: " << entry.selected << endl;
        out << "Видалений: " << entry.deleted << endl;

        return out;
    }
};

// =====================================================
// КЛАС DIRECTORY
// =====================================================

class Directory : public Array<FileEntry>
{
private:
    string parentName;

public:

    // Конструктор без параметрів
    Directory() : Array<FileEntry>()
    {
        parentName = "Root";
    }

    // Конструктор з назвою каталогу
    Directory(const string& name) : Array<FileEntry>()
    {
        SetParentName(name);
    }

    // Конструктор з розміром та однаковими елементами
    Directory(const string& name, int size,
        const FileEntry& value)
        : Array<FileEntry>(size, value)
    {
        SetParentName(name);
    }

    // Конструктор копіювання
    Directory(const Directory& other)
        : Array<FileEntry>(other)
    {
        parentName = other.parentName;
    }

    // Оператор присвоювання
    Directory& operator=(const Directory& other)
    {
        if (this != &other)
        {
            Array<FileEntry>::operator=(other);
            parentName = other.parentName;
        }

        return *this;
    }

    // Встановлення назви каталогу
    void SetParentName(const string& value)
    {
        if (value.empty())
            errorExit("Назва каталогу не може бути порожньою!");

        parentName = value;
    }

    // Отримання назви каталогу
    string GetParentName() const
    {
        return parentName;
    }

    // Додавання файлу
    void AddFile(const FileEntry& file)
    {
        if (!AddElement(file))
            cout << "Такий файл уже існує!" << endl;
    }

    // Фізичне видалення файлу за індексом
    void DeleteFile(int index)
    {
        RemoveElementAt(index);
    }

    // Логічне позначення файлу як видаленого
    void MarkDeleted(int index)
    {
        if (!rangeCheck(index))
            errorExit("Некоректний індекс файлу!");

        (*this)[index].SetDeleted(1);
    }

    // Пошук за назвою
    int SearchByName(const string& name) const
    {
        for (int i = 0; i < count; i++)
        {
            if ((*this)[i].GetInfo().GetName() == name)
                return i;
        }

        return -1;
    }

    // Пошук за розширенням
    int SearchByExtension(const string& extension) const
    {
        for (int i = 0; i < count; i++)
        {
            if ((*this)[i].GetInfo().GetExtension() == extension)
                return i;
        }

        return -1;
    }

    // Пошук за датою створення
    int SearchByDate(const string& date) const
    {
        if (!validDate(date))
            errorExit("Некоректна дата пошуку!");

        for (int i = 0; i < count; i++)
        {
            if ((*this)[i].GetInfo().GetCreationDate() == date)
                return i;
        }

        return -1;
    }

    // Обчислення загального об'єму каталогу
    // Враховуються файли, які не позначені видаленими
    long long TotalSize() const
    {
        long long total = 0;

        for (int i = 0; i < count; i++)
            total += (*this)[i].GetInfo().GetSize();

        return total;
    }

    // Перевірка наявності файлу
    bool Contains(const FileEntry& file) const
    {
        return ContainsElement(file);
    }

    // Об'єднання каталогів
    Directory operator+(const Directory& other) const
    {
        Directory result("Об'єднаний каталог");

        for (int i = 0; i < count; i++)
            result.AddFile((*this)[i]);

        for (int i = 0; i < other.count; i++)
            result.AddFile(other[i]);

        return result;
    }

    // Перетин каталогів
    Directory operator*(const Directory& other) const
    {
        Directory result("Перетин каталогів");

        for (int i = 0; i < count; i++)
        {
            if (other.Contains((*this)[i]))
                result.AddFile((*this)[i]);
        }

        return result;
    }

    // Різниця каталогів
    Directory operator-(const Directory& other) const
    {
        Directory result("Різниця каталогів");

        for (int i = 0; i < count; i++)
        {
            if (!other.Contains((*this)[i]))
                result.AddFile((*this)[i]);
        }

        return result;
    }

    // Перетворення каталогу в рядок
    operator string() const
    {
        return "Каталог: " + parentName +
            ", кількість файлів: " + to_string(count);
    }

    // Виведення каталогу
    void Display() const
    {
        cout << "\n==================================" << endl;
        cout << "Каталог: " << parentName << endl;
        cout << "Кількість файлів: " << count << endl;
        cout << "==================================" << endl;

        for (int i = 0; i < count; i++)
        {
            cout << "\nФайл №" << i + 1 << endl;
            cout << (*this)[i] << endl;
        }
    }

    // Операція введення каталогу
    friend istream& operator>>(istream& in, Directory& directory)
    {
        string name;
        int size;

        cout << "Назва каталогу: ";
        in >> name;

        cout << "Кількість файлів: ";
        in >> size;

        if (!in)
            return in;

        if (size < 0 || size > MAX_SIZE)
        {
            in.setstate(ios::failbit);
            return in;
        }

        Directory temp(name);

        for (int i = 0; i < size; i++)
        {
            cout << "\nВведення файлу №" << i + 1 << endl;

            FileEntry file;
            in >> file;

            if (!in)
                return in;

            temp.AddFile(file);
        }

        directory = temp;

        return in;
    }

    // Операція виведення каталогу
    friend ostream& operator<<(ostream& out, const Directory& directory)
    {
        out << "Каталог: " << directory.parentName << endl;
        out << "Кількість файлів: " << directory.count << endl;

        for (int i = 0; i < directory.count; i++)
        {
            out << "\nФайл №" << i + 1 << endl;
            out << directory[i] << endl;
        }

        return out;
    }
};

// =====================================================
// КЛАС GROUP
// =====================================================

class Group : public Array<FileEntry>
{
private:
    string groupName;

public:

    // Конструктор без параметрів
    Group() : Array<FileEntry>()
    {
        groupName = "Без назви";
    }

    // Конструктор з параметрами
    Group(const string& name) : Array<FileEntry>()
    {
        SetGroupName(name);
    }

    // Встановлення назви групи
    void SetGroupName(const string& value)
    {
        if (value.empty())
            errorExit("Назва групи не може бути порожньою!");

        groupName = value;
    }

    // Отримання назви групи
    string GetGroupName() const
    {
        return groupName;
    }

    // Додавання елемента до групи
    void Add(const FileEntry& file)
    {
        if (!AddElement(file))
            return;
    }

    // Формування групи за ознакою видалення
    static Group ByDeleted(const Directory& dir, bool deleted)
    {
        Group result("Група за ознакою видалення");

        for (int i = 0; i < dir.GetCount(); i++)
        {
            if (dir[i].GetDeleted() == deleted)
                result.Add(dir[i]);
        }

        return result;
    }

    // Формування групи за атрибутом
    // 1 - ReadOnly
    // 2 - Hidden
    // 3 - System
    static Group ByAttribute(const Directory& dir, int attribute)
    {
        if (attribute < 1 || attribute > 3)
            errorExit("Невідомий атрибут!");

        Group result("Група за атрибутом");

        for (int i = 0; i < dir.GetCount(); i++)
        {
            FileInfo info = dir[i].GetInfo();

            bool condition = false;

            if (attribute == 1)
                condition = info.GetReadOnly();

            if (attribute == 2)
                condition = info.GetHidden();

            if (attribute == 3)
                condition = info.GetSystem();

            if (condition)
                result.Add(dir[i]);
        }

        return result;
    }

    // Групування за датою
    // before = true  - до вказаної дати
    // before = false - після вказаної дати
    static Group ByDate(const Directory& dir,
        const string& date, bool before)
    {
        if (!validDate(date))
            errorExit("Некоректна дата!");

        Group result("Група за датою");

        for (int i = 0; i < dir.GetCount(); i++)
        {
            string fileDate =
                dir[i].GetInfo().GetCreationDate();

            if (before && fileDate < date)
                result.Add(dir[i]);

            if (!before && fileDate > date)
                result.Add(dir[i]);
        }

        return result;
    }

    // Групування за розміром
    // less = true  - менші за вказаний розмір
    // less = false - більші за вказаний розмір
    static Group BySize(const Directory& dir,
        long long size, bool less)
    {
        if (size < 0)
            errorExit("Розмір не може бути від'ємним!");

        Group result("Група за розміром");

        for (int i = 0; i < dir.GetCount(); i++)
        {
            long long fileSize =
                dir[i].GetInfo().GetSize();

            if (less && fileSize < size)
                result.Add(dir[i]);

            if (!less && fileSize > size)
                result.Add(dir[i]);
        }

        return result;
    }

    // Перетворення групи в рядок
    operator string() const
    {
        return "Група: " + groupName +
            ", кількість файлів: " + to_string(count);
    }

    // Виведення групи
    void Display() const
    {
        cout << "\n==================================" << endl;
        cout << "Група: " << groupName << endl;
        cout << "Кількість файлів: " << count << endl;
        cout << "==================================" << endl;

        for (int i = 0; i < count; i++)
        {
            cout << "\nФайл №" << i + 1 << endl;
            cout << (*this)[i] << endl;
        }
    }

    // Оператор виведення
    friend ostream& operator<<(ostream& out, const Group& group)
    {
        out << "Група: " << group.groupName << endl;
        out << "Кількість файлів: " << group.count << endl;

        for (int i = 0; i < group.count; i++)
        {
            out << "\nФайл №" << i + 1 << endl;
            out << group[i] << endl;
        }

        return out;
    }
};


int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    Directory dir1;
    Directory dir2;

    // ==========================================
    // 1. ВВЕДЕННЯ ДВОХ КАТАЛОГІВ
    // ==========================================

    cout << "========== ВВЕДЕННЯ ПЕРШОГО КАТАЛОГУ ==========\n";
    cin >> dir1;

    if (!cin)
        errorExit("Помилка введення першого каталогу!");

    cout << "\n========== ВВЕДЕННЯ ДРУГОГО КАТАЛОГУ ==========\n";
    cin >> dir2;

    if (!cin)
        errorExit("Помилка введення другого каталогу!");

    // ==========================================
    // 2. ВИВЕДЕННЯ КАТАЛОГІВ
    // ==========================================

    cout << "\n========== ПЕРШИЙ КАТАЛОГ ==========\n";
    dir1.Display();

    cout << "\n========== ДРУГИЙ КАТАЛОГ ==========\n";
    dir2.Display();

    cout << "\nМасив першого каталогу:\n";
    dir1.DisplayArray();

    cout << "\nМасив другого каталогу:\n";
    dir2.DisplayArray();

    // ==========================================
    // 3. ОБЧИСЛЕННЯ ОБ'ЄМУ
    // ==========================================

    cout << "\n========== ОБ'ЄМ КАТАЛОГІВ ==========\n";

    cout << "Об'єм першого каталогу: "
        << dir1.TotalSize() << " байт\n";

    cout << "Об'єм другого каталогу: "
        << dir2.TotalSize() << " байт\n";

    // ==========================================
    // 4. ПОШУК ФАЙЛІВ
    // ==========================================

    int directoryChoice;
    int index;

    cout << "\n========== ПОШУК ФАЙЛІВ ==========\n";

    cout << "У якому каталозі виконати пошук?\n";
    cout << "1 - Перший каталог\n";
    cout << "2 - Другий каталог\n";
    cout << "Ваш вибір: ";
    cin >> directoryChoice;

    if (!cin || (directoryChoice != 1 && directoryChoice != 2))
        errorExit("Некоректний вибір каталогу!");

    Directory& searchDir =
        (directoryChoice == 1) ? dir1 : dir2;

    int searchChoice;

    cout << "\nВиберіть спосіб пошуку:\n";
    cout << "1 - За іменем\n";
    cout << "2 - За розширенням\n";
    cout << "3 - За датою створення\n";
    cout << "Ваш вибір: ";
    cin >> searchChoice;

    if (!cin || searchChoice < 1 || searchChoice > 3)
        errorExit("Некоректний спосіб пошуку!");

    if (searchChoice == 1)
    {
        string name;

        cout << "Введіть ім'я файлу: ";
        cin >> name;

        index = searchDir.SearchByName(name);
    }
    else if (searchChoice == 2)
    {
        string extension;

        cout << "Введіть розширення: ";
        cin >> extension;

        index = searchDir.SearchByExtension(extension);
    }
    else
    {
        string date;

        cout << "Введіть дату створення (РРРР-ММ-ДД): ";
        cin >> date;

        index = searchDir.SearchByDate(date);
    }

    if (index != -1)
    {
        cout << "\nФайл знайдено!\n";
        cout << searchDir[index];
    }
    else
    {
        cout << "\nФайл не знайдено!\n";
    }

    // ==========================================
    // 5. ДОДАВАННЯ ФАЙЛУ
    // ==========================================

    int operation;

    cout << "\n========== ДОДАВАННЯ ФАЙЛУ ==========\n";
    cout << "1 - Додати файл до першого каталогу\n";
    cout << "2 - Додати файл до другого каталогу\n";
    cout << "0 - Пропустити\n";
    cout << "Ваш вибір: ";
    cin >> operation;

    if (!cin || operation < 0 || operation > 2)
        errorExit("Некоректний вибір!");

    if (operation != 0)
    {
        FileEntry newFile;

        cout << "\nВведіть інформацію про файл:\n";
        cin >> newFile;

        if (!cin)
            errorExit("Помилка введення файлу!");

        if (operation == 1)
            dir1.AddFile(newFile);
        else
            dir2.AddFile(newFile);

        cout << "\nКаталог після додавання:\n";

        if (operation == 1)
            dir1.Display();
        else
            dir2.Display();
    }

    // ==========================================
    // 6. ВИДАЛЕННЯ ФАЙЛУ
    // ==========================================

    cout << "\n========== ВИДАЛЕННЯ ФАЙЛУ ==========\n";

    cout << "1 - Видалити з першого каталогу\n";
    cout << "2 - Видалити з другого каталогу\n";
    cout << "0 - Пропустити\n";
    cout << "Ваш вибір: ";
    cin >> operation;

    if (!cin || operation < 0 || operation > 2)
        errorExit("Некоректний вибір!");

    if (operation != 0)
    {
        Directory& deleteDir =
            (operation == 1) ? dir1 : dir2;

        if (deleteDir.GetCount() == 0)
        {
            cout << "Каталог порожній!\n";
        }
        else
        {
            int deleteIndex;

            cout << "Введіть номер файлу для видалення (від 0 до "
                << deleteDir.GetCount() - 1 << "): ";

            cin >> deleteIndex;

            if (!cin || !deleteDir.rangeCheck(deleteIndex))
                errorExit("Некоректний індекс!");

            deleteDir.DeleteFile(deleteIndex);

            cout << "\nКаталог після видалення:\n";
            deleteDir.Display();
        }
    }

    // ==========================================
    // 7. ПОВТОРНЕ ОБЧИСЛЕННЯ ОБ'ЄМУ
    // ==========================================

    cout << "\n========== ОБ'ЄМ ПІСЛЯ ЗМІН ==========\n";

    cout << "Перший каталог: "
        << dir1.TotalSize() << " байт\n";

    cout << "Другий каталог: "
        << dir2.TotalSize() << " байт\n";

    // ==========================================
    // 8. ОБ'ЄДНАННЯ КАТАЛОГІВ
    // ==========================================

    cout << "\n========== ОБ'ЄДНАННЯ КАТАЛОГІВ ==========\n";

    Directory united = dir1 + dir2;

    united.Display();

    cout << "\nМасив об'єднаного каталогу:\n";
    united.DisplayArray();

    // ==========================================
    // 9. ПЕРЕТИН КАТАЛОГІВ
    // ==========================================

    cout << "\n========== ПЕРЕТИН КАТАЛОГІВ ==========\n";

    Directory intersection = dir1 * dir2;

    intersection.Display();

    cout << "\nМасив перетину:\n";
    intersection.DisplayArray();

    // ==========================================
    // 10. РІЗНИЦЯ КАТАЛОГІВ
    // ==========================================

    cout << "\n========== РІЗНИЦЯ КАТАЛОГІВ ==========\n";

    Directory difference = dir1 - dir2;

    difference.Display();

    cout << "\nМасив різниці:\n";
    difference.DisplayArray();

    // ==========================================
    // 11. ГРУПУВАННЯ ФАЙЛІВ
    // ==========================================

    cout << "\n========== ГРУПУВАННЯ ФАЙЛІВ ==========\n";

    cout << "З якого каталогу створити групу?\n";
    cout << "1 - Перший каталог\n";
    cout << "2 - Другий каталог\n";
    cout << "3 - Об'єднаний каталог\n";
    cout << "Ваш вибір: ";

    int groupDirectory;
    cin >> groupDirectory;

    if (!cin || groupDirectory < 1 || groupDirectory > 3)
        errorExit("Некоректний вибір каталогу!");

    Directory& groupSource =
        (groupDirectory == 1) ? dir1 :
        (groupDirectory == 2) ? dir2 : united;

    cout << "\nВиберіть критерій групування:\n";
    cout << "1 - За ознакою видалення\n";
    cout << "2 - За атрибутом\n";
    cout << "3 - За датою створення\n";
    cout << "4 - За розміром файлу\n";
    cout << "Ваш вибір: ";

    int groupChoice;
    cin >> groupChoice;

    if (!cin || groupChoice < 1 || groupChoice > 4)
        errorExit("Некоректний критерій групування!");

    Group resultGroup;

    if (groupChoice == 1)
    {
        int deleted;

        cout << "Виберіть файли:\n";
        cout << "0 - Не видалені\n";
        cout << "1 - Видалені\n";
        cout << "Ваш вибір: ";

        cin >> deleted;

        if (!cin || (deleted != 0 && deleted != 1))
            errorExit("Некоректне значення!");

        resultGroup = Group::ByDeleted(
            groupSource, deleted == 1
        );
    }
    else if (groupChoice == 2)
    {
        int attribute;

        cout << "Виберіть атрибут:\n";
        cout << "1 - Тільки читання (ReadOnly)\n";
        cout << "2 - Прихований (Hidden)\n";
        cout << "3 - Системний (System)\n";
        cout << "Ваш вибір: ";

        cin >> attribute;

        if (!cin || attribute < 1 || attribute > 3)
            errorExit("Некоректний атрибут!");

        resultGroup = Group::ByAttribute(
            groupSource, attribute
        );
    }
    else if (groupChoice == 3)
    {
        string date;
        int direction;

        cout << "Введіть дату (РРРР-ММ-ДД): ";
        cin >> date;

        if (!validDate(date))
            errorExit("Некоректна дата!");

        cout << "1 - Файли до цієї дати\n";
        cout << "2 - Файли після цієї дати\n";
        cout << "Ваш вибір: ";

        cin >> direction;

        if (!cin || (direction != 1 && direction != 2))
            errorExit("Некоректний вибір!");

        resultGroup = Group::ByDate(
            groupSource, date, direction == 1
        );
    }
    else
    {
        long long size;
        int direction;

        cout << "Введіть розмір у байтах: ";
        cin >> size;

        if (!cin || size < 0)
            errorExit("Некоректний розмір!");

        cout << "1 - Файли менші за цей розмір\n";
        cout << "2 - Файли більші за цей розмір\n";
        cout << "Ваш вибір: ";

        cin >> direction;

        if (!cin || (direction != 1 && direction != 2))
            errorExit("Некоректний вибір!");

        resultGroup = Group::BySize(
            groupSource, size, direction == 1
        );
    }

    cout << "\n========== РЕЗУЛЬТАТ ГРУПУВАННЯ ==========\n";

    resultGroup.Display();

    cout << "\n========================================\n";
    cout << "РОБОТУ ПРОГРАМИ ЗАВЕРШЕНО\n";
    cout << "========================================\n";

    return 0;
}

