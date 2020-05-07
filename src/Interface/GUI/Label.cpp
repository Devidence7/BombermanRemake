#include "Label.hpp"
#include "Theme.hpp"

namespace GameGUI
{

Label::Label(const sf::String& string)
{
	m_text.setFont(Theme::getFont());
	m_text.setPosition(Theme::PADDING, Theme::PADDING);
	//m_text.setColor(Theme::label.textColor);
	m_text.setFillColor(Theme::label.textColor);
	m_text.setCharacterSize(Theme::textCharacterSize);
	setText(string);

	setSelectable(false);
}

void Label::setText(const sf::String& string)
{
	m_text.setString(string);
	updateGeometry();
}

const sf::String& Label::getText() const
{
	return m_text.getString();
}

void Label::setColor(const sf::Color& color)
{
	//m_text.setColor(color);
	 m_text.setFillColor();
	//m_text.getFillColor();
}

const sf::Color& Label::getColor() const
{
	//return m_text.getFillColor();
	//return m_text.getFillColor();
	return m_text.getColor();
}

void Label::setTextSize(size_t size)
{
	m_text.setCharacterSize(size);
	updateGeometry();
}

size_t Label::getTextSize() const
{
	return m_text.getCharacterSize();
}

void Label::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(m_text, states);
}

void Label::updateGeometry()
{
	Widget::setSize(sf::Vector2f(m_text.getLocalBounds().width + Theme::PADDING * 2,
								 m_text.getLocalBounds().height + Theme::PADDING * 2));
}

}
