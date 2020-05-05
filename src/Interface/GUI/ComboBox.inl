#include "ComboBox.hpp"

// TODO: we shouldn't be outputting anything, get rid of this
#include <iostream>

namespace GameGUI
{

template <class T>
ComboBox<T>::ComboBox(sf::RenderWindow* window) : m_current_index(-1), m_box(Box::Type::Input), m_dropDownArrow(Arrow(Arrow::Direction::Bottom)), m_parentWindow(window)
{
	// Build visual components
	m_box.item().setFont(Theme::getFont());
	m_box.item().setCharacterSize(Theme::textCharacterSize);
	m_box.setSize(Theme::minWidgetWidth, Theme::getBoxHeight());

	// Pack right arrow
	m_dropDownArrow.setSize(Theme::getBoxHeight(), Theme::getBoxHeight());
	m_dropDownArrow.setPosition(m_box.getSize().x - 1, 0);
//	m_dropDownArrow.setPosition(m_box.getSize().x - Theme::getBoxHeight(), 0);
	m_dropDownArrow.centerItem(m_dropDownArrow.item());

	// Widget local bounds
	setSize(sf::Vector2f(m_box.getSize().x + m_dropDownArrow.getSize().x - 1, m_box.getSize().y));
}

template <class T>
void ComboBox<T>::addItem(const sf::String& label, const T& value, bool select)
{
	m_items.push_back(Item(label, value));

	m_box.item().setString(label);
	int width = m_box.item().getLocalBounds().width + (Theme::getBoxHeight() * 2) + (Theme::PADDING * 2);

	// Check if box needs to be resized
	if (width > getSize().x)
	{
		m_box.setSize(width, Theme::getBoxHeight());
		m_dropDownArrow.setPosition(m_box.getSize().x - 1, 0);
		m_dropDownArrow.centerItem(m_dropDownArrow.item());
		setSize(sf::Vector2f(m_box.getSize().x + m_dropDownArrow.getSize().x - 1, m_box.getSize().y));
	}

	selectItem(m_items.size() - 1);
}

template <class T>
void ComboBox<T>::selectItem(size_t item_index)
{
	if (item_index < m_items.size())
	{
		m_current_index = item_index;
		m_box.item().setString(m_items[item_index].label);
		m_box.centerText(m_box.item());
	}
}

template <class T>
const T& ComboBox<T>::getSelectedValue() const
{
	return m_items[m_current_index].value;
}

template <class T>
size_t ComboBox<T>::getSelectedIndex() const
{
	return m_current_index;
}

template <class T>
void ComboBox<T>::selectNext()
{
	if (m_items.size() > 1)
	{
		// Get next item index
		selectItem(m_current_index == (m_items.size() - 1) ? 0 : m_current_index + 1);
		triggerCallback();
	}
}

template <class T>
void ComboBox<T>::selectPrevious()
{
	if (m_items.size() > 1)
	{
		// Get previous item index
		selectItem(m_current_index == 0 ? m_items.size() - 1 : m_current_index - 1);
		triggerCallback();
	}
}

template<class T>
inline void ComboBox<T>::open()
{
	int stepSize = 20;
	m_dropDownBox.create(sf::VideoMode(m_box.getSize().x + m_dropDownArrow.getSize().x - 1, m_items.size() * stepSize), "shhhhh bb is ok", sf::Style::None);

	// TODO: make this not suck
	// cross platform (it will be misaligned on linux and osx
	// it will be misaligned on windows that don't have borders
	sf::Vector2i pos(m_parentWindow->getPosition().x + getAbsolutePosition().x + 8, m_parentWindow->getPosition().y + getAbsolutePosition().y + getSize().y + 29);
	m_dropDownBox.setPosition(pos);

	m_open = true;
	redrawDropDown();
}

template<class T>
inline void ComboBox<T>::close()
{
	m_dropDownBox.close();
	m_open = false;
}

template <class T>
void ComboBox<T>::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(m_box, states);
	target.draw(m_dropDownArrow, states);
}

template <class T>
void ComboBox<T>::updateArrow(ItemBox<Arrow>& arrow, float x, float y)
{
	if (arrow.containsPoint(x, y))
	{
		if (getState() == State::Pressed)
			arrow.press();
		else
			arrow.applyState(State::Hovered);
	}
	else
	{
		arrow.applyState(isFocused() ? State::Focused : State::Default);
	}
}

template <class T>
void ComboBox<T>::redrawDropDown()
{
	// TODO: Take theme::characterSize into account
	int stepSize = 20;

	if (m_open)
	{
		m_dropDownBox.clear(Theme::windowBgColor);

		sf::RectangleShape sel;
		sel.setFillColor(Theme::hexToRgb("#958831"));
		sel.setSize(sf::Vector2f(m_dropDownBox.getSize().x, stepSize));
		sel.move(sf::Vector2f(0, getSelectedIndex() * stepSize));
		m_dropDownBox.draw(sel);

		for (size_t i = 0; i < m_items.size(); i++)
		{
			sf::Text t;
			t.setFont(Theme::getFont());
			t.setCharacterSize(Theme::textCharacterSize);
			t.setString(m_items[i].label);
			//t.setFillColor(GameGUI::Theme::click.textColor);
			t.setColor(GameGUI::Theme::click.textColor);
			t.setPosition(sf::Vector2f(10, (stepSize * i) + 3));

			m_dropDownBox.draw(t);
		}

		sf::RectangleShape outline;
		outline.setSize(sf::Vector2f(m_dropDownBox.getSize() - sf::Vector2u(2, 2)));
		outline.setFillColor(sf::Color::Transparent);
		outline.setOutlineColor(sf::Color::White);
		outline.setOutlineThickness(1);
		outline.move(sf::Vector2f(1, 1));
		m_dropDownBox.draw(outline);

		m_dropDownBox.display();
	}
}

template<class T>
inline bool ComboBox<T>::containsPoint(const sf::Vector2f& point) const
{
	// test for the drop down
	if (m_open)
	{
		sf::FloatRect widget(sf::Vector2f(sf::Vector2f(0, 0)), sf::Vector2f(m_dropDownBox.getSize()));
		if (widget.contains(sf::Vector2f(sf::Mouse::getPosition(m_dropDownBox))))
		{
			return true;
		}
	}

	// test for the widget
	sf::FloatRect widget(getAbsolutePosition(), getSize());

	sf::RectangleShape t;
	t.setSize(sf::Vector2f(widget.width, widget.height));
	t.setPosition(widget.left, widget.top);

	if (widget.contains(m_parentWindow->mapPixelToCoords(sf::Mouse::getPosition(*m_parentWindow))))
	{
		return true;

		//t.setFillColor(sf::Color::Red);
		t.setFillColor(sf::Color::Red);
	}

	sf::CircleShape m;
	m.setRadius(4);
	m.setPosition(sf::Vector2f(sf::Mouse::getPosition(*m_parentWindow)));

//	m_parentWindow->draw(t);
//	m_parentWindow->draw(m);
//	m_parentWindow->display();

	return false;
}

// callbacks 

template <class T>
void ComboBox<T>::onStateChanged(State state)
{
	// Hovered state is handled in the onMouseMoved callback
	if (state == State::Pressed)
		state = State::Focused;

	if (state == State::Default || state == State::Focused)
	{
		m_dropDownArrow.applyState(state);
		m_box.applyState(state);

		if (state == State::Default)
			close();
	}
}

template <class T>
void ComboBox<T>::onMouseMoved(float x, float y)
{
	// FIXME: everytime mouseMoved is called
	// the arrow is destroyed and a new one is created.
	updateArrow(m_dropDownArrow, x, y);

	if (m_open)
	{
		int stepSize = 20;
		for (size_t i = 0; i < m_items.size(); i++)
		{
			sf::Vector2f pos(0, i * stepSize);
			sf::Vector2f size(m_dropDownBox.getSize().x, stepSize);
			sf::FloatRect itemArea(pos, size);

			if (itemArea.contains(m_dropDownBox.mapPixelToCoords(sf::Mouse::getPosition(m_dropDownBox))))
			{
				selectItem(i);
				redrawDropDown();
				std::cout << "in drop down option: " << std::string(m_items[i].label) << std::endl;
			}
		}
	}
}

template <class T>
void ComboBox<T>::onMousePressed(float x, float y)
{
	if (m_dropDownArrow.containsPoint(x, y))
		m_dropDownArrow.press();
	else
		m_dropDownArrow.release();
	
	// Check if mouse in dropdownwindow
}

template <class T>
void ComboBox<T>::onMouseReleased(float x, float y)
{
	if (m_dropDownArrow.containsPoint(x, y))
	{
		if (!m_open)
		{
			m_dropDownArrow.release();

			open();
		}
		else
		{
			close();
		}
	}
}

template <class T>
void ComboBox<T>::onKeyPressed(sf::Keyboard::Key key)
{
	// TODO: home, end, up, down
	 
	if (key == sf::Keyboard::Key::Escape)
	{
		close();
	}
	else if (key == sf::Keyboard::Key::Return)
	{
		if (m_open)
		{
			// TODO: select item
			close();
		}
		else
		{
			m_dropDownArrow.press();
			open();
		}
	}
	// FIXME: we need to use the up and down keys
	else if (key == sf::Keyboard::Key::Up) 
	{
		abort();
	}
}

template <class T>
void ComboBox<T>::onKeyReleased(sf::Keyboard::Key key)
{
	// TODO: find out if this works everytime
	if (key == sf::Keyboard::Key::Return)
		m_dropDownArrow.release();

	// Nothing to do here
}

template <class T>
ComboBox<T>::Item::Item(const sf::String& string, const T& val) :
	label(string),
	value(val)
{
}

}
