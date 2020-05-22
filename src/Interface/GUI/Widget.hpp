#ifndef GameGUI_WIDGET_HPP
#define GameGUI_WIDGET_HPP

#include <SFML/Graphics.hpp>

namespace GameGUI
{

enum class State
{
	Default,
	Hovered,
	Pressed,
	Focused,
	Focused_Hovered,
	Focused_Pressed,
};

class Layout;

// Abstract base class for GameGUI widgets
class Widget : public sf::Drawable
{
public:
	Widget();

	// Set Callback ID
	void setID(int id);
	// Return Callback ID
	int getID() const;

	bool transformableOptions = true;
	bool editedWidget = true;
	int goUpUp = 1;
	int goUpLeft = 1;
	int goDownDown = 1;
	int goDownRight = 1;


	// set the Widget's position
	// this is relative to the origin of the Layout in which the Widget is held.
	// FIXME: this is overridden when any widget is added to a layout
	void setPosition(const sf::Vector2f& pos);
	// Return widget's position
	const sf::Vector2f& getPosition() const;

	// Returns the widget's Window coordinates.
	sf::Vector2f getAbsolutePosition() const;

	// Get widget's dimensions
	const sf::Vector2f& getSize() const;

	// Check if a point is inside the widget
	virtual bool containsPoint(const sf::Vector2f& point) const;

	// Check if the widget can be selected and trigger events
	bool isSelectable() const;

	// Check if the widget is currently focused
	bool isFocused() const;

	// callbacks
	virtual void onStateChanged(State state);
	virtual void onWindowLostFocus();
	virtual void onMouseMoved(float x, float y);
	virtual void onMousePressed(float x, float y);
	virtual void onMouseReleased(float x, float y);
	virtual void onMouseWheelMoved(int delta);
	virtual Widget* searchFinalFocussedWidget();
	virtual void onKeyPressed(sf::Keyboard::Key key);
	virtual void onKeyReleased(sf::Keyboard::Key key);
	virtual void onTextEntered(sf::Uint32 unicode);

protected:
	void setSize(const sf::Vector2f& size);

	friend class Layout;
	friend class FormLayout;
	friend class HorizontalBoxLayout;
	friend class VerticalBoxLayout;

	void setSelectable(bool selectable);

	// Notify parent that the widget has been triggered by user input
	virtual void triggerCallback(const Widget* triggered = NULL);

	void setState(State state);
	State getState() const;

	// Set the widget's container (parent)
	void setParent(Layout* parent);
	Layout* getParent() { return m_parent; }

	// Returns the layout, if the widget is one.
	virtual Layout* toLayout() { return NULL; }

	virtual void recomputeGeometry() {};

	const sf::Transform& getTransform() const;

private:
	// TODO: perhaps put parent, previous and next into
	// Protected so that derived classes can access them
	// Layout this Widget is a part of.
	Layout*      m_parent;
	// Widget before this one in layout.
	Widget*      m_previous;
	// Widget after this one in layout.
	Widget*      m_next;

	State        m_state;
	sf::Vector2f m_position;
	sf::Vector2f m_size;
	int          m_id;
	bool         m_selectable;

	// TODO: look into not using transforms for widgets
	sf::Transform m_transform;
};

}

#endif // !GameGUI_WIDGET_HPP
