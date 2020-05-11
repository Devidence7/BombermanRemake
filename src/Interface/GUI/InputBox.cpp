#include "InputBox.hpp"
#include "Theme.hpp"

#include <SFML/OpenGL.hpp>

namespace GameGUI
{

InputBox::InputBox(float width) : m_box(Box::Type::Input), m_cursor_pos(0), m_max_characters(-1), m_blink_period(1.0f)
{
	m_box.setSize(width, Theme::getBoxHeight());

	float offset = Theme::borderSize + Theme::PADDING;
	m_text.setFont(Theme::getFont());
	m_text.setPosition(offset, offset);
	///m_text.setFillColor(Theme::input.textColor);
	m_text.setFillColor(Theme::input.textColor);
	m_text.setCharacterSize(Theme::textCharacterSize);

	// Build cursor
	m_cursor.setPosition(offset, offset);
	m_cursor.setSize(sf::Vector2f(1.f, Theme::getLineSpacing()));
	m_text.setFillColor(Theme::input.textColor);
	//m_text.setFillColor(Theme::input.textColor);
	setCursorPosition(0);

	// TODO: is this necessary?
	setText("");

	setSize(m_box.getSize());
}

void InputBox::setText(const sf::String& string)
{
	m_text.setString(string);
	setCursorPosition(string.getSize());

	m_oldText = m_text.getString();
}

bool InputBox::isEmpty() const
{
	return m_text.getString().isEmpty();
}

const sf::String& InputBox::getText() const
{
	return m_text.getString();
}

void InputBox::setCursorPosition(size_t index)
{
	if (index <= m_text.getString().getSize())
	{
		m_cursor_pos = index;

		float padding = Theme::borderSize + Theme::PADDING;
		m_cursor.setPosition(m_text.findCharacterPos(index).x, padding);
		m_cursor_timer.restart();

		if (m_cursor.getPosition().x > getSize().x - padding)
		{
			// Shift text on left
			float diff = m_cursor.getPosition().x - getSize().x + padding;
			m_text.move(-diff, 0);
			m_cursor.move(-diff, 0);
		}
		else if (m_cursor.getPosition().x < padding)
		{
			// Shift text on right
			float diff = padding - m_cursor.getPosition().x;
			m_text.move(diff, 0);
			m_cursor.move(diff, 0);
		}

		float text_width = m_text.getLocalBounds().width;

		if (m_text.getPosition().x < padding
			&& m_text.getPosition().x + text_width < getSize().x - padding)
		{
			float diff = (getSize().x - padding) - (m_text.getPosition().x + text_width);
			m_text.move(diff, 0);
			m_cursor.move(diff, 0);

			// If text is smaller than the textbox, force align on left
			if (text_width < (getSize().x - padding * 2))
			{
				diff = padding - m_text.getPosition().x;
				m_text.move(diff, 0);
				m_cursor.move(diff, 0);
			}
		}
	}
}

size_t InputBox::getCursorPosition() const
{
	return m_cursor_pos;
}

void InputBox::setBlinkPeriod(float period)
{
	m_blink_period = period;
}

float InputBox::getBlinkPeriod() const
{
	return m_blink_period;
}

void InputBox::onKeyPressed(sf::Keyboard::Key key)
{
	switch (key)
	{
	case sf::Keyboard::Left:
		setCursorPosition(m_cursor_pos - 1);
		break;

	case sf::Keyboard::Right:
		setCursorPosition(m_cursor_pos + 1);
		break;

	case sf::Keyboard::BackSpace:
		// Erase character before cursor
		if (m_cursor_pos > 0)
		{
			sf::String string = m_text.getString();
			string.erase(m_cursor_pos - 1);
			m_text.setString(string);

			setCursorPosition(m_cursor_pos - 1);
		}
		break;

	case sf::Keyboard::Delete:
		// Erase character after cursor
		if (m_cursor_pos < m_text.getString().getSize())
		{
			sf::String string = m_text.getString();
			string.erase(m_cursor_pos);
			m_text.setString(string);

			setCursorPosition(m_cursor_pos);
		}
		break;

	case sf::Keyboard::Home:
		setCursorPosition(0);
		break;

	case sf::Keyboard::End:
		setCursorPosition(m_text.getString().getSize());
		break;

	case sf::Keyboard::Return:
		triggerCallback();
		break;

	case sf::Keyboard::Escape:
		m_text.setString(m_oldText);
		setCursorPosition(m_text.getString().getSize());

	default:
		break;
	}
}

void InputBox::onMousePressed(float x, float y)
{
	for (int i = m_text.getString().getSize(); i >= 0; --i)
	{
		// Place cursor after the character under the mouse
		sf::Vector2f glyph_pos = m_text.findCharacterPos(i);
		if (glyph_pos.x <= x)
		{
			setCursorPosition(i);
			break;
		}
	}
}

void InputBox::onTextEntered(sf::Uint32 unicode)
{
	if (unicode > 30 && (unicode < 127 || unicode > 159))
	{
		sf::String string = m_text.getString();
		if (m_max_characters == -1 || static_cast<int>(string.getSize() < m_max_characters))
		{
			// Insert character in string at cursor position
			string.insert(m_cursor_pos, unicode);
			m_text.setString(string);

			setCursorPosition(m_cursor_pos + 1);
		}
	}
}

void InputBox::onStateChanged(State state)
{
	if (state == State::Default)
		m_oldText = m_text.getString();

	m_box.applyState(state);
}

void InputBox::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(m_box, states);

	glEnable(GL_SCISSOR_TEST);

	sf::Vector2f pos = getAbsolutePosition();
	glScissor((pos.x + Theme::borderSize) - 1, target.getSize().y - (pos.y + getSize().y) + 1, getSize().x - 2, getSize().y - 2);
	target.draw(m_text, states);

	glDisable(GL_SCISSOR_TEST);

	// Show cursor if focused
	if (isFocused())
	{
		// Make it blink
		float timer = m_cursor_timer.getElapsedTime().asSeconds();
		if (timer >= m_blink_period)
			m_cursor_timer.restart();

		// Updating in the drawing method, deal with it
		sf::Color color = Theme::input.textColor;
		color.a = 255 - (255 * timer / m_blink_period);
		m_cursor.setFillColor(color);

		target.draw(m_cursor, states);
	}
}

}
