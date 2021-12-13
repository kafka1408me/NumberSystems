#pragma once

/*************************************************
 *
 * FileHandler - класс для работы с бинарным файлом,
 * содержащим данные пользователей
 *
 *************************************************/

#include <QVector>
#include "userdata.h"


class FileHandler
{
public:
    /**
     * @brief getUsers - функция для получения записей всех пользователей
     * @return контейнер, содержащий данные всех пользователей
     */
    static Users getUsers();

    /**
     * @brief authorizeUser - функция, вызываемая при авторизации пользователя
     * @param name - имя пользователя
     * @param surname - фамилия пользователя
     * @return данные пользователя
     */
    static UserData authorizeUser(const QString& name, const QString& surname);

    /**
     * @brief addTime - функция добавление времени (проведенного в приложении) пользователю
     * @param userId - идентицикатор пользователя
     * @param sec - добавляемое время в секундах
     */
    static void addTime(UserIdType userId, UserTimeType sec);

    /**
     * @brief addTests - функция для добавления пройденных заданий
     * (только тех заданий, которые были в тесте, который пользователь прошел, не прервав)
     * @param userId - идентификатор пользователя
     * @param countTests - количество новых решенных тестов
     * @param countRightTests - количество правильно решенных тестов
     */
    static void addTests(UserIdType userId, UserTestType countTests, UserTestType countRightTests);

private:
    // Объекты этого класса не будут создаваться,
    // поэтому удаляем все конструкторы
    FileHandler() = delete;
    FileHandler(const FileHandler&) = delete;  // Удаляем конструктор копирования
    FileHandler(FileHandler&&) = delete;       // Удаляем конструктор перемещения
};


