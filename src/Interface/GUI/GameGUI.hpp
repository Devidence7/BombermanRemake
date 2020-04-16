#ifndef GameGUI_HPP
#define GameGUI_HPP

// TODO: don't want to include all this stuff if we don't want to

// Widgets
#include "Button.hpp"
#include "CheckBox.hpp"
#include "ComboBox.hpp"
#include "Image.hpp"
#include "Label.hpp"
#include "OptionsBox.hpp"
#include "ProgressBar.hpp"
#include "Slider.hpp"
#include "SpriteButton.hpp"
#include "InputBox.hpp"
#include "MultilineInputBox.hpp"

// TODO: look into making Widgets more easily added on to
// ComboBox
// OptionsBox
// InputBox

// TODO: Subwindow
// a widget that can be used by other widgets.
// used to create things like dropdowns, tooltips, and popups

// Layouts
#include "Layouts/FormLayout.hpp"
#include "Layouts/HorizontalBoxLayout.hpp"
#include "Layouts/VerticalBoxLayout.hpp"
#include "Layouts/Menu.hpp"

namespace GameGUI
{
	extern const int LIB_VERSION;
}

#endif // !GameGUI_HPP
