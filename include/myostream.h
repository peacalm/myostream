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

// ==================== declarations ====================

// types

namespace internal {

template <typename StringT>
struct fmt_params;

}  // namespace internal

template <
    typename OstreamBaseT,
    typename FmtParamsT = internal::fmt_params<std::basic_string<
        typename OstreamBaseT::char_type, typename OstreamBaseT::traits_type>>>
class basic_ostream;

using ostream        = basic_ostream<std::ostream>;
using wostream       = basic_ostream<std::wostream>;
using ostringstream  = basic_ostream<std::ostringstream>;
using wostringstream = basic_ostream<std::wostringstream>;

template <typename StringT>
using std_basic_ostringstream =
    std::basic_ostringstream<typename StringT::value_type,
                             typename StringT::traits_type,
                             typename StringT::allocator_type>;

template <typename StringT>
using basic_ostringstream = basic_ostream<std_basic_ostringstream<StringT>,
                                          internal::fmt_params<StringT>>;

static_assert(
    std::is_same<ostringstream, basic_ostringstream<std::string>>::value,
    "never happen");
static_assert(
    std::is_same<wostringstream, basic_ostringstream<std::wstring>>::value,
    "never happen");

// output methods

template <typename OstreamBaseT, typename FmtParamsT, typename FirstT,
          typename SecondT>
basic_ostream<OstreamBaseT, FmtParamsT>& operator<<(
    basic_ostream<OstreamBaseT, FmtParamsT>& os,
    const std::pair<FirstT, SecondT>&        p);

template <typename OstreamBaseT, typename FmtParamsT>
basic_ostream<OstreamBaseT, FmtParamsT>& operator<<(
    basic_ostream<OstreamBaseT, FmtParamsT>& os, const std::tuple<>& t);

template <typename OstreamBaseT, typename FmtParamsT, typename... Args>
basic_ostream<OstreamBaseT, FmtParamsT>& operator<<(
    basic_ostream<OstreamBaseT, FmtParamsT>& os, const std::tuple<Args...>& t);

#define DECLARE_MY_OSTREAM(container)                                     \
  template <typename OstreamBaseT, typename FmtParamsT, typename... Args> \
  basic_ostream<OstreamBaseT, FmtParamsT>& operator<<(                    \
      basic_ostream<OstreamBaseT, FmtParamsT>& os,                        \
      const std::container<Args...>&           c)

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

//! convert all `args` into std::string joined with `", "`
template <typename... Args>
std::string tostr(const Args&... args);

//! convert all `args` into std::wstring joined with `L", "`
template <typename... Args>
std::wstring towstr(const Args&... args);

// ==================== definitions ====================

namespace internal {

template <typename StringT>
struct fmt_param_unit {
  using string_type = StringT;
  using char_type   = typename string_type::value_type;

  fmt_param_unit() {}

  fmt_param_unit(const string_type& left_border, const string_type& separator,
                 const string_type& right_border)
      : lb(left_border), sep(separator), rb(right_border) {}

  fmt_param_unit(string_type&& left_border, string_type&& separator,
                 string_type&& right_border)
      : lb(std::move(left_border)),
        sep(std::move(separator)),
        rb(std::move(right_border)) {}

  template <typename T>
  fmt_param_unit& with_lb(T&& s) {
    lb = std::forward<T>(s);
    return *this;
  }

  template <typename T>
  fmt_param_unit& with_sep(T&& s) {
    sep = std::forward<T>(s);
    return *this;
  }

  template <typename T>
  fmt_param_unit& with_rb(T&& s) {
    rb = std::forward<T>(s);
    return *this;
  }

  template <typename T>
  fmt_param_unit& with(T&& left_border, T&& separator, T&& right_border) {
    lb  = std::forward<T>(left_border);
    sep = std::forward<T>(separator);
    rb  = std::forward<T>(right_border);
    return *this;
  }

  string_type lb, sep, rb;
};

template <typename StringT>
struct fmt_params {
  using string_type         = StringT;
  using char_type           = typename string_type::value_type;
  using fmt_param_unit_type = fmt_param_unit<string_type>;

  // clang-format off
  fmt_param_unit_type
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
                 map_kv_fmt{{   }, {':', ' '}, {   }},
               multimap_fmt{{'{'}, {',', ' '}, {'}'}},
            multimap_kv_fmt{{   }, {':', ' '}, {   }},
          unordered_map_fmt{{'{'}, {',', ' '}, {'}'}},
       unordered_map_kv_fmt{{   }, {':', ' '}, {   }},
     unordered_multimap_fmt{{'{'}, {',', ' '}, {'}'}},
  unordered_multimap_kv_fmt{{   }, {':', ' '}, {   }},

                  print_fmt{{   }, {',', ' '}, {   }},
            print_range_fmt{{   }, {',', ' '}, {   }};
  // clang-format on
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
  static void print(OstreamT& os, const TupleT& t) { os << std::get<0>(t); }
};

}  // namespace internal

template <typename OstreamBaseT, typename FmtParamsT>
class basic_ostream : public OstreamBaseT {
  using base_type           = OstreamBaseT;
  using fmt_params_type     = FmtParamsT;
  using fmt_param_unit_type = typename fmt_params_type::fmt_param_unit_type;

public:
  template <typename... Args>
  explicit basic_ostream(Args&&... args)
      : base_type(std::forward<Args>(args)...) {}

  template <typename... Args>
  basic_ostream& print(const Args&... args) {
    *this << fmt.print_fmt.lb;
    __print(args...);
    *this << fmt.print_fmt.rb;
    return *this;
  }

  template <typename... Args>
  basic_ostream& println(const Args&... args) {
    print(args...) << std::endl;
    return *this;
  }

  template <typename Iterator>
  basic_ostream& print_range(Iterator begin, Iterator end) {
    return print_range(begin, end, fmt.print_range_fmt);
  }

  template <typename Iterator>
  basic_ostream& print_range(Iterator begin, Iterator end,
                             const fmt_param_unit_type& range_fmt) {
    *this << range_fmt.lb;
    for (auto it = begin; it != end; ++it) {
      if (it != begin) *this << range_fmt.sep;
      *this << *it;
    }
    *this << range_fmt.rb;
    return *this;
  }

  basic_ostream& with_fmt(const fmt_params_type& f) {
    fmt = f;
    return *this;
  }

  basic_ostream& with_fmt(fmt_params_type&& f) {
    fmt = std::move(f);
    return *this;
  }

  fmt_params_type fmt;

private:
  template <typename T>
  basic_ostream& __print(const T& t) {
    *this << t;
    return *this;
  }

  template <typename T, typename... Args>
  basic_ostream& __print(const T& t, const Args&... args) {
    *this << t;
    *this << fmt.print_fmt.sep;
    __print(args...);
    return *this;
  }
};

template <typename OstreamBaseT, typename FmtParamsT, typename FirstT,
          typename SecondT>
basic_ostream<OstreamBaseT, FmtParamsT>& operator<<(
    basic_ostream<OstreamBaseT, FmtParamsT>& os,
    const std::pair<FirstT, SecondT>&        p) {
  os << os.fmt.pair_fmt.lb;
  os << p.first;
  os << os.fmt.pair_fmt.sep;
  os << p.second;
  os << os.fmt.pair_fmt.rb;
  return os;
}

template <typename OstreamBaseT, typename FmtParamsT>
basic_ostream<OstreamBaseT, FmtParamsT>& operator<<(
    basic_ostream<OstreamBaseT, FmtParamsT>& os, const std::tuple<>& t) {
  os << os.fmt.tuple_fmt.lb;
  os << os.fmt.tuple_fmt.rb;
  return os;
}

template <typename OstreamBaseT, typename FmtParamsT, typename... Args>
basic_ostream<OstreamBaseT, FmtParamsT>& operator<<(
    basic_ostream<OstreamBaseT, FmtParamsT>& os, const std::tuple<Args...>& t) {
  os << os.fmt.tuple_fmt.lb;
  internal::tuple_printer<basic_ostream<OstreamBaseT, FmtParamsT>,
                          std::tuple<Args...>, sizeof...(Args)>::print(os, t);
  os << os.fmt.tuple_fmt.rb;
  return os;
}

#define DEFINE_MY_OSTREAM(container)                      \
  DECLARE_MY_OSTREAM(container) {                         \
    return internal::output_all(os, c.cbegin(), c.cend(), \
                                os.fmt.container##_fmt);  \
  }

#define DEFINE_MY_OSTREAM_FOR_MAP(container)                \
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

template <typename... Args>
std::string tostr(const Args&... args) {
  ostringstream o;
  o.print(args...);
  return o.str();
}

template <typename... Args>
std::wstring towstr(const Args&... args) {
  wostringstream o;
  o.print(args...);
  return o.str();
}

}  // namespace myostream
