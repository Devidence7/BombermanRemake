#ifndef GameGUI_COMBOBOX_HPP
#define GameGUI_COMBOBOX_HPP

#include "Widget.hpp"
#include "Utils/Arrow.hpp"
#include "Utils/ItemBox.hpp"
#include <vector>

namespace GameGUI
{

// TODO: scrollbar for combobox
// TODO: finish
// TODO: borders for dropdown
// TODO: minimum length decidable

// Selection widget that displays the current item, 
// and can pop up a list of selectable items.
// A callback is triggered when selection is changed.
template <class T>
class ComboBox : public Widget
{
public:
	ComboBox(sf::RenderWindow* window);

	// Append a new item in the list
	// label: displayed label when selected
	// value: value associated
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

	// Opens the drop down menu
	void open();
	// Closes the drop down menu
	void close();

	// Is the drop down active?
	bool isOpen() const { return m_open; }
	
	// custom overrides
	bool containsPoint(const sf::Vector2f& point) const override;

	// callbacks ---------------------------------------------------------------
	void onStateChanged(State state);
	void onMouseMoved(float x, float y);
	void onMousePressed(float x, float y);
	void onMouseReleased(float x, float y);
	void onKeyPressed(sf::Keyboard::Key key);
	void onKeyReleased(sf::Keyboard::Key key);

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void updateArrow(ItemBox<Arrow>& arrow, float x, float y);

	void redrawDropDown();

	struct Item
	{
		Item(const sf::String& string, const T& value);

		sf::String label;
		T value;
	};

	// TODO: make this a Map
	typedef std::vector<Item> ItemVector;

	ItemVector         m_items;
	size_t             m_current_index;

	// Visual components
	// TODO: InputBox
	ItemBox<sf::Text>  m_box;
	ItemBox<Arrow>     m_dropDownArrow;

	bool m_open = false;

	sf::RenderWindow m_dropDownBox;
	// FIXME: referencing another window here is probably overkill.
	// I'd like to find a way to to do this without any outside help.
	sf::RenderWindow* m_parentWindow;
};

}

#include "ComboBox.inl"

#endif // GameGUI_COMBOBOX_HPP
