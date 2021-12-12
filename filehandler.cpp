#include "filehandler.h"
#include <QFile>
#include <QDataStream>
#include <fstream>
#include <QDebug>

using namespace std;

const char* usersFileName = "D:\\users.bin";
constexpr auto offsetUserData = sizeof (UserData) - sizeof (UserData::userId);


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
        const int countUsers =  stream_size(f) / sizeof (UserData);
        for(int i = 0; i < countUsers; ++i)
        {
            f.read((char*)&userData.userId, sizeof (userData.userId));

            if(userData.userId == userId)
            {
                f.read((char*)&userData.userName, offsetUserData);
                return userData;
            }
            f.seekg(offsetUserData, ios::cur);
        }
        userData.userId = userId;
        userData.setNameAndSurname(name, surname);
        f.write((char*)&userData, sizeof (UserData));

        f.close();
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

