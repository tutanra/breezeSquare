#include "breezetextbutton.h"
#include "breezedecoration.h"

#include <KColorUtils>

#include <QPainter>

using namespace KDecoration2;
namespace BreezeSquare
{

    using KDecoration2::ColorGroup;
    using KDecoration2::ColorRole;
    using KDecoration2::DecorationButtonType;

    TextButton::TextButton(const QPointer<KDecoration2::Decoration> &decoration, int row, QObject *parent) : DecorationButton(DecorationButtonType::Custom, decoration, parent), m_animation(new QVariantAnimation(this)), m_buttonIndex(row)
    {
        setCheckable(true);

        m_animation->setStartValue(0.0);
        m_animation->setEndValue(1.0);
        m_animation->setEasingCurve(QEasingCurve::InOutQuad);
        m_animation->stop();

        connect(m_animation, &QVariantAnimation::valueChanged, this, [this](const QVariant &value)
                { setOpacity(value.toReal()); });
        auto d = qobject_cast<Decoration *>(decoration);

        connect(this, &KDecoration2::DecorationButton::hoveredChanged, this, &TextButton::updateAnimationState);

        // animation
        if (d)
            m_animation->setDuration(d->animationsDuration());
    }

    QAction *TextButton::action() const
    {
        return m_action;
    }

    void TextButton::setAction(QAction *newAction)
    {
        m_action = newAction;
    }

    QColor TextButton::foregroundColor() const
    {
        const auto *d = qobject_cast<Decoration *>(decoration());
        const auto *bg = qobject_cast<AppMenuButtonGroup *>(parent());

        if (isPressed() || (isHovered() && !bg->isMenuOpen()) || isChecked())
        {
            return d->titleBarColor();
        }
        return d->fontColor();
    }

    QColor TextButton::backgroundColor() const
    {
        const auto *d = qobject_cast<Decoration *>(decoration());
        const auto *bg = qobject_cast<AppMenuButtonGroup *>(parent());

        if (isPressed())
        {
            return KColorUtils::mix(d->titleBarColor(), d->fontColor(), 0.3);
        }
        if ((isHovered() && !bg->isMenuOpen()) || isChecked())
        {
            return d->fontColor();
        }
        return d->titleBarColor();
    }

    const QString &TextButton::text() const
    {
        return m_text;
    }

    void TextButton::setText(const QString &newText)
    {
        auto d = qobject_cast<Decoration *>(decoration());
        if (m_text == newText)
            return;
        m_text = newText;

        auto noMnemonicText = newText;
        noMnemonicText.remove('&');

        const QSizeF textSize = {d->textWidth(noMnemonicText), static_cast<double>(d->captionHeight())};

        auto geom = geometry();
        geom.setSize({textSize.width() + (Metrics::TitleBar_SideMargin * d->settings()->smallSpacing() * 2), textSize.height()});
        setGeometry(geom);

        emit textChanged();
    }

    void TextButton::paint(QPainter *painter, const QRect &repaintArea)
    {

        Q_UNUSED(repaintArea)
        const auto *bg = qobject_cast<AppMenuButtonGroup *>(parent());

        if (!decoration() || !bg)
            return;

        painter->save();

        painter->setBrush(backgroundColor());
        painter->setPen(foregroundColor());

        if (isPressed() || (isHovered() && !bg->isMenuOpen()) || isChecked())
        {
            painter->drawRoundedRect(geometry(), 3, 3);
        }

        painter->drawText(geometry(),
                          Qt::TextSingleLine | Qt::AlignCenter | Qt::TextHideMnemonic, m_text);
        painter->restore();
    }
    void TextButton::updateAnimationState(bool hovered)
    {
        auto d = qobject_cast<Decoration *>(decoration());
        if (!(d && d->animationsDuration() > 0))
            return;

        m_animation->setDirection(hovered ? QAbstractAnimation::Forward : QAbstractAnimation::Backward);
        if (m_animation->state() != QAbstractAnimation::Running)
            m_animation->start();
    }

}
