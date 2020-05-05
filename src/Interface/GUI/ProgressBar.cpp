#include "ProgressBar.hpp"
#include "Theme.hpp"

namespace GameGUI
{

const sf::Color BAR_COLOR = sf::Color(2, 175, 0);

ProgressBar::ProgressBar(float width) : m_box(Box::Type::Progress), m_value(0.f)
{
	m_box.setSize(width, Theme::getBoxHeight());
	m_text.setFont(Theme::getFont());
	m_text.setColor(Theme::input.textColor);
	//m_text.setFillColor(Theme::input.textColor);
	m_text.setCharacterSize(Theme::textCharacterSize);

	// Build bar
	m_bar[0].position = sf::Vector2f(Theme::borderSize, Theme::borderSize);
	m_bar[1].position = sf::Vector2f(Theme::borderSize, Theme::borderSize);
	m_bar[2].position = sf::Vector2f(Theme::borderSize, Theme::getBoxHeight() - Theme::borderSize);
	m_bar[3].position = sf::Vector2f(Theme::borderSize, Theme::getBoxHeight() - Theme::borderSize);

	for (int i = 0; i < 4; ++i)
		m_bar[i].color = BAR_COLOR;

	setValue(m_value);
	setSelectable(false);
	setSize(m_box.getSize());

//	m_blink_period = .5f;
}

void ProgressBar::setValue(float value)
{
	float x = Theme::borderSize + (getSize().x - (Theme::borderSize * 2)) * value / 100;
	m_bar[1].position.x = m_bar[2].position.x = x;

	m_text.setString(std::to_string(static_cast<int>(value)) + "%");
	m_box.centerText(m_text);

	m_value = value;
}

float ProgressBar::getValue() const
{
	return m_value;
}

void setColorOfQuad(sf::Vertex (&quads)[4], sf::Color& color)
{
	for (int i = 0; i < 4; ++i)
		quads[i].color = color;
}

void ProgressBar::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(m_box, states);

	/*
	// Make it blink
	float timer = m_cursor_timer.getElapsedTime().asSeconds();
	if (timer >= m_blink_period)
		m_cursor_timer.restart();

	// Updating in the drawing method, deal with it
	sf::Uint8 a = 255 - (255 * timer / m_blink_period);

//		setColorOfQuad(m_bar, color);

	for (int i = 0; i < 4; ++i)
		m_bar[i].color = sf::Color(BAR_COLOR.r, BAR_COLOR.g, BAR_COLOR.b, a);
	*/

	target.draw(m_bar, 4, sf::Quads, states);

	target.draw(m_text, states);
}

}
