#include "breezemenuoverflowbutton.h"
namespace BreezeSquare
{

    MenuOverflowButton::MenuOverflowButton(QPointer<KDecoration2::Decoration> deco, QObject *parent)
        : KDecoration2::DecorationButton(KDecoration2::DecorationButtonType::Custom, deco, parent)
    {
    }

    void MenuOverflowButton::paint(QPainter *painter, const QRect &repaintArea)
    {
    }

}
