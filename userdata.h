#pragma once

/*************************************************
 *
 * UserData - класс, содержащий информацию о пользователе
 *
 *************************************************/

#include <QString>

// Максимальная длина имени (и фамилии)
constexpr int maxNameLength = 20;

using UserIdType = quint32;
using UserTimeType = quint64;
using UserTestType = quint32;

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

    /**
     * @brief setNameAndSurname - установить имя и фамилию в объект
     * @param name - имя пользователя
     * @param surname - фамилия пользователя
     */
    void setNameAndSurname(const QString& name, const QString& surname);

    /**
     * @brief getFullName - получить полное имя пользователя (имя + фамилия)
     * @return полное имя пользователя
     */
    QString getFullName() const;

    UserIdType userId;                        // Идентификатор
    // Имя и фамилию записываем в массив определенного размера
    // для удобного перемещения по записям в файле
    // Используются 2х-байтовые символы (для записи букв русского алфавита)
    // +1 элемент для нулевого символа (символ завершения строки)
    wchar_t userName[maxNameLength + 1];      // Имя
    wchar_t userSurname[maxNameLength + 1];   // Фамилия
    UserTestType countTests;                  // Кол-во решенных задач
    UserTestType countRightTasks;             // Правильно решенные задачи
    UserTimeType secInApp;                    // Количество секунд, проведенных в приложении
};

using Users = QVector<UserData>;

