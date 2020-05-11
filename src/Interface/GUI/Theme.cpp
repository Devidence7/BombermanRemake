#include "Theme.hpp"

namespace GameGUI
{

size_t    Theme::textCharacterSize = 12;
Theme::TextStyle Theme::label;
Theme::TextStyle Theme::click;
Theme::TextStyle Theme::input;
sf::Color Theme::tickColor = sf::Color::Black;
sf::Color Theme::arrowColor = sf::Color::White;
sf::Color Theme::windowBgColor;
float     Theme::borderSize = 1.f;
float     Theme::minWidgetWidth = 86;
float     Theme::PADDING = 1.f;
float     Theme::MARGIN = 7.f;

sf::Keyboard::Key Theme::nextWidgetKey = sf::Keyboard::Down;
sf::Keyboard::Key Theme::previousWidgetKey = sf::Keyboard::Up;

sf::Font Theme::m_font;
sf::Texture Theme::m_texture;
sf::IntRect Theme::m_subrects[_TEX_COUNT];

bool Theme::loadFont(const std::string& filename)
{
	return m_font.loadFromFile(filename);
}

bool Theme::loadFont(const sf::Font* font)
{
	if (font)
	{
		m_font = *font;
		return true;
	}

	return false;
}

const sf::Font& Theme::getFont()
{
	return m_font;
}

bool Theme::loadTexture(const std::string& filename, bool debug)
{
	// if the texture loaded successfully
	if (m_texture.loadFromFile(filename))
		// if we are able to successfully update the texture
		if (updateTexture(debug))
			return true;

	return false;
}

bool Theme::loadTexture(const sf::Texture* texture, bool debug)
{
	if (texture) // if it's actually a valid pointer
	{
		m_texture = *texture;

		// if we are able to successfully update the texture
		if (updateTexture(debug))
			return true;
	}

	return false;
}

const sf::Texture& Theme::getTexture()
{
	return m_texture;
}

// TODO: improve this
const sf::IntRect& Theme::getTextureRect(Box::Type type, State state)
{
	TextureID id;

	if (state == State::Default)
	{
		if (type == Box::Type::PushButton)
			id = BOX_CLICK_DEFAULT;
		else if (type == Box::Type::Progress)
			id = BOX_PROGRESS_DEFAULT;
		else if (type == Box::Type::Input)
			id = BOX_INPUT_DEFAULT;
		else if (type == Box::Type::Slider)
			id = BOX_SLIDER_DEFAULT;
		else if (type == Box::Type::SliderHandle)
			id = BOX_SLIDER_HANDLE_DEFAULT;
		else if (type == Box::Type::Tick)
			id = BOX_TICK_DEFAULT;
	}
	else if (state == State::Hovered)
	{
		if (type == Box::Type::PushButton)
			id = BOX_CLICK_HOVERED;
		else if (type == Box::Type::Progress)
			id = BOX_PROGRESS_HOVERED;
		else if (type == Box::Type::Input)
			id = BOX_INPUT_HOVERED;
		else if (type == Box::Type::Slider)
			id = BOX_SLIDER_HOVERED;
		else if (type == Box::Type::SliderHandle)
			id = BOX_SLIDER_HANDLE_HOVERED;
		else if (type == Box::Type::Tick)
			id = BOX_TICK_HOVERED;
	}
	else if (state == State::Pressed)
	{
		if (type == Box::Type::PushButton)
			id = BOX_CLICK_PRESSED;
		else if (type == Box::Type::Progress)
			id = BOX_PROGRESS_PRESSED;
		else if (type == Box::Type::Input)
			id = BOX_INPUT_PRESSED;
		else if (type == Box::Type::Slider)
			id = BOX_SLIDER_PRESSED;
		else if (type == Box::Type::SliderHandle)
			id = BOX_SLIDER_HANDLE_PRESSED;
		else if (type == Box::Type::Tick)
			id = BOX_TICK_PRESSED;
	}
	else if (state == State::Focused)
	{
		if (type == Box::Type::PushButton)
			id = BOX_CLICK_FOCUSED;
		else if (type == Box::Type::Progress)
			id = BOX_PROGRESS_FOCUSED;
		else if (type == Box::Type::Input)
			id = BOX_INPUT_FOCUSED;
		else if (type == Box::Type::Slider)
			id = BOX_SLIDER_FOCUSED;
		else if (type == Box::Type::SliderHandle)
			id = BOX_SLIDER_HANDLE_FOCUSED;
		else if (type == Box::Type::Tick)
			id = BOX_TICK_FOCUSED;
	}

	return m_subrects[id];
}

const sf::IntRect& Theme::getCrossTextureRect()
{
	return m_subrects[CROSS];
}

const sf::IntRect& Theme::getArrowTextureRect()
{
	return m_subrects[ARROW];
}

float Theme::getBoxHeight()
{
	return getLineSpacing() + borderSize * 2 + PADDING * 2;
}

float Theme::getLineSpacing()
{
	return m_font.getLineSpacing(textCharacterSize);
}

sf::Color Theme::hexToRgb(std::string hexcolor)
{
	// TODO: make this more versatile

//	if (hexcolor.length > 7)
//		return;

//	std::cout << "converting " << hexcolor << std::endl;

	// TODO: improve this
	if (hexcolor[0] != '#')
		hexcolor.insert(0, "#");

	sf::Color color(sf::Color::White);
	if (hexcolor.size() == 7) // #ffffff
	{
		color.r = sf::Uint8( strtoul(hexcolor.substr(1, 2).c_str(), NULL, 16) );
		color.g = sf::Uint8( strtoul(hexcolor.substr(3, 2).c_str(), NULL, 16) );
		color.b = sf::Uint8( strtoul(hexcolor.substr(5, 2).c_str(), NULL, 16) );
	}
	else if (hexcolor.size() == 4) // #fff, shorthand
	{
		color.r = sf::Uint8( strtoul(hexcolor.substr(1, 1).c_str(), NULL, 16) * 17 );
		color.g = sf::Uint8( strtoul(hexcolor.substr(2, 1).c_str(), NULL, 16) * 17 );
		color.b = sf::Uint8( strtoul(hexcolor.substr(3, 1).c_str(), NULL, 16) * 17 );
	}

//	std::cout << "result " << std::to_string(color.r) << ", " << std::to_string(color.g) << ", " << std::to_string(color.b) << std::endl;

	return color;
}

bool Theme::updateTexture(bool debug)
{
	// TODO: get color from style sheet
	// get the color of the last row of pixels in the style sheet
	// perhaps the last block of 8 x 8 pixels can be retrieved
	// GameGUI::Theme::windowBgColor = GameGUI::Theme::getThemeColor(2);
	// the color of the 2nd pixel from 0

	sf::IntRect subrect;
	//		subrect.width = m_texture.getSize().x;
	//		subrect.height = m_texture.getSize().y / _TEX_COUNT;

	subrect.width = 6;
	subrect.height = 6;

	if (debug)
	{
		// TODO: completely reload the texture
		subrect.left = 6;
	}

	for (int i = 0; i < _TEX_COUNT; ++i)
	{
		m_subrects[i] = subrect;
		subrect.top += subrect.height;
	}

	borderSize = subrect.width / 3;

	return true;
}

}
