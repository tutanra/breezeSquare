#ifndef BREEZEAPPMENUBUTTON_H
#define BREEZEAPPMENUBUTTON_H
#include <KDecoration2/DecorationButton>

#include <QObject>
#include <QMenu>
#include <QString>
#include <QVariantAnimation>

namespace BreezeSquare
{
    class TextButton : public KDecoration2::DecorationButton
    {
        Q_OBJECT

    public:
        explicit TextButton(const QPointer<KDecoration2::Decoration> &decoration, int row, QObject *parent);

    private:
        QVariantAnimation *m_animation;
        QString m_text;
        QAction *m_action = nullptr;
        qreal m_opacity;
        int m_buttonIndex;
    private slots:
        void updateAnimationState(bool hovered);

        // DecorationButton interface
    public:
        //*@name active state change animation
        //@{
        void setOpacity(qreal value)
        {
            if (m_opacity == value)
                return;
            m_opacity = value;

            setVisible(m_opacity > 0.0);

            update();
        }

        qreal opacity() const
        {
            return m_opacity;
        }

        //@}

        virtual void paint(QPainter *painter, const QRect &repaintArea) override;
        const QString &text() const;
        void setText(const QString &newText);

        const QVariantAnimation &animation() const;
        void setAnimation(const QVariantAnimation &newAnimation);

        int buttonIndex() const
        {
            return m_buttonIndex;
        }

        QAction *action() const;
        void setAction(QAction *newAction);

        void setPressed(bool pressed)
        {
            m_isPressed = pressed;
            update();
        }

        bool isPressed() const
        {
            return KDecoration2::DecorationButton::isPressed() || m_isPressed;
        }

    private:
        bool m_isPressed = false;
        QColor foregroundColor() const;
        QColor backgroundColor() const;

    signals:
        void menuChanged();
        void textChanged();
    };
}
#endif // BREEZEAPPMENUBUTTON_H
