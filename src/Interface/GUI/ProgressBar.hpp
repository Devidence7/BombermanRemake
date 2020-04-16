#ifndef GameGUI_PROGRESS_BAR_HPP
#define GameGUI_PROGRESS_BAR_HPP

#include "Widget.hpp"
#include "Utils/Box.hpp"

namespace GameGUI
{

// TODO: make text optional

// Horizontal progress bar.
// Passive: Cannot be focused or trigger an event.
class ProgressBar : public Widget
{
public:
	ProgressBar(float width = 200.f);

	// Value as a percentage of 100
	void setValue(float value);
	// Value as a percentage of 100
	float getValue() const;

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	Box        m_box;
	mutable sf::Vertex m_bar[4];
	sf::Text   m_text;
	float      m_value;

//	mutable sf::Clock  m_cursor_timer;
//	float			   m_blink_period;
};

}

#endif // !PROGRESS_BAR_HPP

