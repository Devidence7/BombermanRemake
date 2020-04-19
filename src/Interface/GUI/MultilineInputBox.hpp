#ifndef GameGUI_MULTILINE_INPUT_BOX
#define GameGUI_MULTILINE_INPUT_BOX

#include "Widget.hpp"
#include "Utils/Box.hpp"

namespace GameGUI
{

 // Multi-line text editor.
class MultilineInputBox : public Widget
{
public:
	MultilineInputBox(float width = 200.f, unsigned int lines = 3);

	void setText(const sf::String& string);
	const sf::String& getText() const;

	bool isEmpty() const;

	void setLines(unsigned int lines);
	unsigned int getLines() const;

	void setCursorPosition(size_t index);
	size_t getCursorPosition() const;

	void setBlinkPeriod(float period);
	float getBlinkPeriod() const;

protected:
	// Callbacks
	void onKeyPressed(sf::Keyboard::Key key);
	void onMousePressed(float x, float y);
	void onTextEntered(sf::Uint32 unicode);

	// FIXME: sometimes these keep the hovered state
	void onStateChanged(State state);

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	Box         m_box;

	std::string m_oldText;
	sf::Text    m_text;

	mutable sf::RectangleShape m_cursor;
	mutable sf::Clock  m_cursor_timer;
	float			   m_blink_period;
	// TODO: add cursor delay (.25s)

	size_t m_max_characters;
	size_t m_cursor_pos;
};

}

#endif // !GameGUI_MULTILINE_INPUT_BOX
