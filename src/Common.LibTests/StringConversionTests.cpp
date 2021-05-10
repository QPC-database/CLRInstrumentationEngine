#include "pch.h"
#include "../Common.Lib/systemstring.h"

using namespace std;
using namespace CommonLib;
TEST(StringConversionTests, u8u16test) {
    SystemString ss = u8"①Ⅻㄨㄩ 啊阿鼾齄丂丄狚狛狜狝﨨﨩ˊˋ˙– ⿻〇㐀㐁䶴䶵";
    EXPECT_FALSE(ss.HasError());
    tstring u16test = _WS("①Ⅻㄨㄩ 啊阿鼾齄丂丄狚狛狜狝﨨﨩ˊˋ˙– ⿻〇㐀㐁䶴䶵");
    tstring tresult;
    EXPECT_TRUE(ss.ToString(tresult));
    EXPECT_EQ(u16test, tresult);
}

TEST(StringConversionTests, u16u8test) {
    SystemString ss = _WS("①Ⅻㄨㄩ 啊阿鼾齄丂丄狚狛狜狝﨨﨩ˊˋ˙– ⿻〇㐀㐁䶴䶵");
    EXPECT_FALSE(ss.HasError());
    string u8test = u8"①Ⅻㄨㄩ 啊阿鼾齄丂丄狚狛狜狝﨨﨩ˊˋ˙– ⿻〇㐀㐁䶴䶵";
    string u8result;
    EXPECT_TRUE(ss.ToString(u8result));
    EXPECT_EQ(u8test, u8result);
}