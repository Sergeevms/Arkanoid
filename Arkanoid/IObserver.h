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

	class IObservable
	{
	public:
		virtual std::shared_ptr<IObservable> GetObservablePtr() = 0;
		virtual void AddObserver(std::weak_ptr<IObserver> observer);
	protected:
		virtual void Emit();
		std::vector<std::weak_ptr<IObserver>> observers;
	};
}

