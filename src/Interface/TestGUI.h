#pragma once
#include <SFML/Graphics.hpp>
#include "GUI/GameGUI.hpp"
#include "GUI/Theme.hpp"


void testGUI() {

	enum Callbacks {
		TEXT,
		TEXT_LARGE,
		COLOR,
		ROTATION,
		BOLD,
		UNDERLINED,
		SCALE,
		NEW_BUTTON,
		VSLIDER,
		THEME_TEXTURE,
		TERMINATE,
		QUIT
	};

	bool doTerminateProgram(false);

	do {
		sf::RenderWindow window(sf::VideoMode(640, 480), "GameGUI 2", sf::Style::Titlebar | sf::Style::Close);

		GameGUI::Menu menu(window);
		menu.setPosition(sf::Vector2f(10, 10));

		GameGUI::Theme::loadFont("../textures/mainMenu/BOMBERMAN.ttf");
		GameGUI::Theme::loadTexture("../textures/interface/bomberman.png");
		GameGUI::Theme::textCharacterSize = 11;
		GameGUI::Theme::click.textColor = GameGUI::Theme::hexToRgb("#191B18");
		GameGUI::Theme::click.textColorHover = GameGUI::Theme::hexToRgb("#191B18");
		GameGUI::Theme::click.textColorFocus = GameGUI::Theme::hexToRgb("#000000");
		GameGUI::Theme::input.textColor = GameGUI::Theme::hexToRgb("#000000");
		GameGUI::Theme::input.textColorHover = GameGUI::Theme::hexToRgb("#CC7A00");
		GameGUI::Theme::input.textColorFocus = GameGUI::Theme::hexToRgb("#000000");
		GameGUI::Theme::windowBgColor = GameGUI::Theme::hexToRgb("#dddbde");
		GameGUI::Theme::PADDING = 2.f;

		
		GameGUI::HorizontalBoxLayout* hbox = menu.addHorizontalBoxLayout();
		GameGUI::FormLayout* form = hbox->addFormLayout();
		GameGUI::VerticalBoxLayout* vbox = hbox->addVerticalBoxLayout();
		GameGUI::HorizontalBoxLayout* hbox2 = vbox->addHorizontalBoxLayout();
		GameGUI::HorizontalBoxLayout* hbox3 = vbox->addHorizontalBoxLayout();

		// Textbox
		GameGUI::InputBox* textbox = new GameGUI::InputBox();
		textbox->setText("Hello, World!");
		form->addRow("Text", textbox, Callbacks::TEXT);

		form->addRow("Lots of Text", new GameGUI::MultilineInputBox, Callbacks::TEXT_LARGE);

		// Slider for rotation
		GameGUI::Slider* sliderRotation = new GameGUI::Slider();
		sliderRotation->setQuantum(1);
		form->addRow("Rotation", sliderRotation, Callbacks::ROTATION);

		// Slider for scale
		GameGUI::Slider* sliderScale = new GameGUI::Slider();
		form->addRow("Scale", sliderScale, Callbacks::SCALE);

		// Progress bars
		GameGUI::HorizontalBoxLayout* pbarbox = form->addHorizontalBoxLayout();
		GameGUI::ProgressBar* pbar0 = new GameGUI::ProgressBar(96);
		GameGUI::ProgressBar* pbar1 = new GameGUI::ProgressBar(96);
		pbarbox->add(pbar0);
		pbarbox->add(pbar1);

		// OptionsBox for color
		GameGUI::OptionsBox<sf::Color>* opt = new GameGUI::OptionsBox<sf::Color>();
		opt->addItem("Red", sf::Color::Red);
		opt->addItem("Blue", sf::Color::Blue);
		opt->addItem("Green", sf::Color::Green);
		opt->addItem("Yellow", sf::Color::Yellow);
		opt->addItem("White", sf::Color::White);
		form->addRow("Color", opt, Callbacks::COLOR);

		// OptionsBox for color
		GameGUI::ComboBox<sf::Color>* optcb = new GameGUI::ComboBox<sf::Color>(&window);
		optcb->addItem("Red", sf::Color::Red);
		optcb->addItem("Blue", sf::Color::Blue);
		optcb->addItem("Green", sf::Color::Green);
		optcb->addItem("Yellow", sf::Color::Yellow);
		optcb->addItem("White", sf::Color::White);
		form->addRow("Color", optcb);
		
		// Checbkox
		GameGUI::CheckBox* checkboxBold = new GameGUI::CheckBox();
		form->addRow("Bold text", checkboxBold, Callbacks::BOLD);

		GameGUI::CheckBox* checkboxUnderlined = new GameGUI::CheckBox();
		form->addRow("Underlined text", checkboxUnderlined, Callbacks::UNDERLINED);

		// Custom button
		sf::Texture spriteButton_texture;
		spriteButton_texture.loadFromFile("../resources/interface/themed-button.png");

		GameGUI::SpriteButton* customButton = new GameGUI::SpriteButton(spriteButton_texture, "Custom Button");

		customButton->setTextSize(18);
		form->add(customButton);

		// Textbox
		GameGUI::InputBox* changeTextureBox = new GameGUI::InputBox();
		changeTextureBox->setText("../resources/interface/texture_square.png");
		form->addRow("Theme Texture", changeTextureBox, Callbacks::THEME_TEXTURE);

		vbox->addLabel("This panel is on the left");

		// Textbox
		GameGUI::InputBox* textbox2 = new GameGUI::InputBox(100);
		textbox2->setText("Button name");
		hbox2->add(textbox2, Callbacks::NEW_BUTTON);
		hbox2->addButton("Create button", Callbacks::NEW_BUTTON);

		// Small progress bar
		hbox3->addLabel("Small progress bar");
		GameGUI::ProgressBar* pbar = new GameGUI::ProgressBar(40);
		hbox3->add(pbar);

		GameGUI::Slider* vslider = new GameGUI::Slider(100, GameGUI::Slider::Type::Vertical);
		hbox->add(vslider, Callbacks::VSLIDER);

		GameGUI::CheckBox* terminateProgram = new GameGUI::CheckBox();
		form->addRow("Terminate Program?", terminateProgram, Callbacks::TERMINATE);

		menu.addButton("Quit", Callbacks::QUIT);
		
		sf::Text text(textbox->getText(), GameGUI::Theme::getFont());
		text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
		text.setPosition(window.getSize().x / 2, 400);

		while (window.isOpen()) {
			sf::Event event;
			while (window.pollEvent(event)) {
				int id = menu.onEvent(event);
				switch (id) {
					case Callbacks::TEXT:
						text.setString(textbox->getText());
						text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
						break;
					case Callbacks::COLOR:
						//text.setFillColor(opt->getSelectedValue());
						text.setColor(opt->getSelectedValue());
						break;
					case Callbacks::ROTATION:
						text.setRotation(sliderRotation->getValue() * 360 / 100.f);
						pbar0->setValue(sliderRotation->getValue());
						break;

					case Callbacks::UNDERLINED:
					{
						int style = text.getStyle();
						if (checkboxUnderlined->isChecked())
							style |= sf::Text::Underlined;
						else
							style &= ~sf::Text::Underlined;
						text.setStyle(style);
						break;
					}
					case Callbacks::BOLD:
					{
						int style = text.getStyle();
						if (checkboxBold->isChecked())
							style |= sf::Text::Bold;
						else
							style &= ~sf::Text::Bold;
						text.setStyle(style);
						break;
					}
					case Callbacks::SCALE:
					{
						float scale = 1 + sliderScale->getValue() * 2 / 100.f;
						text.setScale(scale, scale);
						pbar1->setValue(sliderScale->getValue());
						break;
					}
					case Callbacks::QUIT:
						window.close();
						break;
					case Callbacks::VSLIDER:
						pbar->setValue(vslider->getValue());
						break;
					case Callbacks::NEW_BUTTON:
						vbox->addButton(textbox2->getText());
						break;
					case Callbacks::THEME_TEXTURE:
						GameGUI::Theme::loadTexture(changeTextureBox->getText());
					case Callbacks::TERMINATE:
						doTerminateProgram = !doTerminateProgram;
				}
				if (event.type == sf::Event::Closed)
					window.close();
			}

			window.clear(GameGUI::Theme::windowBgColor);

			window.draw(menu);
			//window.draw(text);

			window.display();
		}
	} while (!doTerminateProgram);
}
