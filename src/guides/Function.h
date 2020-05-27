//------------------------------------------------------------------------------
// Function.h

// Copyright (c) 2020 glensand
// All rights reserved.
//
// Date: 24.05.2020
// Author: Bezborodoff Gleb
//------------------------------------------------------------------------------

#pragma once

#include "CustomTraits/TrueType.h"
#include "CustomTraits/Declval.h"
#include "IFunctorBridge.h"

template <typename Signature>
class Function;

template <typename R, typename... Args>
class Function<R(Args...)>
{
public:
	Function(const Function& other);
	Function(Function& other)
		: Function(static_cast<const Function&>(other))
	{
	}

	constexpr Function(Function&& other) noexcept
		: m_bridge(other.m_bridge)
	{
		other.m_bridge = nullptr;
	}

	template <typename F>
	explicit Function(F&& f)
		: m_bridge(nullptr)
	{
		using Functor = std::decay_t<F>;
		using Bridge = Detail::FunctorBridge<Functor, R, Args...>;
		m_bridge = new Bridge(std::forward<F>(f));
	}

	Function& operator= (const Function& other)
	{
		Function tmp(other);
		swap(*this, other);
		return *this;
	}

	Function& operator= (Function&& other) noexcept
	{
		delete m_bridge;
		m_bridge = other.m_bridge;
		other.m_bridge = nullptr;
		return *this;
	}

	template<typename F>
	Function& operator=(F&& f)
	{
		Function tmp(std::forward<F>(f));
		swap(*this, tmp);
		return *this;
	}

	~Function()
	{
		delete m_bridge;
	}

	friend void swap(Function& f1, Function& f2) noexcept
	{
		std::swap(f1.m_bridge, f2.m_bridge);
	}

	explicit operator bool() const
	{
		return m_bridge == nullptr;
	}

	R operator()(Args... args) const
	{
		return m_bridge->Invoke(std::forward<Args>(args...));
	}
	
private:
	Detail::IFunctorBridge<R, Args...>* m_bridge{ nullptr };
};