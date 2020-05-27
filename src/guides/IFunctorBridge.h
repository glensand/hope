//------------------------------------------------------------------------------
// Functor.h

// Copyright (c) 2020 glensand
// All rights reserved.
//
// Date: 26.05.2020
// Author: Bezborodoff Gleb
//------------------------------------------------------------------------------

#pragma once

#include <type_traits>

namespace Detail
{
	
	template <typename R, typename... Args>
	class IFunctorBridge
	{
	public:
		virtual ~IFunctorBridge(){}

        [[nodiscard]] virtual IFunctorBridge*	Clone() const = 0;
		
		virtual R				Invoke(Args... args) const = 0;
	};

	template <typename Functor, typename R, typename... Args>
	class FunctorBridge final : public IFunctorBridge<R, Args...>
	{
	public:
		template <typename ForwardedFunctor>
		explicit FunctorBridge(ForwardedFunctor&& functor)
			:m_functor(std::forward<ForwardedFunctor>(functor))
		{
		}

		FunctorBridge* Clone() const override
		{
			return new FunctorBridge(m_functor);
		}

		R Invoke(Args... args) const override
		{
			return m_functor(std::forward<Args>(args)...);
		}
		
	private:
		Functor	m_functor;
	};

}
