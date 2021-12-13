#include "usersmodel.h"
#include "functions.h"

UsersModel::UsersModel():
    roles(QAbstractTableModel::roleNames())
{
    roles[Roles::UserFullName] = "Имя пользователя";
    roles[Roles::UserCountTests] = "Количество решенных задач";
    roles[Roles::UserCountRightTasks] = "Правильно решенные задачи";
    roles[Roles::UserCountFailedTasks] = "Неверно решенные задачи";
    roles[Roles::UserTimeInApp] = "Время в приложении";
}

void UsersModel::setUsers(const Users &appUsers)
{
    if(!users.isEmpty())
    {
        auto currentSize = users.size();
        beginRemoveRows(QModelIndex(), 0, currentSize - 1);
        users.clear();
        endRemoveRows();
    }
    beginInsertRows(QModelIndex(), 0, appUsers.size()-1);
    users = appUsers;
    endInsertRows();
}

int UsersModel::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid())
    {
        return 0;
    }
    return users.size();
}

int UsersModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 5;
}

QVariant UsersModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
    {
        return QVariant();
    }

    if(role == Qt::DisplayRole)
    {
        int row = index.row();
        int col = UserFullName + index.column();
        const UserData& userData = users.at(row);

        switch (col)
        {
        case UserFullName:
            return userData.getFullName();
        case UserCountTests:
            return userData.countTests;
        case UserCountRightTasks:
            return userData.countRightTasks;
        case UserCountFailedTasks:
            return userData.countTests - userData.countRightTasks;
        case UserTimeInApp:
            return getTimeStrFromSec(userData.secInApp);
        default:
            break;
        }
    }
    return {};
}

QHash<int, QByteArray> UsersModel::roleNames() const
{
    return roles;
}

QVariant UsersModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
    {
        return {};
    }

    return (orientation == Qt::Horizontal) ? roles[UserFullName + section] : QString::number(section);
}
