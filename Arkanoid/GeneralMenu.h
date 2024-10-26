#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Utility.h"
#include "IListDrawable.h"

namespace Arkanoid
{
	enum class MenuNodeActivateReaction
	{
		None,
		Play,
		Records,
		SwitchOption,
		SwitchDifficulty,
		Exit,
		MainMenu,
		EnterName,
		SkipName
	};

	struct MenuStyle
	{
		Orientation orientation{ Orientation::Vertical };
		Alignment alignment{ Alignment::Middle };
		float spacing{ 5 };
	};

	class ListDrawableText;

	class MenuNode : public IListDrawable
	{
	public:
		virtual ~MenuNode() = default;
		virtual void Init(MenuNode * parent, const std::wstring& newName, MenuStyle* newSubMenuStyle = nullptr);
		virtual void Draw(sf::RenderWindow& window, const sf::Vector2f& position, const Orientation orientation, const Alignment alignment) override;
		virtual sf::FloatRect GetRect() const override;
		virtual void SetStyle(const TextStyle* newStyle);
		virtual MenuNode* GetCurrentlySelectedChild() const;
		MenuNode* GetParent() const;
		MenuStyle* GetMenuStyle() const;
		virtual void AddChild(std::unique_ptr<MenuNode> child);
		virtual void SelectNextChild();
		virtual void SelectPreviousChild();
		virtual std::vector<MenuNode*> GetChilds();
		virtual void SetSelectedChildID(int id);
		virtual void ClearChildNodes();
	protected:
		std::unique_ptr<ListDrawableText> text = std::make_unique<ListDrawableText>();
		std::vector<std::unique_ptr<MenuNode>> childNodes;
	private:
		int selectedChildID{ -1 };
		MenuStyle* subMenuStyle{ nullptr };
		MenuNode* parentNode{};
	};	

	class GeneralMenu : public IListDrawable
	{
	public:
		virtual ~GeneralMenu() {};
		virtual void Draw(sf::RenderWindow& window, const sf::Vector2f& position, RelativePosition origin = RelativePosition::TopMiddle) const;
		virtual void Draw(sf::RenderWindow& window, const sf::Vector2f& position, const Orientation orientation, const Alignment alignment) override;
		virtual bool ExpandSelected();
		virtual void ReturnToPrevious();
		virtual void SelectNext() const;
		virtual void SelectPrevious() const;
		virtual sf::FloatRect GetRect() const override;
		MenuNodeActivateReaction GetReaction() const;
	protected:
		TextStyle headerStyle;
		TextStyle selectedStyle;
		TextStyle normalStyle;
		std::unique_ptr<MenuNode> rootNode;
		MenuNode* currentNode{ nullptr };
		MenuStyle subMenuStyle;
		std::unordered_map<MenuNode*, MenuNodeActivateReaction> activateReactions;
		virtual MenuNode* InitializeRootNode(const std::wstring& newName, TextStyle* nodeStyle,
			MenuNodeActivateReaction reaction, MenuStyle* newSubMenuStyle = nullptr);
		virtual MenuNode* InitializeNode(MenuNode* parent, const std::wstring& newName, TextStyle* nodeStyle,
			MenuNodeActivateReaction reaction, MenuStyle* newSubMenuStyle = nullptr);
		void ConfigurateNode(MenuNode* node, MenuNode* parent,
			const std::wstring& newName, TextStyle* nodeStyle, MenuNodeActivateReaction reaction, MenuStyle* newSubMenuStyle = nullptr);
	};
}

