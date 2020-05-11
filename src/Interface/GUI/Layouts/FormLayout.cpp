#include "FormLayout.hpp"
#include "../Theme.hpp"
#include "../Label.hpp"

namespace GameGUI
{

FormLayout::FormLayout():
	m_labelWidth(0.f)
{
}

Widget* FormLayout::add(Widget* widget, int id)
{
	return addRow("", widget, id);
}

Widget* FormLayout::addRow(const sf::String& str, Widget* widget, int id)
{
	GameGUI::Label* label = new GameGUI::Label(str);
	if (label->getSize().x > m_labelWidth)
		m_labelWidth = label->getSize().x;
	push(label);

	widget->setID(id);
	push(widget);
	return widget;
}

Widget* FormLayout::addRow(const sf::String& str, Widget* widget, int id, int labelSize) {
	GameGUI::Label* label = new GameGUI::Label(str);
	label->setTextSize(labelSize);
	if (label->getSize().x > m_labelWidth)
		m_labelWidth = label->getSize().x;
	push(label);

	widget->setID(id);
	push(widget);
	return widget;
}

void FormLayout::recomputeGeometry()
{
	size_t i = 0;
	sf::Vector2f pos;
	sf::Vector2f size;

	for (Widget* widget = getFirstWidget(); widget != NULL; widget = widget->m_next)
	{
		if ((i % 2) == 0)
		{
			// Left-side: label
			widget->setPosition(sf::Vector2f(0, pos.y));

			if (widget->getSize().x > m_labelWidth)
				m_labelWidth = widget->getSize().x;
		}
		else
		{
			// Right-side: widget
			widget->setPosition(sf::Vector2f(m_labelWidth + Theme::MARGIN, pos.y));

			// Row height is at least Theme::getBoxHeight()
			pos.y += std::max(widget->getSize().y, Theme::getBoxHeight()) + Theme::MARGIN;
			if (widget->getSize().x > size.x)
				size.x = widget->getSize().x;
		}
		++i;
	}

	size.x += m_labelWidth + Theme::MARGIN;
	size.y = pos.y - Theme::MARGIN;
	Widget::setSize(size);
}

}
