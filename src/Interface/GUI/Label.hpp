#ifndef GameGUI_LABEL_HPP
#define GameGUI_LABEL_HPP

#include "Widget.hpp"


namespace GameGUI
{

// Displays a line of text.
// Passive: Cannot be focused or trigger an event.
class Label : public Widget
{
public:
	Label(const sf::String& string = "");

	void setText(const sf::String& string);
	const sf::String& getText() const;

	void setColor(const sf::Color& color);
	const sf::Color& getColor() const;

	void setTextSize(size_t size);
	size_t getTextSize() const;

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	// update the size of this Widget according to the text size.
	void updateGeometry();

	sf::Text m_text;
};

}

#endif // GameGUI_LABEL_HPP
