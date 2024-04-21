#include <utility>
#include <memory>

export module MyForward_List;
export
template<typename T, typename Allocator = std::allocator<T>>
class MyForward_List
{
	struct BaseNode
	{
		BaseNode* next;
	};

	struct Node : BaseNode
	{
		template <typename... Args>
		Node(Args&&... args) : BaseNode(nullptr), value(std::forward<Args>(args)...) {}
		Node(const T& value) : BaseNode(nullptr), value(value) {}

		T value;
	};
	using allocator_type = std::allocator_traits<Allocator>::template rebind_alloc<Node>;
	using allocator_traits = std::allocator_traits<allocator_type>;

	allocator_type alloc;
	BaseNode* head = nullptr;
public:
	MyForward_List() : head(nullptr) {}
	explicit MyForward_List(T value)
	{
		Node* node = allocator_traits::allocate(alloc, 1);
		allocator_traits::construct(alloc, node, value);
		static_cast<BaseNode*>(node)->next = head;
		head = static_cast<BaseNode*>(node);
	}

	MyForward_List(size_t count, const T& value)
	{
		for (size_t i = 0; i < count; ++i)
		{
			Node* node = allocator_traits::allocate(alloc, 1); 
			allocator_traits::construct(alloc, node, value);
			static_cast<BaseNode*>(node)->next = head;
			head = static_cast<BaseNode*>(node);
		}
	}

	explicit MyForward_List(size_t count)
	{
		for (size_t i = 0; i < count; ++i)
		{
			Node* node = allocator_traits::allocate(alloc, 1);
			static_cast<BaseNode*>(node)->next = head;
			head = static_cast<BaseNode*>(node);
		}
	}

	MyForward_List(MyForward_List&& other) : alloc(other.alloc), head(other.head)
	{
		other.head = nullptr;
	}

	class iterator {
		friend class MyForward_List<T>;

		BaseNode* node;

	public:
		using iterator_category = std::forward_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;

		explicit iterator(BaseNode* node) : node(node) {}

		T& operator*() const 
		{
			return static_cast<Node*>(node)->value;
		}

		T* operator->() const 
		{
			return &(static_cast<Node*>(node)->value);
		}

		bool operator!=(const iterator& other) const 
		{
			return node != other.node;
		}

		bool operator==(const iterator& other) const 
		{
			return node == other.node;
		}

		iterator& operator++() 
		{
			node = node->next;
			return *this;
		}

		iterator operator++(int) 
		{
			iterator temp = *this;
			node = node->next;
			return temp;
		}
	};
	friend class iterator;

	iterator begin()
	{
		return iterator(head);
	}

	iterator end()
	{
		return iterator(nullptr);
	}

	bool empty()
	{
		return head;
	}

	iterator find(const T& value)
	{
		for (auto it = this->begin(); it != this->end(); ++it)
			if (*it = value)
				return it;
		return this->end();
	}

	void push_front(const T& value)
	{
		Node* node = allocator_traits::allocate(alloc, 1);
		allocator_traits::construct(alloc, node, value);
		static_cast<BaseNode*>(node)->next = head;
		head = static_cast<BaseNode*>(node);
	}

	template<typename... Args>
	void emplace_front(Args&&... args)
	{
		Node* node = allocator_traits::allocate(alloc, 1);
		allocator_traits::construct(alloc, node, std::forward<Args>(args)...);
		static_cast<BaseNode*>(node)->next = head;
		head = static_cast<BaseNode*>(node);
	}

	void pop_front()
	{
		if (head) 
		{  
			BaseNode* firstNode = head;  
			head = firstNode->next;
			allocator_traits::destroy(alloc, firstNode);
			allocator_traits::deallocate(alloc, static_cast<Node*>(firstNode), 1);
		}
	}

	void insert_after(MyForward_List<T>::iterator where, T value)
	{
		if (where.node != nullptr)
		{
			Node* newNode = allocator_traits::allocate(alloc, 1);
			allocator_traits::construct(alloc, newNode, value);
			newNode->next = where.node->next;
			where.node->next = newNode;
		}
	}

	template<typename... Args>
	void emplace_after(MyForward_List<T>::iterator where, Args&&... args)
	{
		if (where.node != nullptr)
		{
			Node* newNode = allocator_traits::allocate(alloc, 1);
			allocator_traits::construct(alloc, newNode, std::forward<Args>(args)...);
			newNode->next = where.node->next;
			where.node->next = newNode;
		}
	}

	void erase_after(MyForward_List<T>::iterator from)
	{
		if (from.node->next != nullptr)
		{
			Node* temp = static_cast<Node*>(from.node->next);
			from.node->next = temp->next;
			allocator_traits::destroy(alloc, temp);
			allocator_traits::deallocate(alloc, temp, 1);
		}
	}
};