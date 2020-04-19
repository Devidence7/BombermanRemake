#ifndef GameGUI_ARROW_HPP
#define GameGUI_ARROW_HPP

#include <SFML/Graphics.hpp>

namespace GameGUI
{

class Arrow : public sf::Drawable
{
public:
	enum class Direction { Left, Right, Top, Bottom };

	Arrow(Direction direction);

	void setFillColor(const sf::Color& color);

	void move(float dx, float dy);
	void setPosition(float x, float y);

	sf::Vector2f getSize() const;

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	// Updates the size of this object according to size of the Arrow texture.
	void updateGeometry(float x, float y, Direction direction);

	sf::Vertex m_vertices[4];
	Direction m_direction;
};

}

#endif // !GameGUI_ARROW_HPP
