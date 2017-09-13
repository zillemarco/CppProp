#pragma once

enum AccessType
{
	Private,
	Public
};

template<
	typename T,
	typename Container,
	const T&(Container::*GetCallbackFunc)(),
	void(Container::*SetCallbackFunc)(const T&),
	AccessType GetAccessType = Public,
	AccessType SetAccessType = Public
>
class Property
{
	friend Container;

public:
	Property(Container& container = Container())
		: _container(container)
	{ }

	Property(const T& intitialValue, Container& container = Container())
		: _value(intitialValue)
		, _container(container)
	{ }

public:
	const T& Get() const
	{
		return (_container.*GetCallbackFunc)();
	}

	void Set(const T& newValue)
	{
		(_container.*SetCallbackFunc)(newValue);
	}

private:
	T _value;
	Container& _container;
};

template<
	typename T,
	typename Container,
	const T&(Container::*GetCallbackFunc)(),
	void(Container::*SetCallbackFunc)(const T&)
>
class Property<T, Container, GetCallbackFunc, SetCallbackFunc, Private, Public>
{
	friend Container;

public:
	Property(Container& container = Container())
		: _container(container)
	{ }

	Property(const T& intitialValue, Container& container = Container())
		: _value(intitialValue)
		, _container(container)
	{ }

private:
	const T& Get() const
	{
		return (_container.*GetCallbackFunc)();
	}

public:
	void Set(const T& newValue)
	{
		(_container.*SetCallbackFunc)(newValue);
	}

private:
	T _value;
	Container& _container;
};

template<
	typename T,
	typename Container,
	const T&(Container::*GetCallbackFunc)(),
	void(Container::*SetCallbackFunc)(const T&)
>
class Property<T, Container, GetCallbackFunc, SetCallbackFunc, Public, Private>
{
	friend Container;

public:
	Property(Container& container = Container())
		: _container(container)
	{ }

	Property(const T& intitialValue, Container& container = Container())
		: _value(intitialValue)
		, _container(container)
	{ }

public:
	const T& Get() const
	{
		return (_container.*GetCallbackFunc)();
	}

private:
	void Set(const T& newValue)
	{
		(_container.*SetCallbackFunc)(newValue);
	}

private:
	T _value;
	Container& _container;
};

template<
	typename T,
	typename Container,
	const T&(Container::*GetCallbackFunc)(),
	void(Container::*SetCallbackFunc)(const T&)
>
class Property<T, Container, GetCallbackFunc, SetCallbackFunc, Private, Private>
{
	friend Container;

public:
	Property(Container& container = Container())
		: _container(container)
	{ }

	Property(const T& intitialValue, Container& container = Container())
		: _value(intitialValue)
		, _container(container)
	{ }

private:
	const T& Get() const
	{
		return (_container.*GetCallbackFunc)();
	}

	void Set(const T& newValue)
	{
		(_container.*SetCallbackFunc)(newValue);
	}

private:
	T _value;
	Container& _container;
};

#define PROPERTY(name, type, container, getAccessType, setAccessType, propertyAccess)			\
	private:																					\
		const type& Get ##name() { return name._value; }										\
		void Set ##name(const type& value) { name._value = value; }								\
	propertyAccess:																				\
		Property<type, container, &Get ##name, &Set ##name, getAccessType, setAccessType> name; \

#define PROPERTYEX(container, type, getAccessType, setAccessType, getFn, setFn) \
	Property<type, container, &getFn, &setFn, getAccessType, setAccessType> name;