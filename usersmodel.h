#pragma once

#include <QAbstractTableModel>
#include "userdata.h"


class UsersModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum Roles{
        UserFullName = Qt::UserRole + 1,
        UserCountTests,
        UserCountRightTasks,
        UserCountFailedTasks,
        UserTimeInApp
    };

    UsersModel();

    void setUsers(const Users& appUsers);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

    QVariant headerData(int section, Qt::Orientation orientation,
                                    int role = Qt::DisplayRole) const override;
private:
    Users users;
    QHash<int, QByteArray> roles;
};

