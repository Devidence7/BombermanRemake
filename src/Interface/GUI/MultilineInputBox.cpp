#include "MultilineInputBox.hpp"

#include "Theme.hpp"

#ifdef _WIN32
	#include <SFML/OpenGL.hpp>
#endif

// TODO: this should be removed. we don't need to be outputting any information.
#include <iostream>

namespace GameGUI
{

MultilineInputBox::MultilineInputBox(float width, unsigned int lines) : m_box(Box::Type::Input), m_cursor_pos(0), m_max_characters(-1), m_blink_period(1.0f)
{
	float offset = Theme::borderSize + Theme::PADDING;

	m_box.setSize(width, ((Theme::getLineSpacing()) * lines) + (offset * 2) + 2);

	m_text.setFont(Theme::getFont());
	m_text.setPosition(offset, offset);
	m_text.setFillColor(Theme::input.textColor);
	m_text.setCharacterSize(Theme::textCharacterSize);

	// Build cursor
	m_cursor.setPosition(offset, offset);
	m_cursor.setSize(sf::Vector2f(1.f, Theme::getLineSpacing()));
	m_cursor.setFillColor(Theme::input.textColor);
	setCursorPosition(0);

	// TODO: is this necessary?
	setText("");

	setSize(m_box.getSize());
}

void MultilineInputBox::setText(const sf::String& string)
{
	m_text.setString(string);
	setCursorPosition(string.getSize());

	m_oldText = m_text.getString();
}

const sf::String& MultilineInputBox::getText() const
{
	return m_text.getString();
}

bool MultilineInputBox::isEmpty() const
{
	return m_text.getString().isEmpty();
}

void MultilineInputBox::setLines(unsigned int lines)
{
	float offset = Theme::borderSize + Theme::PADDING;
	m_box.setSize(m_box.getSize().x, ((Theme::getLineSpacing()) * lines) + (offset * 2) + 2);
}

unsigned int MultilineInputBox::getLines() const
{
	return (m_box.getSize().y / Theme::getBoxHeight());
}

void MultilineInputBox::setCursorPosition(size_t index)
{
	if (index <= m_text.getString().getSize())
	{
		m_cursor_pos = index;

		float padding = Theme::borderSize + Theme::PADDING;
		m_cursor.setPosition(m_text.findCharacterPos(index).x, m_text.findCharacterPos(index).y);
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

		if (m_cursor.getPosition().y + m_cursor.getSize().y > getSize().y - padding)
		{
			std::cout << "shifting up" << std::endl;
			// Shift text up
			float diff = m_cursor.getPosition().y - getSize().y + padding;
			m_text.move(0, -diff);
			m_cursor.move(0, -diff);
		}
		else if (m_cursor.getPosition().y < padding)
		{
			std::cout << "shifting down" << std::endl;
			// Shift text down
			float diff = padding - m_cursor.getPosition().y;
			m_text.move(0, diff);
			m_cursor.move(0, diff);
		}

		sf::Vector2f textSize = { m_text.getLocalBounds().width, m_text.getLocalBounds().height };

		if (m_text.getPosition().x < padding
			&& m_text.getPosition().x + textSize.x < getSize().x - padding)
		{
			float diff = (getSize().x - padding) - (m_text.getPosition().x + textSize.x);
			m_text.move(diff, 0);
			m_cursor.move(diff, 0);

			// If text is smaller than the textbox, force align on left
			if (textSize.x < (getSize().x - padding * 2))
			{
				diff = padding - m_text.getPosition().x;
				m_text.move(diff, 0);
				m_cursor.move(diff, 0);
			}
		}
	}
}

size_t MultilineInputBox::getCursorPosition() const
{
	return m_cursor_pos;
}

void MultilineInputBox::setBlinkPeriod(float period)
{
	m_blink_period = period;
}

float MultilineInputBox::getBlinkPeriod() const
{
	return m_blink_period;
}

void MultilineInputBox::onKeyPressed(sf::Keyboard::Key key)
{
	switch (key)
	{
	case sf::Keyboard::Left:
		setCursorPosition(m_cursor_pos - 1);
		break;

	case sf::Keyboard::Right:
		setCursorPosition(m_cursor_pos + 1);
		break;

	/*
	case sf::Keyboard::Up:
		m_text.move(0, Theme::getBoxHeight());
		break;

	case sf::Keyboard::Down:
		m_text.move(0, -Theme::getBoxHeight());
		break;
	*/

	// Erase character before cursor
	case sf::Keyboard::BackSpace:
		if (m_cursor_pos > 0)
		{
			sf::String string = m_text.getString();
			string.erase(m_cursor_pos - 1);
			m_text.setString(string);

			setCursorPosition(m_cursor_pos - 1);
		}
		break;

	// Erase the character before the cursor
	case sf::Keyboard::Delete:
		if (m_cursor_pos < m_text.getString().getSize())
		{
			sf::String string = m_text.getString();
			string.erase(m_cursor_pos);
			m_text.setString(string);

			setCursorPosition(m_cursor_pos);
		}
		break;

	case sf::Keyboard::Home:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
			setCursorPosition(0);
		else
		{
			std::string string = m_text.getString();
			size_t pos = string.substr(0, m_cursor_pos).rfind("\n"); // find the previous linebreak
			setCursorPosition(pos == std::string::npos ? 0 : pos + 1);
		}
		break;

	case sf::Keyboard::End:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
			setCursorPosition(m_text.getString().getSize());
		else
		{
			std::string string = m_text.getString();
			size_t endPos = string.size();
			size_t pos = string.substr(m_cursor_pos, string.length()).find("\n"); // find the next linebreak
			setCursorPosition(pos == std::string::npos ? endPos : m_cursor_pos + pos);
		}
		break;

		/*
	case sf::Keyboard::Return:
		// if the cursor is at the bottom, and there are more lines than available height
		setCursorPosition(0);
		break;
		*/

	case sf::Keyboard::Escape:
		m_text.setString(m_oldText);
		setCursorPosition(m_text.getString().getSize());
		break;

	case sf::Keyboard::Key::V:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl))
		{
		/*	std::string clipboard = sf::Clipboard::getString();
			std::string string = m_text.getString();
			string.insert(m_cursor_pos, clipboard);
			m_text.setString(string);
			setCursorPosition(m_cursor_pos + clipboard.length());*/
		}
		break;

	default:
		break;
	}
}

void MultilineInputBox::onMousePressed(float x, float y)
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

void MultilineInputBox::onTextEntered(sf::Uint32 unicode)
{
	// TODO: need to accpet unicode 13
	if ((unicode > 30 && (unicode < 127 || unicode > 159)) || unicode == 13)
	{
		sf::String string = m_text.getString();
		if (m_max_characters == -1 || static_cast<int>(string.getSize() < m_max_characters))
		{
			// Insert character in string at cursor position
			if (unicode == 13)
				string.insert(m_cursor_pos, "\n");
			else
			{
				if (unicode == 'v' || unicode == 'V') // 86 is 'V'
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl))
						return;

				string.insert(m_cursor_pos, unicode);
			}

			m_text.setString(string);
			setCursorPosition(m_cursor_pos + 1);
		}
	}
}

void MultilineInputBox::onStateChanged(State state)
{
	if (state == State::Default)
		m_oldText = m_text.getString();

	m_box.applyState(state);
}

void MultilineInputBox::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(m_box, states);

#ifdef _WIN32
	// Crop the text with GL Scissor
	glEnable(GL_SCISSOR_TEST);

	sf::Vector2f pos = getAbsolutePosition();
	glScissor((pos.x + Theme::borderSize) - 1, target.getSize().y - (pos.y + getSize().y) + 1, getSize().x - 2, getSize().y - 2);
	target.draw(m_text, states);

	glDisable(GL_SCISSOR_TEST);
#else
    // draw the text even though it won't have been cut.
    // FIXME: this needs to go away when opengl can be linked
	target.draw(m_text, states);
#endif

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
