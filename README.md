# dqmis vector
This is my I hope that less retarded than the preious one c++ ```std::vector``` implementation. After seeing my lecturer's lecture about ``vector`` implementation using pointers without ``new`` and ``delete``, I challenged myself to try to implement one myself. I had a real fun figuring out the principles of standard ```std::vector``` and  at least trying to reverse-engineer and improve them. Below are listed all methods I managed to complete implementing.

## List of ``nr::vector`` methods
### Member types
```c++
std::allocator<T> alloc;
typedef T* iterator;
typedef const T* const_iterator;
typedef std::reverse_iterator<iterator> reverse_iterator;
typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
typedef size_t size_type;
typedef T value_type;
typedef T & reference;
typedef const T & const_reference;
```
### Member functions
```c++
explicit vector()
explicit vector(size_t n)
explicit vector(size_t n, const T & val)
explicit vector(const vector & v)
explicit vector(const_iterator i, const_iterator j)
explicit vector(std::initializer_list<T> il)
~vector()
vector & operator = (const vector & v)
void assign(size_t n, const T & val)
```
### Element access
```c++
reference operator[](int i);
const_reference operator[](int i);
```
### Iterators
```c++
iterator begin();
iterator end();
reverse_iterator rbegin();
reverse_iterator rend();
iterator& operator--();
```
### Capacity
```c++
bool empty();
size_t size() const;
size_t capacity() const;
size_t max_size();
void shrink_to_fit();
```
### Modifiers
```c++
void clear();
iterator insert(iterator it, const T & val);
iterator insert(iterator it, const std::initializer_list<T> & il);
void erase(iterator i);
void erase(iterator i, iterator j);
void push_back(const T & n);
void push_front(const T & n);
iterator resize(size_t n, iterator it = 0);
void swap(vector<T> & vec);
template<typename ...Args>
void emplace_back(Args && ...args);
template<typename ...Args>
void emplace_front(Args && ...args);
template<typename ...Args>
iterator emplace(iterator it, Args && ...args);
void pop_back();
```
### Non-member functions
```c++
friend std::ostream & operator << (std::ostream & out, dq::vector<T> & a);
```
## Benchmarking
Below were made tests to ensure how my ```dq::vector``` compares to ```std::vector```.  First benchmark shows performance of ```push_back()```. The test was made with code written below:  
```c++
int main() {
	Timer t;
	unsigned int sz = 1000000;
	std::vector<int> v1;
	dq::vector<int> v2;
	nr::vector<int> v3;

	t.start_clock();
	for (int i = 0; i < sz; i++)
		v1.push_back(i);
	t.end_clock("v1 push_back");

	t.start_clock();
	for (int i = 0; i < sz; i++)
		v2.push_back(i);
	t.end_clock("v2 push_back");

	t.start_clock();
	for (int i = 0; i < sz; i++)
		v3.push_back(i);
	t.end_clock("v3 push_back");

	return 0;
}
```
The results are:  

| Size     | std::vector | dq::vector  | nr::vector |
|----------|-------------|-------------|------------|
| 1000     | 0.0005786 s | 4.4e-05 s   | 7.84e-05   |
| 10 000   | 0.0037287 s | 0.0001593 s | 0.0006662  |
| 100 000  | 0.0314285 s | 0.0016748 s | 0.0087416  |
| 1000 000 | 0.275679 s  | 0.0166994 s | 0.060358   |

Second benchmark was make to see how my vector compares to how many times vectors have to resize to fit all the new data. The test was made by pushing back 1 000 000 elements.  
```C++
int main() {
	Timer t;
	unsigned int sz = 1000000;
	std::vector<int> v1;
	dq::vector<int> v2;
	nr::vector<int> v3;

	int szs = 0;
	int v1_add = 0;
	int v2_add = 0;
	int v3_add = 0;

	t.start_clock();
	for (int i = 0; i < sz; i++) {
		if (v1.capacity() != szs) {
			szs = v1.capacity();
			v1_add++;
		}
		v1.push_back(i);
	}
	t.end_clock("v1 push_back");
	
	szs = 0;

	t.start_clock();
	for (int i = 0; i < sz; i++) {
		if (v2.capacity() != szs) {
			szs = v2.capacity();
			v2_add++;
		}
		v2.push_back(i);
	}
	t.end_clock("v2 push_back");

	szs = 0;

	t.start_clock();
	for (int i = 0; i < sz; i++) {
		if (v3.capacity() != szs) {
			szs = v3.capacity();
			v3_add++;
		}
		v3.push_back(i);
	}
	t.end_clock("v3 push_back");

	cout << v1_add << " " << v2_add << " " << v3_add << endl;

	cin.get();
	return 0;
}
```
The results are: 

| std::vector | dq::vector | nr::vector |
|-------------|------------|------------|
| 35          | 20         | 20         |


