// Copyright (c) 2021 Shuangquan Li. All Rights Reserved.
//
// Licensed under the MIT License (the "License"); you may not use this file
// except in compliance with the License. You may obtain a copy of the License
// at
//
//   http://opensource.org/licenses/MIT
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
// WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
// License for the specific language governing permissions and limitations under
// the License.
// =============================================================================

#include "main.h"

using namespace myostream;

TEST(Tostr, Containers) {
  EXPECT_TRUE(tostr(std::make_pair(1, 2)) == "(1, 2)");
  EXPECT_TRUE(tostr(std::make_tuple()) == "<>");
  EXPECT_TRUE(tostr(std::make_tuple(1, 2.5, "str")) == "<1, 2.5, str>");

  EXPECT_TRUE(tostr(std::array<int, 3>{1, 2, 3}) == "[1, 2, 3]");
  EXPECT_TRUE(tostr(std::deque<int>{1, 2, 3}) == "[1, 2, 3]");
  EXPECT_TRUE(tostr(std::forward_list<int>{1, 2, 3}) == "[1, 2, 3]");
  EXPECT_TRUE(tostr(std::list<int>{1, 2, 3}) == "[1, 2, 3]");
  EXPECT_TRUE(tostr(std::initializer_list<int>{1, 2, 3}) == "[1, 2, 3]");
  EXPECT_TRUE(tostr(std::vector<int>{1, 2, 3}) == "[1, 2, 3]");

  EXPECT_TRUE(tostr(std::set<int>{1, 2, 3}) == "{1, 2, 3}");
  EXPECT_TRUE(tostr(std::multiset<int>{1, 2, 2}) == "{1, 2, 2}");
  EXPECT_TRUE(tostr(std::unordered_set<int>{1}) == "{1}");
  EXPECT_TRUE(tostr(std::unordered_multiset<int>{1, 1, 1}) == "{1, 1, 1}");

  EXPECT_TRUE(tostr(std::map<int, int>{{1, 1}, {2, 2}}) == "{1: 1, 2: 2}");
  EXPECT_TRUE(tostr(std::unordered_map<int, int>{{1, 1}}) == "{1: 1}");

  EXPECT_TRUE(tostr(std::multimap<int, int>{{1, 1}, {1, 1}}) == "{1: 1, 1: 1}");
  EXPECT_TRUE(tostr(std::unordered_multimap<int, int>{{1, 1}, {1, 1}}) ==
              "{1: 1, 1: 1}");
}

TEST(TostrDense, Containers) {
  EXPECT_TRUE(tostr_dense(std::make_pair(1, 2)) == "(1,2)");
  EXPECT_TRUE(tostr_dense(std::make_tuple()) == "<>");
  EXPECT_TRUE(tostr_dense(std::make_tuple(1, 2.5, "str")) == "<1,2.5,str>");

  EXPECT_TRUE(tostr_dense(std::array<int, 3>{1, 2, 3}) == "[1,2,3]");
  EXPECT_TRUE(tostr_dense(std::deque<int>{1, 2, 3}) == "[1,2,3]");
  EXPECT_TRUE(tostr_dense(std::forward_list<int>{1, 2, 3}) == "[1,2,3]");
  EXPECT_TRUE(tostr_dense(std::list<int>{1, 2, 3}) == "[1,2,3]");
  EXPECT_TRUE(tostr_dense(std::initializer_list<int>{1, 2, 3}) == "[1,2,3]");
  EXPECT_TRUE(tostr_dense(std::vector<int>{1, 2, 3}) == "[1,2,3]");

  EXPECT_TRUE(tostr_dense(std::set<int>{1, 2, 3}) == "{1,2,3}");
  EXPECT_TRUE(tostr_dense(std::multiset<int>{1, 2, 2}) == "{1,2,2}");
  EXPECT_TRUE(tostr_dense(std::unordered_set<int>{1}) == "{1}");
  EXPECT_TRUE(tostr_dense(std::unordered_multiset<int>{1, 1, 1}) == "{1,1,1}");

  EXPECT_TRUE(tostr_dense(std::map<int, int>{{1, 1}, {2, 2}}) == "{1:1,2:2}");
  EXPECT_TRUE(tostr_dense(std::unordered_map<int, int>{{1, 1}}) == "{1:1}");

  EXPECT_TRUE(tostr_dense(std::multimap<int, int>{{1, 1}, {1, 1}}) ==
              "{1:1,1:1}");
  EXPECT_TRUE(tostr_dense(std::unordered_multimap<int, int>{{1, 1}, {1, 1}}) ==
              "{1:1,1:1}");
}

TEST(Tostr, Multi) {
  EXPECT_TRUE(tostr(1, 2, 3) == "123");
  EXPECT_TRUE(ptostr(1, 2, 3) == "1, 2, 3");
  EXPECT_TRUE(tostr(std::make_pair(1, 2), std::make_tuple()) == "(1, 2)<>");
  EXPECT_TRUE(ptostr(std::make_pair(1, 2), std::make_tuple()) == "(1, 2), <>");
  EXPECT_TRUE(
      tostr(1, ";", 2, ";", std::vector<std::string>{"aa", "bb", "cc"}, "dd") ==
      "1;2;[aa, bb, cc]dd");
  EXPECT_TRUE(
      ptostr(
          1, ";", 2, ";", std::vector<std::string>{"aa", "bb", "cc"}, "dd") ==
      "1, ;, 2, ;, [aa, bb, cc], dd");
}

TEST(TostrDense, Multi) {
  EXPECT_TRUE(tostr_dense(1, 2, 3) == "123");
  EXPECT_TRUE(ptostr_dense(1, 2, 3) == "1,2,3");
  EXPECT_TRUE(tostr_dense(std::make_pair(1, 2), std::make_tuple()) ==
              "(1,2)<>");
  EXPECT_TRUE(ptostr_dense(std::make_pair(1, 2), std::make_tuple()) ==
              "(1,2),<>");
  EXPECT_TRUE(
      tostr_dense(
          1, ";", 2, ";", std::vector<std::string>{"aa", "bb", "cc"}, "dd") ==
      "1;2;[aa,bb,cc]dd");
  EXPECT_TRUE(
      ptostr_dense(
          1, ";", 2, ";", std::vector<std::string>{"aa", "bb", "cc"}, "dd") ==
      "1,;,2,;,[aa,bb,cc],dd");
}

TEST(Towstr, Containers) {
  EXPECT_TRUE(towstr(std::make_pair(1, 2)) == L"(1, 2)");
  EXPECT_TRUE(towstr(std::make_tuple()) == L"<>");
  EXPECT_TRUE(towstr(std::make_tuple(1, 2.5, "str")) == L"<1, 2.5, str>");

  EXPECT_TRUE(towstr(std::array<int, 3>{1, 2, 3}) == L"[1, 2, 3]");
  EXPECT_TRUE(towstr(std::deque<int>{1, 2, 3}) == L"[1, 2, 3]");
  EXPECT_TRUE(towstr(std::forward_list<int>{1, 2, 3}) == L"[1, 2, 3]");
  EXPECT_TRUE(towstr(std::list<int>{1, 2, 3}) == L"[1, 2, 3]");
  EXPECT_TRUE(towstr(std::initializer_list<int>{1, 2, 3}) == L"[1, 2, 3]");
  EXPECT_TRUE(towstr(std::vector<int>{1, 2, 3}) == L"[1, 2, 3]");

  EXPECT_TRUE(towstr(std::set<int>{1, 2, 3}) == L"{1, 2, 3}");
  EXPECT_TRUE(towstr(std::multiset<int>{1, 2, 2}) == L"{1, 2, 2}");
  EXPECT_TRUE(towstr(std::unordered_set<int>{1}) == L"{1}");
  EXPECT_TRUE(towstr(std::unordered_multiset<int>{1, 1, 1}) == L"{1, 1, 1}");

  EXPECT_TRUE(towstr(std::map<int, int>{{1, 1}, {2, 2}}) == L"{1: 1, 2: 2}");
  EXPECT_TRUE(towstr(std::unordered_map<int, int>{{1, 1}}) == L"{1: 1}");

  EXPECT_TRUE(towstr(std::multimap<int, int>{{1, 1}, {1, 1}}) ==
              L"{1: 1, 1: 1}");
  EXPECT_TRUE(towstr(std::unordered_multimap<int, int>{{1, 1}, {1, 1}}) ==
              L"{1: 1, 1: 1}");
}

TEST(Tostr, Complex) {
  std::vector<std::set<int>> vs{{1, 2}, {3, 4}};
  EXPECT_TRUE(tostr(vs) == "[{1, 2}, {3, 4}]");
  EXPECT_TRUE(tostr_dense(vs) == "[{1,2},{3,4}]");

  std::map<int, std::set<int>> mis{{1, {1, 11}}, {2, {2, 22}}};
  EXPECT_TRUE(tostr(mis) == "{1: {1, 11}, 2: {2, 22}}");
  EXPECT_TRUE(tostr_dense(mis) == "{1:{1,11},2:{2,22}}");

  myostream::ostringstream       oss;
  myostream::ostringstream_dense oss_dense;
  EXPECT_TRUE(oss.print_range(mis.begin(), mis.end()).str() ==
              "(1, {1, 11}), (2, {2, 22})");
  EXPECT_TRUE(oss_dense.print_range(mis.begin(), mis.end()).str() ==
              "(1,{1,11}),(2,{2,22})");

  oss.clear_buf();
  EXPECT_TRUE(oss.print(vs[0]).str() == "{1, 2}");

  oss.clear_buf();
  oss_dense.clear_buf();
  mis[1].insert(111);
  EXPECT_TRUE(oss.println(*mis.begin()).str() == "(1, {1, 11, 111})\n");
  EXPECT_TRUE(oss_dense.println(*mis.begin()).str() == "(1,{1,11,111})\n");
}

TEST(Watch, Complex) {
  std::vector<std::set<int>>   vs{{1, 2}, {3, 4}};
  std::map<int, std::set<int>> mis{{1, {1, 11}}, {2, {2, 22}}};
  watch(std::vector<std::set<int>>{{1, 2}, {3, 4}},
        std::map<int, std::set<int>>{{1, {1, 11}}, {2, {2, 22}}});
  watch(vs, mis);

  EXPECT_TRUE(MYOSTREAM_WATCH_TO_STRING(std::string, "=", ", ", "", vs, mis) ==
              "vs=[{1, 2}, {3, 4}], mis={1: {1, 11}, 2: {2, 22}}");

  myostream::ostringstream oss;
  EXPECT_TRUE(MYOSTREAM_WATCH(oss, ": ", "; ", ";", vs, mis).str() ==
              "vs: [{1, 2}, {3, 4}]; mis: {1: {1, 11}, 2: {2, 22}};");

  myostream::ostringstream_dense oss_dense;
  EXPECT_TRUE(MYOSTREAM_WATCH(oss_dense, ": ", "; ", ";", vs, mis).str() ==
              "vs: [{1,2},{3,4}]; mis: {1:{1,11},2:{2,22}};");
}
