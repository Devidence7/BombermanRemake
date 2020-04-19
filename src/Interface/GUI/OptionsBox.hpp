#ifndef GameGUI_OPTIONSBOX_HPP
#define GameGUI_OPTIONSBOX_HPP

#include "Widget.hpp"
#include "Utils/Arrow.hpp"
#include "Utils/ItemBox.hpp"

namespace GameGUI
{

// Widget for selecting one item from a list.
// Callback is triggered when selection is changed.
template <class T>
class OptionsBox : public Widget
{
public:
	OptionsBox();

	// Append a new item in the list
	// label: displayed label when selected
	// value: value associated
	// select: change the selected value to this when added
	void addItem(const sf::String& label, const T& value, bool select = false);

	// Make an item the current one
	// item_index: position of the item in the list
	void selectItem(size_t item_index);

	// Get the value of the selected item
	const T& getSelectedValue() const;

	// Get the index of the selected item
	size_t getSelectedIndex() const;

	// Select next item in the list
	void selectNext();

	// Select previous item in the list
	void selectPrevious();

	// callbacks
	void onStateChanged(State state);
	void onMouseMoved(float x, float y);
	void onMousePressed(float x, float y);
	void onMouseReleased(float x, float y);
	void onKeyPressed(sf::Keyboard::Key key);
	void onKeyReleased(sf::Keyboard::Key key);

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void updateArrow(ItemBox<Arrow>& arrow, float x, float y);

	struct Item
	{
		Item(const sf::String& string, const T& value);

		sf::String label;
		T value;
	};

	typedef std::vector<Item> ItemVector;
	ItemVector         m_items;
	size_t             m_current_index;

	// Visual components
	ItemBox<sf::Text>  m_box;
	ItemBox<Arrow>     m_arrow_left;
	ItemBox<Arrow>     m_arrow_right;
};

}

#include "OptionsBox.inl"

#endif // GameGUI_OPTIONSBOX_HPP
