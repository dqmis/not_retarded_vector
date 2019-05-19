// vector_new.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iterator>
#include <memory>

#if _WIN32 || _WIN64
#if _WIN64
#define ENVIRONMENT64 true
#else
#define ENVIRONMENT32 true
#endif
#endif

// Check GCC
#if __GNUC__
#if __x86_64__ || __ppc64__
#define ENVIRONMENT64 true
#else
#define ENVIRONMENT32 true
#endif
#endif

namespace nr {
	template<typename T> class vector {
	public:
		// Member types
		std::allocator<T> alloc;
		typedef T* iterator;
		typedef const T* const_iterator;
		typedef std::reverse_iterator<iterator> reverse_iterator;
		typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
		typedef size_t size_type;
		typedef T value_type;
		typedef T & reference;
		typedef const T & const_reference;

		// Member functions
		explicit vector() { arr = _capacity = _size = nullptr; }
		explicit vector(size_t n) {
			arr = alloc.allocate(n);
			_size = _capacity = arr + n;
		}
		explicit vector(size_t n, const T & val) {
			arr = alloc.allocate(n);
			_size = _capacity = arr + n;
			std::uninitialized_fill(arr, _size, val);
		}
		explicit vector(const vector & v) {
			arr = alloc.allocate(v.end() - v.begin());
			_size = _capacity = std::uninitialized_copy(v.begin(), v.end(), arr);
		}
		explicit vector(const_iterator i, const_iterator j) {
			arr = alloc.allocate(j - i);
			_size = _capacity = std::uninitialized_copy(i, j, arr);
		}
		explicit vector(std::initializer_list<T> il) {
			arr = alloc.allocate(il.end() - il.begin());
			_size = _capacity = std::uninitialized_copy(il.begin(), il.end(), arr);
		}
		~vector() { clear(); }
		vector & operator = (const vector & v) {
			if (&v == this) return *this;
			clear();
			arr = alloc.allocate(v.end() - v.begin());
			_size = _capacity = std::uninitialized_copy(v.begin(), v.end(), arr);
		}
		void assign(size_t n, const T & val) {
			clear();
			arr = alloc.allocate(n);
			_size = _capacity = arr + n;
			std::uninitialized_fill(arr, _size, val);
		}

		// Element access
		reference operator [](size_t i) {
			if (i < 0 || arr + i > _size) throw std::out_of_range("index is out of range");
			return arr[i];
		}
		const_reference operator [](size_t i) const {
			if (i < 0 || arr + i > _size) throw std::out_of_range("index is out of range");
			return arr[i];
		}

		// Iterators
		iterator begin() {
			if (_size - arr == 0) throw std::out_of_range("vector is empty");
			return arr;
		}
		iterator end() {
			if (_size - arr == 0) throw std::out_of_range("vector is empty");
			return _size;
		}
		reverse_iterator rbegin() { return reverse_iterator(end()); }
		reverse_iterator rend() { return reverse_iterator(begin()); }
		iterator & operator--() {
			--ch;
			return *this;
		}

		// Capacity
		bool empty() { return _size - arr == 0; }
		size_t size() const { return _size - arr; }
		size_t capacity() const { return _capacity - arr; }
		size_t max_size() {
			if (ENVIRONMENT64) return (pow(2, 64) / sizeof(T) - 1);
			return (pow(2, 32) / sizeof(T) - 1);
		}
		void shrink_to_fit() {
			if (_size == _capacity) return;
			iterator c_arr = alloc.allocate(_size - arr);
			iterator c_size;
			iterator c_capacity;
			std::move(arr, _size, c_arr);
			c_size = c_capacity = c_arr + (_size - arr);
			clear();
			arr = c_arr;
			_size = c_size;
			_capacity = c_capacity;
		}
		// Modifiers
		void clear() {
			if (arr) {
				iterator it = _capacity;
				while (it != arr) alloc.destroy(--it);
				alloc.deallocate(arr, _size - arr);
				arr = _size = _capacity = nullptr;
			}
		}
		iterator insert(iterator it, const T & val) {
			if (it < arr) throw std::out_of_range("index is out of range");
			if (_size == _capacity) it = enlarge(2 * (_capacity - arr), it);
			_size += 1;
			std::move(it, _size, it + 1);
			*(it) = val;
			return it;
		}
		iterator insert(iterator it, const std::initializer_list<T> & il) {
			if (it < arr) throw std::out_of_range("index is out of range");
			size_t dist = il.size();
			if (_size + dist >= _capacity) it = enlarge(2 * (dist + (_capacity - arr)), it);
			_size += dist;
			std::move(it, _size - dist, it + dist);
			std::move(il.begin(), il.end(), it);
			return it;
		}
		template<typename ...Args>
		iterator emplace(iterator it, Args && ...args) {
			if (it < arr) throw std::out_of_range("index is out of range");
			if (_size == _capacity) it = enlarge(2 * (_capacity - arr), it);
			_size += 1;
			std::move(it, _size, it + 1);
			*(it) = std::move(T(std::forward<Args>(args) ...));
			return it;
		}
		template<typename ...Args>
		void emplace_back(Args && ...args) {
			if (_size == _capacity) enlarge(2 * (_capacity - arr));
			*(_size) = std::move(T(std::forward<Args>(args) ...));
			_size += 1;
		}
		template<typename ...Args>
		void emplace_front(Args && ...args) {
			if (_size == _capacity) enlarge(2 * (_capacity - arr));
			std::move(arr, _size, arr + 1);
			*(arr) = std::move(T(std::forward<Args>(args) ...));
			_size += 1;
		}
		void push_back(const T & val) {
			if (_size == _capacity) enlarge(2*(_capacity - arr));
			*(_size) = val;
			_size += 1;
		}
		void push_front(const T & val) {
			if (_size == _capacity) enlarge(2 * (_capacity - arr));
			std::move(arr, _size, arr + 1);
			*(arr) = val;
			_size += 1;
		}
		void erase(iterator i) {
			if (i < arr || i >= _size) throw std::out_of_range("index is out of range");
			std::move(i + 1, _size, i);
			_size -= 1;
		}
		void erase(iterator i, iterator j) {
			if (i > j || i < arr || j >= _size) throw std::out_of_range("bad index");
			std::move(j + 1, _size, i);
			_size -= (j - i) + 1;
		}
		void reserve(size_t n) {
			if (n <= _capacity - arr) return;

			iterator c_arr = alloc.allocate(n);
			iterator c_size;
			iterator c_capacity;
			iterator end;

			if (n < _size - arr) end = arr + n;
			else end = _size;

			std::move(arr, end, c_arr);
			c_size = c_arr + (end - arr);
			c_capacity = c_arr + n;

			clear();

			arr = c_arr;
			_size = c_size;
			_capacity = c_capacity;
		}
		void resize(size_t n) {
			if (n == 0) { clear(); return; }
			if (n == _capacity - arr) return;

			iterator c_arr = alloc.allocate(n);
			iterator c_size;
			iterator c_capacity;
			iterator end;

			if (n < _size - arr) end = arr + n;
			else end = _size;

			std::move(arr, end, c_arr);
			c_size = c_arr + (end - arr);
			c_capacity = c_arr + n;

			while (c_size != c_capacity) {
				*(c_size) = T();
				c_size += 1;
			}

			clear();

			arr = c_arr;
			_size = c_size;
			_capacity = c_capacity;
		}
		void swap(vector & v) {
			iterator c_arr = v.arr;
			iterator c_size = v._size;
			iterator c_capacity = v._capacity;
			v.arr = arr;
			v._size = _size;
			v._capacity = _capacity;
			arr = c_arr;
			_size = c_size;
			_capacity = c_capacity;
		}
		void pop_back() {
			_size -= 1;
			alloc.destroy(_size + 1);
		}
	private:
		iterator arr;
		iterator _size;
		iterator _capacity;

		iterator enlarge(size_t n, iterator it = 0) {
			if (n == 0) { clear(); return arr; }
			if (n == _capacity - arr) return arr;

			iterator c_arr = alloc.allocate(n);
			iterator c_size;
			iterator c_capacity;

			auto pos = it - arr;
			iterator end;

			if (n < _size - arr) {
				end = arr + n;
			}
			else end = _size;

			std::move(arr, end, c_arr);
			c_size = c_arr + (end - arr);
			c_capacity = c_arr + n;

			clear();

			arr = c_arr;
			_size = c_size;
			_capacity = c_capacity;

			return arr + pos;
		}
	};
};
