#ifndef GameGUI_BUTTON_HPP
#define GameGUI_BUTTON_HPP

#include "Widget.hpp"
#include "Utils/ItemBox.hpp"

namespace GameGUI
{

	// Simple press button.
// A callback is triggered when the button is activated.
class Button : public Widget
{
public:
	Button(const sf::String& string);

	// Set the displayed button label
	void setString(const sf::String& string);
	// Return the displayed button label.
	const sf::String& getString() const;

	// callbacks 
	void onStateChanged(State state);
	void onMouseMoved(float x, float y);
	void onMouseReleased(float x, float y);
	void onKeyPressed(sf::Keyboard::Key key);
	void onKeyReleased(sf::Keyboard::Key key);

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	ItemBox<sf::Text> m_box;
};

}

#endif // !GameGUI_BUTTON_HPP
