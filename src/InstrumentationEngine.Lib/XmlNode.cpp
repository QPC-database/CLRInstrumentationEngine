// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include "stdafx.h"
#include "XmlNode.h"
#include "../Common.Lib/systemstring.h"

#ifndef PLATFORM_UNIX
CXmlNode::CXmlNode(_In_ IXMLDOMNode* pNode)
{
    m_pNode = pNode;
}
#else
CXmlNode::CXmlNode(_In_ xmlNode* pNode)
{
    m_pNode = pNode;
}
#endif

HRESULT CXmlNode::GetChildNode(_Out_ CXmlNode** ppNode)
{
    HRESULT hr = S_OK;

    IfNullRet(ppNode);
    IfNullRet(m_pNode);
    *ppNode = nullptr;

#ifndef PLATFORM_UNIX
    CComPtr<IXMLDOMNode> pNode;
    IfFailRet(m_pNode->get_firstChild(&pNode));
#else
    xmlNode* pNode = m_pNode->children;
#endif

    if (pNode != nullptr)
    {
        *ppNode = new CXmlNode(pNode);
    }

    return S_OK;
}

CXmlNode* CXmlNode::Next()
{
    if (m_pNode == nullptr)
    {
        return nullptr;
    }

#ifndef PLATFORM_UNIX
    CComPtr<IXMLDOMNode> pNextSibling;
    if (SUCCEEDED(m_pNode->get_nextSibling(&pNextSibling)) &&
        pNextSibling != nullptr)
    {
        return new CXmlNode(pNextSibling);
    }
#else
    xmlNode* pNextSibling = m_pNode->next;
    if (pNextSibling != nullptr)
    {
        return new CXmlNode(pNextSibling);
    }
#endif

    return nullptr;
}

HRESULT CXmlNode::GetName(_Inout_ tstring& name)
{
    HRESULT hr = S_OK;

    IfNullRet(m_pNode);
    SystemString sName;
    name = _T("");

#ifndef PLATFORM_UNIX
    CComBSTR pName;
    IfFailRet(m_pNode->get_nodeName(&pName));
    IfFailRet(SystemString::FromString(pName.m_str, sName));

#else
    IfFailRet(SystemString::FromString((const char*)m_pNode->name, sName));
#endif

    return sName.ToString(name);
}

HRESULT CXmlNode::GetStringValue(_Inout_ tstring& value)
{
    HRESULT hr = S_OK;

    IfNullRet(m_pNode);
    SystemString sValue;
    value = _T("");
#ifndef PLATFORM_UNIX
    CComVariant varNodeValue;
    IfFailRet(m_pNode->get_nodeValue(&varNodeValue));
    IfFailRet(SystemString::FromString(varNodeValue.bstrVal, sValue));
#else
    IfFailRet(SystemString::FromString((const char*)m_pNode->content, sValue));
#endif

    return sValue.ToString(value);
}

HRESULT CXmlNode::GetAttribute(_In_ LPCWSTR wszAttributeName, _Inout_ tstring& value)
{
    HRESULT hr = S_OK;

    IfNullRet(m_pNode);
    SystemString sValue;
    value = _T("");

#ifndef PLATFORM_UNIX

    // Get attributes
    CComPtr<IXMLDOMNamedNodeMap> pAttributes;
    IfFailRet(m_pNode->get_attributes(&pAttributes));

    // Get specific attribute by name.
    CComPtr<IXMLDOMNode> pAttributeNode;
    pAttributes->getNamedItem(const_cast<BSTR>(wszAttributeName), &pAttributeNode);

    if (pAttributeNode != nullptr)
    {
        // Get the attribute value.
        CComVariant varValueAttr;
        pAttributeNode->get_nodeValue(&varValueAttr);
        IfFailRet(SystemString::FromString(varValueAttr.bstrVal, sValue));
    }

    return S_OK;
#else
    SystemString attrName(wszAttributeName);
    IfFailRet(attrName.Error());

    // Note on linux systems, SystemString is backed by a utf8 string.
    xmlChar* utf8AttrValue = xmlGetProp(m_pNode, (const xmlChar *)attrName.c_str());

    if (utf8AttrValue != nullptr)
    {
        IfFailRet(SystemString::FromString((const char*)utf8AttrValue, sValue));
        xmlFree(utf8AttrValue);
    }
    else
    {
        // TODO: willxie do something better than this if no attribute found (ie. xmlGetProp() returns NULL)
        return E_FAIL;
    }
#endif

    return sValue.ToString(value);
}