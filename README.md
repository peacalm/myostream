# MyOStream: Extension for std::basic_ostream in C++

[![Build](https://github.com/peacalm/myostream/actions/workflows/cmake.yml/badge.svg)](https://github.com/peacalm/myostream/actions)
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)

[[中文博文链接]](http://lishuangquan.cn/post/2022/myostream/)

This lib supports convenient output for all item-iterable container types and 
their composited types, and supports mutable preferences which specify the 
output format.

* C++ standard requirement: >= C++11

Supported container or container-like types:
`std::pair`, `std::tuple`,
`std::array`, `std::deque`, `std::forward_list`, `std::initializer_list`, `std::list`, `std::vector`, 
`std::set`, `std::multiset`, `std::unordered_set`, `std::unordered_multiset`, 
`std::map`, `std::multimap`, `std::unordered_map`, `std::unordered_multimap`. 

For output result string, this lib supports std::string, std::wstring, or any
string type specialized by template std::basic_string<CharT, Traits, Allc>.

## Introduction


### Struct: default_preferences<StringT, DenseStyle = false>
Preferences control output style.  
StringT: Some string type. e.g. std::string, std::wstring, or string type 
specialized by template std::basic_string<CharT, Traits, Allc>.  
DenseStyle: Bool parameter, use dense style or sparse style. Dense style means 
won't output spaces between items. For example, for `vector<int>{1,2,3}`,
if set DenseStyle=true, we get `[1,2,3]`, or we get `[1, 2, 3]` if DenseStyle=false.
Sparse style as default value.

What's more, user can modify each container's output style by this preferences
struct. For each container, there are 3 parameters to describe the style, they are
left-border, separator, right-border. For above example, default style for
`vector<int>{1,2,3}`, the left-border is `"["`, right-border is `"]"`, and 
separator is `", "` for sparse style or `","` for dense style.

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
What's more, there are useful pre-defined ostream types with default preferences:
* ostream  = basic_ostream\<std::ostream>
* wostream = basic_ostream\<std::wostream>
* ostringstream  = basic_ostringstream\<std::ostringstream>
* wostringstream = basic_ostringstream\<std::wostringstream>

and with dense style:
* ostream_dense = basic_ostream<std::ostream, default_preferences_by_ostream_base<std::ostream, true>>
* wostream_dense = basic_ostream<std::wostream, default_preferences_by_ostream_base<std::wostream, true>>
* ostringstream_dense = basic_ostringstream<std::ostringstream, default_preferences_by_ostream_base<std::ostringstream, true>>
* wostringstream_dense = basic_ostringstream<std::wostringstream, default_preferences_by_ostream_base<std::wostringstream, true>>

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

### Function: tostr function family

* template <typename... Args> tostr(const Args&... args)  
One line code to convert multiple arguments into std::string, separated by "".

* template <typename... Args> towstr(const Args&... args)  
All same as `myostream::tostr` except result into std::wstring.

* template <typename... Args> ptostr(const Args&... args)  
One line code to convert multiple arguments into std::string, separated by ", ".

* template <typename... Args> ptowstr(const Args&... args)  
All same as `myostream::ptostr` except result into std::wstring and separated 
by L", ".

* template <typename... Args> tostr_dense(const Args&... args)  
One line code to convert multiple arguments into std::string, separated by "".
Use preferences with dense style.

* template <typename... Args> towstr_dense(const Args&... args)  
All same as `myostream::tostr_dense` except result into std::wstring.
Use preferences with dense style.

* template <typename... Args> ptostr_dense(const Args&... args)  
One line code to convert multiple arguments into std::string, separated by ",".
Use preferences with dense style.

* template <typename... Args> ptowstr_dense(const Args&... args)  
All same as `myostream::ptostr_dense` except result into std::wstring and separated 
by L",".
Use preferences with dense style.


Example:
```c++
std::vector<int> vi{1, 2, 3};
std::cout << myostream::tostr("show \"tostr\": ", vi, ", ok?") << std::endl;
std::cout << myostream::tostr_dense("show \"tostr_dense\": ", vi, ", ok?") << std::endl;
```
Result:
```text
show "tostr": [1, 2, 3], ok?
show "tostr_dense": [1,2,3], ok?
```

Example:
```c++
std::vector<int> vi{1, 2, 3};
std::cout << myostream::ptostr("show \"ptostr\"", vi, 123) << std::endl;
```
Result:
```text
show "ptostr", [1, 2, 3], 123
```

### Macro Definition: watch family
* Macro: MYOSTREAM_WATCH(out_stream, kv_sep, param_sep, final_delim, ...)  
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

* Macro: MYOSTREAM_WATCH_TO_STRING(string_type, kv_sep, param_sep, final_delim, ...)  
Like `MYOSTREAM_WATCH`, but convert the result to string with type 
`string_type`.

Example:
```c++
#define watch(...) std::cout << MYOSTREAM_WATCH_TO_STRING(std::string, " = ", "\n", "\n\n", __VA_ARGS__)

int i = 123;
std::set<int> si{1,2,3};
std::cout << MYOSTREAM_WATCH_TO_STRING(std::string, ":", "\n", "\n----\n", i, si, 123);
std::wcout << MYOSTREAM_WATCH_TO_STRING(std::wstring, L" = ", "\n", "\n", i, si, 123);

double d = 1.4;
std::vector<int> vi(si.begin(), si.end());
watch(d, i, si, vi);

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
d = 1.4
i = 123
si = {1, 2, 3}
vi = [1, 2, 3]
```


#### Use watch to debug for ACMer/OIer

Since online judgers usually have a pre-defined macro `ONLINE_JUDGE`,
so we can only enable watch locally to output variables, and let it do nothing 
when the program is running on online judger.

```C++
#ifndef ONLINE_JUDGE
#include <myostream.h>
myostream::ostream mycout(std::cout.rdbuf());
#define watch(...) MYOSTREAM_WATCH(mycout, " = ", "\n", "\n\n", __VA_ARGS__)
#else
#define watch(...)
#endif

// then happy to use watch to debug arbitrarily...
```

## Install
Install the lib to your computer:  
```shell script
git clone https://github.com/peacalm/myostream.git
cd myostream
mkdir build
cd build
cmake ..
make install
```
Package name is "MyOStream" which can be used in cmake, e.g. `find_package(MyOStream REQUIRED)`.
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
