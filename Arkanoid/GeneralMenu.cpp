#include <assert.h>
#include "GeneralMenu.h"
#include "Settings.h"

namespace Arkanoid
{
	void MenuNode::Init(MenuNode * parent, const std::wstring & newName, MenuStyle* newSubMenuStyle)
	{
		parentNode = parent;
		text->setString(newName);
		if (newSubMenuStyle)
		{
			subMenuStyle = newSubMenuStyle;
		}		
	}
	
	void MenuNode::Draw(sf::RenderWindow& window, const sf::Vector2f& position, const Orientation orientation, const Alignment alignment)
	{		
		text->Draw(window, position, orientation, alignment);
	}

	sf::FloatRect MenuNode::GetRect() const
	{
		return text->GetRect();
	}

	void MenuNode::SetStyle(const TextStyle* newStyle)
	{
		text->SetStyle(newStyle);
	}

	MenuNode* MenuNode::GetCurrentlySelectedChild() const
	{
		if (selectedChildID == -1)
		{
			return nullptr;
		}
		else
		{
			return childNodes[selectedChildID].get();
		}
	}

	MenuNode* MenuNode::GetParent() const
	{
		return parentNode;
	}

	MenuStyle* MenuNode::GetMenuStyle() const
	{
		return subMenuStyle;
	}

	void MenuNode::AddChild(std::unique_ptr<MenuNode> child)
	{
		childNodes.emplace_back(std::move(child));
		if (selectedChildID == -1)
		{
			selectedChildID = 0;
		}
	}

	void MenuNode::SelectNextChild()
	{
		if (childNodes.size() >= 1)
		{			
			selectedChildID = (selectedChildID + childNodes.size() + 1) % static_cast<int>(childNodes.size());
		}
	}

	void MenuNode::SelectPreviousChild()
	{
		if (childNodes.size() >= 1)
		{
			selectedChildID = (selectedChildID + childNodes.size() - 1) % static_cast<int>(childNodes.size());
		}
	}

	std::vector<MenuNode*> MenuNode::GetChilds()
	{
		std::vector<MenuNode*> childList;
		for (auto& child : childNodes)
		{
			childList.push_back(child.get());
		}
		return childList;
	}

	void MenuNode::SetSelectedChildID(int id)
	{
		if (id >= 0 && id < childNodes.size())
		{
			selectedChildID = id;
		}
	}

	void GeneralMenu::Draw(sf::RenderWindow& window, const sf::Vector2f& position, RelativePosition origin) const
	{
		MenuStyle* style = currentNode->GetMenuStyle();

		std::vector<IListDrawable*> itemList;
		itemList.push_back(currentNode);

		DrawableList subList(style->orientation, style->alignment, style->spacing);
		std::vector<MenuNode*> childNodes = currentNode->GetChilds();
		for (auto& currentChild : childNodes)
		{
			subList.AddItem(currentChild);
		}
		itemList.push_back(&subList);

		sf::FloatRect rect = GetListRect(itemList, position, RelativePositionByOrientationAndAlignment(style->orientation, style->alignment), 
			style->orientation, style->alignment, style->spacing * 3.f);
		sf::Vector2f correctedPosition;
		correctedPosition.x = position.x - rect.width * (relativePositions.at(origin).x - 
			relativePositions.at(RelativePositionByOrientationAndAlignment(style->orientation, style->alignment)).x);
		correctedPosition.y = position.y - rect.height * (relativePositions.at(origin).y - 
			relativePositions.at(RelativePositionByOrientationAndAlignment(style->orientation, style->alignment)).y);

		DrawList(window, itemList, correctedPosition, 
			RelativePositionByOrientationAndAlignment(style->orientation, style->alignment), style->orientation, style->alignment, style->spacing * 3.f);
	}

	void GeneralMenu::Draw(sf::RenderWindow& window, const sf::Vector2f& position, const Orientation orientation, const Alignment alignment)
	{
		RelativePosition origin = RelativePositionByOrientationAndAlignment(orientation, alignment);
		Draw(window, position, origin);
	}

	void GeneralMenu::PressSelected()
	{
		MenuNode* selectedNode = currentNode->GetCurrentlySelectedChild();
		if (selectedNode == nullptr)
		{
			return;
		}
		else if (auto callBack = selectedNode->GetCallBack())
		{
			callBack(selectedNode);
			return;
		}
		else
		{
			currentNode = selectedNode;
			currentNode->SetStyle(&headerStyle);
			currentNode->GetCurrentlySelectedChild()->SetStyle(&selectedStyle);
			return;
		}
	}

	bool GeneralMenu::ExpandSelected()
	{		
		MenuNode* newNode = currentNode->GetCurrentlySelectedChild();
		if (newNode->GetCurrentlySelectedChild())
		{			
			currentNode = newNode;
			currentNode->SetStyle(&headerStyle);
			currentNode->GetCurrentlySelectedChild()->SetStyle(&selectedStyle);
			return true;
		}
		else
		{
			return false;
		}
	}

	void GeneralMenu::ReturnToPrevious()
	{
		MenuNode* newNode = currentNode->GetParent();
		if (newNode)
		{
			currentNode->SetStyle(&selectedStyle);
			currentNode = newNode;
			currentNode->SetStyle(&headerStyle);
		}
	}

	void GeneralMenu::SelectNext() const
	{
		currentNode->GetCurrentlySelectedChild()->SetStyle(&normalStyle);
		currentNode->SelectNextChild();
		currentNode->GetCurrentlySelectedChild()->SetStyle(&selectedStyle);
	}

	void GeneralMenu::SelectPrevious() const
	{
		currentNode->GetCurrentlySelectedChild()->SetStyle(&normalStyle);
		currentNode->SelectPreviousChild();
		currentNode->GetCurrentlySelectedChild()->SetStyle(&selectedStyle);
	}

	sf::FloatRect GeneralMenu::GetRect() const
	{
		MenuStyle* style = currentNode->GetMenuStyle();

		std::vector<IListDrawable*> itemList;
		itemList.push_back(currentNode);

		DrawableList subList(style->orientation, style->alignment, style->spacing);
		std::vector<MenuNode*> childNodes = currentNode->GetChilds();
		for (auto& currentChild : childNodes)
		{
			subList.AddItem(currentChild);
		}
		itemList.push_back(&subList);
		return GetListRect(itemList, {0.f, 0.f}, RelativePositionByOrientationAndAlignment(style->orientation, style->alignment),
			style->orientation, style->alignment, style->spacing * 3.f);
		
	}

	MenuNode* GeneralMenu::InitializeRootNode(const std::wstring& newName, TextStyle* nodeStyle, MenuStyle* newSubMenuStyle)
	{
		rootNode = std::make_unique<MenuNode>();
		ConfigurateNode(rootNode.get(), nullptr, newName, nodeStyle, nullptr, newSubMenuStyle);
		return rootNode.get();
	}

	MenuNode* GeneralMenu::InitializeNode(MenuNode* parent, const std::wstring& newName, 
		TextStyle* nodeStyle, std::function<void(MenuNode*)> onPressCallback, MenuStyle* newSubMenuStyle)
	{
		if (parent)
		{
			parent->AddChild(std::make_unique<MenuNode>());
			MenuNode* newNode = parent->GetChilds().back();
			ConfigurateNode(newNode, parent, newName, nodeStyle, onPressCallback, newSubMenuStyle);
			return newNode;
		}
		else
		{
			return nullptr;
		}
	}

	void GeneralMenu::ConfigurateNode(MenuNode* node, MenuNode* parent, const std::wstring& newName, 
		TextStyle* nodeStyle, std::function<void(MenuNode*)> onPressCallback, MenuStyle* newSubMenuStyle)
	{
		node->Init(parent, newName, newSubMenuStyle);
		node->SetCallBack(onPressCallback);
		if (nodeStyle)
		{
			node->SetStyle(nodeStyle);
		}
		else
		{
			node->SetStyle(&normalStyle);
		}
	}

	void MenuNode::ClearChildNodes()
	{
		childNodes.clear();
	}

	std::function<void(MenuNode*)> MenuNode::GetCallBack()
	{
		return onPressCallBack;
	}

	void MenuNode::SetCallBack(std::function<void(MenuNode*)> callBack)
	{
		onPressCallBack = callBack;
	}
}
