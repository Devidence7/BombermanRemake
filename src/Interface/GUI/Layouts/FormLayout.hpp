#ifndef GameGUI_FORMLAYOUT_HPP
#define GameGUI_FORMLAYOUT_HPP

#include "Layout.hpp"

namespace GameGUI
{

 // Horizontally stacked layout with a label before each widget.
class FormLayout : public Layout
{
public:
	FormLayout();

	Widget* add(Widget* widget, int id = -1) override;

	// label: label displayed before the widget
	// widget: widget to be added
	Widget* addRow(const sf::String& label, Widget* widget, int id = -1);

	Widget* addRow(const sf::String& str, Widget* widget, int id, int labelSize);

private:
 	// Update the bounds of this Widget according to the widgets inside of it.
	void recomputeGeometry() override;

	float m_labelWidth;
};

}

#endif // GameGUI_FORMLAYOUT_HPP
