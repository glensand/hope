/* Copyright (C) 2020 - 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include "hope/function/function_bridge.h"

namespace hope {
	template <typename Signature>
	class function;

	template <typename R, typename... Args>
	class function<R(Args...)> {
	public:
		function(const function& other) = default;
		function(function& other)
			: function(static_cast<const function&>(other)) { }

		constexpr function(function&& other) noexcept
			: m_bridge(other.m_bridge) {
			other.m_bridge = nullptr;
		}

		template <typename F>
		explicit function(F&& f)
			: m_bridge(nullptr) {
			using functor_t = std::decay_t<F>;
			using bridge_t = detail::function_bridge_impl<functor_t, R, Args...>;
			m_bridge = new bridge_t(std::forward<F>(f));
		}

		function& operator= (const function& other) {
			function tmp(other);
			swap(*this, other);
			return *this;
		}

		function& operator= (function&& other) noexcept {
			delete m_bridge;
			m_bridge = other.m_bridge;
			other.m_bridge = nullptr;
			return *this;
		}

		template<typename F>
		function& operator=(F&& f) {
			function tmp(std::forward<F>(f));
			swap(*this, tmp);
			return *this;
		}

		~function() {
			delete m_bridge;
		}

		friend void swap(function& f1, function& f2) noexcept {
			std::swap(f1.m_bridge, f2.m_bridge);
		}

		explicit operator bool() const {
			return m_bridge == nullptr;
		}

		R operator()(Args... args) const {
			return m_bridge->invoke(std::forward<Args>(args)...);
		}

	private:
		detail::function_bridge<R, Args...>* m_bridge{ nullptr };
	};
}
