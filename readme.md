### PMR allocator example


//#define UTILS_MEM_OPERATORS_OVERLOADED
//#define UTILS_MEM_OUTPUT_ALLOCATION_PTR
//add_compile_definitions (UTILS_MEM_OUTPUT_ALLOCATION_PTR)
//add_compile_definitions WRONG_ITERATOR_COMPILE_FAILURE)
//add_compile_definitions WRONG_TYPE_UPCAST_COMPILE_FAILURE
//add_compile_definitions WRONG_TIMESTAMP_CAST_COMPILE_FAILURE
//add_compile_definitions WRONG_ELEMENT_CTOR_COMPILE_FAILURE
//add_compile_definitions WRONG_VALUE_CTOR_COMPILE_FAILURE


//write on the std::array helper in serie container 

//default container is inherited from std::pmr:: deque, guarantees data is sorted but not unique
see bench
//write on a stack size of different systems, as well as their thread size - see the notes 

//for Value<ValueType> comparison to double is defined
If you do upcast of the serie you'd better have upcast defined for a mapped_type

//list the defines
//list the global objects
- memuse
- float comp
- randomer

//pmr container
keep something on a stack
if thread locals, them mem volume is abut 460 Kb
if full stack then mem volume is about 8Mb
list is used for better multithreading

//input
problem of reading date
input is different - IB gives you an instance of C++ object, Coinbase - json, and C/T SV files something else
input should be implemented by lib user




## Zip in C++
### Reasoning
I like small useful snippets, like this one. Let myself have an entertaining night while coding this exercise.
Although this one is a bit advanced, compared to the others. Iterator's big five is properly defined in the class: value_type, reference, difference_type, pointer, iterator_category. So the other C++ code will consider this zip_iterator as an iterator as well.

### Usage
Pretty much straightforward, see the test file. Here is the most vivid example:
```c++
	std::vector<int> v{ 1,2,3,4,5 };
	std::map<int, std::string> m { {1, "one"s}, {2, "two"s}, {3, "three"s}, };
	std::string s { "abcdefghhlk" };
	
	/// Here it is, see the for loop expr
	for (auto const& [i, pair, ch] : itertools::zip(v, m, s)) {
		std::cout << i << ' ' << pair.first << ' ' << pair.second << ' ' << ch << '\n';
	}
```


### Standard and Compilers
Works like a charm in 17 and 20.
Is compiled by gcc, clang for Linux and something for Windows (thanks to Github Actions).

### Dependencies
Header only, STL only.

### Disclaimer
Feel free to use it for your needs at your own risk. No guarantees of any kind is given :)

### License
MIT License