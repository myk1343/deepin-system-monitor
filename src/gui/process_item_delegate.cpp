#include <DApplication>
#include <DApplicationHelper>
#include <DPalette>
#include <DStyle>
#include <DStyleHelper>
#include <QDebug>
#include <QModelIndex>
#include <QPainter>
#include <QStyleOptionViewItem>

#include "process_item_delegate.h"

DWIDGET_USE_NAMESPACE

ProcessItemDelegate::ProcessItemDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

void ProcessItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                                const QModelIndex &index) const
{
    if (!index.isValid()) {
        QStyledItemDelegate::paint(painter, option, index);
        return;
    }

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setOpacity(1);

    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);

#ifdef ENABLE_INACTIVE_DISPLAY
    QWidget *wnd = DApplication::activeWindow();
#endif
    DPalette::ColorGroup cg;
    if (!(opt.state & DStyle::State_Enabled)) {
        cg = DPalette::Disabled;
    } else {
#ifdef ENABLE_INACTIVE_DISPLAY
        if (!wnd) {
            cg = DPalette::Inactive;
        } else {
            if (wnd->isModal()) {
                cg = DPalette::Inactive;
            } else {
                cg = DPalette::Active;
            }
        }
#else
        cg = DPalette::Active;
#endif
    }

    DStyle *style = dynamic_cast<DStyle *>(DApplication::style());

    int radius = style->pixelMetric(DStyle::PM_FrameRadius, &option);
    int margin = style->pixelMetric(DStyle::PM_ContentsMargins, &option);

    DApplicationHelper *dAppHelper = DApplicationHelper::instance();
    DPalette palette = dAppHelper->applicationPalette();
    QBrush background;
    QPen forground;
    if (opt.features & QStyleOptionViewItem::Alternate) {
        background = palette.color(cg, DPalette::AlternateBase);
    } else {
        background = palette.color(cg, DPalette::Base);
    }

    forground.setColor(palette.color(cg, DPalette::Text));
    forground.setColor(qvariant_cast<QBrush>(index.data(Qt::ForegroundRole)).color());
    if (opt.state & DStyle::State_Enabled) {
        if (opt.state & DStyle::State_Selected) {
            background = palette.color(cg, DPalette::Highlight);
            forground.setColor(palette.color(cg, DPalette::HighlightedText));
        }
    }
    painter->setPen(forground);

    QRect rect = opt.rect;
    QFontMetrics fm(opt.font);
    QPainterPath path, clipPath;
    QRect textRect = rect;

    switch (opt.viewItemPosition) {
        case QStyleOptionViewItem::Beginning: {
            rect.setX(rect.x() + margin);  // left margin

            QPainterPath rectPath, roundedPath;
            roundedPath.addRoundedRect(rect.x(), rect.y(), rect.width() * 2, rect.height(), radius,
                                       radius);
            rectPath.addRect(rect.x() + rect.width(), rect.y(), rect.width(), rect.height());
            clipPath = roundedPath.subtracted(rectPath);
            painter->setClipPath(clipPath);
            path.addRect(rect);
        } break;
        case QStyleOptionViewItem::Middle: {
            path.addRect(rect);
        } break;
        case QStyleOptionViewItem::End: {
            rect.setWidth(rect.width() - margin);  // right margin

            QPainterPath rectPath, roundedPath;
            roundedPath.addRoundedRect(rect.x() - rect.width(), rect.y(), rect.width() * 2,
                                       rect.height(), radius, radius);
            rectPath.addRect(rect.x() - rect.width(), rect.y(), rect.width(), rect.height());
            clipPath = roundedPath.subtracted(rectPath);
            painter->setClipPath(clipPath);
            path.addRect(rect);
        } break;
        default: {
            painter->restore();
            QStyledItemDelegate::paint(painter, option, index);
            return;
        }
    }

    QString text;
    QRect iconRect;
    if (opt.viewItemPosition == QStyleOptionViewItem::Beginning) {
        textRect = rect;
        textRect.setX(textRect.x() + margin * 2 + 24);
        textRect.setWidth(textRect.width() - margin);
        text = fm.elidedText(opt.text, opt.textElideMode, textRect.width());

        iconRect = rect;
        iconRect.setX(rect.x() + margin);
        iconRect.setWidth(24);
    } else {
        textRect = rect;
        textRect.setX(textRect.x() + margin);
        textRect.setWidth(textRect.width() - margin);
        text = fm.elidedText(opt.text, opt.textElideMode, textRect.width());
    }

    painter->fillPath(path, background);
    if (opt.viewItemPosition == QStyleOptionViewItem::Beginning) {
        //        opt.icon.paint(painter, iconRect);
        QIcon icon = opt.icon;
        icon.paint(painter, iconRect);
    }
    painter->drawText(textRect, static_cast<int>(opt.displayAlignment), text);

    painter->restore();
}

QWidget *ProcessItemDelegate::createEditor(QWidget *, const QStyleOptionViewItem &,
                                           const QModelIndex &) const
{
    return nullptr;
}

QSize ProcessItemDelegate::sizeHint(const QStyleOptionViewItem &option,
                                    const QModelIndex &index) const
{
    QSize size = QStyledItemDelegate::sizeHint(option, index);
    size.setHeight(std::max(36, size.height()));
    return size;
}

void ProcessItemDelegate::initStyleOption(QStyleOptionViewItem *option,
                                          const QModelIndex &index) const
{
    option->showDecorationSelected = true;
    bool ok = false;
    if (index.data(Qt::TextAlignmentRole).isValid()) {
        uint value = index.data(Qt::TextAlignmentRole).toUInt(&ok);
        option->displayAlignment = static_cast<Qt::Alignment>(value);
    }
    if (!ok)
        option->displayAlignment = Qt::AlignLeft | Qt::AlignVCenter;
    option->textElideMode = Qt::ElideRight;
    option->features = QStyleOptionViewItem::HasDisplay;
    if (index.row() % 2 == 0)
        option->features |= QStyleOptionViewItem::Alternate;
    if (index.data(Qt::DisplayRole).isValid())
        option->text = index.data().toString();

    if (index.data(Qt::DecorationRole).isValid()) {
        option->features |= QStyleOptionViewItem::HasDecoration;
        option->icon = qvariant_cast<QIcon>(index.data(Qt::DecorationRole));
    }
}
