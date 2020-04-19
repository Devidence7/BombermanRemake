#ifndef GameGUI_ITEMBOX_HPP
#define GameGUI_ITEMBOX_HPP

#include "Box.hpp"

namespace GameGUI
{

// TODO: alignment settings for m_item

// Utility class used by widgets for holding visual components
template <class T>
class ItemBox : public Box
{
public:
	ItemBox(Box::Type type = Box::Type::PushButton);
	ItemBox(const T& item, Box::Type type = Box::Type::PushButton);

	void applyState(State state);

	// returns the Item in the Box
	inline T& item() { return m_item;}
	inline const T& item() const { return m_item;}

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	
	void onPress() override;
	void onRelease() override;

	T m_item;
};

}

#include "ItemBox.inl"

#endif // !GameGUI_ITEMBOX_HPP
