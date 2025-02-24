/*******************************************************************************
 *
 * MIT License
 *
 * Copyright (c) 2024 Advanced Micro Devices, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 *******************************************************************************/
#include <miopen/env.hpp>
#include "adam.hpp"

MIOPEN_DECLARE_ENV_VAR_STR(MIOPEN_TEST_FLOAT_ARG)
MIOPEN_DECLARE_ENV_VAR_BOOL(MIOPEN_TEST_ALL)

namespace env = miopen::env;

namespace adam {

bool CheckFloatArg(std::string arg)
{
    if(!MIOPEN_TEST_ALL ||
       (env::enabled(MIOPEN_TEST_ALL) && (env::value(MIOPEN_TEST_FLOAT_ARG) == arg)))
    {
        return true;
    }
    return false;
}

struct GPU_Adam_FP32 : AdamTest<float, float>
{
};

struct GPU_Adam_FP16 : AdamTest<half_float::half, half_float::half>
{
};

struct GPU_AmpAdam_FP32 : AdamTest<float, half_float::half>
{
};

} // namespace adam
using namespace adam;

TEST_P(GPU_Adam_FP32, AdamFloatTestFw)
{
    if(CheckFloatArg("--float"))
    {
        RunTest();
        Verify();
    }
    else
    {
        GTEST_SKIP();
    }
};

TEST_P(GPU_Adam_FP16, AdamFloat16TestFw)
{
    if(CheckFloatArg("--half"))
    {
        RunTest();
        Verify();
    }
    else
    {
        GTEST_SKIP();
    }
};

TEST_P(GPU_AmpAdam_FP32, AmpAdamTestFw)
{
    if(CheckFloatArg("--float"))
    {
        RunTest();
        Verify();
    }
    else
    {
        GTEST_SKIP();
    }
};

INSTANTIATE_TEST_SUITE_P(Full, GPU_Adam_FP32, testing::ValuesIn(AdamTestConfigs()));
INSTANTIATE_TEST_SUITE_P(Full, GPU_Adam_FP16, testing::ValuesIn(AdamTestConfigs()));
INSTANTIATE_TEST_SUITE_P(Full, GPU_AmpAdam_FP32, testing::ValuesIn(AdamTestConfigs()));
