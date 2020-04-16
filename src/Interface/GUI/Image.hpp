#ifndef GameGUI_IMAGE_HPP
#define GameGUI_IMAGE_HPP

#include "Widget.hpp"

namespace GameGUI
{

// Widget for displaying a picture.
// Passive: Cannot be focused or trigger an event.
class Image : public Widget
{
public:
	Image();
	Image(const sf::Texture& texture);

	void setTexture(const sf::Texture& texture);

	// Sets a tint on the texture of the image.
	void setColor(const sf::Color& color);

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	sf::Vertex         m_vertices[4];
	const sf::Texture* m_texture;
};

}

#endif // !GameGUI_IMAGE_HPP
