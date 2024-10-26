#pragma once
#include "GeneralMenu.h"
#include "IListDrawable.h"

namespace SnakeGame
{
	class CheckBoxMenuNode;
	class LevelSelectMenuNode;

	class MainMenu : public GeneralMenu
	{
	public:
		MainMenu();
		virtual ~MainMenu() = default;
		void SwitchChecked();
		int GetSelectedDifficulty();
		void SwitchLevelMode();
	private:
		CheckBoxMenuNode* InitializeCheckBoxNode(MenuNode* parent, const std::wstring& newName, bool checked, float spacing, const sf::Texture& checkTexture,
			TextStyle* nodeStyle = nullptr, MenuNodeActivateReaction reaction = MenuNodeActivateReaction::None, bool* correspondingOption = nullptr, MenuStyle* newSubMenuStyle = nullptr);
		std::unordered_map<MenuNode*, int> nodeToDifficultyLevel;
		std::unordered_map<MenuNode*, bool*> nodeToCorrespodingOption;
		MenuNode* levelSelectionNode;
		CheckBoxMenuNode* levelModeNodeOption;
		sf::Texture checkTexture;
	};

	class CheckBox : public IListDrawable
	{ 
	public:
		CheckBox() = delete;
		CheckBox(const sf::Texture& checkTexture);
		virtual ~CheckBox() = default;
		virtual sf::FloatRect GetRect() const override;
		virtual void Draw(sf::RenderWindow& window, const sf::Vector2f& position, const Orientation orientation, const Alignment alignment) override;
		void SetStyle(const sf::Color& color, const float size);
		void SetChecked(const bool checked);
	private:
		sf::RectangleShape box;
		sf::Sprite check;
		bool isChecked{ false };
	};

	class CheckBoxMenuNode : public MenuNode
	{
	public:
		CheckBoxMenuNode() = delete;
		CheckBoxMenuNode(const sf::Texture& checkTexture) : checkBox(std::make_unique<CheckBox>(checkTexture)) {};
		virtual ~CheckBoxMenuNode() = default;
		std::wstring GetString() const;
		void virtual Draw(sf::RenderWindow& window, const sf::Vector2f& position, const Orientation orientation, const Alignment alignment) override;
		virtual sf::FloatRect GetRect() const override;
		void SetChecked(const bool checked);
		virtual void SetStyle(const TextStyle* newStyle) override;
		void SetSpacing(const float newSpacing);
	private:
		std::unique_ptr<CheckBox> checkBox;
		float spacing{ 30.f };
	};

	class LevelSelectMenuNode : public MenuNode
	{
	public:
		virtual ~LevelSelectMenuNode() = default;
	private:
	};
}