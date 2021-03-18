# Extension for std::basic_ostream in C++
This lib supports convenient output for all item-iterable container types, and 
their composited types.

* C++ standard requirement: >= C++11

usages:
## myostream::basic_ostream<OstreamBaseType, FmtParamsType=fmt_params>

You need to put at least an `OstreamBaseType` into the first template parameter 
as a base class, e.g. `myostream::basic_ostream<std::ostringstream>`, which 
indicates where the output writes to. 
And you can put an optional second template parameter `FmtParamsType` 
to specify your preferred left-border, right-border and separator for each 
container type, or just use the default value.

What's more, there are useful pre-defined ostream types with default format:
* ostream  = basic_ostream\<std::ostream>
* wostream = basic_ostream\<std::wostream>
* ostringstream  = basic_ostream\<std::ostringstream>
* wostringstream = basic_ostream\<std::wostringstream>

example:
```c++
// output to stdout
myostream::ostream mycout(std::cout.rdbuf());
std::vector<int> vi{1, 2, 3};
mycout << vi << std::endl;  // [1, 2, 3]
std::map<std::string, std::vector<double>> msvd{
    {"Alice", {95, 87.5}}, {"Bob", {79, 99.5}}};
mycout << msvd << std::endl;  // {Alice: [95, 87.5], Bob: [79, 99.5]}

// convert to std::string
myostream::ostringstream myoss;
std::vector<std::set<int>> vsi{{1, 2, 3}, {100, 200}};
myoss << vsi;
std::cout << myoss.str() << std::endl;  // [{1, 2, 3}, {100, 200}]

```

## myostream::tostr(const Args&... args)
one line code to convert multiple arguments into std::string, seperated by ", ".

example:
```c++
std::vector<int> vi{1, 2, 3};
std::cout << myostream::tostr("show \"tostr\"", vi) << std::endl;
// show "tostr", [1, 2, 3]
```

## myostream::towstr(const Args&... args)
all same as `myostream::tostr` except result into std::wstring type.
