#pragma once

#include <QVector>
#include "userdata.h"

using Users = QVector<UserData>;


class FileHandler
{
public:
    static Users getUsers();

    static UserData authorizeUser(const QString& name, const QString& surname);

    static void addTime(UserIdType userId, UserTimeType sec);

    static void addTests(UserIdType userId, UserTestType countTests, UserTestType countRightTests);

private:
    // не используется
    // static void writeUserData(const UserData& userData);

private:
    FileHandler();
    FileHandler(const FileHandler&) = delete;
    FileHandler(FileHandler&&) = delete;
};


