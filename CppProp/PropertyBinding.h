#pragma once

#include <unordered_map>

enum AccessType
{
	Private,
	Public
};

enum BindingMode
{
	OneWay,
	OneWayToSource,
	TwoWay
};

class GenericProperty
{
	friend class BindingContext;

protected:
	typedef const void*(*fnGenericGet)(const GenericProperty*);
	typedef void(*fnGenericSet)(const GenericProperty*, const void*);

protected:
	GenericProperty(fnGenericGet genericGet, fnGenericSet genericSet)
		: _genericGet(genericGet)
		, _genericSet(genericSet)
	{ }

protected:
	virtual inline bool CanGet() const = 0;
	virtual inline bool CanSet() const = 0;

private:
	inline const void* Binding_GenericGet() const
	{
		if (!CanGet())
			throw "Cannot get private value of a property";

		return _genericGet(this);
	}

	inline void Binding_GenericSet(const void* value)
	{
		if (!CanSet())
			throw "Cannot set private value of a property";

		_genericSet(this, value);
	}

private:
	fnGenericGet _genericGet;
	fnGenericSet _genericSet;
};

template<typename T>
class PropertyBase : public GenericProperty
{
	friend class BindingContext;

public:
	typedef const T&(*fnGet)(const PropertyBase*);
	typedef void(*fnSet)(const PropertyBase*, const T&);

protected:
	PropertyBase(fnGet get, fnSet set, fnGenericGet genericGet, fnGenericSet genericSet)
		: GenericProperty(genericGet, genericSet)
		, _get(get)
		, _set(set)
	{ }

private:
	inline const T& Binding_Get() const
	{
		if (!CanGet())
			throw "Cannot get private value of a property";

		return _get(this);
	}

	inline void Binding_Set(const T& value)
	{
		if (!CanSet())
			throw "Cannot set private value of a property";

		_set(this, value);
	}

private:
	fnGet _get;
	fnSet _set;
};

template<
	typename T,
	typename Container,
	AccessType GetAccessType = Public,
	AccessType SetAccessType = Public
>
class Property : public PropertyBase<T>
{
	friend Container;

public:
	Property(Container& container)
		: PropertyBase(&Internal_Get, &Internal_Set, &Internal_GenericGet, &Internal_GenericSet)
		, _container(container)
	{ }

	Property(const T& intitialValue, Container& container)
		: PropertyBase(&Internal_Get, &Internal_Set, &Internal_GenericGet, &Internal_GenericSet)
		, _value(intitialValue)
		, _container(container)
	{ }

public:
	inline const T& Get() const { return _value; }
	inline void Set(const T& value) { _value = value; }

private:
	static const T& Internal_Get(const PropertyBase<T>* prop)
	{
		return ((Property*)(prop))->Get();
	}

	static void Internal_Set(const PropertyBase<T>* prop, const T& value)
	{
		((Property*)(prop))->Set(value);
	}

	static const void* Internal_GenericGet(const GenericProperty* prop)
	{
		return (const void*)(&(((Property*)(prop))->Get()));
	}

	static void Internal_GenericSet(const GenericProperty* prop, const void* value)
	{
		((Property*)(prop))->Set(*((const T*)value));
	}
	
protected:
	inline bool CanGet() const override final { return GetAccessType == Public; }
	inline bool CanSet() const override final { return SetAccessType == Public; }

private:
	T _value;
	Container& _container;
};

template<
	typename T,
	typename Container
>
class Property<T, Container, Private, Public> : public PropertyBase<T>
{
	friend Container;

public:
	Property(Container& container)
		: PropertyBase(&Internal_Get, &Internal_Set, &Internal_GenericGet, &Internal_GenericSet)
		, _container(container)
	{ }

	Property(const T& intitialValue, Container& container)
		: PropertyBase(&Internal_Get, &Internal_Set, &Internal_GenericGet, &Internal_GenericSet)
		, _value(intitialValue)
		, _container(container)
	{ }

private:
	inline const T& Get() const { return _value; }

public:
	inline void Set(const T& value) { _value = value; }

private:
	static const T& Internal_Get(const PropertyBase<T>* prop)
	{
		return ((Property*)(prop))->Get();
	}

	static void Internal_Set(const PropertyBase<T>* prop, const T& value)
	{
		((Property*)(prop))->Set(value);
	}

	static const void* Internal_GenericGet(const GenericProperty* prop)
	{
		return (const void*)(&(((Property*)(prop))->Get()));
	}

	static void Internal_GenericSet(const GenericProperty* prop, const void* value)
	{
		((Property*)(prop))->Set(*((const T*)value));
	}

protected:
	inline bool CanGet() const override final { return false; }
	inline bool CanSet() const override final { return true; }

private:
	T _value;
	Container& _container;
};

template<
	typename T,
	typename Container
>
class Property<T, Container, Public, Private> : public PropertyBase<T>
{
	friend Container;

public:
	Property(Container& container)
		: PropertyBase(&Internal_Get, &Internal_Set, &Internal_GenericGet, &Internal_GenericSet)
		, _container(container)
	{ }

	Property(const T& intitialValue, Container& container)
		: PropertyBase(&Internal_Get, &Internal_Set, &Internal_GenericGet, &Internal_GenericSet)
		, _value(intitialValue)
		, _container(container)
	{ }

public:
	inline const T& Get() const { return _value; }

private:
	inline void Set(const T& value) { _value = value; }

private:
	static const T& Internal_Get(const PropertyBase<T>* prop)
	{
		return ((Property*)(prop))->Get();
	}

	static void Internal_Set(const PropertyBase<T>* prop, const T& value)
	{
		((Property*)(prop))->Set(value);
	}

	static const void* Internal_GenericGet(const GenericProperty* prop)
	{
		return (const void*)(&(((Property*)(prop))->Get()));
	}

	static void Internal_GenericSet(const GenericProperty* prop, const void* value)
	{
		((Property*)(prop))->Set(*((const T*)value));
	}

protected:
	inline bool CanGet() const override final { return true; }
	inline bool CanSet() const override final { return false; }

private:
	T _value;
	Container& _container;
};

template<
	typename T,
	typename Container
>
class Property<T, Container, Private, Private> : public PropertyBase<T>
{
	friend Container;

public:
	Property(Container& container)
		: PropertyBase(&Internal_Get, &Internal_Set, &Internal_GenericGet, &Internal_GenericSet)
		, _container(container)
	{ }

	Property(const T& intitialValue, Container& container)
		: PropertyBase(&Internal_Get, &Internal_Set, &Internal_GenericGet, &Internal_GenericSet)
		, _value(intitialValue)
		, _container(container)
	{ }

private:
	inline const T& Get() const { return _value; }
	inline void Set(const T& value) { _value = value; }

private:
	static const T& Internal_Get(const PropertyBase<T>* prop)
	{
		return ((Property*)(prop))->Get();
	}

	static void Internal_Set(const PropertyBase<T>* prop, const T& value)
	{
		((Property*)(prop))->Set(value);
	}

	static const void* Internal_GenericGet(const GenericProperty* prop)
	{
		return (const void*)(&(((Property*)(prop))->Get()));
	}

	static void Internal_GenericSet(const GenericProperty* prop, const void* value)
	{
		((Property*)(prop))->Set(*((const T*)value));
	}

protected:
	inline bool CanGet() const override final { return false; }
	inline bool CanSet() const override final { return false; }

private:
	T _value;
	Container& _container;
};

template<
	typename T,
	typename Container,
	const T&(Container::*GetCallbackFunc)(),
	void(Container::*SetCallbackFunc)(const T&),
	AccessType GetAccessType = Public,
	AccessType SetAccessType = Public
>
class PropertyEx : public PropertyBase<T>
{
	friend Container;

public:
	PropertyEx(Container& container)
		: PropertyBase(&Internal_Get, &Internal_Set, &Internal_GenericGet, &Internal_GenericSet)
		, _container(container)
	{ }

	PropertyEx(const T& intitialValue, Container& container)
		: PropertyBase(&Internal_Get, &Internal_Set, &Internal_GenericGet, &Internal_GenericSet)
		, _value(intitialValue)
		, _container(container)
	{ }

public:
	inline const T& Get() const
	{
		return (_container.*GetCallbackFunc)();
	}

	inline void Set(const T& value)
	{
		(_container.*SetCallbackFunc)(value);
	}

protected:
	inline bool CanGet() const override final { return GetAccessType == Public; }
	inline bool CanSet() const override final { return SetAccessType == Public; }

private:
	static const T& Internal_Get(const PropertyBase<T>* prop)
	{
		return ((PropertyEx*)(prop))->Get();
	}

	static void Internal_Set(const PropertyBase<T>* prop, const T& value)
	{
		((PropertyEx*)(prop))->Set(value);
	}

	static const void* Internal_GenericGet(const GenericProperty* prop)
	{
		return (const void*)(&(((PropertyEx*)(prop))->Get()));
	}

	static void Internal_GenericSet(const GenericProperty* prop, const void* value)
	{
		((PropertyEx*)(prop))->Set(*((const T*)value));
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
class PropertyEx<T, Container, GetCallbackFunc, SetCallbackFunc, Private, Public> : public PropertyBase<T>
{
	friend Container;

public:
	PropertyEx(Container& container)
		: PropertyBase(&Internal_Get, &Internal_Set, &Internal_GenericGet, &Internal_GenericSet)
		, _container(container)
	{ }

	PropertyEx(const T& intitialValue, Container& container)
		: PropertyBase(&Internal_Get, &Internal_Set, &Internal_GenericGet, &Internal_GenericSet)
		, _value(intitialValue)
		, _container(container)
	{ }

private:
	inline const T& Get() const
	{
		return (_container.*GetCallbackFunc)();
	}

public:
	inline void Set(const T& value)
	{
		(_container.*SetCallbackFunc)(value);
	}
	
protected:
	inline bool CanGet() const override final { return false; }
	inline bool CanSet() const override final { return true; }

private:
	static const T& Internal_Get(const PropertyBase<T>* prop)
	{
		return ((PropertyEx*)(prop))->Get();
	}

	static void Internal_Set(const PropertyBase<T>* prop, const T& value)
	{
		((PropertyEx*)(prop))->Set(value);
	}

	static const void* Internal_GenericGet(const GenericProperty* prop)
	{
		return (const void*)(&(((PropertyEx*)(prop))->Get()));
	}

	static void Internal_GenericSet(const GenericProperty* prop, const void* value)
	{
		((PropertyEx*)(prop))->Set(*((const T*)value));
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
class PropertyEx<T, Container, GetCallbackFunc, SetCallbackFunc, Public, Private> : public PropertyBase<T>
{
	friend Container;

public:
	PropertyEx(Container& container)
		: PropertyBase(&Internal_Get, &Internal_Set, &Internal_GenericGet, &Internal_GenericSet)
		, _container(container)
	{ }

	PropertyEx(const T& intitialValue, Container& container)
		: PropertyBase(&Internal_Get, &Internal_Set, &Internal_GenericGet, &Internal_GenericSet)
		, _value(intitialValue)
		, _container(container)
	{ }

public:
	inline const T& Get() const
	{
		return (_container.*GetCallbackFunc)();
	}

private:
	inline void Set(const T& value)
	{
		(_container.*SetCallbackFunc)(value);
	}

protected:
	inline bool CanGet() const override final { return true; }
	inline bool CanSet() const override final { return false; }

private:
	static const T& Internal_Get(const PropertyBase<T>* prop)
	{
		return ((PropertyEx*)(prop))->Get();
	}

	static void Internal_Set(const PropertyBase<T>* prop, const T& value)
	{
		((PropertyEx*)(prop))->Set(value);
	}

	static const void* Internal_GenericGet(const GenericProperty* prop)
	{
		return (const void*)(&(((PropertyEx*)(prop))->Get()));
	}

	static void Internal_GenericSet(const GenericProperty* prop, const void* value)
	{
		((PropertyEx*)(prop))->Set(*((const T*)value));
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
class PropertyEx<T, Container, GetCallbackFunc, SetCallbackFunc, Private, Private> : public PropertyBase<T>
{
	friend Container;

public:
	PropertyEx(Container& container)
		: PropertyBase(&Internal_Get, &Internal_Set, &Internal_GenericGet, &Internal_GenericSet)
		, _container(container)
	{ }

	PropertyEx(const T& intitialValue, Container& container)
		: PropertyBase(&Internal_Get, &Internal_Set, &Internal_GenericGet, &Internal_GenericSet)
		, _value(intitialValue)
		, _container(container)
	{ }

private:
	inline const T& Get() const
	{
		return (_container.*GetCallbackFunc)();
	}

	inline void Set(const T& value)
	{
		(_container.*SetCallbackFunc)(value);
	}

protected:
	inline bool CanGet() const override final { return false; }
	inline bool CanSet() const override final { return false; }

private:
	static const T& Internal_Get(const PropertyBase<T>* prop)
	{
		return ((PropertyEx*)(prop))->Get();
	}

	static void Internal_Set(const PropertyBase<T>* prop, const T& value)
	{
		((PropertyEx*)(prop))->Set(value);
	}

	static const void* Internal_GenericGet(const GenericProperty* prop)
	{
		return (const void*)(&(((PropertyEx*)(prop))->Get()));
	}

	static void Internal_GenericSet(const GenericProperty* prop, const void* value)
	{
		((PropertyEx*)(prop))->Set(*((const T*)value));
	}

private:
	T _value;
	Container& _container;
};

class BindingContext
{
public:
	void AddProperty(const std::string& name, GenericProperty* prop)
	{
		_properties[name] = prop;
	}

	GenericProperty* FindProperty(const std::string& name)
	{
		auto& findRes = _properties.find(name);
		if (findRes != _properties.end())
			return findRes->second;
		return nullptr;
	}

	const GenericProperty* FindProperty(const std::string& name) const
	{
		const auto& findRes = _properties.find(name);
		if (findRes != _properties.end())
			return findRes->second;
		return nullptr;
	}

	template<typename T>
	bool SetPropertyValue(const std::string& name, const T& value)
	{
		GenericProperty* prop = FindProperty(name);
		if (prop == nullptr)
			return false;

		PropertyBase<T>* propBase = dynamic_cast<PropertyBase<T>*>(prop);
		if (propBase == nullptr)
			return false;

		propBase->Binding_Set(value);
		return true;
	}

	template<typename T>
	const T& GetPropertyValue(const std::string& name, const T& defaultValue)
	{
		GenericProperty* prop = FindProperty(name);
		if (prop == nullptr)
			return defaultValue;

		PropertyBase<T>* propBase = dynamic_cast<PropertyBase<T>*>(prop);
		if (propBase == nullptr)
			return defaultValue;

		return propBase->Binding_Get();
	}

	template<typename T>
	const T& GetPropertyValue(const std::string& name)
	{
		GenericProperty* prop = FindProperty(name);
		if (prop == nullptr)
			throw "Property not found";

		PropertyBase<T>* propBase = dynamic_cast<PropertyBase<T>*>(prop);
		if (propBase == nullptr)
			throw "Property not compatible with the specified type";

		return propBase->Binding_Get();
	}

	bool SetPropertyValueGeneric(const std::string& name, const void* value)
	{
		GenericProperty* prop = FindProperty(name);
		if (prop == nullptr)
			return false;

		prop->Binding_GenericSet(value);
		return true;
	}

	const void* GetPropertyValueGeneric(const std::string& name, const void* defaultValue)
	{
		GenericProperty* prop = FindProperty(name);
		if (prop == nullptr)
			return defaultValue;

		return prop->Binding_GenericGet();
	}

	const void* GetPropertyValueGeneric(const std::string& name)
	{
		GenericProperty* prop = FindProperty(name);
		if (prop == nullptr)
			throw "Property not found";

		return prop->Binding_GenericGet();
	}

private:
	std::unordered_map<std::string, GenericProperty*> _properties;
};