#pragma once
#include <vector>
#include <functional>
#include <SFML/Graphics.hpp>
#include <unordered_map>

namespace Arkanoid
{
	//Enum for action type to connect it to input key
	enum class ActionsTypesOnInput
	{
		Up,
		Right,
		Down,
		Left,
		Back,
		Forward,
		Pause,
		BackSpace,

//Temporary Action type for testing Record table on Game Over
		RecordTable
	};

	class BaseInputHandler;
	typedef std::function<void(BaseInputHandler*)> HandlerAction;

	/* Base class for input handler
	* Have default implementation for events
	* based on mapping ActionsTypesOnInput -> HandlerAction
	* each derrived class filling actionMapping for it's own purpose
	*/
	class BaseInputHandler
	{
	public:
		BaseInputHandler() {};
		virtual void HandleInputEvents(const std::vector<sf::Event>& input);
		virtual ~BaseInputHandler() = default;
	protected:
		std::unordered_map<ActionsTypesOnInput, HandlerAction> actionMapping;
	};
}

