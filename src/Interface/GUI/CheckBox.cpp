#include "CheckBox.hpp"
#include "Theme.hpp"

namespace GameGUI
{

CheckBox::CheckBox(bool checked) : m_box(Box::Type::Tick)
{
	float offset = Theme::PADDING + Theme::borderSize;
	float box_size = m_cross.getSize().x + offset * 2.0f;

	m_box.setSize(box_size, box_size);
	m_cross.setPosition(offset, offset);
	m_cross.setColor(Theme::tickColor);

	check(checked);

	setSize(m_box.getSize());
}

bool CheckBox::isChecked() const
{
	return m_checked;
}

void CheckBox::check(bool checked)
{
	// TODO: maybe triggerCallback() should be called in here?
	// it seems to trail every call to check(), except in the constructor.

	m_checked = checked;
}

void CheckBox::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(m_box, states);

	if (m_checked)
		target.draw(m_cross, states);
}

// callbacks -------------------------------------------------------------------

void CheckBox::onStateChanged(State state)
{
	m_box.applyState(state);
}

void CheckBox::onMouseReleased(float x, float y)
{
	if (containsPoint(sf::Vector2f(x, y)))
	{
		check(!m_checked);
		triggerCallback();
	}
}

void CheckBox::onKeyPressed(sf::Keyboard::Key key)
{
	if (key == sf::Keyboard::Space || key == sf::Keyboard::Key::Return)
	{
		check(!m_checked);
		triggerCallback();
	}
	else if (key == sf::Keyboard::Key::Y)
	{
		if (!m_checked)
		{
			check(true);
			triggerCallback();
		}
	}
	else if (key == sf::Keyboard::Key::N)
	{
		if (m_checked)
		{
			check(false);
			triggerCallback();
		}
	}
}

}
