#include "pch.h"
#include "MemoryPool.h"

MemoryPool::MemoryPool(int32 allocsize)
: _allocSize(allocsize)
{
}

MemoryPool::~MemoryPool()
{
}

void MemoryPool::Push(MemoryHeader* ptr)
{
	WRITE_LOCK;

	ptr->_allocSize = 0;

	_queue.push(ptr);

	_allocCount.fetch_sub(1);
}

MemoryHeader* MemoryPool::Pop()
{
	MemoryHeader* header = nullptr;

	{
		WRITE_LOCK;
		if (_queue.empty() == false)
		{
			header = _queue.front();
			_queue.pop();
		}
	}

	if (header == nullptr)
	{
		header = reinterpret_cast<MemoryHeader*>(::malloc(_allocSize));
	}
	else
	{
		ASSERT_CRASH(header->_allocSize == 0);
	}

	_allocCount.fetch_add(1);

	return header;
}
