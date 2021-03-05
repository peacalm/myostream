// Author: Shuangquan Li (lishq991@gmail.com)
// 2021.3 Beijing China

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

struct fmt_param_unit;

struct fmt_params;

}  // namespace internal

template <typename OstreamBaseT, typename FmtParamsT = internal::fmt_params>
class my_ostream;

template <typename OstreamBaseT, typename FmtParamsT,
          typename FirstT, typename SecondT>
my_ostream<OstreamBaseT, FmtParamsT>& operator<<(
    my_ostream<OstreamBaseT, FmtParamsT>& os,
    const std::pair<FirstT, SecondT>& p);

template <typename OstreamBaseT, typename FmtParamsT>
my_ostream<OstreamBaseT, FmtParamsT>& operator<<(
    my_ostream<OstreamBaseT, FmtParamsT>& os, const std::tuple<>& t);

template <typename OstreamBaseT, typename FmtParamsT, typename ...Args>
my_ostream<OstreamBaseT, FmtParamsT>& operator<<(
    my_ostream<OstreamBaseT, FmtParamsT>& os, const std::tuple<Args...>& t);

#define DECLARE_MY_OSTREAM(container)                                   \
template <typename OstreamBaseT, typename FmtParamsT, typename ...Args> \
my_ostream<OstreamBaseT, FmtParamsT>& operator<<(                       \
    my_ostream<OstreamBaseT, FmtParamsT>& os,                           \
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

template <typename T, typename FmtParamsT = internal::fmt_params>
std::string tostr(const T& t);

// definitions

namespace internal {

struct fmt_param_unit {
  fmt_param_unit(const char* left_border, const char* separator,
                 const char* right_border)
      : lb(left_border), sep(separator), rb(right_border) {}

  std::string lb, sep, rb;
};

struct fmt_params {
  fmt_param_unit
                   pair_fmt{"(", ", ", ")"},
                  tuple_fmt{"<", ", ", ">"},

                  array_fmt{"[", ", ", "]"},
                  deque_fmt{"[", ", ", "]"},
           forward_list_fmt{"[", ", ", "]"},
       initializer_list_fmt{"[", ", ", "]"},
                   list_fmt{"[", ", ", "]"},
                 vector_fmt{"[", ", ", "]"},

                    set_fmt{"{", ", ", "}"},
               multiset_fmt{"{", ", ", "}"},
          unordered_set_fmt{"{", ", ", "}"},
     unordered_multiset_fmt{"{", ", ", "}"},

                    map_fmt{"{", ", ", "}"},
                 map_kv_fmt{"" , ": ", "" },
               multimap_fmt{"{", ", ", "}"},
            multimap_kv_fmt{"" , ": ", "" },
          unordered_map_fmt{"{", ", ", "}"},
       unordered_map_kv_fmt{"" , ": ", "" },
     unordered_multimap_fmt{"{", ", ", "}"},
  unordered_multimap_kv_fmt{"" , ": ", "" };
};

template <typename OstreamT, typename IteratorT>
OstreamT& output_all(OstreamT& os, IteratorT b, IteratorT e,
                     const fmt_param_unit& f) {
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
                     const fmt_param_unit& f, const fmt_param_unit& kv_f) {
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
class my_ostream : public OstreamBaseT {
  using base_t       = OstreamBaseT;
  using fmt_params_t = FmtParamsT;

public:
  template <typename ...Args>
  explicit my_ostream(Args&&... args) : base_t(std::forward<Args>(args)...) {}

  fmt_params_t fmt;
};


template <typename OstreamBaseT, typename FmtParamsT,
    typename FirstT, typename SecondT>
my_ostream<OstreamBaseT, FmtParamsT>& operator<<(
    my_ostream<OstreamBaseT, FmtParamsT>& os,
    const std::pair<FirstT, SecondT>& p) {
  os << os.fmt.pair_fmt.lb;
  os << p.first;
  os << os.fmt.pair_fmt.sep;
  os << p.second;
  os << os.fmt.pair_fmt.rb;
  return os;
}

template <typename OstreamBaseT, typename FmtParamsT>
my_ostream<OstreamBaseT, FmtParamsT>& operator<<(
    my_ostream<OstreamBaseT, FmtParamsT>& os, const std::tuple<>& t) {
  os << os.fmt.tuple_fmt.lb;
  os << os.fmt.tuple_fmt.rb;
  return os;
}

template <typename OstreamBaseT, typename FmtParamsT, typename ...Args>
my_ostream<OstreamBaseT, FmtParamsT>& operator<<(
    my_ostream<OstreamBaseT, FmtParamsT>& os, const std::tuple<Args...>& t) {
  os << os.fmt.tuple_fmt.lb;
  internal::tuple_printer<
      my_ostream<OstreamBaseT, FmtParamsT>, std::tuple<Args...>, sizeof...(Args)
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
  my_ostream<std::ostringstream, FmtParamsT> oss;
  oss << t;
  return oss.str();
}

}  // namespace myostream
