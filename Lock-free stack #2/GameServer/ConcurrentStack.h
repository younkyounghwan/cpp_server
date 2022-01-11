#pragma once
#include <mutex>

template<typename T>
class LockStack
{
public:
	LockStack()
	{}
	LockStack(const LockStack&) = delete;
	LockStack& operator=(const LockStack&) = delete;

	void Push(T value)
	{
		lock_guard<mutex> lock(_mutex);
		_stack.push(std::move(value));
		_condVar.notify_one();
	}

	bool TryPop(T& value)
	{
		lock_guard<mutex> lock(_mutex);
		if (_stack.empty())
			return false;
		// empty -> top -> pop
		value = std::move(_stack.top());
		_stack.pop();
		return true;
	}

	bool Empty()
	{
		lock_guard<mutex> lock(_mutex);
		return _stack.empty();
	}

	void WaitPop(T& value)
	{
		unique_lock<mutex> lock(_mutex);
		_condVar.wait(lock, [this] {return _stack.empty() == false; }); 
		// lambda
		value = std::move(_stack.top());
		_stack.pop();
	}

private:
	stack<T> _stack;
	mutex _mutex;

	condition_variable _condVar;
};


template<typename T>
class LockFreeStack
{
	struct Node
	{
		Node(const T& value) : data(value), next(nullptr)
		{

		}
		T data;
		Node* next;
	};

public:
	// 1) 새 노드를 만들고
	// 2) 새 노드의 next = head
	// 3) head = 새 노드 

	// [ ][ ][ ][ ][ ]
	// [head]
	void Push(const T& value)
	{
		// multi threading 환경에서는 불가능하다.
		Node* node = new Node(value);
		node->next = _head;


		while (_head.compare_exchange_weak(node->next, node) == false)
		{
			//node->next = _head;
		}
		// 이 사이에 새치기 당하면?
		//_head = node;
	}

	// 1) head 읽기
	// 2) head->next 읽기
	// 3) head = head->next
	// 4) data 추출해서 반환
	// 5) 추출한 노드를 삭제

	bool TryPop(T& value)
	{
		++_popCount;
		Node* oldHead = _head;

		while (oldHead && _head.compare_exchange_weak(oldHead, oldHead->next) == false)
		{
		}

		if (oldHead == nullptr)
		{
			--_popCount;
			return false;
		}


		value = oldHead->data;
		TryDelete(oldHead);
		return true;
	}

	// 1) 데이터 분리
	// 2) count 체크
	// 3) 나 혼자면 삭제

	void TryDelete(Node* oldHead)
	{
		//나 외에 누가 있는가?
		if (_popCount == 1)
		{
			// 나 혼자네?

			// 이황 혼자인거, 삭제 예약된 다른 데이터들도 삭제해보자.
			Node* node = _pendingList.exchange(nullptr); // exchange read와 write가 동시에 됨.
			
			if (--_popCount == 0)
			{
				// 중간에 끼어든 애가 없음 -> 삭제 진행
				// 이제와서누군가가 끼어 들어도, 어차피 데이터는 분리해둔 상태!
				DeleteNodes(node);
			}

			else if (node)
			{
				// 누가 끼어들었으니 다시 갖다 놓자.
				ChainPendingNodeList(node);
			}

			// 내 데이터는 삭제

			delete oldHead;
		}

		else
		{
			// 누가 있네? 그럼 지금 삭제하지 않고, 삭제 예약만 하자
			ChainPendingNode(oldHead);
			--_popCount;
		}
	}

	void ChainPendingNodeList(Node* first, Node* last)
	{
		last->next = _pendingList;

		while (_pendingList.compare_exchange_weak(last->next, first) == false)
		{}
	}

	// [ ][ ][ ][ ][ ][ ]
	void ChainPendingNodeList(Node* node)
	{
		Node* last = node;
		while (last->next)
			last = last->next;

		ChainPendingNodeList(node, last);
	}

	void ChainPendingNode(Node* node)
	{
		ChainPendingNodeList(node, node);
	}

	//[ ][ ] [ ][ ] [ ][ ] [ ][ ]
	static void DeleteNodes(Node* node)
	{
		while (node)
		{
			Node* next = node->next;
			delete node;
			node = next;
		}
	}


private:
	// [ ][ ][ ][ ][ ]
	// [head]
	atomic<Node*> _head;

	atomic<uint32> _popCount = 0; // Pop을 실행중인 쓰레드 개수
	atomic<Node*> _pendingList; // 삭제되어야 할 노드들 (첫번째 노드)
};