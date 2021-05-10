#include "stdafx.h"
#include "systemstring.h"
#include <vector>
#include <memory>


using namespace std;

namespace CommonLib
{
#ifdef PLATFORM_UNIX
    SystemString::SystemString(_In_ const tstring& original)
    {
        m_error = !FromString(original.c_str(), *this);
    }

    SystemString::SystemString(_In_ const WCHAR* lpzwStr)
    {
        m_error = !FromString(lpzwStr, *this);
    }

    bool SystemString::ToString(_Inout_ tstring& str)
    {
        iconv_t icnv = iconv_open("UTF-16LE", "UTF-8");
        size_t inputLength = length();
        size_t outputMax = inputLength * 4;
        unique_ptr<WCHAR[]> buffer(new WCHAR[outputMax]);

        const char* pOrig = c_str();
        char* pNew = (char*)buffer.get();
        if (iconv(icnv, (char**)&pOrig, &inputLength, &pNew, &outputMax) > 0)
        {
            return true;
        }

        iconv_close(icnv);
        str = u"";
        return false;
    }

    bool SystemString::ToString(_Inout_ string& str)
    {
        str = *this;
        return true;
       
    }

    bool SystemString::FromString(_In_ const tstring& original, _Inout_ SystemString& result)
    {
        return FromString(original.c_str(), result);
    }

    bool SystemString::FromString(_In_ const std::string& original, _Inout_ SystemString& result)
    {
        result = original;
        return true;
    }

    bool SystemString::FromString(_In_ const WCHAR* lpzwStr, _Inout_ SystemString& result)
    {
        int i = 0;
        const WCHAR* p = lpzwStr;
        // scan to a resonable length for the end of the string.
        for (size_t i = 0; i < 10000; ++i, ++p)
        {
            if (*p == (WCHAR)0)
            {
                break;
            }
        }

        if (i > 10000)
        {
            result = "";
            return false;
        }


        iconv_t icnv = iconv_open("UTF-8", "UTF-16");

        // The resulting buffer should be smaller in bytes than the original buffer.
        size_t length = i*4;
        unique_ptr<char[]> buffer(new char[length]);
        char* input = (char*)&lpzwStr;
        char* output = buffer.get();
        size_t count = iconv(icnv, &input, &length, &output, &length);
        if (count == (size_t)-1)
        {
            result = "";
            return false;
        }

        result = buffer.get();
        return true;
    }

    bool SystemString::FromString(_In_ const CHAR* lpzStr, _Inout_ SystemString& result)
    {
        result = SystemString(lpzStr);
        return true;
    }
#else
    SystemString::SystemString(_In_ const string& original)
    {
        m_error = !FromString(original.c_str(), *this);
    }

    SystemString::SystemString(_In_ const CHAR* lpzStr)
    {
        m_error = !FromString(lpzStr, *this);
    }

    bool SystemString::ToString(_Inout_ tstring& str)
    {
        str = *this;
        return true;
    }

    bool SystemString::ToString(_Inout_ string& str)
    {
        int required = WideCharToMultiByte(CP_UTF8, 0, c_str(), /*null terminated*/-1, nullptr, 0, 0, 0);
        if (required > 0)
        {
            unique_ptr<char[]> buffer(new char[required]);
            int written = WideCharToMultiByte(CP_UTF8, 0, c_str(), /*null terminated*/-1, buffer.get(), required, 0, 0);
            str = buffer.get();
            return true;
        }

        str = "";
        return false;
    }


    static bool FromString(_In_ const tstring& original, _Inout_ SystemString& result)
    {
        return FromString(original.c_str(), result);
    }

    static bool FromString(_In_ const std::string& original, _Inout_ SystemString& result)
    {
        return FromString(original.c_str(), result);
    }

    bool SystemString::FromString(_In_ const WCHAR* lpzwStr, _Inout_ SystemString& result)
    {
        result = SystemString(lpzwStr);
        return true;
    }

    bool SystemString::FromString(_In_ const CHAR* lpzStr, _Inout_ SystemString& result)
    {
        int required = MultiByteToWideChar(CP_UTF8, 0, lpzStr, /*null terminated*/ -1, nullptr, 0);
        if (required > 0)
        {
            unique_ptr<WCHAR[]> buffer(new WCHAR[required]);
            int written = MultiByteToWideChar(CP_UTF8, 0, lpzStr, /*null terminated*/ -1, buffer.get(), required);
            result = buffer.get();
            return true;
        }

        result = L"";
        return false;
    }
#endif
}