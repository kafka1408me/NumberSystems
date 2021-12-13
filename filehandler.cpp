#include "filehandler.h"
#include <fstream>

using namespace std;

// Название файла
const char* usersFileName = "users.bin";

// Отступ от id в структуре до ее конца
constexpr auto offsetUserData = sizeof (UserData) - sizeof (UserData::userId);

// Отступ от id в структуре до времени
constexpr long long offsetTime = offsetUserData - sizeof (UserData::secInApp);

// Отступ от id до количества решенных задач
constexpr long long offsetTests = offsetTime - 2*sizeof (UserTestType);


/**
 * @brief stream_size - функция для вычисления длины потока (размера файла)
 * @param f - поток ввода
 * @return длину потока
 */
std::streamoff stream_size(std::istream& f)
{
    // Запоминаем текущую позицию чтения
    std::istream::pos_type current_pos = f.tellg();
    if(current_pos == -1) // Если ошибка
    {
        return -1;
    }
    // Передвигаем курсор чтения в конец потока
    f.seekg(0, std::istream::end);
    // Получаем значения курсора в конце потока
    std::istream::pos_type end_pos = f.tellg();
    // Восстанавливаем позицию чтения
    f.seekg(current_pos);
    return end_pos - current_pos;  // Возвращаем разницу
}

/**
 * @brief findUserInFile - функция для нахождения пользователя в файле
 * @param f - ссылка на файл (функция предполагает, что файл уже открыт)
 * @param id - идентификатор пользователя
 * @return true, если запись о пользователе есть в файле
 */
bool findUserInFile(std::istream& f, UserIdType id)
{
    // кол-во пользователей в файле = размер_файла / размер_записи_об_одном_пользователе
    const int countUsers =  stream_size(f) / sizeof (UserData);
    UserIdType userId = 0;  // в эту переменную будем считывать id-шники из файла
    // Перебираем всех пользователей
    for(int i = 0; i < countUsers; ++i)
    {
        // Читаем id-шник пользоватееля из файла
        f.read((char*)&userId, sizeof (UserIdType));

        // Если считанный id-шник равен id искомому пользователю
        if(id == userId)
        {
            return true;
        }
        // Перемещаем курсор чтения к следующей записи пользователя
        f.seekg(offsetUserData, ios::cur);
    }
    return false;
}



Users FileHandler::getUsers()
{
    Users users;
    std::ifstream f(usersFileName,
                    ios::in | ios::binary  // Открываем для чтения бинарный файл
                    );
    if(f)  // Если файл удалось открыть
    {
        const int countUsers =  stream_size(f) / sizeof (UserData);
        // Меняем размер вектора, чтобы записать туда всех пользователей
        users.resize(countUsers);
        for(int i = 0; i < countUsers; ++i)
        {
            // Читаем данные пользователя
            f.read((char*)&users[i], sizeof (UserData));
        }

        // Закрываем файл
        f.close();
    }
    return users;
}

UserData FileHandler::authorizeUser(const QString &name, const QString &surname)
{
    // Идентификатор пользователя - результат применения фунции хэширования
    quint32 userId = qHash(name + '$' + surname);
    UserData userData;

    // Открываем ля записи и чтения бинарный файл;
    // флаг ios::app указывает, что запись должна вестись в конец файла
    std::fstream f(usersFileName,
                   ios::in | ios::out | ios::binary | ios::app
                   );
    if(f)   // Если файл удалось открыть
    {
        userData.userId = userId;
        bool result = findUserInFile(f, userId);
        if(result)  // Если пользователь уже есть в файле
        {
            // Читаем запись о пользователе, начиная с имени
            f.read((char*)&userData.userName, offsetUserData);
        }
        else
        {
            // Устанавливаем имя и фамилию пользователя в объект userData
            userData.setNameAndSurname(name, surname);
            // Записываем в конец файла данные пользователя
            f.write((char*)&userData, sizeof (UserData));
        }

        // Закрываем файл
        f.close();
        return userData;
    }
    else  // Если файл не открыт (его не существует)
    {
        // Открыть бинарный файл для записи
        f.open(usersFileName, ios::out | ios::binary);
        userData.userId = userId;
        userData.setNameAndSurname(name, surname);
        // Записываем данные пользователя в файл
        f.write((char*)&userData, sizeof (UserData));

        // Закрываем файл
        f.close();
    }

    return userData;
}


void FileHandler::addTime(UserIdType userId, UserTimeType sec)
{
    // Открываем бинарный файл для записи и чтения
    std::fstream f(usersFileName, ios::in | ios::out | ios::binary);
    if(f)    // Если файл удалось открыть
    {
        bool result = findUserInFile(f, userId);
        if(result)  // Если пользователь есть в файле
        {
            // Уже записанное время пользователя
            UserTimeType curUserTime = 0;
            // Перемещаем курсор для чтения
            // в позицию, где находится запись времени
            f.seekg(offsetTime, ios::cur);
            // Получаем текущую позицию в файле
            auto timePos = f.tellg();
            // Читаем время пользователя из файла
            f.read((char*)&curUserTime, sizeof (UserTimeType));

            // Перемещаем курсор записи к месту записи времени
            f.seekp(timePos, ios::beg);
            curUserTime += sec;
            f.write((char*)&curUserTime, sizeof (UserTimeType));
        }

        // Закрываем файл
        f.close();
    }
}

void FileHandler::addTests(UserIdType userId, UserTestType countTests, UserTestType countRightTests)
{
    // Открываем бинарный файл для записи и чтения
    std::fstream f(usersFileName, ios::in | ios::out | ios::binary);
    if(f)    // Если файл удалось открыть
    {
        bool result = findUserInFile(f, userId);
        if(result)    // Если пользователь есть в файле
        {
            // Массив с кол-вом решенных задач и
            // кол-вом правильно решенных задач
            UserTestType userTestsParams[2];
            // Перемещаем курсор к записям о решенных задачах
            f.seekg(offsetTests, ios::cur);
            auto posTests = f.tellg();  // Позиция в файле перед записью о решенных задачах

            // Считываем разом два числа: кол-во решенных задач и
            // кол-во правильно решенных задач
            f.read((char*)userTestsParams, sizeof (userTestsParams));

            // Увеличиваем кол-во задач
            userTestsParams[0] += countTests;
            userTestsParams[1] += countRightTests;

            // Перемещаем курсор записи к позиции перед
            // записью о решенных задач
            f.seekp(posTests, ios::beg);
            f.write((char*)userTestsParams, sizeof (userTestsParams));
        }
        // Закрываем файл
        f.close();
    }
}

