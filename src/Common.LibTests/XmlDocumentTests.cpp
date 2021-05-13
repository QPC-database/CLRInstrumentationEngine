// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include "pch.h"
#include "../Common.Lib/Macros.h"
#include "../Common.Lib/systemstring.h"
#include "../Common.Lib/XmlNode.h"
#include "../Common.Lib/XmlDocWrapper.h"

using namespace std;
using namespace CommonLib;

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

TEST(XmlTests, SimpleXmlTest)
{
    // Initialze for COM, if needed.
    CoInit co;
    tstring xmlString =
        _T("<First>")
            _T("<Second>")
                _T("<Third attr1='stuff' attr2='things'/>")
                _T("<Fourth>Internal Text</Fourth>")
            _T("</Second>")
        _T("</First>");

    CComPtr<CXmlDocWrapper> pDocWrapper;
    pDocWrapper.Attach(new CXmlDocWrapper());
    EXPECT_OK(pDocWrapper->LoadContent(xmlString.c_str()));

    CComPtr<CXmlNode> pCurrNode;
    EXPECT_OK(pDocWrapper->GetRootNode(&pCurrNode));

    tstring output;
    EXPECT_OK(pCurrNode->GetName(output));
    EXPECT_EQ(tstring(_T("First")), output);

    {
        CComPtr<CXmlNode> pChild;
        EXPECT_OK(pCurrNode->GetChildNode(&pChild));
        pCurrNode = pChild;
    }

    EXPECT_OK(pCurrNode->GetName(output));
    EXPECT_EQ(tstring(_T("Second")), output);

    {
        CComPtr<CXmlNode> pChild;
        EXPECT_OK(pCurrNode->GetChildNode(&pChild));
        pCurrNode = pChild;
        EXPECT_NOT_NULL(pCurrNode.p);
    }

    EXPECT_OK(pCurrNode->GetName(output));
    EXPECT_EQ(tstring(_T("Third")), output);

    EXPECT_OK(pCurrNode->GetAttribute(_T("attr1"), output));
    EXPECT_EQ(tstring(_T("stuff")), output);

    EXPECT_OK(pCurrNode->GetAttribute(_T("attr2"), output));
    EXPECT_EQ(tstring(_T("things")), output);

    EXPECT_FAIL(pCurrNode->GetAttribute(_T("attr3"), output));
    EXPECT_FAIL(pCurrNode->GetStringValue(output));

    {
        CComPtr<CXmlNode> pNext;
        pNext.Attach(pCurrNode->Next());
        pCurrNode = pNext;
        EXPECT_NOT_NULL(pCurrNode.p);
    }

    EXPECT_OK(pCurrNode->GetName(output));
    EXPECT_EQ(tstring(_T("Fourth")), output);

    {
        CComPtr<CXmlNode> pChild;
        EXPECT_OK(pCurrNode->GetChildNode(&pChild));
        pCurrNode = pChild;
        EXPECT_NOT_NULL(pCurrNode.p);
    }

    EXPECT_OK(pCurrNode->GetStringValue(output));
    EXPECT_EQ(tstring(_T("Internal Text")), output);

    {
        CComPtr<CXmlNode> pNext;
        pNext.Attach(pCurrNode->Next());
        pCurrNode = pNext;

        // end of the xml
        EXPECT_NULL(pCurrNode.p);
    }
}