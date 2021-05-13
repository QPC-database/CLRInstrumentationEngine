// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
#include "pch.h"
#include <fstream>
//#include <experimental/filesystem>
#include "Common.Lib/Macros.h"
#include "Common.Lib/systemstring.h"
#include "Common.Lib/XmlNode.h"
#include "Common.Lib/XmlDocWrapper.h"

#include "InstrumentationEngine.Api/InstrumentationEngine.h"
#include "InstrumentationEngine.Lib/InstrumentationMethod.h"
#include "InstrumentationEngine.Lib/ConfigurationLoader.h"

using namespace std;
using namespace CommonLib;
using namespace MicrosoftInstrumentationEngine;
//namespace fs = std::experimental::filesystem;

#ifdef PLATFORM_UNIX
using sysfstream = fstream;
#else
using sysfstream = wfstream;
#endif


struct CoInit
{

#ifndef PLATFORM_UNIX
    CoInit()
    {
        HRESULT _ = CoInitializeEx(nullptr, 0);
    }

    ~CoInit()
    {
        CoUninitialize();
    }

#endif

};

TEST(ConfigurationTests, LoadConfigurationLoadsOneInstrumentationMethodCorrectly)
{
    // Initialze for COM, if needed.
    CoInit co;
    auto fileName = _ST("/Config.xml");
    //if (exists(fileName))
    //{
    //    remove(fileName);
    //}

    sysfstream output;
    output.open(fileName, ios_base::app | ios_base::out);
    WCHAR error[256] = { 0 };

    EXPECT_TRUE(!output.fail() && !output.bad());

    output << _ST("<InstrumentationEngineConfiguration>") << std::endl;
    output << _ST("<InstrumentationMethod>") << std::endl;
    output << _ST("  <Name>Squid Instrumentation</Name>") << std::endl;
    output << _ST("  <Description>Dynamically make squids swim</Description>") << std::endl;
    output << _ST("  <Module>SeafoodInstrumentation.dll</Module>") << std::endl;
    output << _ST("  <ClassGuid>{249E89A6-12D9-4E03-82FF-7FEAA41310E9}</ClassGuid>") << std::endl;
    output << _ST("  <Priority>50</Priority>") << std::endl;
    output << _ST("</InstrumentationMethod>") << std::endl;
    output << _ST("</InstrumentationEngineConfiguration>") << std::endl;

    output.close();

    CConfigurationLoader loader;
    std::vector<CInstrumentationMethod*> methods;

    EXPECT_OK(loader.LoadConfiguration((BSTR)fileName, methods));

    EXPECT_EQ((size_t)1, methods.size());
    auto method = methods.at(0);

    GUID expected = { 0x249E89A6, 0x12D9, 0x4E03, { 0x82 ,0xFF, 0x7F, 0xEA, 0xA4, 0x13, 0x10, 0xE9 } };
    EXPECT_TRUE(method->GetClassId() == expected);
}

TEST(ConfigurationTEsts, LoadConfigurationDoesNotCrashWhenDescriptionIsMissing)
{
    CoInit co;
    auto fileName = _ST("/Config.xml");
    //if (exists(fileName))
    //{
    //    remove(fileName);
    //}

    wfstream output;
    output.open(fileName, ios_base::app | ios_base::out);
    WCHAR error[256] = { 0 };
    EXPECT_TRUE(!output.fail() && !output.bad());

    output << _ST("<InstrumentationEngineConfiguration>") << std::endl;
    output << _ST("<InstrumentationMethod>") << std::endl;
    output << _ST("  <Name>Squid Instrumentation</Name>") << std::endl;
    output << _ST("  <Module>SeafoodInstrumentation.dll</Module>") << std::endl;
    output << _ST("  <ClassGuid>{249E89A6-12D9-4E03-82FF-7FEAA41310E9}</ClassGuid>") << std::endl;
    output << _ST("  <Priority>50</Priority>") << std::endl;
    output << _ST("</InstrumentationMethod>") << std::endl;
    output << _ST("</InstrumentationEngineConfiguration>") << std::endl;

    output.close();

    CConfigurationLoader loader;
    std::vector<CInstrumentationMethod*> methods;

    EXPECT_EQ(E_FAIL, loader.LoadConfiguration((BSTR)fileName, methods));

    EXPECT_EQ((size_t)0, methods.size());
}