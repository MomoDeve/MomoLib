#include <vector>
#include <cstdlib>
#include <cstdint>
#include <limits>
#include <functional>

namespace momo
{
	struct random_int64
	{
		static int64_t get()
		{
			return (((int64_t)rand()) << 32) | (int64_t)rand();
		}
	};

	template<typename Treap>
	class iterator
	{
		using PointerType = typename Treap::TreapNodePointer;
		using ValueType = typename Treap::ValueType;

		PointerType _ptr;
	public:
		inline explicit iterator() noexcept
			: _ptr(nullptr)
		{

		}

		inline explicit iterator(PointerType p) noexcept
			: _ptr(p)
		{

		}

		iterator(const iterator&) = default;
		iterator(iterator&&) = default;
		iterator& operator=(const iterator&) = default;
		iterator& operator=(iterator&&) = default;

		inline bool operator==(const iterator& it) const noexcept
		{
			return _ptr == it._ptr;
		}

		inline bool operator!=(const iterator& it) const noexcept
		{
			return _ptr != it._ptr;
		}

		inline ValueType& operator*() noexcept
		{
			return _ptr->value;
		}

		inline const ValueType& operator*() const noexcept
		{
			return _ptr->value;
		}

		inline const ValueType& operator->() const noexcept
		{
			return _ptr->value;
		}

		inline ValueType& operator->() noexcept
		{
			return _ptr->value;
		}
	};

	template <typename T, typename Priority = uint64_t, typename Random = random_int64, template<typename> class Alloc = std::allocator>
	class treap
	{
		#define ON_UPDATE(ROOT) \
		ROOT->_update();\
		if(_on_update) _on_update(ROOT)

		template<typename U, typename P = int64_t>
		struct Node
		{
			U value;
			P priority;
			size_t sub_tree_size = 1;
			Node* left = nullptr;
			Node* right = nullptr;

			inline Node(const U& value, P priority)
				: value(value), priority(priority)
			{

			}

			inline Node(U&& value, P priority)
				: value(std::move(value)), priority(priority)
			{

			}

			inline void _update() noexcept
			{
				this->sub_tree_size = 1;
				if (left != nullptr) this->sub_tree_size += left->sub_tree_size;
				if (right != nullptr) this->sub_tree_size += right->sub_tree_size;
			}
		};
	public:
		using ValueType = T;
		using TreapNode = Node<ValueType, Priority>;
		using TreapNodePointer = TreapNode*;
		using Iterator = iterator<treap>;
		using TreapNodePointerPair = typename std::pair<TreapNodePointer, TreapNodePointer>;
		using TreapPair = typename std::pair<treap, treap>;
		using Allocator = Alloc<TreapNode>;
		using UpdateCallback = std::function<void(TreapNodePointer)>;

	private:
		inline TreapNodePointer _construct_node(const ValueType& value)
		{
			TreapNodePointer node = _alloc.allocate(1);
			Priority priority = Random::get();
			_alloc.construct(node, value, priority);
			return node;
		}

		inline TreapNodePointer _construct_node(ValueType&& value)
		{
			TreapNodePointer node = _alloc.allocate(1);
			Priority priority = Random::get();
			_alloc.construct(node, std::move(value), priority);
			return node;
		}

		inline void _destroy_node(TreapNodePointer node) noexcept
		{
			_alloc.destroy(node);
			_alloc.deallocate(node, 1);
		}

		void _destroy_node_recursive(TreapNodePointer node) noexcept
		{
			if (node->left != nullptr) _destroy_node_recursive(node->left);
			if (node->right != nullptr) _destroy_node_recursive(node->right);
			_destroy_node(node);
		}

		TreapNodePointer _deep_copy_recursive(TreapNodePointer from)
		{
			if (from == nullptr) return nullptr;
			Allocator alloc;
			TreapNodePointer node = alloc.allocate(1);
			alloc.construct(node, from->value, from->priority);
			node->sub_tree_size = from->sub_tree_size;
			node->left = _deep_copy_recursive(from->left);
			node->right = _deep_copy_recursive(from->left);
			return node;
		}

		const TreapNodePointer _most_left(const TreapNodePointer root) const
		{
			if (root->left != nullptr)
				return _most_left(root->left);
			else
				return root;
		}

		const TreapNodePointer _most_right(const TreapNodePointer root) const
		{
			if (root->right != nullptr)
				return _most_right(root->right);
			else
				return root;
		}

		TreapNodePointer _insert_node(TreapNodePointer root, TreapNodePointer node)
		{
			if (root == nullptr)
			{
				return node;
			}

			if (root->priority < node->priority)
			{
				if (root->value < node->value)
				{
					root->right = _insert_node(root->right, node);
				}
				else
				{
					root->left = _insert_node(root->left, node);
				}
				ON_UPDATE(root);
				return root;
			}
			else
			{
				TreapNodePointerPair subTree = _split(root, node->value);
				node->left = subTree.first;
				node->right = subTree.second;
				ON_UPDATE(node);
				return node;
			}
		}

		void _insert_val(const ValueType& value)
		{
			TreapNodePointer node = _construct_node(value);
			_root = _insert_node(_root, node);
		}

		void _insert_val(ValueType&& value)
		{
			TreapNodePointer node = _construct_node(std::move(value));
			_root = _insert_node(_root, node);
		}

		TreapNodePointerPair _split(TreapNodePointer root, const ValueType& K)
		{
			if (root == nullptr)
				return { nullptr, nullptr };

			if (K < root->value)
			{
				auto subTree = _split(root->left, K);
				root->left = subTree.second;
				ON_UPDATE(root);
				return { subTree.first, root };
			}
			else
			{
				auto subTree = _split(root->right, K);
				root->right = subTree.first;
				ON_UPDATE(root);
				return { root, subTree.second };
			}
		}

		TreapNodePointer _merge(TreapNodePointer tree_left, TreapNodePointer tree_right)
		{
			if (tree_left == nullptr) return tree_right;
			if (tree_right == nullptr) return tree_left;

			if (tree_left->priority < tree_right->priority)
			{
				TreapNodePointer root = tree_left;
				root->right = _merge(root->right, tree_right);
				ON_UPDATE(root);
				return root;
			}
			else
			{
				TreapNodePointer root = tree_right;
				root->left = _merge(tree_left, root->left);
				ON_UPDATE(root);
				return root;
			}
		}

		TreapNodePointer _erase(TreapNodePointer root, const ValueType& value)
		{
			if (root == nullptr) return nullptr;

			if (root->value < value)
			{
				root->right = _erase(root->right, value);
				ON_UPDATE(root);
				return root;
			}
			else if (value < root->value)
			{
				root->left = _erase(root->left, value);
				ON_UPDATE(root);
				return root;
			}
			else // equality
			{
				TreapNodePointer toDelete = root;
				root = _merge(root->left, root->right);

				_destroy_node(toDelete);

				if (root != nullptr)
				{
					ON_UPDATE(root);
				}
				return root;
			}
		}

		TreapNodePointer _find(const TreapNodePointer root, const ValueType& value) const
		{
			if (root == nullptr)
				return nullptr;

			else if (root->value < value)
			{
				return _find(root->right, value);
			}
			else if (value < root->value)
			{
				return _find(root->left, value);
			}
			else // value found
			{
				return root;
			}
		}

		template<typename Func>
		void _apply(const TreapNodePointer root, Func&& func) const
		{
			if (root == nullptr) return;

			_apply(root->left, std::forward<Func>(func));
			func(root->value);
			_apply(root->right, std::forward<Func>(func));
		}

		template<typename SortedIt>
		TreapNodePointer _build(const SortedIt& first, const SortedIt& last)
		{
			std::vector<TreapNodePointer> st;
			TreapNodePointer root = nullptr;

			for (SortedIt it = first; it != last; it++)
			{
				TreapNodePointer current = _construct_node(*it);
				TreapNodePointer last_popped = nullptr;

				while (!st.empty())
				{
					if (st.back()->priority < current->priority)
					{
						st.back()->right = current;
						break;
					}
					last_popped = st.back();
					ON_UPDATE(last_popped);
					st.pop_back();
				}
				if (st.empty())
					root = current;

				current->left = last_popped;
				st.push_back(current);
			}
			while (!st.empty())
			{
				ON_UPDATE(st.back());
				st.pop_back();
			}
			return root;
		}

		TreapNodePointer _root;
		size_t _size;
		Allocator _alloc;
		UpdateCallback _on_update;

	public:
		inline treap()
			: _root(nullptr), _size(0), _alloc(), _on_update()
		{

		}

		inline ~treap()
		{
			if (_root != nullptr)
			{
				_destroy_node_recursive(_root);
			}
		}

		inline treap(treap&& tr) noexcept
			: _root(tr._root), _size(tr._size), _alloc(std::move(tr._alloc)), _on_update(std::move(tr._on_update))
		{
			tr._size = 0;
			tr._root = nullptr;
		}

		inline treap(const treap& tr)
			: _root(_deep_copy_recursive(tr._root)), _size(tr._size), _alloc(tr._alloc), _on_update(tr._on_update)
		{
			
		}

		template<typename SortedIt>
		inline treap(const SortedIt& first, const SortedIt& last)
		{
			_root = _build(first, last);
			if (_root != nullptr) _size = _root->sub_tree_size;
		}

		inline treap& operator=(treap&& tr) noexcept
		{
			_root = tr._root;
			_size = tr._size;
			_alloc = std::move(tr._alloc);
			_on_update = std::move(tr._on_update);
			tr._root = nullptr;
			tr._size = 0;
			return *this;
		}

		inline treap& operator=(const treap& tr)
		{
			_root = _deep_copy_recursive(tr._root);
			_size = tr._size;
			_alloc = tr._alloc;
			_on_update = tr._on_update;
		}

		inline Iterator end() const
		{
			return Iterator(nullptr);
		}

		inline size_t size() const noexcept
		{
			return _size;
		}

		inline size_t max_size() const noexcept
		{
			return std::numeric_limits<size_t>::max();
		}

		inline bool empty() const noexcept
		{
			return _size == 0;
		}

		inline void clear() noexcept
		{
			if (_root != nullptr)
			{
				_destroy_node_recursive(_root);
				_root = nullptr;
				_size = 0;
			}
		}

		inline Allocator& get_allocator()
		{
			return _alloc;
		}

		inline const ValueType& root() const
		{
			return _root->value;
		}

		inline const ValueType& left() const
		{
			return _most_left(_root)->value;
		}

		inline const ValueType& right() const
		{
			return _most_right(_root)->value;
		}

		template<typename SortedIt>
		inline void rebuild(const SortedIt& first, const SortedIt& last)
		{
			if (_root != nullptr) _destroy_node_recursive(_root);
			_root = _build(first, last);
			_size = _root->sub_tree_size;
		}

		inline void insert(const ValueType& value)
		{
			_insert_val(value);
			_size = _root->sub_tree_size;
		}

		inline void insert(ValueType&& value)
		{
			_insert_val(std::move(value));
			_size = _root->sub_tree_size;
		}

		inline Iterator find(const ValueType& value) const
		{
			return Iterator(_find(_root, value));
		}

		inline void erase(const ValueType& value)
		{
			_root = _erase(_root, value);
			_size = (_root == nullptr) ? 0 : _root->sub_tree_size;
		}

		template<typename Func>
		inline void apply_visitor(Func&& func) const
		{
			_apply(_root, std::forward<Func>(func));
		}

		inline TreapPair split(const ValueType& value)
		{
			TreapNodePointerPair sub_trees = _split(_root, value);
			TreapPair p;
			if (sub_trees.first != nullptr)
			{
				p.first._root = sub_trees.first;
				p.first._size = sub_trees.first->sub_tree_size;
				p.first._alloc = _alloc;
				p.first._on_update = _on_update;
			}
			if (sub_trees.second != nullptr)
			{
				p.second._root = sub_trees.second;
				p.second._size = sub_trees.second->sub_tree_size;
				p.second._alloc = _alloc;
				p.second._on_update = _on_update;
			}

			_root = nullptr;
			_size = 0;
			return p; // NRVO
		}

		inline void merge(TreapPair& treaps)
		{
			if (_root != nullptr) _destroy_node_recursive(_root);

			TreapNodePointer root = _merge(treaps.first._root, treaps.second._root);
			_root = root;
			_size = (root == nullptr) ? 0 : root->sub_tree_size;
			if (treaps.first._root != nullptr)
			{
				_alloc = std::move(treaps.first._alloc);
				_on_update = std::move(treaps.first._on_update);
			}
			else
			{
				_alloc = std::move(treaps.second._alloc);
				_on_update = std::move(treaps.second._on_update);
			}

			treaps.first._root = treaps.second._root = nullptr;
			treaps.first._size = treaps.second._size = 0;
		}

		void on_update(UpdateCallback callback)
		{
			_on_update = std::move(callback);
		}

		friend inline void swap(treap& tr1, treap& tr2);

		inline void swap(treap& tr)
		{
			swap(*this, tr);
		}
	};

	template<typename T, typename P, typename R, template<typename> class A>
	inline void swap(treap<T, P, R, A>& tr1, treap<T, P, R, A>& tr2)
	{
		auto root_tmp = tr1._root;
		tr1._root = tr2._root;
		tr2._root = root_tmp;
		tr1._size = tr1._root->sub_tree_size;
		tr2._size = tr2._root->sub_tree_size;
	}
}
