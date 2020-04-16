#include "SpriteButton.hpp"
#include "Theme.hpp"

// TODO: fix button not showing unpressed state

namespace GameGUI
{

SpriteButton::SpriteButton(const sf::Texture& texture, const sf::String& string) : m_pressed(false)
{
	size_t width = texture.getSize().x;
	size_t height = texture.getSize().y / 4; // default, hover, pressed, focus

	m_background.setTexture(texture);
	m_background.setTextureRect(sf::IntRect(0, 0, width, height));

	setSize(sf::Vector2f(width, height));

	m_text.setFont(Theme::getFont());
	m_text.setCharacterSize(Theme::textCharacterSize);

	setString(string);
}

void SpriteButton::setString(const sf::String& string)
{
	m_text.setString(string);
	centerText(m_text);
}

const sf::String& SpriteButton::getString() const
{
	return m_text.getString();
}

void SpriteButton::setFont(const sf::Font& font)
{
	m_text.setFont(font);
	centerText(m_text);
}

const sf::Font& SpriteButton::getFont() const
{
	return *m_text.getFont();
}

void SpriteButton::setTextSize(size_t size)
{
	m_text.setCharacterSize(size);
	centerText(m_text);
}

void SpriteButton::setTexture(const sf::Texture & texture)
{
	size_t width = texture.getSize().x;
	size_t height = texture.getSize().y / 4; // default, hover, pressed, focus

	m_background.setTexture(texture);
	m_background.setTextureRect(sf::IntRect(0, 0, width, height));

	setSize(sf::Vector2f(width, height));
}

void SpriteButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(m_background, states);
	target.draw(m_text, states);
}

// Callbacks

void SpriteButton::onStateChanged(State state)
{
	sf::Vector2f size = getSize();
	switch (state)
	{
	case State::Default:
		m_background.setTextureRect(sf::IntRect(0, 0, size.x, size.y));
		break;
	case State::Hovered:
		m_background.setTextureRect(sf::IntRect(0, size.y, size.x, size.y));
		break;
	case State::Pressed:
		m_background.setTextureRect(sf::IntRect(0, size.y * 2, size.x, size.y));
		break;
	case State::Focused:
		m_background.setTextureRect(sf::IntRect(0, size.y * 3, size.x, size.y));
		break;
	}
}

void SpriteButton::onMouseMoved(float x, float y)
{
	if (isFocused())
	{
		if (containsPoint({ x, y }) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
			press();
		else
			release();
	}

	if (getState() == State::Pressed)
	{
		if (containsPoint({ x, y })) // TODO: check if this should check if left mouse button is pressed
			press();
		else
			release();
	}
}

void SpriteButton::onMousePressed(float, float)
{
	press();
}

void SpriteButton::onMouseReleased(float x, float y)
{
	release();

	if (containsPoint({x, y}))
		triggerCallback();
}

void SpriteButton::onKeyPressed(sf::Keyboard::Key key)
{
	if (key == sf::Keyboard::Return)
	{
		press();
		triggerCallback();
		// HACK: we don't have to do this in onMousePressed, why do we have to do it here?
		onStateChanged(State::Pressed);
	}
}

void SpriteButton::onKeyReleased(sf::Keyboard::Key key)
{
	if (key == sf::Keyboard::Return)
	{
		release();
		// HACK: we don't have to do this in onMousePressed, why do we have to do it here?
		onStateChanged(State::Focused);
	}
}

void SpriteButton::press()
{
	if (!m_pressed)
	{
		m_pressed = true;
		m_text.move(0, 1);
	}
}

void SpriteButton::release()
{
	if (m_pressed)
	{
		m_pressed = false;
		m_text.move(0, -1);
	}
}

void SpriteButton::centerText(sf::Text& text)
{
	sf::FloatRect t = text.getLocalBounds();
	sf::FloatRect b = m_background.getLocalBounds();

	text.setPosition(int(b.width - t.width) / 2, (int(b.height - t.height) / 2) - 4);
}

}

