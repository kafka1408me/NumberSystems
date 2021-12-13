#include "filehandler.h"
#include <QFile>
#include <QDataStream>
#include <fstream>
#include <QDebug>

using namespace std;

const char* usersFileName = "users.bin";
constexpr auto offsetUserData = sizeof (UserData) - sizeof (UserData::userId);
constexpr long long offsetTime = offsetUserData - sizeof (UserData::secInApp);
constexpr long long offsetTests = offsetTime - 2*sizeof (UserTestType);


// Вычисление длины потока
std::streamoff stream_size(std::istream& f)
{
    std::istream::pos_type current_pos = f.tellg();
    if(current_pos == -1)
    {
        return -1;
    }
    f.seekg(0, std::istream::end);
    std::istream::pos_type end_pos = f.tellg();
    f.seekg(current_pos);
    return end_pos - current_pos;
}

bool findUserInFile(std::istream& f, UserIdType id)
{
    const int countUsers =  stream_size(f) / sizeof (UserData);
    UserIdType userId = 0;
    for(int i = 0; i < countUsers; ++i)
    {
        f.read((char*)&userId, sizeof (UserIdType));

        if(id == userId)
        {
            return true;
        }
        f.seekg(offsetUserData, ios::cur);
    }
    return false;
}



Users FileHandler::getUsers()
{
    Users users;
    std::ifstream f(usersFileName, ios::in | ios::binary);
    if(f)
    {
        const int countUsers =  stream_size(f) / sizeof (UserData);
        users.resize(countUsers);
        for(int i = 0; i < countUsers; ++i)
        {
            f.read((char*)&users[i], sizeof (UserData));
        }

        f.close();
    }
    return users;
}

UserData FileHandler::authorizeUser(const QString &name, const QString &surname)
{
    quint32 userId = qHash(name + '$' + surname);
    UserData userData;

    std::fstream f(usersFileName, ios::in | ios::out | ios::binary | ios::app);
    if(f)
    {
        userData.userId = userId;
        bool result = findUserInFile(f, userId);
        if(result)
        {
            f.read((char*)&userData.userName, offsetUserData);
        }
        else
        {
            userData.setNameAndSurname(name, surname);
            f.write((char*)&userData, sizeof (UserData));
        }

        f.close();
        return userData;
    }
    else
    {
        f.open(usersFileName, ios::out | ios::binary);
        userData.userId = userId;
        userData.setNameAndSurname(name, surname);
        f.write((char*)&userData, sizeof (UserData));

        f.close();
    }

    return userData;
}

/*
void FileHandler::writeUserData(const UserData &userData)
{
    std::fstream f(usersFileName, ios::in | ios::out | ios::binary);
    if(f)
    {
        const int countUsers =  stream_size(f) / sizeof (UserData);
        quint32 id = 0;
        for(int i = 0; i < countUsers; ++i)
        {
            f.read((char*)&id, sizeof (UserIdType));

            if(id == userData.userId)
            {
                auto curPos = f.tellg();
                f.seekp(curPos, ios::beg);
                f.write((char*)&userData.userName, offsetUserData);
                return;
            }
            f.seekg(offsetUserData, ios::cur);
        }
        f.close();
    }
}
*/

void FileHandler::addTime(UserIdType userId, UserTimeType sec)
{
    std::fstream f(usersFileName, ios::in | ios::out | ios::binary);
    if(f)
    {
        bool result = findUserInFile(f, userId);
        if(result)
        {
            UserTimeType curUserTime = 0;
            f.seekg(offsetTime, ios::cur);
            auto timePos = f.tellg();
            f.read((char*)&curUserTime, sizeof (UserTimeType));

            f.seekp(timePos, ios::beg);
            curUserTime += sec;
            f.write((char*)&curUserTime, sizeof (UserTimeType));
        }

        f.close();
    }
}

void FileHandler::addTests(UserIdType userId, UserTestType countTests, UserTestType countRightTests)
{
    std::fstream f(usersFileName, ios::in | ios::out | ios::binary);
    if(f)
    {
        bool result = findUserInFile(f, userId);
        if(result)
        {
            UserTestType userTestsParams[2];
            f.seekg(offsetTests, ios::cur);
            auto posTests = f.tellg();
            f.read((char*)userTestsParams, sizeof (userTestsParams));

            userTestsParams[0] += countTests;
            userTestsParams[1] += countRightTests;

            f.seekp(posTests, ios::beg);
            f.write((char*)userTestsParams, sizeof (userTestsParams));
        }
        f.close();
    }
}

