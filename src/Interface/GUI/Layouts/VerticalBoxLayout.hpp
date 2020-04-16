#ifndef GameGUI_VBOXLAYOUT_HPP
#define GameGUI_VBOXLAYOUT_HPP

#include "Layout.hpp"

namespace GameGUI
{

// Vertically stacked layout
class VerticalBoxLayout : public Layout
{
public:
	Widget* add(Widget* widget, int id = -1) override;

private:
	// Update this widget's size according to the widgets inside it.
	void recomputeGeometry() override;
};

}


#endif // !GameGUI_VBOXLAYOUT_HPP
