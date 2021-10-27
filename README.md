# MyOStream: Extension for std::basic_ostream in C++
This lib supports convenient output for all item-iterable container types and 
their composited types, and supports mutable preferences which specify the 
output format.

* C++ standard requirement: >= C++11

Supported container or container-like types:
* std::pair
* std::tuple

* std::array
* std::deque
* std::forward_list
* std::initializer_list
* std::list
* std::vector

* std::set
* std::multiset
* std::unordered_set
* std::unordered_multiset

* std::map
* std::multimap
* std::unordered_map
* std::unordered_multimap


## Introduction

### Class: myostream::basic_ostream<OstreamBaseT, PreferencesT=default_preferences>
You need to put at least an `OstreamBaseT` into the first template parameter 
as a base class, e.g. `myostream::basic_ostream<std::ostream>` or 
`myostream::basic_ostream<std::wostringstream>`, which indicates where the 
output writes to. 
And you can put an optional second template parameter `PreferencesT` 
to specify your preferred left-border, right-border and separator for each 
container type, or just use the default preferences.

### Class: myostream::basic_ostringstream<OstreamBaseT, PreferencesT=default_preferences>
This class `myostream::basic_ostringstream` is derived from 
`myostream::basic_ostream`, and has the same template parameters, a required 
`OstreamBaseT` and an optional `PreferencesT`. But here the `OstreamBaseT` 
should be a "stringstream type", which has a member method `str()` to get a 
string representing the output result. e.g. 
`myostream::basic_ostringstream<std::ostringstream>` is valid, but 
`myostream::basic_ostringstream<std::ostream>` will get a compile error.

### Pre-defined convenient types
What's more, there are useful pre-defined ostream types with 
default preferences:
* ostream  = basic_ostream\<std::ostream>
* wostream = basic_ostream\<std::wostream>
* ostringstream  = basic_ostringstream\<std::ostringstream>
* wostringstream = basic_ostringstream\<std::wostringstream>

### Details
This lib overloads `operator<<` for the class `basic_ostream` with each 
supported container type, the declaration pseudocode looks like(the key word
`SomeSupportedContainerType` in the following code actually might be 
`std::array`, `std::vector`, etc.):
```C++
template <typename OstreamBaseT, typename PreferencesT, typename... Args>
basic_ostream<OstreamBaseT, PreferencesT>& operator<<(
    basic_ostream<OstreamBaseT, PreferencesT>&,
    const SomeSupportedContainerType<Args...>&);
```
And the class `basic_ostream` has more useful member functions to print multi 
arguments, or a range of values designated by a pair of iterators:
```c++
// print all args in some format, e.g. on default, with a separator ", "
template <typename... Args>
basic_ostream& print(const Args&... args);

// print all args in specified format
template <typename... Args>
basic_ostream& print(const format_type& fmt, const Args&... args)

// all same with "print" except put a std::endl at last
template <typename... Args>
basic_ostream& println(const Args&... args);

// support specified format
template <typename... Args>
basic_ostream& println(const format_type& fmt, const Args&... args)

// print values in range [begin, end) in some format, e.g. on default, with a 
// separator ", "
template <typename Iterator>
basic_ostream& print_range(Iterator begin, Iterator end);

// print values in range [begin, end) in format the third parameter 
// "range_fmt" specified
template <typename Iterator>
basic_ostream& print_range(Iterator begin, Iterator end,
                           const format_type& range_fmt);
```

The class `basic_ostringstream` is derived from `basic_ostream` by public, so 
it has all member functions of the latter as mentioned above. Moreover, the 
former has unique member functions:
```c++
// print args into a std::vector<string_type>
template <typename... Args>
string_vector_type to_string_vector(const Args&... args);

// clear the string buffer which stores all outputs beforehand
void clear_buf();
```

Example:
```c++
// output to stdout
myostream::ostream mycout(std::cout.rdbuf());
std::vector<int> vi{1, 2, 3};
mycout << vi << std::endl;
std::map<std::string, std::vector<double>> msvd{
    {"Alice", {95, 87.5}}, {"Bob", {79, 99.5}}};
mycout << msvd << std::endl;

// convert to std::string
myostream::ostringstream myoss;
std::vector<std::set<int>> vsi{{1, 2, 3}, {100, 200}};
myoss << vsi;
std::cout << myoss.str() << std::endl;

// use print, println
mycout.print("hello ").println("world", 123, vi);
```
Result:
```text
[1, 2, 3]
{Alice: [95, 87.5], Bob: [79, 99.5]}
[{1, 2, 3}, {100, 200}]
hello world, 123, [1, 2, 3]
```

### Function: myostream::ptostr(const Args&... args)
One line code to convert multiple arguments into std::string, separated by ", ".

Example:
```c++
std::vector<int> vi{1, 2, 3};
std::cout << myostream::ptostr("show \"ptostr\"", vi) << std::endl;
```
Result:
```text
show "ptostr", [1, 2, 3]
```

### Function: myostream::ptowstr(const Args&... args)
All same as `myostream::ptostr` except result into std::wstring and separated 
by L", ".

### Function: myostream::tostr(const Args&... args)
Like `myostream::ptostr` but separated by empty string "".

### Function: myostream::towstr(const Args&... args)
Like `myostream::ptowstr` but separated by empty wstring L"".

### Macro: MYOSTREAM_WATCH(out_stream, kv_sep, param_sep, final_delim, ...)
Print all variables in parameter `...` along with their names to `out_stream` 
in format "var1-name kv_sep var1-value param_sep var2-name kv_sep var2-value 
param_sep ... final_delim".

Example:
```c++
myostream::ostream mycout(std::cout.rdbuf());
#define watch(...) MYOSTREAM_WATCH(mycout, " = ", "\n", "\n\n", __VA_ARGS__)

int i = 123;
std::set<int> si{1,2,3};
watch(i, 123, si, std::set<int>{1,2,3}, "strings");
```
Result:
```text
i = 123
123 = 123
si = {1, 2, 3}
std::set<int>{1,2,3} = {1, 2, 3}
"strings" = strings
```

### Macro: MYOSTREAM_WATCH_TO_STRING(string_type, kv_sep, param_sep, final_delim, ...)
Like `MYOSTREAM_WATCH`, but convert the result to string with type 
`string_type`.

Example:
```c++
int i = 123;
std::set<int> si{1,2,3};
std::cout << MYOSTREAM_WATCH_TO_STRING(std::string, ':', '\n', "\n----\n", i, si, 123);
std::wcout << MYOSTREAM_WATCH_TO_STRING(std::wstring, L" = ", '\n', '\n', i, si, 123);
```
Result:
```text
i:123
si:{1, 2, 3}
123:123
----
i = 123
si = {1, 2, 3}
123 = 123
```

## Install
Install the lib to your computer:  
```shell script
git clone https://github.com/TheLonelyHeracles/myostream.git
cd myostream
mkdir build
cd build
cmake ..
make install
```
Then happy coding with `#include <myostream.h>`.

## Test
First, you need to install [GoogleTest](https://github.com/google/googletest).
Then enter the "build" directory like above, run:
```shell script
# suppose now at directory myostream/build, then run:
cmake .. -DBUILD_TEST=TRUE
make
make test  # or ctest
```
