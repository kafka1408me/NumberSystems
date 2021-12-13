#pragma once

/*************************************************
 *
 * UsersModel - табличная модель со всеми пользователями
 *
 *************************************************/

#include <QAbstractTableModel>
#include "userdata.h"


class UsersModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    // Перечисление ролей
    enum Roles{
        UserFullName = Qt::UserRole + 1, // Полное имя пользователя
        UserCountTasks,                  // Количество решенных задач
        UserCountRightTasks,             // Количество правильно решенных задач
        UserCountFailedTasks,            // Количество неправильно решеннхы задач
        UserTimeInApp                    // Время в приложении
    };

    UsersModel();

    /**
     * @brief setUsers - установить пользователей в модель
     * @param appUsers - контейнер пользователей
     */
    void setUsers(const Users& appUsers);

    /**
     * @brief rowCount - получить количество записей (строк) в модели
     * @param parent - родительский элемент
     * @return количество строк
     */
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    /**
     * @brief columnCount - получить количество столбцов в таблице
     * @param parent - родительский элемент
     * @return количество столбцов
     */
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    /**
     * @brief data - получить данные в ячейке
     * @param index - индекс ячейки (строка, столбец)
     * @param role - роль (аспект) данных
     * @return данные ячейки для указанной роли
     */
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    /**
     * @brief roleNames - получить роли и описание ролей модели
     * @return роли и описание ролей модели
     */
    QHash<int, QByteArray> roleNames() const override;

    /**
     * @brief headerData - получить название строк или столбцов
     * @param section - номер секции (строка или столбец)
     * @param orientation - ориентация (вертикальная или горизонтальная)
     * @param role - роль
     * @return название строки/столбца
     */
    QVariant headerData(int section, Qt::Orientation orientation,
                                    int role = Qt::DisplayRole) const override;
private:
    Users users;                  // Контейнер, содержащий всех пользователей
    QHash<int, QByteArray> roles; // Роли модели и соответствующее описание роли
};

