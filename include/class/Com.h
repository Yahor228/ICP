#pragma once

struct Unknown
{
	virtual ~Unknown() = default;
	long AddRef() { ref++; };
	void Release() { if (!--ref)delete this; }
private:
	long ref = 0;
};

template<class T>
class com_ptr
{
public:
	com_ptr(Unknown* ptr)
		:abi(ptr)
	{
		abi->AddRef();
	}
	~com_ptr()
	{
		abi->Release();
	}
public:
	Unknown* get()const
	{
		return abi;
	}
	Unknown** put()
	{
		abi->Release();
		return &abi;
	}
	Unknown* operator->()
	{
		return abi;
	}
private:
	Unknown* abi;
};