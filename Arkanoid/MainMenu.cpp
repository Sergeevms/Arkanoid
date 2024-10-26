#include "MainMenu.h"
#include "Settings.h"
#include "IListDrawable.h"

namespace Arkanoid
{
	MainMenu::MainMenu() : GeneralMenu()
	{
		headerStyle.Init("Roboto-Regular.ttf", sf::Color::Green, sf::Text::Style(sf::Text::Style::Bold | sf::Text::Style::Underlined), 60);
		normalStyle.Init("Roboto-Regular.ttf");
		selectedStyle.Init("Roboto-Regular.ttf", sf::Color::Green);
		LoadTexture("Check.png", checkTexture);

		currentNode = InitializeRootNode(L"Змейка", &headerStyle, MenuNodeActivateReaction::None, &subMenuStyle);
		InitializeNode(currentNode, L"Начать игру", &selectedStyle, MenuNodeActivateReaction::Play);
		InitializeNode(currentNode, L"Таблица рекордов", &normalStyle, MenuNodeActivateReaction::Records);
		MenuNode* settingsNode = InitializeNode(currentNode, L"Настройки", &normalStyle, MenuNodeActivateReaction::None, &subMenuStyle);
		InitializeNode(currentNode, L"Выход", &normalStyle, MenuNodeActivateReaction::Exit);
		
		Settings* settings = Settings::GetSettings();

		InitializeCheckBoxNode(settingsNode, L"Звук", settings->soundOn, 30.f, checkTexture, &selectedStyle, MenuNodeActivateReaction::SwitchOption, &(settings->soundOn));
		InitializeCheckBoxNode(settingsNode, L"Музыка", settings->musicOn, 30.f, checkTexture, &normalStyle, MenuNodeActivateReaction::SwitchOption,  &(settings->musicOn));
	}

	void MainMenu::SwitchChecked()
	{
		CheckBoxMenuNode* selectedNode;
		if (selectedNode = dynamic_cast<CheckBoxMenuNode*>(currentNode->GetCurrentlySelectedChild()))
		{
			
			*(nodeToCorrespodingOption.at(selectedNode)) = !(*(nodeToCorrespodingOption.at(selectedNode)));
			selectedNode->SetChecked(*(nodeToCorrespodingOption.at(selectedNode)));
			
		}
	}


	CheckBoxMenuNode* MainMenu::InitializeCheckBoxNode(MenuNode* parent, const std::wstring& newName, bool checked,
		float spacing, const sf::Texture& checkTexture, TextStyle* nodeStyle, MenuNodeActivateReaction reaction, bool* correspondingOption, MenuStyle* newSubMenuStyle)
	{
		if (parent)
		{	
			parent->AddChild(std::make_unique<CheckBoxMenuNode>(checkTexture));
			CheckBoxMenuNode* newNode = dynamic_cast<CheckBoxMenuNode*>(parent->GetChilds().back());
			ConfigurateNode(newNode, parent, newName, nodeStyle, reaction, newSubMenuStyle);
			newNode->SetChecked(checked);
			newNode->SetSpacing(spacing);
			if (correspondingOption)
			{
				nodeToCorrespodingOption[newNode] = correspondingOption;
			}
			return newNode;
		}
		else
		{
			return nullptr;
		}
	}

	void CheckBoxMenuNode::Draw(sf::RenderWindow& window, const sf::Vector2f& position, const Orientation orientation, const Alignment alignment)
	{
		DrawList(window, { text.get(), checkBox.get() }, position, RelativePositionByOrientationAndAlignment(orientation, alignment), Orientation::Horizontal, Alignment::Middle, spacing);
	}

	sf::FloatRect CheckBoxMenuNode::GetRect() const
	{		
		return GetListRect(std::vector<IListDrawable*> { text.get(), checkBox.get() }, {0.f, 0.f}, RelativePosition::TopLeft, Orientation::Horizontal, Alignment::Middle, spacing);
	}

	void CheckBoxMenuNode::SetChecked(const bool checked)
	{
		checkBox->SetChecked(checked);
	}

	void CheckBoxMenuNode::SetStyle(const TextStyle* newStyle)
	{
		MenuNode::SetStyle(newStyle);
		checkBox->SetStyle(newStyle->color, static_cast<float>(newStyle->characterSize));
	}

	void CheckBoxMenuNode::SetSpacing(const float newSpacing)
	{
		spacing = newSpacing;
	}

	CheckBox::CheckBox(const sf::Texture& checkTexture)
	{
		check.setTexture(checkTexture);
		box.setFillColor({ 255, 255, 255, 0 });
		box.setOutlineThickness(3.f);
	}

	sf::FloatRect CheckBox::GetRect() const
	{
		return box.getLocalBounds();
	}

	void CheckBox::SetStyle(const sf::Color& color, const float size)
	{
		box.setOutlineColor(color);
		box.setSize({ size, size });
		check.setColor(color);
		SetScaleBySize(check, { size, size });
	}

	void CheckBox::SetChecked(const bool checked)
	{
		isChecked = checked;
	}

	void CheckBox::Draw(sf::RenderWindow& window, const sf::Vector2f& position, const Orientation orientation, const Alignment alignment)
	{
		SetOriginByRelative(box, relativePositions.at(RelativePositionByOrientationAndAlignment(orientation, alignment)));
		box.setPosition(position);
		window.draw(box);
		if (isChecked)
		{
			SetOriginByRelative(check, relativePositions.at(RelativePositionByOrientationAndAlignment(orientation, alignment)));
			check.setPosition(position);
			window.draw(check);
		}
	}

	std::wstring CheckBoxMenuNode::GetString() const
	{
		return text.get()->getString().toWideString();
	}
}