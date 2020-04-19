#ifndef GameGUI_MENU_HPP
#define GameGUI_MENU_HPP

#include "VerticalBoxLayout.hpp"

#include <SFML/Graphics.hpp>

// FIXME: adding a widget to a menu more than once causes an infinite loop
// look into this and determine the cause and if it's a problem, or just misuse.
// it's probably due to the autistic way we handle containers

namespace GameGUI
{

// This object is required to use GameGUI layouts.
class Menu : public GameGUI::VerticalBoxLayout
{
public:
	Menu(sf::RenderTarget& window);

	// Handle event and send it to widgets
	// triggered widget ID, or -1 if none
	int onEvent(const sf::Event& event);

	void triggerCallback(const Widget* widget) override;

private:
	// Get mouse cursor relative position
	// x: absolute x position from the event
	// y: absolute y position from the event
	// relative: if not NULL, compute mouse position relative to this widget
	sf::Vector2f convertMousePosition(int x, int y) const;

	sf::RenderTarget& m_window;
	const Widget*     m_triggered;
};

}

#endif // !GameGUI_MENU_HPP
