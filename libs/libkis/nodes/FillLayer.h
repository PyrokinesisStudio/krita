/*
 *  Copyright (c) 2017 Wolthera van Hövell tot Westerflier <griffinvalley@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#ifndef LIBKIS_FILLLAYER_H
#define LIBKIS_FILLLAYER_H

#include <QObject>
#include "Node.h"
#include <InfoObject.h>
#include <Selection.h>

#include <kis_types.h>

#include "kritalibkis_export.h"
#include "libkis.h"
/**
 * @brief The FillLayer class
 * A fill layer is much like a filter layer in that it takes a name
 * and filter. It however specialises in filters that fill the whole canvas,
 * such as a pattern or full color fill.
 */
class KRITALIBKIS_EXPORT FillLayer : public Node
{
    Q_OBJECT
    Q_DISABLE_COPY(FillLayer)

public:
    explicit FillLayer(KisImageSP image, QString name,KisFilterConfigurationSP filter, Selection &selection, QObject *parent = 0);
    ~FillLayer() override;
public Q_SLOTS:

    /**
     * @brief type Krita has several types of nodes, split in layers and masks. Group
     * layers can contain other layers, any layer can contain masks.
     *
     * @return The type of the node. Valid types are:
     * <ul>
     *  <li>paintlayer
     *  <li>grouplayer
     *  <li>filelayer
     *  <li>filterlayer
     *  <li>filllayer
     *  <li>clonelayer
     *  <li>vectorlayer
     *  <li>transparencymask
     *  <li>filtermask
     *  <li>transformmask
     *  <li>selectionmask
     *  <li>colorizemask
     * </ul>
     *
     * If the Node object isn't wrapping a valid Krita layer or mask object, and
     * empty string is returned.
     */
    virtual QString type() const override;

    void setFilter(QString filterName, InfoObject &filterConfig);

    QString filterName();
    InfoObject *filterConfig();
};

#endif // LIBKIS_FILLLAYER_H
