#pragma once
#include <memory>
#include <vector>

namespace Arkanoid
{
	class IObservable;

	class IObserver
	{
	public:
		virtual void Notify(std::shared_ptr<IObservable> observable) = 0;
	};

	class IObservable : public std::enable_shared_from_this<IObservable>
	{
	public:
		void AddObserver(std::weak_ptr<IObserver> observer);
	protected:
		virtual void Emit();
	private:
		std::vector<std::weak_ptr<IObserver>> observers;
	};
}

