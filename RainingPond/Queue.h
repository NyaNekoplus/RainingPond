#ifndef QUEUE_H_
#define QUEUE_H_

#include <iostream>
#include "Ptr.h"

namespace Basic {
	template<typename T>
	class Queue {
	public:
		Queue(size_t capcity = 100);
		~Queue() = default;

		size_t Capcity()const { return capc; }
		size_t Size()const { return (rear - front + capc) % capc; }
		bool IsEmpty()const { return front == rear ? true : false; }
		bool IsFull()const { return (rear + 1) % capc == front ? true : false; }
		uPtr<T[]> Data()const { return queue; }

		T GetFront()const;
		bool Push(const T& data);
		T Pop();

		void Traverse()const;

	private:
		size_t front;
		size_t rear;
		size_t capc;
		uPtr<T[]> queue;
	};

	template<typename T>
	Queue<T>::Queue(size_t capcity) :front(0), rear(0), capc(capcity), queue(nullptr) {
		queue.reset(new T[capc]);
	}

	template<typename T>
	bool Queue<T>::Push(const T& data) {
		if (IsFull()) {
			std::cout << "The queue is full. Push Failed" << std::endl;
			return false;
		}
		queue[rear] = data;
		rear = (rear + 1) % capc;
		return true;
	}

	template<typename T>
	T Queue<T>::Pop() {
		T data;
		if (IsEmpty()) {
			std::cout << "The queue is Empty. Pop Failed" << std::endl;
			return data;
		}
		data = queue[front];
		front = (front + 1) % capc;
		return data;
	}

	template<typename T>
	T Queue<T>::GetFront()const {
		if (IsEmpty()) {
			std::cout << "The queue is Empty." << std::endl;
			return false;
		}
		return queue[front];
	}

	template<typename T>
	void Queue<T>::Traverse()const {
		
	}
}

#endif