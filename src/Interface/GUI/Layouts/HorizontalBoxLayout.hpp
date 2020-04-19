#ifndef GameGUI_HBOXLAYOUT_HPP
#define GameGUI_HBOXLAYOUT_HPP

#include "Layout.hpp"

namespace GameGUI
{

// Horizontally stacked layout
class HorizontalBoxLayout : public Layout
{
public:
	Widget* add(Widget* widget, int id = -1) override;

protected:
	// Update this widget's size according to the widgets inside of it.
	void recomputeGeometry() override;
};

}


#endif // !GameGUI_HBOXLAYOUT_HPP
