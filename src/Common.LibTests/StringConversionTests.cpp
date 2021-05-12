﻿#include "pch.h"
#include "../Common.Lib/systemstring.h"
#include <stdio.h>

using namespace std;
using namespace CommonLib;
TEST(StringConversionTests, u8u16test) {
    SystemString ss = u8"①Ⅻㄨㄩ 啊阿鼾齄丂丄狚狛狜狝﨨﨩ˊˋ˙– ⿻〇㐀㐁䶴䶵";
    EXPECT_FALSE(ss.HasError());
    tstring u16test = _WS("①Ⅻㄨㄩ 啊阿鼾齄丂丄狚狛狜狝﨨﨩ˊˋ˙– ⿻〇㐀㐁䶴䶵");
    tstring tresult;
    EXPECT_EQ(0, ss.ToString(tresult));
    EXPECT_EQ(u16test, tresult);
}

TEST(StringConversionTests, u16u8test) {
    SystemString ss = _WS("①Ⅻㄨㄩ 啊阿鼾齄丂丄狚狛狜狝﨨﨩ˊˋ˙– ⿻〇㐀㐁䶴䶵");
    EXPECT_FALSE(ss.HasError());
    string u8test = u8"①Ⅻㄨㄩ 啊阿鼾齄丂丄狚狛狜狝﨨﨩ˊˋ˙– ⿻〇㐀㐁䶴䶵";
    string u8result;
    EXPECT_EQ(0, ss.ToString(u8result));
    EXPECT_EQ(u8test, u8result);
}

TEST(StringConversionTests, sysToU16Test) {
    SystemString ss = _SS("①Ⅻㄨㄩ 啊阿鼾齄丂丄狚狛狜狝﨨﨩ˊˋ˙– ⿻〇㐀㐁䶴䶵");
    tstring tresult;
    EXPECT_EQ(0, ss.ToString(tresult));
    SystemString systemResult(tresult);
    EXPECT_FALSE(systemResult.HasError());
    EXPECT_EQ(ss, systemResult);
}

TEST(StringConversionTests, sysToU8Test) {
    SystemString ss = _SS("①Ⅻㄨㄩ 啊阿鼾齄丂丄狚狛狜狝﨨﨩ˊˋ˙– ⿻〇㐀㐁䶴䶵");
    string result;
    EXPECT_EQ(0, ss.ToString(result));
    SystemString systemResult(result);
    EXPECT_FALSE(systemResult.HasError());
    EXPECT_EQ(ss, systemResult);
}