#ifndef MENUOVERFLOWBUTTON_H
#define MENUOVERFLOWBUTTON_H

#include <KDecoration2/DecorationButton>

namespace BreezeSquare
{

    class MenuOverflowButton : public KDecoration2::DecorationButton
    {
        Q_OBJECT
    public:
        explicit MenuOverflowButton(QPointer<KDecoration2::Decoration> deco, QObject *parent = nullptr);

        // DecorationButton interface
    public:
        virtual void paint(QPainter *painter, const QRect &repaintArea) override;
    };

}
#endif // MENUOVERFLOWBUTTON_H
