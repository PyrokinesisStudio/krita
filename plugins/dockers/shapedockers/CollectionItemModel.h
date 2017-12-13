/* This file is part of the KDE project
 * Copyright (C) 2008 Peter Simonsson <peter.simonsson@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */
#ifndef KIVIOSHAPETEMPLATEMODEL_H
#define KIVIOSHAPETEMPLATEMODEL_H

#include <KoShape.h>

#include <QAbstractItemModel>
#include <QList>
#include <QString>
#include <QIcon>

class KoProperties;

/**
 * Struct containing the information stored in CollectionItemModel item
 */
struct KoCollectionItem {
    KoCollectionItem()
    {
        properties = 0;
    };

    QString id;
    QString name;
    QString toolTip;
    QIcon icon;
    const KoProperties *properties;
};

class CollectionItemModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit CollectionItemModel(QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QMimeData *mimeData(const QModelIndexList &indexes) const override;
    QStringList mimeTypes() const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    /**
     * Set the list of KoCollectionItem to be stored in the model
     */
    void setShapeTemplateList(const QList<KoCollectionItem> &newlist);
    QList<KoCollectionItem> shapeTemplateList() const
    {
        return m_shapeTemplateList;
    }

    const KoProperties *properties(const QModelIndex &index) const;

    Qt::DropActions supportedDragActions() const;

private:
    QList<KoCollectionItem> m_shapeTemplateList;
    QString m_family;
};

#endif //KIVIOSHAPETEMPLATEMODEL_H
