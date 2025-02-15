#pragma once
class RefCountable
{
public:
	RefCountable() : _refCount(1) {}
	virtual ~RefCountable() { }

	int32 GetRefCount() { return _refCount; }

	int32 AddRef() { return ++_refCount; }
	int32 ReleaseRef()
	{
		int32 refCount = --_refCount;

		if (refCount == 0) 
		{
			delete this;
		}

		return refCount;
	}
	

private:
	atomic<int32> _refCount;
};

template <typename T>
class TSharedPtr
{
public:
	TSharedPtr() {}
	TSharedPtr(T* ptr) { Set(ptr); }
	
	TSharedPtr(const TSharedPtr& other) { Set(other._ptr); }
	TSharedPtr(TSharedPtr&& rv) { _ptr = rv._ptr; rv._ptr = nullptr; }

	template<typename U>
	TSharedPtr(const TSharedPtr<U>& child) { Set(static_cast<T*>(child._ptr)); }

	~TSharedPtr() { Release(); }

public:
	TSharedPtr& operator=(const TSharedPtr& other)
	{
		if (_ptr != other._ptr)
		{
			Release();
			Set(other._ptr);
		}

		return *this;
	}

	TSharedPtr& operator=(TSharedPtr&& rv)
	{
		Release();
		_ptr = rv._ptr;
		rv._ptr = nullptr;
		return *this;
	}

	bool operator==(const TSharedPtr& other) const { return _ptr == other._ptr; }
	bool operator!=(const TSharedPtr& other) const { return _ptr != other._ptr; }
	bool operator==(T* ptr) const { return _ptr == ptr; }
	bool operator!=(T* ptr) const { return _ptr != ptr; }
	bool operator<(const TSharedPtr& other) const { return _ptr < other._ptr; }
	T*	 operator*() { return _ptr; }
	const T* operator*() const { return _ptr; }
	T*	 operator->() { return _ptr; }
	const T* operator->() const { return _ptr; }

	bool IsNull() { return _ptr == nullptr; }

private:
	void Set(T* ptr)
	{
		_ptr = ptr;
		if(ptr)
			ptr->AddRef();
	}

	void Release()
	{
		if (_ptr != nullptr)
		{
			_ptr->ReleaseRef();
			_ptr = nullptr;
		}
	}

private:
	T* _ptr = nullptr;
};

