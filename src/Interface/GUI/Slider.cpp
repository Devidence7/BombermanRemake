#include "Slider.hpp"
#include "Theme.hpp"

// TODO: fix the thing looking wrong
// when the slider is focused, if you hover over it's bar,
// it acts like it's hovered

namespace GameGUI
{

Slider::Slider(float length, Type type) : m_type(type), m_slideStep(10), m_value(0), m_box(Box::Type::Slider), m_handle(Box::Type::SliderHandle)
{
	int handleHeight = Theme::getBoxHeight();
	int handleWidth = handleHeight / 2;
	int boxHeight = Theme::borderSize * 3;
	int boxOffset = (handleHeight - boxHeight) / 2;

	if (type == Type::Horizontal)
	{
		m_box.setSize(length, boxHeight);
		m_box.setPosition(0, boxOffset);
		m_handle.setSize(handleWidth, handleHeight);

		setSize(sf::Vector2f(length, handleHeight));

		for (int i = 0; i < 4; ++i)
		{
			// TODO: custom color definition for this
			m_progression[i].color = Theme::windowBgColor;
			m_progression[i].position.x = m_box.getPosition().x + Theme::borderSize;
			m_progression[i].position.y = m_box.getPosition().y + Theme::borderSize;
		}

		m_progression[2].position.y += m_box.getSize().y - Theme::borderSize * 2;
		m_progression[3].position.y += m_box.getSize().y - Theme::borderSize * 2;
	}
	else
	{
		m_box.setSize(boxHeight, length);
		m_box.setPosition(boxOffset, 0);
		m_handle.setSize(handleHeight, handleWidth);

		setSize(sf::Vector2f(handleHeight, length));

		for (int i = 0; i < 4; ++i)
		{
			// TODO: custom color definition for this
			m_progression[i].color = Theme::windowBgColor;
			m_progression[i].position.x = m_box.getPosition().x + Theme::borderSize;
			m_progression[i].position.y = m_box.getSize().y - Theme::borderSize;
		}

		m_progression[1].position.x += m_box.getSize().x - Theme::borderSize * 2;
		m_progression[2].position.x += m_box.getSize().x - Theme::borderSize * 2;
	}

	updateHandlePosition();
}

int Slider::getQuantum() const
{
	return m_slideStep;
}

void Slider::setQuantum(int slideStep)
{
	if (slideStep > 0 && slideStep < 100)
		m_slideStep = slideStep;
}

int Slider::getValue() const
{
	return m_value;
}

void Slider::setValue(int value)
{
	// TODO: std::clamp ?
	// Ensure value is in bounds
	if (value < 0)
		value = 0;
	else if (value > 100)
		value = 100;
	else
	{
		// Round value to the closest slideStep multiple
		int temp = value + m_slideStep / 2;
		value = temp - temp % m_slideStep;
	}

	// If value has changed
	if (value != m_value)
	{
		m_value = value;
		triggerCallback();
		// Move the handle on the slider
		updateHandlePosition();
	}
}

void Slider::updateHandlePosition()
{
	// TODO: move relative to mouse position

//	this->getParent()->window.getSize();

	if (m_type == Type::Horizontal)
	{
		int max = getSize().x - m_handle.getSize().x - Theme::borderSize * 2;
		int x = max * m_value / 100 + Theme::borderSize;
		m_handle.setPosition(x, 0);
		m_progression[1].position.x = x;
		m_progression[2].position.x = x;
	}
	else
	{
		int max = getSize().y - m_handle.getSize().y - Theme::borderSize * 2;
		int reverse_value = 100 - m_value;
		int y = max * reverse_value / 100 + Theme::borderSize;
		m_handle.setPosition(0, y);
		m_progression[0].position.y = y;
		m_progression[1].position.y = y;
	}
}

void Slider::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(m_box, states);
	target.draw(m_progression, 4, sf::Quads, states);
	target.draw(m_handle, states);
}

// callbacks ------------------------------------------------------------------

void Slider::onKeyPressed(sf::Keyboard::Key key)
{
	switch (key)
	{
	case sf::Keyboard::Left:
	case sf::Keyboard::Subtract:
		setValue(m_value - m_slideStep);
		break;
	case sf::Keyboard::Up:
	case sf::Keyboard::Right:
		setValue(m_value + m_slideStep);
		break;
	case sf::Keyboard::Home:
		setValue(0);
		break;
	case sf::Keyboard::End:
		setValue(100);
		break;
	default:
		break;
	}
}

void Slider::onMousePressed(float x, float y)
{
//TODO:	std::cout << "teleporting to pos (x: " << x << ", " << "y: " << y << ")(rx: " << sf::Mouse::getPosition().x << ", " << "ry: " << sf::Mouse::getPosition().y << ")" << std::endl;

	if (m_type == Type::Horizontal)
		setValue(100 * x / getSize().x);
	else
		setValue(100 - (100 * (y) / getSize().y));

	m_handle.press();
}

void Slider::onMouseMoved(float x, float y)
{
	// if the handle is pressed (i.e. is being dragged)
	if (getState() == State::Pressed)
	{
		if (m_type == Type::Horizontal)
			setValue(100 * x / getSize().x);
		else
			setValue(100 - (100 * y / getSize().y));
	}
	else if (m_handle.containsPoint(x, y))
	{
		m_handle.applyState(State::Hovered);
	}
	else
	{
		m_handle.applyState(State::Default);
	}
}

void Slider::onMouseReleased(float x, float y)
{
	m_handle.release();
}

void Slider::onMouseWheelMoved(int delta)
{
	setValue(m_value + (delta > 0 ? m_slideStep : -m_slideStep));
}

void Slider::onStateChanged(State state)
{
	if (state == State::Focused || state == State::Default)
	{
		m_handle.applyState(state);
	}
}

}
