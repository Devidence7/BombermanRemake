#include "Widget.hpp"
#include "Layouts/Menu.hpp"
#include "Layouts/Layout.hpp"

namespace GameGUI
{

Widget::Widget(): m_parent(NULL), m_previous(NULL), m_next(NULL), m_state(State::Default), m_id(-1), m_selectable(true)
{
}

void Widget::setID(int id)
{
	m_id = id;
}

int Widget::getID() const
{
	return m_id;
}

void Widget::setPosition(const sf::Vector2f& pos)
{
	m_position = pos;
	m_transform = sf::Transform(
		1, 0, (int) pos.x,
		0, 1, (int) pos.y,
		0, 0, 1);
}

const sf::Vector2f& Widget::getPosition() const
{
	return m_position;
}

sf::Vector2f Widget::getAbsolutePosition() const
{
	sf::Vector2f position = m_position;

	for (Widget* parent = m_parent; parent != NULL; parent = parent->m_parent)
	{
		position.x += parent->m_position.x;
		position.y += parent->m_position.y;
	}

	return position;
}

void Widget::setSize(const sf::Vector2f& size)
{
	m_size = size;
	if (m_parent != NULL)
	{
		Widget* parent = m_parent;
		parent->recomputeGeometry();
	}
}

const sf::Vector2f& Widget::getSize() const
{
	return m_size;
}

bool Widget::containsPoint(const sf::Vector2f& point) const
{
	return point.x > 0.f && point.x < m_size.x &&
	       point.y > 0.f && point.y < m_size.y;
}

bool Widget::isSelectable() const
{
	return m_selectable;
}

bool Widget::isFocused() const
{
	return m_state == State::Focused || m_state == State::Pressed;
}

void Widget::setSelectable(bool selectable)
{
	m_selectable = selectable;
}

void Widget::triggerCallback(const Widget* triggered)
{
	// Notify parent that a widget is triggered
	// If NULL, this is the widget being triggered
	if (m_parent != NULL)
		m_parent->triggerCallback(triggered == NULL ? this : triggered);
}

void Widget::setParent(Layout* parent)
{
	m_parent = parent;
}

void Widget::setState(State state)
{
	m_state = state;
	onStateChanged(state);
}

State Widget::getState() const
{
	return m_state;
}

const sf::Transform& Widget::getTransform() const
{
	return m_transform;
}

// callbacks

void Widget::onStateChanged(State) {}
void Widget::onWindowLostFocus() {}
void Widget::onMouseMoved(float, float) {}
void Widget::onMousePressed(float, float) {}
void Widget::onMouseReleased(float, float) {}
void Widget::onMouseWheelMoved(int) {}
void Widget::onKeyPressed(sf::Keyboard::Key) {}
void Widget::onKeyReleased(sf::Keyboard::Key) {}
void Widget::onTextEntered(sf::Uint32) {}

}
