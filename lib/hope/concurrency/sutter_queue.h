/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include <atomic>

namespace hope::concurrency {

    template <typename T>
	class sutter_queue final
	{
	private:
		struct Node
		{
			template<typename TVal>
			Node(TVal&& val) : value(std::forward<TVal>(val)), next(nullptr) { }
			T value;
			Node* next;
		};
	
		Node* m_first;	// Producer Only
		std::atomic<Node*> 	m_divider;	// Producer / Consumer
		std::atomic<Node*> 	m_last;		// Producer / Consumer
	
	public:
		// This queue must be fully constructed before being used in another thread.
		sutter_queue(std::size_t pre_alloc = 0)
		{
			m_first = m_divider = m_last = new Node(T());
		}
	
		~sutter_queue()
		{
			while (m_first != nullptr)
			{
				Node* temp = m_first;
				m_first = temp->next;
				delete temp;
			}
		}
	
		template<typename TVal>
		void enqueue(TVal&& item)
		{
			Node* tmpLast = m_last.load();
			tmpLast->next = new Node(std::forward<TVal>(item));
	
			ProduceImpl(tmpLast);
		}
	
		bool Consume(T& item)
		{
			Node* divNode = m_divider.load();
			Node* lastNode = m_last.load();
			if (divNode != lastNode)
			{
				item = std::move(divNode->next->value);
				divNode = divNode->next;
				m_divider.store(divNode);
	
				return true;
			}
			return false;
		}

		T* dequeue()
		{
			T value;
			Consume(value);
			return nullptr;
		}

		bool Peek(T& item)
		{
			Node* divNode = m_divider.load();
			Node* lastNode = m_last.load();
			if (divNode != lastNode)
			{
				item = divNode->next->value;
				return true;
			}
			return false;
		}
	
	private:
		void ProduceImpl(Node* tmpLast)
		{
			tmpLast = tmpLast->next;
	
			m_last.store(tmpLast);
	
			Node* div = m_divider.load();
			while (m_first != div)
			{
				Node* temp = m_first;
				m_first = m_first->next;
				delete temp;
	
				div = m_divider.load();
			}
		}
	};
}