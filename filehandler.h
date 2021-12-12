#pragma once

#include <QVector>
#include "userdata.h"

using Users = QVector<UserData>;


class FileHandler
{
public:
    static Users getUsers();

    static UserData authorizeUser(const QString& name, const QString& surname);

    static void writeUserData(const UserData& userData);

private:
    FileHandler();
    FileHandler(const FileHandler&) = delete;
    FileHandler(FileHandler&&) = delete;
};


