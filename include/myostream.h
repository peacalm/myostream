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

template <typename StringT>
struct fmt_params;

}  // namespace internal

template <typename OstreamBaseT,
          typename FmtParamsT = internal::fmt_params<std::basic_string<
              typename OstreamBaseT::char_type,
              typename OstreamBaseT::traits_type>>>
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

template <typename BasicStringT, typename T,
          typename FmtParamsT = internal::fmt_params<BasicStringT>>
BasicStringT to_basic_string(const T& t);

template <typename StringT = std::string,
          typename FmtParamsT = internal::fmt_params<StringT>>
struct printer;

template <typename T, typename FmtParamsT = internal::fmt_params<std::string>>
std::string tostr(const T& t);

template <typename T, typename FmtParamsT = internal::fmt_params<std::wstring>>
std::wstring towstr(const T& t);

// definitions

namespace internal {

template <typename StringT> struct fmt_param_unit {
  using string_type = StringT;
  using char_type   = typename string_type::value_type;

  fmt_param_unit(const string_type& left_border,
                 const string_type& separator,
                 const string_type& right_border)
      : lb(left_border), sep(separator), rb(right_border) {}

  string_type lb, sep, rb;
};

template <typename StringT> struct fmt_params {
  using string_type         = StringT;
  using char_type           = typename string_type::value_type;
  using fmt_param_unit_type = fmt_param_unit<string_type>;

  fmt_param_unit<string_type>
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

template <typename OstreamT, typename IteratorT, typename FmtParamUnitT>
OstreamT& output_all(OstreamT& os, IteratorT b, IteratorT e,
                     const FmtParamUnitT& f) {
  os << f.lb;
  for (IteratorT it = b; it != e; ++it) {
    if (it != b) os << f.sep;
    os << *it;
  }
  os << f.rb;
  return os;
}

template <typename OstreamT, typename IteratorT, typename FmtParamUnitT>
OstreamT& output_all(OstreamT& os, IteratorT b, IteratorT e,
                     const FmtParamUnitT& f, const FmtParamUnitT& kv_f) {
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


template <typename BasicStringT, typename T, typename FmtParamsT>
BasicStringT to_basic_string(const T& t) {
  using base_oss_t = std::basic_ostringstream<
      typename BasicStringT::value_type,
      typename BasicStringT::traits_type,
      typename BasicStringT::allocator_type>;
  ostream<base_oss_t, FmtParamsT> oss;
  oss << t;
  return oss.str();
}

template <typename StringT, typename FmtParamsT>
struct printer {
  using string_type     = StringT;
  using fmt_params_type = FmtParamsT;

  printer() {}
  printer(const string_type& lb, const string_type& sep, const string_type& rb)
    : lb_(lb), sep_(sep), rb_(rb) {}

  printer& with_lb (const string_type& lb)  {lb_  = lb;  return *this;}
  printer& with_sep(const string_type& sep) {sep_ = sep; return *this;}
  printer& with_rb (const string_type& rb)  {rb_  = rb;  return *this;}

  template <typename ...Args>
  typename std::enable_if<sizeof...(Args) != 0, string_type>::type
  print(const Args&... args) const {
    return lb_ + __print(args...) + rb_;
  }

  string_type print() const { return lb_ + rb_; }

private:
  template <typename T>
  string_type
  __print(const T& t) const {
    return to_basic_string<string_type, T, fmt_params_type>(t);
  }

  template <typename T, typename ...Args>
  typename std::enable_if<sizeof...(Args) != 0, string_type>::type
  __print(const T& t, const Args&... args) const {
    return __print(t) + sep_ + __print(args...);
  }

  string_type lb_;
  string_type sep_{',', ' '};
  string_type rb_;
};

template <typename T, typename FmtParamsT>
std::string tostr(const T& t) {
  ostream<std::ostringstream, FmtParamsT> oss;
  oss << t;
  return oss.str();
}

template <typename T, typename FmtParamsT>
std::wstring towstr(const T& t) {
  return to_basic_string<std::wstring, T, FmtParamsT>(t);
}

}  // namespace myostream
