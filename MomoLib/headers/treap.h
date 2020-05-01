#include <vector>
#include <cstdint>
#include <limits>
#include <functional>
#include <random>

namespace momo
{
	/*
	random that returns int64. Is used to generate keys for treap nodes
	*/
	struct random_int64
	{
		using RandomReturnType = std::uint_fast64_t;

		using Generator = std::mersenne_twister_engine<RandomReturnType, 64, 312, 156, 31,
			0xb5026f5aa96619e9, 29,
			0x5555555555555555, 17,
			0x71d67fffeda60000, 37,
			0xfff7eee000000000, 43, 6364136223846793005>;

		static Generator generator;

		static RandomReturnType get()
		{
			return generator();
		}
	};

	/*
	treap iterator. Only provides compare and dereference functions
	sadly, treap class is not iterable due to it inner structure, so ++/-- are not supported
	*/
	template<typename Treap>
	class iterator
	{
		using pointer_type = typename Treap::treap_node_ptr;
		using value_type = typename Treap::value_type;

		pointer_type _ptr;
	public:
		inline explicit iterator() noexcept
			: _ptr(nullptr)
		{

		}

		inline explicit iterator(pointer_type p) noexcept
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

		inline value_type& operator*() noexcept
		{
			return _ptr->value;
		}

		inline const value_type& operator*() const noexcept
		{
			return _ptr->value;
		}

		inline const value_type& operator->() const noexcept
		{
			return _ptr->value;
		}

		inline value_type& operator->() noexcept
		{
			return _ptr->value;
		}
	};

	/*
	treap class. Tried by best to make it more STL-compatible. 
	Tested on multiple programming contest tasks, so you are free to use it.
	*/
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
		using value_type = T;
		using treap_node = Node<value_type, Priority>;
		using treap_node_ptr = treap_node*;
		using iterator = iterator<treap>;
		using treap_node_ptr_pair = typename std::pair<treap_node_ptr, treap_node_ptr>;
		using treap_pair = typename std::pair<treap, treap>;
		using allocator = Alloc<treap_node>;
		using update_callback = std::function<void(treap_node_ptr)>;

	private:
		inline treap_node_ptr _construct_node(const value_type& value)
		{
			treap_node_ptr node = _alloc.allocate(1);
			Priority priority = Random::get();
			_alloc.construct(node, value, priority);
			return node;
		}

		inline treap_node_ptr _construct_node(value_type&& value)
		{
			treap_node_ptr node = _alloc.allocate(1);
			Priority priority = Random::get();
			_alloc.construct(node, std::move(value), priority);
			return node;
		}

		inline void _destroy_node(treap_node_ptr node) noexcept
		{
			_alloc.destroy(node);
			_alloc.deallocate(node, 1);
		}

		void _destroy_node_recursive(treap_node_ptr node) noexcept
		{
			if (node->left != nullptr) _destroy_node_recursive(node->left);
			if (node->right != nullptr) _destroy_node_recursive(node->right);
			_destroy_node(node);
		}

		treap_node_ptr _deep_copy_recursive(treap_node_ptr from)
		{
			if (from == nullptr) return nullptr;
			allocator alloc;
			treap_node_ptr node = alloc.allocate(1);
			alloc.construct(node, from->value, from->priority);
			node->sub_tree_size = from->sub_tree_size;
			node->left = _deep_copy_recursive(from->left);
			node->right = _deep_copy_recursive(from->left);
			return node;
		}

		const treap_node_ptr _most_left(const treap_node_ptr root) const
		{
			if (root->left != nullptr)
				return _most_left(root->left);
			else
				return root;
		}

		const treap_node_ptr _most_right(const treap_node_ptr root) const
		{
			if (root->right != nullptr)
				return _most_right(root->right);
			else
				return root;
		}

		treap_node_ptr _insert_node(treap_node_ptr root, treap_node_ptr node)
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
				treap_node_ptr_pair subTree = _split(root, node->value);
				node->left = subTree.first;
				node->right = subTree.second;
				ON_UPDATE(node);
				return node;
			}
		}

		void _insert_val(const value_type& value)
		{
			treap_node_ptr node = _construct_node(value);
			_root = _insert_node(_root, node);
		}

		void _insert_val(value_type&& value)
		{
			treap_node_ptr node = _construct_node(std::move(value));
			_root = _insert_node(_root, node);
		}

		treap_node_ptr_pair _split(treap_node_ptr root, const value_type& K)
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

		treap_node_ptr _merge(treap_node_ptr tree_left, treap_node_ptr tree_right)
		{
			if (tree_left == nullptr) return tree_right;
			if (tree_right == nullptr) return tree_left;

			if (tree_left->priority < tree_right->priority)
			{
				treap_node_ptr root = tree_left;
				root->right = _merge(root->right, tree_right);
				ON_UPDATE(root);
				return root;
			}
			else
			{
				treap_node_ptr root = tree_right;
				root->left = _merge(tree_left, root->left);
				ON_UPDATE(root);
				return root;
			}
		}

		treap_node_ptr _erase(treap_node_ptr root, const value_type& value)
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
				treap_node_ptr toDelete = root;
				root = _merge(root->left, root->right);

				_destroy_node(toDelete);

				if (root != nullptr)
				{
					ON_UPDATE(root);
				}
				return root;
			}
		}

		treap_node_ptr _find(const treap_node_ptr root, const value_type& value) const
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
		void _apply(const treap_node_ptr root, Func&& func) const
		{
			if (root == nullptr) return;

			_apply(root->left, std::forward<Func>(func));
			func(root->value);
			_apply(root->right, std::forward<Func>(func));
		}

		template<typename SortedIt>
		treap_node_ptr _build(const SortedIt& first, const SortedIt& last)
		{
			std::vector<treap_node_ptr> st;
			treap_node_ptr root = nullptr;

			for (SortedIt it = first; it != last; it++)
			{
				treap_node_ptr current = _construct_node(*it);
				treap_node_ptr last_popped = nullptr;

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

		treap_node_ptr _root;
		size_t _size;
		allocator _alloc;
		update_callback _on_update;

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

		inline iterator end() const
		{
			return iterator(nullptr);
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

		inline allocator& get_allocator()
		{
			return _alloc;
		}

		inline const value_type& root() const
		{
			return _root->value;
		}

		inline const value_type& left() const
		{
			return _most_left(_root)->value;
		}

		inline const value_type& right() const
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

		inline void insert(const value_type& value)
		{
			_insert_val(value);
			_size = _root->sub_tree_size;
		}

		inline void insert(value_type&& value)
		{
			_insert_val(std::move(value));
			_size = _root->sub_tree_size;
		}

		inline iterator find(const value_type& value) const
		{
			return iterator(_find(_root, value));
		}

		inline void erase(const value_type& value)
		{
			_root = _erase(_root, value);
			_size = (_root == nullptr) ? 0 : _root->sub_tree_size;
		}

		template<typename Func>
		inline void apply_visitor(Func&& func) const
		{
			_apply(_root, std::forward<Func>(func));
		}

		inline treap_pair split(const value_type& value)
		{
			treap_pair p;
			treap_node_ptr_pair sub_trees = _split(_root, value);
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

			treap_node_ptr root = _merge(treaps.first._root, treaps.second._root);
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
