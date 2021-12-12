#pragma once

#include <QString>

constexpr int maxNameLength = 20;

using UserIdType = quint32;

struct UserData
{
    UserData():
        userId(0),
        countTests(0),
        countRightTasks(0),
        secInApp(0)
    {
        userName[0] = L'\0';
        userSurname[0] = L'\0';
    }

    void setNameAndSurname(const QString& name, const QString& surname);

    UserIdType userId;
    wchar_t userName[maxNameLength + 1];
    wchar_t userSurname[maxNameLength + 1];
    quint32 countTests;
    quint32 countRightTasks;
    quint64 secInApp;
};

