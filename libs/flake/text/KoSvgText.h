/*
 *  Copyright (c) 2017 Dmitry Kazakov <dimula73@gmail.com>
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

#ifndef KOSVGTEXT_H
#define KOSVGTEXT_H

#include <QVector>
#include <QVariant>
#include <QList>
#include <QFont>
#include <QTextCharFormat>
#include <boost/optional.hpp>
#include <boost/operators.hpp>

#include <kritaflake_export.h>

class KoXmlElement;
class SvgLoadingContext;
class QDebug;

namespace KoSvgText
{
enum WritingMode {
    LeftToRight,
    RightToLeft,
    TopToBottom
};

enum Direction {
    DirectionLeftToRight,
    DirectionRightToLeft
};

enum UnicodeBidi {
    BidiNormal,
    BidiEmbed,
    BidiOverride
};

enum TextAnchor {
    AnchorStart,
    AnchorMiddle,
    AnchorEnd
};

enum DominantBaseline {
    DominantBaselineAuto,
    DominantBaselineUseScript,
    DominantBaselineNoChange,
    DominantBaselineResetSize,
    DominantBaselineIdeographic,
    DominantBaselineAlphabetic,
    DominantBaselineHanging,
    DominantBaselineMathematical,
    DominantBaselineCentral,
    DominantBaselineMiddle,
    DominantBaselineTextAfterEdge,
    DominantBaselineTextBeforeEdge
};

enum AlignmentBaseline {
    AlignmentBaselineAuto,
    AlignmentBaselineDominant,
    AlignmentBaselineIdeographic,
    AlignmentBaselineAlphabetic,
    AlignmentBaselineHanging,
    AlignmentBaselineMathematical,
    AlignmentBaselineCentral,
    AlignmentBaselineMiddle,
    AlignmentBaselineTextAfterEdge,
    AlignmentBaselineTextBeforeEdge
};

enum BaselineShiftMode {
    ShiftNone,
    ShiftSub,
    ShiftSuper,
    ShiftPercentage
    // note: we convert all the <length> values into the relative font values!
};

enum LengthAdjust {
    LengthAdjustSpacing,
    LengthAdjustSpacingAndGlyphs
};

struct AutoValue : public boost::equality_comparable<AutoValue>
{
    AutoValue() {}
    AutoValue(qreal _customValue) : isAuto(false), customValue(_customValue) {}

    bool isAuto = true;
    qreal customValue = 0.0;

    bool operator==(const AutoValue & other) const {
        return isAuto == other.isAuto && (isAuto || customValue == other.customValue);
    }
};

QDebug KRITAFLAKE_EXPORT operator<<(QDebug dbg, const KoSvgText::AutoValue &value);

inline QVariant fromAutoValue(const KoSvgText::AutoValue &value) {
    return QVariant::fromValue(value);
}

AutoValue parseAutoValueX(const QString &value, const SvgLoadingContext &context, const QString &autoKeyword = "auto");
AutoValue parseAutoValueY(const QString &value, const SvgLoadingContext &context, const QString &autoKeyword = "auto");
AutoValue parseAutoValueXY(const QString &value, const SvgLoadingContext &context, const QString &autoKeyword = "auto");
AutoValue parseAutoValueAngular(const QString &value, const SvgLoadingContext &context, const QString &autoKeyword = "auto");

WritingMode parseWritingMode(const QString &value);
Direction parseDirection(const QString &value);



UnicodeBidi parseUnicodeBidi(const QString &value);
TextAnchor parseTextAnchor(const QString &value);
DominantBaseline parseDominantBaseline(const QString &value);
AlignmentBaseline parseAlignmentBaseline(const QString &value);
BaselineShiftMode parseBaselineShiftMode(const QString &value);
LengthAdjust parseLengthAdjust(const QString &value);

struct CharTransformation : public boost::equality_comparable<AutoValue>
{
    boost::optional<qreal> xPos;
    boost::optional<qreal> yPos;
    boost::optional<qreal> dxPos;
    boost::optional<qreal> dyPos;
    boost::optional<qreal> rotate;

    void mergeInParentTransformation(const CharTransformation &t);
    bool isNull() const;
    QPointF adjustedTextPos(const QPointF &pos) const;
    bool startsNewChunk() const;
    bool hasRelativeOffset() const;

    QPointF absolutePos() const;
    QPointF adjustRelativePos(const QPointF &pos = QPointF()) const;

    bool operator==(const CharTransformation & other) const;
};

QDebug KRITAFLAKE_EXPORT operator<<(QDebug dbg, const KoSvgText::CharTransformation &t);

struct KoSvgCharChunkFormat : public QTextCharFormat
{
    KoSvgCharChunkFormat() {}
    KoSvgCharChunkFormat(const KoSvgCharChunkFormat &rhs) : QTextCharFormat(rhs) {}
    KoSvgCharChunkFormat(const QTextCharFormat &rhs) : QTextCharFormat(rhs) {}

    enum SvgCharProperty {
        TextAnchor = UserProperty + 1
    };

    inline void setTextAnchor(KoSvgText::TextAnchor value) {
        setProperty(TextAnchor, int(value));
    }
    inline KoSvgText::TextAnchor textAnchor() const {
        return KoSvgText::TextAnchor(intProperty(TextAnchor));
    }

    inline Qt::Alignment calculateAlignment() const {
        const KoSvgText::TextAnchor anchor = textAnchor();

        Qt::Alignment result;

        if (layoutDirection() == Qt::LeftToRight) {
            result = anchor == AnchorEnd ? Qt::AlignRight :
                     anchor == AnchorMiddle ? Qt::AlignHCenter :
                     Qt::AlignLeft;
        } else {
            result = anchor == AnchorEnd ? Qt::AlignLeft :
                     anchor == AnchorMiddle ? Qt::AlignHCenter :
                     Qt::AlignRight;
        }

        return result;
    }

};

struct Style
{
    QList<qreal> globalX;
    QList<qreal> globalY;

    QList<qreal> globalDX;
    QList<qreal> globalDY;

    QList<qreal> globalRotate;

    qreal expectedLength = -1.0;

    //// inheriting properties

    QList<QFont> fontList;

    boost::optional<AutoValue> kerning;
    boost::optional<WritingMode> writingMode;
    boost::optional<Direction> direction;
    boost::optional<UnicodeBidi> unicodeBidi;

    boost::optional<AutoValue> glyphOrientationVertical;
    boost::optional<AutoValue> glyphOrientationHorizontal;

    boost::optional<TextAnchor> textAnchor;

    //// non-inheriting properties
    DominantBaseline dominantBaseline = DominantBaselineAuto;
    AlignmentBaseline alignmentBaseline = AlignmentBaselineAuto;
    BaselineShiftMode baselineShiftMode = ShiftNone;
    qreal baselineShiftPercentage = 0;
};

}

Q_DECLARE_METATYPE(KoSvgText::AutoValue)

#endif // KOSVGTEXT_H