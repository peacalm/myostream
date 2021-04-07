#include "main.h"

myostream::ostream  mycout(std::cout.rdbuf());
myostream::ostream  mycerr(std::cerr.rdbuf());
myostream::wostream mywcout(std::wcout.rdbuf());
myostream::wostream mywcerr(std::wcerr.rdbuf());

int main(int argc, char** argv) {
  mycout << "mycout:  testing myostream!\n";
  mycerr << "mycerr:  testing myostream!\n";
  mywcout << "mywcout: testing myostream!\n";
  mywcerr << "mywcerr: testing myostream!\n";
  std::cout << std::endl;

  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}