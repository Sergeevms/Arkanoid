#include "IObserver.h"
#include <algorithm>

namespace Arkanoid
{
	void IObservable::AddObserver(std::weak_ptr<IObserver> observer)
	{
		observers.push_back(observer);
	}

	void IObservable::Emit()
	{
		auto self = GetObservablePtr();
		std::for_each(observers.begin(), observers.end(),
			[self](auto observer)
			{
				auto lockedObserver = observer.lock();
				if (lockedObserver)
				{
					lockedObserver->Notify(self);
				}
			});
	}
}