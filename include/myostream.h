// Copyright © 2021 Shuangquan Li. All Rights Reserved.
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

#pragma once

#include <array>
#include <deque>
#include <forward_list>
#include <initializer_list>
#include <list>
#include <map>
#include <ostream>
#include <set>
#include <sstream>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace myostream {

// declarations

namespace internal {

template <typename CharT>
struct fmt_params;

}  // namespace internal

template <typename OstreamBaseT, typename FmtParamsT =
    internal::fmt_params<typename OstreamBaseT::char_type>>
class ostream;

template <typename OstreamBaseT, typename FmtParamsT,
          typename FirstT, typename SecondT>
ostream<OstreamBaseT, FmtParamsT>& operator<<(
    ostream<OstreamBaseT, FmtParamsT>& os,
    const std::pair<FirstT, SecondT>& p);

template <typename OstreamBaseT, typename FmtParamsT>
ostream<OstreamBaseT, FmtParamsT>& operator<<(
    ostream<OstreamBaseT, FmtParamsT>& os, const std::tuple<>& t);

template <typename OstreamBaseT, typename FmtParamsT, typename ...Args>
ostream<OstreamBaseT, FmtParamsT>& operator<<(
    ostream<OstreamBaseT, FmtParamsT>& os, const std::tuple<Args...>& t);

#define DECLARE_MY_OSTREAM(container)                                   \
template <typename OstreamBaseT, typename FmtParamsT, typename ...Args> \
ostream<OstreamBaseT, FmtParamsT>& operator<<(                          \
    ostream<OstreamBaseT, FmtParamsT>& os,                              \
    const std::container<Args...>& c)

DECLARE_MY_OSTREAM(array);
DECLARE_MY_OSTREAM(deque);
DECLARE_MY_OSTREAM(forward_list);
DECLARE_MY_OSTREAM(initializer_list);
DECLARE_MY_OSTREAM(list);
DECLARE_MY_OSTREAM(vector);

DECLARE_MY_OSTREAM(set);
DECLARE_MY_OSTREAM(multiset);
DECLARE_MY_OSTREAM(unordered_set);
DECLARE_MY_OSTREAM(unordered_multiset);

DECLARE_MY_OSTREAM(map);
DECLARE_MY_OSTREAM(multimap);
DECLARE_MY_OSTREAM(unordered_map);
DECLARE_MY_OSTREAM(unordered_multimap);

template <typename T, typename FmtParamsT = internal::fmt_params<char>>
std::string tostr(const T& t);

// definitions

namespace internal {

template <typename CharT> struct fmt_param_unit {
  using char_type   = CharT;
  using string_type = std::basic_string<char_type>;

  fmt_param_unit(const string_type& left_border,
                 const string_type& separator,
                 const string_type& right_border)
      : lb(left_border), sep(separator), rb(right_border) {}

  string_type lb, sep, rb;
};

template <typename CharT> struct fmt_params {
  using char_type   = CharT;
  using string_type = std::basic_string<char_type>;

  fmt_param_unit<char_type>
                   pair_fmt{{'('}, {',', ' '}, {')'}},
                  tuple_fmt{{'<'}, {',', ' '}, {'>'}},

                  array_fmt{{'['}, {',', ' '}, {']'}},
                  deque_fmt{{'['}, {',', ' '}, {']'}},
           forward_list_fmt{{'['}, {',', ' '}, {']'}},
       initializer_list_fmt{{'['}, {',', ' '}, {']'}},
                   list_fmt{{'['}, {',', ' '}, {']'}},
                 vector_fmt{{'['}, {',', ' '}, {']'}},

                    set_fmt{{'{'}, {',', ' '}, {'}'}},
               multiset_fmt{{'{'}, {',', ' '}, {'}'}},
          unordered_set_fmt{{'{'}, {',', ' '}, {'}'}},
     unordered_multiset_fmt{{'{'}, {',', ' '}, {'}'}},

                    map_fmt{{'{'}, {',', ' '}, {'}'}},
                 map_kv_fmt{{},    {':', ' '}, {}   },
               multimap_fmt{{'{'}, {',', ' '}, {'}'}},
            multimap_kv_fmt{{},    {':', ' '}, {}   },
          unordered_map_fmt{{'{'}, {',', ' '}, {'}'}},
       unordered_map_kv_fmt{{},    {':', ' '}, {}   },
     unordered_multimap_fmt{{'{'}, {',', ' '}, {'}'}},
  unordered_multimap_kv_fmt{{},    {':', ' '}, {}   };
};

template <typename OstreamT, typename IteratorT>
OstreamT& output_all(OstreamT& os, IteratorT b, IteratorT e,
                     const fmt_param_unit<typename OstreamT::char_type>& f) {
  os << f.lb;
  for (IteratorT it = b; it != e; ++it) {
    if (it != b) os << f.sep;
    os << *it;
  }
  os << f.rb;
  return os;
}

template <typename OstreamT, typename IteratorT>
OstreamT& output_all(OstreamT& os, IteratorT b, IteratorT e,
                     const fmt_param_unit<typename OstreamT::char_type>& f,
                     const fmt_param_unit<typename OstreamT::char_type>& kv_f) {
  os << f.lb;
  for (IteratorT it = b; it != e; ++it) {
    if (it != b) os << f.sep;
    os << kv_f.lb;
    os << it->first;
    os << kv_f.sep;
    os << it->second;
    os << kv_f.rb;
  }
  os << f.rb;
  return os;
}

template <typename OstreamT, typename TupleT, size_t N>
struct tuple_printer {
  static void print(OstreamT& os, const TupleT& t) {
    tuple_printer<OstreamT, TupleT, N - 1>::print(os, t);
    os << os.fmt.tuple_fmt.sep;
    os << std::get<N - 1>(t);
  }
};

template <typename OstreamT, typename TupleT>
struct tuple_printer<OstreamT, TupleT, 1> {
  static void print(OstreamT& os, const TupleT& t) {
    os << std::get<0>(t);
  }
};

}  // namespace internal

template <typename OstreamBaseT, typename FmtParamsT>
class ostream : public OstreamBaseT {
  using base_type       = OstreamBaseT;
  using fmt_params_type = FmtParamsT;

public:
  template <typename ...Args>
  explicit ostream(Args&&... args) : base_type(std::forward<Args>(args)...) {}

  fmt_params_type fmt;
};


template <typename OstreamBaseT, typename FmtParamsT,
    typename FirstT, typename SecondT>
ostream<OstreamBaseT, FmtParamsT>& operator<<(
    ostream<OstreamBaseT, FmtParamsT>& os,
    const std::pair<FirstT, SecondT>& p) {
  os << os.fmt.pair_fmt.lb;
  os << p.first;
  os << os.fmt.pair_fmt.sep;
  os << p.second;
  os << os.fmt.pair_fmt.rb;
  return os;
}

template <typename OstreamBaseT, typename FmtParamsT>
ostream<OstreamBaseT, FmtParamsT>& operator<<(
    ostream<OstreamBaseT, FmtParamsT>& os, const std::tuple<>& t) {
  os << os.fmt.tuple_fmt.lb;
  os << os.fmt.tuple_fmt.rb;
  return os;
}

template <typename OstreamBaseT, typename FmtParamsT, typename ...Args>
ostream<OstreamBaseT, FmtParamsT>& operator<<(
    ostream<OstreamBaseT, FmtParamsT>& os, const std::tuple<Args...>& t) {
  os << os.fmt.tuple_fmt.lb;
  internal::tuple_printer<
      ostream<OstreamBaseT, FmtParamsT>, std::tuple<Args...>, sizeof...(Args)
  >::print(os, t);
  os << os.fmt.tuple_fmt.rb;
  return os;
}

#define DEFINE_MY_OSTREAM(container)                      \
DECLARE_MY_OSTREAM(container) {                           \
  return internal::output_all(os, c.cbegin(), c.cend(),   \
                              os.fmt.container##_fmt);    \
}

#define DEFINE_MY_OSTREAM_FOR_MAP(container)              \
DECLARE_MY_OSTREAM(container) {                           \
  return internal::output_all(os, c.cbegin(), c.cend(),   \
                              os.fmt.container##_fmt,     \
                              os.fmt.container##_kv_fmt); \
}

DEFINE_MY_OSTREAM(array)
DEFINE_MY_OSTREAM(deque)
DEFINE_MY_OSTREAM(forward_list)
DEFINE_MY_OSTREAM(initializer_list)
DEFINE_MY_OSTREAM(list)
DEFINE_MY_OSTREAM(vector)

DEFINE_MY_OSTREAM(set)
DEFINE_MY_OSTREAM(multiset)
DEFINE_MY_OSTREAM(unordered_set)
DEFINE_MY_OSTREAM(unordered_multiset)

DEFINE_MY_OSTREAM_FOR_MAP(map)
DEFINE_MY_OSTREAM_FOR_MAP(multimap)
DEFINE_MY_OSTREAM_FOR_MAP(unordered_map)
DEFINE_MY_OSTREAM_FOR_MAP(unordered_multimap)

#undef DEFINE_MY_OSTREAM_FOR_MAP
#undef DEFINE_MY_OSTREAM
#undef DECLARE_MY_OSTREAM

template <typename T, typename FmtParamsT>
std::string tostr(const T& t) {
  ostream<std::ostringstream, FmtParamsT> oss;
  oss << t;
  return oss.str();
}

}  // namespace myostream
