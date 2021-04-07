#pragma once

#include <gtest/gtest.h>

#include "myostream.h"

extern myostream::ostream  mycout;
extern myostream::ostream  mycerr;
extern myostream::wostream mywcout;
extern myostream::wostream mywcerr;

#define watch(...) MYOSTREAM_WATCH(mycout, " = ", "\n", "\n\n", __VA_ARGS__)
