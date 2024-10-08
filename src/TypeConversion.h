#ifndef _TYPECONVERSION_H_
#define _TYPECONVERSION_H_

#include <errno.h>
#include <limits.h>
#include <math.h>  // for HUGE_VALF, HUGE_VALL
#include <cstdio>
#include <stdlib.h>
#include <set>
#include <sstream>
#include <vector>

////////////////////////////////////////////////////////////////////////
// define HUGE_VALF, HUGE_VALL for Solaris 10
/* HUGE_VALF is a 'float' Infinity.  */
#ifndef HUGE_VALF
#if defined _MSC_VER
/* The Microsoft MSVC 9 compiler chokes on the expression 1.0f / 0.0f.  */
#define HUGE_VALF (1e25f * 1e25f)
#else
#define HUGE_VALF (1.0f / 0.0f)
#endif
#endif

/* HUGE_VAL is a 'double' Infinity.  */
#ifndef HUGE_VAL
#if defined _MSC_VER
/* The Microsoft MSVC 9 compiler chokes on the expression 1.0 / 0.0.  */
#define HUGE_VAL (1e250 * 1e250)
#else
#define HUGE_VAL (1.0 / 0.0)
#endif
#endif

/* HUGE_VALL is a 'long double' Infinity.  */
#ifndef HUGE_VALL
#if defined _MSC_VER
/* The Microsoft MSVC 9 compiler chokes on the expression 1.0L / 0.0L.  */
#define HUGE_VALL (1e250L * 1e250L)
#else
#define HUGE_VALL (1.0L / 0.0L)
#endif
#endif

// convert double/int/byte to string type
template <class T>
inline std::string toString(T i) {
  std::stringstream ss;
  ss << i;
  return ss.str();
}

template <class T>
inline std::string toString(const std::set<T>& in, const std::string& sep) {
  if (in.empty()) {
    return "";
  }
  std::stringstream ss;
  typename std::set<T>::const_iterator iter = in.begin();
  for (; iter != in.end(); ++iter) {
    ss << *iter;
    ss << sep;
  }
  std::string ret = ss.str();
  ret.resize(ret.size() - sep.size());
  return ret;
}

#ifdef _STDINT_H
// this is only necessary when uint8_t is defined (aka. stdint.h is included)
template <>
inline std::string toString(const std::set<uint8_t>& in,
                            const std::string& sep) {
  std::stringstream ss;
  std::set<uint8_t>::const_iterator iter = in.begin();
  for (; iter != in.end(); ++iter) {
    ss << (int)*iter;
    ss << sep;
  }
  std::string ret = ss.str();
  ret.resize(ret.size() - sep.size());
  return ret;
}
#endif

template <class T>
inline std::string toString(const std::vector<T>& in, const std::string& sep) {
  std::stringstream ss;
  for (size_t i = 0; i != in.size(); ++i) {
    if (i) {
      ss << sep;
    }
    ss << in[i];
  }
  return ss.str();
}

// convert double/float to string type
// we try to mimic the '%g' in printf
template <class T>
inline std::string floatToString(T i) {
  std::stringstream ss;
  ss.precision(6);
  ss << std::noshowpoint << i;
  return ss.str();
}

// convert std::string to integer
// @return true if conversion succeed
inline bool str2int(const char* input, int* output) {
  char* endptr;
  long val;
  errno = 0;
  val = strtol(input, &endptr, 10);

  if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN)) ||
      (errno != 0 && val == 0)) {
    perror("strtol");
    return false;
  }

  if (endptr == input) {
    // no digits found
    return false;
  }

  *output = val;
  return true;
}
inline bool str2int(const std::string& input, int* output) {
  return str2int(input.c_str(), output);
}

inline int toInt(const char* input) { return (atoi(input)); }

inline int toInt(const std::string& input) { return (atoi(input.c_str())); }

inline float toFloat(const char* input) { return (atof(input)); }

inline float toFloat(const std::string& input) { return (atof(input.c_str())); }

inline double toDouble(const char* input) { return (atof(input)); }

inline double toDouble(const std::string& input) {
  return (atof(input.c_str()));
}

// convert std::string to double
// @return true if conversion succeed
inline bool str2double(const char* input, double* output) {
  char* endptr;
  double val;

  errno = 0;
  val = strtod(input, &endptr);

  if ((errno == ERANGE && (val == HUGE_VALF || val == HUGE_VALL)) ||
      (errno != 0 && val == 0.)) {
    perror("strtod");
    return false;
  }

  if (endptr == input) {
    // no digits found
    return false;
  }
  *output = val;
  return true;
}

inline bool str2double(const std::string& input, double* output) {
  return str2double(input.c_str(), output);
}

inline int atoi(const std::string& s) {
  int result;
  bool ret = str2int(s.c_str(), &result);
  if (!ret) {
    return 0;
  }
  return result;
}

inline double atof(const std::string& s) {
  double result;
  bool ret = str2double(s.c_str(), &result);
  if (!ret) {
    return 0.0;
  }
  return result;
}

/**
 * convert @param chrom to integer for easier comparisons
 * leading "chr" will not be considered
 * chr6 -> 6
 * chr6_abc -> 6 + 100
 * chrX -> 23
 * chrY -> 24
 * chrMT -> 25
 * chrOther -> 1000 + ASCII('O')
 * chr -> -1
 */
extern int chrom2int(const std::string& chrom);
#endif /* _TYPECONVERSION_H_ */
