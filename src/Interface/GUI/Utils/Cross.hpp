#ifndef GameGUI_CROSS_HPP
#define GameGUI_CROSS_HPP

#include <SFML/Graphics.hpp>

namespace GameGUI
{

// The tickmark that in toggle boxes
class Cross : public sf::Drawable
{
public:
	Cross();

	void setPosition(float x, float y);

	// Moves the cross delta.
	void move(float dx, float dy);

	void setSize(float size);
	sf::Vector2f getSize() const;

	// Sets the color of the cross.
	void setColor(const sf::Color& color);

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	// Updates the size of the cross according to the size of the texture.
	void updateGeometry(float x, float y);

	sf::Vertex m_vertices[4];
};

}

#endif // !GameGUI_CROSS_HPP
