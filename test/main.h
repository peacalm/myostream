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

#include <gtest/gtest.h>

#include "myostream.h"

extern myostream::ostream  mycout;
extern myostream::ostream  mycerr;
extern myostream::wostream mywcout;
extern myostream::wostream mywcerr;

extern myostream::ostream_dense  mycout_dense;
extern myostream::ostream_dense  mycerr_dense;
extern myostream::wostream_dense mywcout_dense;
extern myostream::wostream_dense mywcerr_dense;

#define watch(...) MYOSTREAM_WATCH(mycout, " = ", "\n", "\n\n", __VA_ARGS__)
