# extension for std::basic_ostream in C++
This lib supports output for all container types, and their composited types.
You need put a ostream type T as a template parameter, like
 `myostream::my_ostream<T>`, which indicates where the output writes to.
What's more, you can specify your preferred left-border, right-border and 
separator for each container type, or just use the default value.

* C++ standard requirement: >= C++11

example:
```c++
// output to stdout
myostream::my_ostream<std::ostream> mycout(std::cout.rdbuf());
std::vector<int> vi{1, 2, 3};
mycout << vi << std::endl;  // [1, 2, 3]
std::map<std::string, std::vector<double>> msvd{
    {"Alice", {95, 87.5}}, {"Bob", {79, 99.5}}};
mycout << msvd << std::endl;  // {Alice: [95, 87.5], Bob: [79, 99.5]}

// convert to std::string
myostream::my_ostream<std::ostringstream> myoss;
std::vector<std::set<int>> vsi{{1, 2, 3}, {100, 200}};
myoss << vsi;
std::cout << myoss.str() << std::endl;  // [{1, 2, 3}, {100, 200}]

```