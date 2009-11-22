/*
 * Copyright (C) 2009 Boudewijn Rempt <boud@valdyas.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "psd_utils_test.h"

#include <QTest>
#include <QCoreApplication>
#include <klocale.h>
#include <qtest_kde.h>
#include "../psd_utils.h"
#include <QByteArray>
#include <QBuffer>

void PSDUtilsTest::test_psdwrite_quint16()
{
    QBuffer buf;
    buf.open(QBuffer::ReadWrite);
    quint16 i = 3;
    Q_ASSERT(psdwrite(&buf, i));
    Q_ASSERT(buf.buffer().size() == 2);
    QCOMPARE(buf.buffer().at(0), '\0');
    QCOMPARE(buf.buffer().at(1), '\3');
}

void PSDUtilsTest::test_psdwrite_quint32()
{
    QBuffer buf;
    buf.open(QBuffer::ReadWrite);
    quint32 i = 100;
    Q_ASSERT(psdwrite(&buf, i));
    Q_ASSERT(buf.buffer().size() == 4);
    QCOMPARE(buf.buffer().at(0), '\0');
    QCOMPARE(buf.buffer().at(1), '\0');
    QCOMPARE(buf.buffer().at(2), '\0');
    QCOMPARE(buf.buffer().at(3), 'd');
}

void PSDUtilsTest::test_psdwrite_qstring()
{
    QBuffer buf;
    buf.open(QBuffer::ReadWrite);
    QString s = "8BPS";
    Q_ASSERT(psdwrite(&buf, s));
    Q_ASSERT(buf.buffer().size() == 4);
    QCOMPARE(buf.buffer().at(0), '8');
    QCOMPARE(buf.buffer().at(1), 'B');
    QCOMPARE(buf.buffer().at(2), 'P');
    QCOMPARE(buf.buffer().at(3), 'S');
}

void PSDUtilsTest::test_psdpad()
{
    QBuffer buf;
    buf.open(QBuffer::ReadWrite);
    Q_ASSERT(psdpad(&buf, 6));
    Q_ASSERT(buf.buffer().size() == 6);
    QCOMPARE(buf.buffer().at(0), '\0');
    QCOMPARE(buf.buffer().at(1), '\0');
    QCOMPARE(buf.buffer().at(2), '\0');
    QCOMPARE(buf.buffer().at(3), '\0');
    QCOMPARE(buf.buffer().at(4), '\0');
    QCOMPARE(buf.buffer().at(5), '\0');
}

QTEST_KDEMAIN(PSDUtilsTest, GUI)

#include "psd_utils_test.moc"
