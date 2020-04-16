#ifndef GameGUI_THEME_HPP
#define GameGUI_THEME_HPP

#include <map>
#include <string>
#include <SFML/Window.hpp>
#include <SFML/Graphics/Font.hpp>

#include "Widget.hpp"
#include "Utils/Box.hpp"

// TODO: make this a static namespace

namespace GameGUI
{

class Theme
{
public:
	// Load the GUI global font from a file
	static bool loadFont(const std::string& path);
	// Load the GUI global font from an already loaded font.
	static bool loadFont(const sf::Font* font);
	static const sf::Font& getFont();

	// Load the GUI spritesheet from a file
	// debug: load the texture's debug strip.
	static bool loadTexture(const std::string& path, bool debug = false);
	// Load the GUI spritesheet from an already loaded texture.
	// debug: load the texture's debug strip.
	static bool loadTexture(const sf::Texture* texture, bool debug = false);
	static const sf::Texture& getTexture();

	// Returns the rect of the Box's corresponding State on the spritesheet
	// e.g. Button, State::Pressed for the pressed texture for GameGUI::Button
	static const sf::IntRect& getTextureRect(Box::Type type, State state);

	// Returns the rect of the Arrow on the Spritesheet
	static const sf::IntRect& getCrossTextureRect();

	static const sf::IntRect& getArrowTextureRect();

	// Widget height based on text size
	static float getBoxHeight();

	// Height of a line of text
	static float getLineSpacing();

	static sf::Color hexToRgb(std::string hexcolor);

	static size_t textCharacterSize;

	// Properties of visual text
	struct TextStyle
	{
		sf::Color textColor; // Color of regular text
		sf::Color textColorHover; // Color of text when hovered
		sf::Color textColorFocus; // Color of text when focused
//		sf::Color textColorSelect;
	};

	// TODO: Text Highlight Color
	// TODO: Text Edit Highlight Color

	// TODO: buttons, labels, input boxes
//	static TextStyle text;
//	static TextStyle label;
//	static TextStyle input;
//	static TextStyle button;

	// Used for text by itself.
	static TextStyle label;
	// Used for text in buttons
	static TextStyle click;
	// Used for text in InputBoxes
	static TextStyle input;

	static sf::Color tickColor;
	static sf::Color arrowColor;

	static sf::Color windowBgColor;
	static float borderSize; // I think this is unused, but normally it would be the size of lines around things like buttons and sliders
	static float minWidgetWidth;

	static float PADDING; // Spacing inside widget
	static float MARGIN;  // Spacing between widgets

	// TODO: more keys for stuff
//	static sf::Keyboard::Key returns = sf::Keyboard::Key::Return;
//	static sf::Keyboard::Key firstWidget = sf::Keyboard::Key::Home;
	static sf::Keyboard::Key previousWidgetKey;
	static sf::Keyboard::Key nextWidgetKey;
//	static sf::Keyboard::Key lastWidget = sf::Keyboard::Key::End;
//	static sf::Keyboard::Key back = sf::Keyboard::Key::BackSpace | sf::Keyboard::Key::Esc;
//	static sf::Keyboard::Key cancel = sf::Keyboard::Key::Esc;

	enum class TriggerCallback
	{
		OnPress,
		OnRelease
	} callbackTriggerMode = TriggerCallback::OnRelease;

private:
	enum TextureID
	{
		// Pink
		BOX_CLICK_DEFAULT,
		BOX_CLICK_HOVERED,
		BOX_CLICK_PRESSED,
		BOX_CLICK_FOCUSED,

		// Blue
		BOX_PROGRESS_DEFAULT,
		BOX_PROGRESS_HOVERED,
		BOX_PROGRESS_PRESSED,
		BOX_PROGRESS_FOCUSED,

		// Red
		BOX_INPUT_DEFAULT,
		BOX_INPUT_HOVERED,
		BOX_INPUT_FOCUSED,
		BOX_INPUT_PRESSED,

		// Lime
		BOX_SLIDER_DEFAULT,
		BOX_SLIDER_HOVERED,
		BOX_SLIDER_FOCUSED,
		BOX_SLIDER_PRESSED,

		// Yellow
		BOX_SLIDER_HANDLE_DEFAULT,
		BOX_SLIDER_HANDLE_HOVERED,
		BOX_SLIDER_HANDLE_FOCUSED,
		BOX_SLIDER_HANDLE_PRESSED,

		// Cyan
		BOX_TICK_DEFAULT,
		BOX_TICK_HOVERED,
		BOX_TICK_FOCUSED,
		BOX_TICK_PRESSED,

		CROSS,
		ARROW,
		_TEX_COUNT
	};

	// FIXME: static sf::Font;
	//		  static sf::Texture;
	// 
	// these cause applications built with GameGUI to crash when in debug.
	// for some reason, these fail to allocate their memory in debug.
	// this results in a full halt of the program. obviously, this is
	// unacceptable, but in order to fix it requires major design changes.
	// before we do this, we must fully complete the library proper.
	static sf::Font    m_font;
	static sf::Texture m_texture;

	static sf::IntRect m_subrects[TextureID::_TEX_COUNT];

	// updates the current texture.
	// debug: whether or not to use the debug side of the texture
	static bool updateTexture(bool debug = false);
};

}

#endif // !GameGUI_THEME_HPP
