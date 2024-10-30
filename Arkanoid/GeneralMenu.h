#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>
#include "Utility.h"
#include "IListDrawable.h"

namespace Arkanoid
{
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
		std::function<void(MenuNode*)> GetCallBack();
		void SetCallBack(std::function<void(MenuNode*)> callBack);
	protected:
		std::unique_ptr<ListDrawableText> text = std::make_unique<ListDrawableText>();
		std::vector<std::unique_ptr<MenuNode>> childNodes;
		std::function<void(MenuNode*)> onPressCallBack;
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
		virtual void PressSelected();
		virtual bool ExpandSelected();
		virtual void ReturnToPrevious();
		virtual void SelectNext() const;
		virtual void SelectPrevious() const;
		virtual sf::FloatRect GetRect() const override;
	protected:
		TextStyle headerStyle;
		TextStyle selectedStyle;
		TextStyle normalStyle;
		std::unique_ptr<MenuNode> rootNode;
		MenuNode* currentNode{ nullptr };
		MenuStyle subMenuStyle;
		virtual MenuNode* InitializeRootNode(const std::wstring& newName, TextStyle* nodeStyle,
			MenuStyle* newSubMenuStyle = nullptr);
		virtual MenuNode* InitializeNode(MenuNode* parent, const std::wstring& newName, TextStyle* nodeStyle,
			std::function<void(MenuNode*)> onPressCallback = nullptr, MenuStyle* newSubMenuStyle = nullptr);
		virtual void ConfigurateNode(MenuNode* node, MenuNode* parent,
				const std::wstring& newName, TextStyle* nodeStyle, std::function<void(MenuNode*)> onPressCallback = nullptr, MenuStyle* newSubMenuStyle = nullptr);
	};
}

