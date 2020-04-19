#include "../Theme.hpp"

namespace GameGUI
{

// TODO: are these also creating boxes in their constructors?
// we might be run into the same problems experience with
// OptionsBox and Arrow

template <class T>
ItemBox<T>::ItemBox(Box::Type type) : Box(type)
{
	applyState(State::Default);
}

template <class T>
ItemBox<T>::ItemBox(const T& item, Box::Type type) : Box(type), m_item(item)
{
	applyState(State::Default);
}


template <class T>
void ItemBox<T>::applyState(State state)
{
	Box::applyState(state);
	switch (state)
	{
		case State::Default:
			m_item.setFillColor(m_type == Box::Type::PushButton ? Theme::click.textColor : Theme::input.textColor);
			break;
		case State::Hovered:
			m_item.setFillColor(m_type == Box::Type::PushButton ? Theme::click.textColorHover : Theme::input.textColorHover);
			break;
		case State::Pressed:
		case State::Focused:
			m_item.setFillColor(m_type == Box::Type::PushButton ? Theme::click.textColorFocus : Theme::input.textColorFocus);
			break;

	}
}

template <class T>
void ItemBox<T>::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	Box::draw(target, states);
	target.draw(m_item, states);
}

template <class T>
void ItemBox<T>::onPress()
{
	m_item.move(0.f, 1.f);
}

template <class T>
void ItemBox<T>::onRelease()
{
	m_item.move(0.f, -1.f);
}

}
