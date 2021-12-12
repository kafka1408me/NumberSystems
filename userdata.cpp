#include "userdata.h"
#include <QDataStream>

inline void writeStringToArray(const QString& s, wchar_t* a)
{
    s.toWCharArray(a);
    a[s.size()] = L'\0';
}


void UserData::setNameAndSurname(const QString &name, const QString &surname)
{
    writeStringToArray(name, userName);
    writeStringToArray(surname, userSurname);
}
