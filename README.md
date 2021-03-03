# extension for std::basic_ostream
support output for all container types.

example:
```c++
// output to stdout
myostream::my_ostream<std::ostream> mycout(std::cout.rdbuf());
std::vector<int> vi{1, 2, 3};
mycout << vi << std::endl;  // [1, 2, 3]

// convert to std::string
myostream::my_ostream<std::ostringstream> myoss;
std::vector<std::set<int>> vsi{{1, 2, 3}, {100, 200}};
myoss << vsi;
std::cout << myoss.str() << std::endl;  // [{1, 2, 3}, {100, 200}]

```