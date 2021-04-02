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
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace myostream {

// type traits

template <typename...>
using void_t = void;

#define MYOSTREAM_DEFINE_TYPE_TRAITS(member_type)                      \
  template <typename T, typename = void>                               \
  struct has_##member_type : public std::false_type {};                \
                                                                       \
  template <typename T>                                                \
  struct has_##member_type<T, void_t<typename T::member_type>>         \
      : public std::true_type {};                                      \
                                                                       \
  template <typename T>                                                \
  constexpr bool has_##member_type##_v = has_##member_type<T>::value;  \
                                                                       \
  template <typename T, typename Def, bool = has_##member_type##_v<T>> \
  struct get_##member_type {                                           \
    using type = typename T::member_type;                              \
  };                                                                   \
                                                                       \
  template <typename T, typename Def>                                  \
  struct get_##member_type<T, Def, false> {                            \
    using type = Def;                                                  \
  };                                                                   \
                                                                       \
  template <typename T, typename Def>                                  \
  using get_##member_type##_t = typename get_##member_type<T, Def>::type

MYOSTREAM_DEFINE_TYPE_TRAITS(string_type);
MYOSTREAM_DEFINE_TYPE_TRAITS(traits_type);
MYOSTREAM_DEFINE_TYPE_TRAITS(allocator_type);
#undef MYOSTREAM_DEFINE_TYPE_TRAITS

template <typename OstreamT, typename CharT = typename OstreamT::char_type>
using string_type_by_ostream = get_string_type_t<
    OstreamT,
    std::basic_string<CharT,
                      get_traits_type_t<OstreamT, std::char_traits<CharT>>,
                      get_allocator_type_t<OstreamT, std::allocator<CharT>>>>;

template <typename StringT>
using std_basic_ostringstream_by_string =
    std::basic_ostringstream<typename StringT::value_type,
                             typename StringT::traits_type,
                             typename StringT::allocator_type>;

static_assert(
    std::is_same<string_type_by_ostream<std::ostream>, std::string>::value,
    "never happen");
static_assert(std::is_same<string_type_by_ostream<std::ostringstream>,
                           std::string>::value,
              "never happen");
static_assert(
    std::is_same<string_type_by_ostream<std::wostream>, std::wstring>::value,
    "never happen");
static_assert(std::is_same<string_type_by_ostream<std::wostringstream>,
                           std::wstring>::value,
              "never happen");
static_assert(std::is_same<std_basic_ostringstream_by_string<std::string>,
                           std::ostringstream>::value,
              "never happen");
static_assert(std::is_same<std_basic_ostringstream_by_string<std::wstring>,
                           std::wostringstream>::value,
              "never happen");

// ==================== declarations ====================

// types

template <typename StringT>
struct default_preferences;

template <typename OstreamBaseT,
          typename PreferencesT =
              default_preferences<string_type_by_ostream<OstreamBaseT>>>
class basic_ostream;

template <typename OstreamBaseT,
          typename PreferencesT =
              default_preferences<string_type_by_ostream<OstreamBaseT>>>
class basic_ostringstream;

using ostream        = basic_ostream<std::ostream>;
using wostream       = basic_ostream<std::wostream>;
using ostringstream  = basic_ostringstream<std::ostringstream>;
using wostringstream = basic_ostringstream<std::wostringstream>;

template <typename StringT>
using basic_ostringstream_by_string =
    basic_ostringstream<std_basic_ostringstream_by_string<StringT>,
                        default_preferences<StringT>>;

static_assert(std::is_same<ostringstream,
                           basic_ostringstream_by_string<std::string>>::value,
              "never happen");
static_assert(std::is_same<wostringstream,
                           basic_ostringstream_by_string<std::wstring>>::value,
              "never happen");

// output methods

template <typename OstreamBaseT,
          typename PreferencesT,
          typename FirstT,
          typename SecondT>
basic_ostream<OstreamBaseT, PreferencesT>& operator<<(
    basic_ostream<OstreamBaseT, PreferencesT>& os,
    const std::pair<FirstT, SecondT>&          p);

template <typename OstreamBaseT, typename PreferencesT>
basic_ostream<OstreamBaseT, PreferencesT>& operator<<(
    basic_ostream<OstreamBaseT, PreferencesT>& os, const std::tuple<>& t);

template <typename OstreamBaseT, typename PreferencesT, typename... Args>
basic_ostream<OstreamBaseT, PreferencesT>& operator<<(
    basic_ostream<OstreamBaseT, PreferencesT>& os,
    const std::tuple<Args...>&                 t);

#define MYOSTREAM_DECLARE_OVERLOAD(container)                               \
  template <typename OstreamBaseT, typename PreferencesT, typename... Args> \
  basic_ostream<OstreamBaseT, PreferencesT>& operator<<(                    \
      basic_ostream<OstreamBaseT, PreferencesT>& os,                        \
      const std::container<Args...>&             c)

MYOSTREAM_DECLARE_OVERLOAD(array);
MYOSTREAM_DECLARE_OVERLOAD(deque);
MYOSTREAM_DECLARE_OVERLOAD(forward_list);
MYOSTREAM_DECLARE_OVERLOAD(initializer_list);
MYOSTREAM_DECLARE_OVERLOAD(list);
MYOSTREAM_DECLARE_OVERLOAD(vector);

MYOSTREAM_DECLARE_OVERLOAD(set);
MYOSTREAM_DECLARE_OVERLOAD(multiset);
MYOSTREAM_DECLARE_OVERLOAD(unordered_set);
MYOSTREAM_DECLARE_OVERLOAD(unordered_multiset);

MYOSTREAM_DECLARE_OVERLOAD(map);
MYOSTREAM_DECLARE_OVERLOAD(multimap);
MYOSTREAM_DECLARE_OVERLOAD(unordered_map);
MYOSTREAM_DECLARE_OVERLOAD(unordered_multimap);

//! convert all `args` into std::string joined with `", "`
template <typename... Args>
std::string tostr(const Args&... args);

//! convert all `args` into std::wstring joined with `L", "`
template <typename... Args>
std::wstring towstr(const Args&... args);

// ==================== definitions ====================

namespace internal {

template <typename StringT>
struct ternary_format {
  using string_type = StringT;
  using char_type   = typename string_type::value_type;

  ternary_format() {}

  ternary_format(const string_type& left_border,
                 const string_type& separator,
                 const string_type& right_border)
      : lb(left_border), sep(separator), rb(right_border) {}

  ternary_format(string_type&& left_border,
                 string_type&& separator,
                 string_type&& right_border)
      : lb(std::move(left_border)),
        sep(std::move(separator)),
        rb(std::move(right_border)) {}

  // clang-format off
  ternary_format& with_lb (const string_type& s) {lb  = s; return *this;}
  ternary_format& with_sep(const string_type& s) {sep = s; return *this;}
  ternary_format& with_rb (const string_type& s) {rb  = s; return *this;}
  ternary_format& with_lb (string_type&& s) {lb  = std::move(s); return *this;}
  ternary_format& with_sep(string_type&& s) {sep = std::move(s); return *this;}
  ternary_format& with_rb (string_type&& s) {rb  = std::move(s); return *this;}
  // clang-format on

  ternary_format& with(const string_type& left_border,
                       const string_type& separator,
                       const string_type& right_border) {
    lb  = left_border;
    sep = separator;
    rb  = right_border;
    return *this;
  }

  ternary_format& with(string_type&& left_border,
                       string_type&& separator,
                       string_type&& right_border) {
    lb  = std::move(left_border);
    sep = std::move(separator);
    rb  = std::move(right_border);
    return *this;
  }

  string_type lb, sep, rb;
};

}  // namespace internal

template <typename StringT>
struct default_preferences {
  using string_type = StringT;
  using char_type   = typename string_type::value_type;
  using format_type = internal::ternary_format<string_type>;

  default_preferences() { reset(); }

  void reset() {
    // clang-format off
                     pair_fmt.with({'('}, {',', ' '}, {')'});
                    tuple_fmt.with({'<'}, {',', ' '}, {'>'});

                    array_fmt.with({'['}, {',', ' '}, {']'});
                    deque_fmt.with({'['}, {',', ' '}, {']'});
             forward_list_fmt.with({'['}, {',', ' '}, {']'});
         initializer_list_fmt.with({'['}, {',', ' '}, {']'});
                     list_fmt.with({'['}, {',', ' '}, {']'});
                   vector_fmt.with({'['}, {',', ' '}, {']'});

                      set_fmt.with({'{'}, {',', ' '}, {'}'});
                 multiset_fmt.with({'{'}, {',', ' '}, {'}'});
            unordered_set_fmt.with({'{'}, {',', ' '}, {'}'});
       unordered_multiset_fmt.with({'{'}, {',', ' '}, {'}'});

                      map_fmt.with({'{'}, {',', ' '}, {'}'});
                   map_kv_fmt.with({   }, {':', ' '}, {   });
                 multimap_fmt.with({'{'}, {',', ' '}, {'}'});
              multimap_kv_fmt.with({   }, {':', ' '}, {   });
            unordered_map_fmt.with({'{'}, {',', ' '}, {'}'});
         unordered_map_kv_fmt.with({   }, {':', ' '}, {   });
       unordered_multimap_fmt.with({'{'}, {',', ' '}, {'}'});
    unordered_multimap_kv_fmt.with({   }, {':', ' '}, {   });

                    print_fmt.with({   }, {',', ' '}, {   });
              print_range_fmt.with({   }, {',', ' '}, {   });
    // clang-format on
  }

  // clang-format off
  format_type
      pair_fmt,
      tuple_fmt,

      array_fmt,
      deque_fmt,
      forward_list_fmt,
      initializer_list_fmt,
      list_fmt,
      vector_fmt,

      set_fmt,
      multiset_fmt,
      unordered_set_fmt,
      unordered_multiset_fmt,

      map_fmt,
      map_kv_fmt,
      multimap_fmt,
      multimap_kv_fmt,
      unordered_map_fmt,
      unordered_map_kv_fmt,
      unordered_multimap_fmt,
      unordered_multimap_kv_fmt,

      print_fmt,
      print_range_fmt;
  // clang-format on
};

namespace internal {

template <typename OstreamT, typename IteratorT, typename FormatT>
OstreamT& output_all(OstreamT& os, IteratorT b, IteratorT e, const FormatT& f) {
  os << f.lb;
  for (IteratorT it = b; it != e; ++it) {
    if (it != b) os << f.sep;
    os << *it;
  }
  os << f.rb;
  return os;
}

template <typename OstreamT, typename IteratorT, typename FormatT>
OstreamT& output_all(OstreamT&      os,
                     IteratorT      b,
                     IteratorT      e,
                     const FormatT& f,
                     const FormatT& kv_f) {
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
    os << os.pref.tuple_fmt.sep;
    os << std::get<N - 1>(t);
  }
};

template <typename OstreamT, typename TupleT>
struct tuple_printer<OstreamT, TupleT, 1> {
  static void print(OstreamT& os, const TupleT& t) { os << std::get<0>(t); }
};

}  // namespace internal

template <typename OstreamBaseT, typename PreferencesT>
class basic_ostream : public OstreamBaseT {
  using base_type = OstreamBaseT;

public:
  using ostream_base_type = OstreamBaseT;
  using preferences_type  = PreferencesT;
  using string_type       = typename preferences_type::string_type;
  using char_type         = typename preferences_type::char_type;
  using format_type       = typename preferences_type::format_type;
  using traits_type       = typename string_type::traits_type;
  using allocator_type    = typename string_type::allocator_type;

  static_assert(
      std::is_same<typename OstreamBaseT::char_type, char_type>::value,
      "OstreamBaseT::char_type must be same type as PreferencesT::char_type");
  static_assert(
      std::is_same<typename OstreamBaseT::traits_type, traits_type>::value,
      "OstreamBaseT::traits_type must be same type as traits_type");

  template <typename... Args>
  explicit basic_ostream(Args&&... args)
      : base_type(std::forward<Args>(args)...) {}

  template <typename... Args>
  basic_ostream& print(const Args&... args) {
    *this << pref.print_fmt.lb;
    __print(args...);
    *this << pref.print_fmt.rb;
    return *this;
  }

  template <typename... Args>
  basic_ostream& println(const Args&... args) {
    print(args...) << std::endl;
    return *this;
  }

  template <typename Iterator>
  basic_ostream& print_range(Iterator begin, Iterator end) {
    return print_range(begin, end, pref.print_range_fmt);
  }

  template <typename Iterator>
  basic_ostream& print_range(Iterator           begin,
                             Iterator           end,
                             const format_type& range_fmt) {
    *this << range_fmt.lb;
    for (auto it = begin; it != end; ++it) {
      if (it != begin) *this << range_fmt.sep;
      *this << *it;
    }
    *this << range_fmt.rb;
    return *this;
  }

  basic_ostream& with_pref(const preferences_type& p) {
    pref = p;
    return *this;
  }

  basic_ostream& with_pref(preferences_type&& p) {
    pref = std::move(p);
    return *this;
  }

  preferences_type pref;

private:
  basic_ostream& __print() { return *this; }

  template <typename T>
  basic_ostream& __print(const T& t) {
    *this << t;
    return *this;
  }

  template <typename T, typename... Args>
  basic_ostream& __print(const T& t, const Args&... args) {
    *this << t;
    *this << pref.print_fmt.sep;
    __print(args...);
    return *this;
  }
};

template <typename OstreamBaseT, typename PreferencesT>
class basic_ostringstream : public basic_ostream<OstreamBaseT, PreferencesT> {
  using base_type = basic_ostream<OstreamBaseT, PreferencesT>;

public:
  using ostream_base_type  = typename base_type::ostream_base_type;
  using preferences_type   = typename base_type::preferences_type;
  using string_type        = typename base_type::string_type;
  using char_type          = typename base_type::char_type;
  using format_type        = typename base_type::format_type;
  using traits_type        = typename base_type::traits_type;
  using allocator_type     = typename base_type::allocator_type;
  using string_vector_type = std::vector<string_type>;

  static_assert(
      std::is_same<typename OstreamBaseT::allocator_type,
                   allocator_type>::value,
      "OstreamBaseT::allocator_type must be same type as allocator_type");

  template <typename... Args>
  basic_ostringstream(Args&&... args)
      : base_type(std::forward<Args>(args)...) {}

  template <typename... Args>
  string_vector_type to_string_vector(const Args&... args) {
    auto old = ostream_base_type::str();
    clear_buf();
    string_vector_type ret;
    __to_string_vector(ret, args...);
    *this << old;
    return ret;
  }

  void clear_buf() { ostream_base_type::str(string_type{}); }

private:
  void __to_string_vector(string_vector_type& ret) {}

  template <typename T, typename... Args>
  void __to_string_vector(string_vector_type& ret,
                          const T&            t,
                          const Args&... args) {
    *this << t;
    ret.push_back(ostream_base_type::str());
    clear_buf();
    __to_string_vector(ret, args...);
  }
};

template <typename OstreamBaseT,
          typename PreferencesT,
          typename FirstT,
          typename SecondT>
basic_ostream<OstreamBaseT, PreferencesT>& operator<<(
    basic_ostream<OstreamBaseT, PreferencesT>& os,
    const std::pair<FirstT, SecondT>&          p) {
  os << os.pref.pair_fmt.lb;
  os << p.first;
  os << os.pref.pair_fmt.sep;
  os << p.second;
  os << os.pref.pair_fmt.rb;
  return os;
}

template <typename OstreamBaseT, typename PreferencesT>
basic_ostream<OstreamBaseT, PreferencesT>& operator<<(
    basic_ostream<OstreamBaseT, PreferencesT>& os, const std::tuple<>& t) {
  os << os.pref.tuple_fmt.lb;
  os << os.pref.tuple_fmt.rb;
  return os;
}

template <typename OstreamBaseT, typename PreferencesT, typename... Args>
basic_ostream<OstreamBaseT, PreferencesT>& operator<<(
    basic_ostream<OstreamBaseT, PreferencesT>& os,
    const std::tuple<Args...>&                 t) {
  os << os.pref.tuple_fmt.lb;
  internal::tuple_printer<basic_ostream<OstreamBaseT, PreferencesT>,
                          std::tuple<Args...>,
                          sizeof...(Args)>::print(os, t);
  os << os.pref.tuple_fmt.rb;
  return os;
}

#define MYOSTREAM_DEFINE_OVERLOAD(container)                \
  MYOSTREAM_DECLARE_OVERLOAD(container) {                   \
    return internal::output_all(                            \
        os, c.cbegin(), c.cend(), os.pref.container##_fmt); \
  }

#define MYOSTREAM_DEFINE_OVERLOAD_FOR_MAP(container)         \
  MYOSTREAM_DECLARE_OVERLOAD(container) {                    \
    return internal::output_all(os,                          \
                                c.cbegin(),                  \
                                c.cend(),                    \
                                os.pref.container##_fmt,     \
                                os.pref.container##_kv_fmt); \
  }

MYOSTREAM_DEFINE_OVERLOAD(array)
MYOSTREAM_DEFINE_OVERLOAD(deque)
MYOSTREAM_DEFINE_OVERLOAD(forward_list)
MYOSTREAM_DEFINE_OVERLOAD(initializer_list)
MYOSTREAM_DEFINE_OVERLOAD(list)
MYOSTREAM_DEFINE_OVERLOAD(vector)

MYOSTREAM_DEFINE_OVERLOAD(set)
MYOSTREAM_DEFINE_OVERLOAD(multiset)
MYOSTREAM_DEFINE_OVERLOAD(unordered_set)
MYOSTREAM_DEFINE_OVERLOAD(unordered_multiset)

MYOSTREAM_DEFINE_OVERLOAD_FOR_MAP(map)
MYOSTREAM_DEFINE_OVERLOAD_FOR_MAP(multimap)
MYOSTREAM_DEFINE_OVERLOAD_FOR_MAP(unordered_map)
MYOSTREAM_DEFINE_OVERLOAD_FOR_MAP(unordered_multimap)

#undef MYOSTREAM_DEFINE_OVERLOAD_FOR_MAP
#undef MYOSTREAM_DEFINE_OVERLOAD
#undef MYOSTREAM_DECLARE_OVERLOAD

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

template <typename OstringstreamT>
inline std::vector<typename OstringstreamT::string_type>
split_macro_param_names(const std::string& s) {
  using str_t = typename OstringstreamT::string_type;
  std::vector<str_t> ret;
  OstringstreamT     oss;
  for (size_t i = 0, n = s.size(); i < n; ++i) {
    bool found_bracket = false;
    for (const auto& p : {"()", "<>", "{}", "[]"}) {
      if (s[i] == p[0]) {
        found_bracket = true;
        for (int sum = 0;; ++i) {
          oss << s[i];
          if (s[i] == p[0]) --sum;
          if (s[i] == p[1]) ++sum;
          if (sum == 0) break;
        }
      }
    }
    if (found_bracket) continue;
    if (s[i] == ',') {
      ret.push_back(oss.str());
      oss.str(str_t{});
      while (i + 1 < n && s[i + 1] == ' ') ++i;
    } else {
      oss << s[i];
    }
  }
  ret.push_back(oss.str());
  return ret;
}

#define MYOSTREAM_WATCH(out_stream, kv_sep, param_sep, final_delim, ...)    \
  do {                                                                      \
    using oss_t = myostream::basic_ostringstream_by_string<decltype(        \
        out_stream)::string_type>;                                          \
    oss_t oss;                                                              \
    auto  names  = myostream::split_macro_param_names<oss_t>(#__VA_ARGS__); \
    auto  values = oss.to_string_vector(__VA_ARGS__);                       \
    assert(names.size() == values.size());                                  \
    for (size_t i = 0; i < names.size(); ++i) {                             \
      if (i > 0) out_stream << param_sep;                                   \
      out_stream << names[i];                                               \
      out_stream << kv_sep;                                                 \
      out_stream << values[i];                                              \
    }                                                                       \
    out_stream << final_delim;                                              \
  } while (0)

}  // namespace myostream
