#include "stdafx.h"
#include "systemstring.h"
#include <vector>
#include <memory>

#ifdef PLATFORM_UNIX
#include <errno.h>
#endif


using namespace std;

namespace CommonLib
{
#ifdef PLATFORM_UNIX
    struct Ciconv {
        iconv_t m_iconv;
        Ciconv(const iconv_t& iconv) : m_iconv(iconv) {}
        ~Ciconv()
        {
            iconv_close(m_iconv);
        }
    };

    HRESULT ErrnoToHResult(int& err) 
    {
        HRESULT hr;
        switch (err)
        {
            case 0:
                return S_OK;
            case E2BIG:
                return E_BOUNDS;
            case EILSEQ:
                return E_INVALIDARG;
            case EINVAL:
                return E_INVALIDARG;
        }
        return E_FAIL;
    }

    SystemString::SystemString(_In_ const tstring& original)
    {
        m_hresult = FromString(original.c_str(), *this);
    }

    SystemString::SystemString(_In_ const WCHAR* lpzwStr)
    {
        m_hresult = FromString(lpzwStr, *this);
    }

    HRESULT SystemString::ToString(_Inout_ tstring& str)
    {
        Ciconv icnv(iconv_open("UTF-16LE", "UTF-8"));
        
        // Include the null terminator.
        size_t inputLength = length()+1;
        size_t outputMax = (inputLength+1) * 4;
        unique_ptr<WCHAR[]> buffer(new WCHAR[outputMax]);

        const char* pOrig = c_str();
        char* pNew = (char*)buffer.get();
        size_t count = iconv(icnv.m_iconv, (char**)&pOrig, &inputLength, &pNew, &outputMax);
        if (count == (size_t)-1)
        {
            HRESULT hr = ErrnoToHResult(errno);
            str = u"";
            return hr;
        }

        str = buffer.get();
        return S_OK;
    }

    HRESULT SystemString::ToString(_Inout_ string& str)
    {
        str = *this;
        return S_OK;
       
    }

    HRESULT SystemString::FromString(_In_ const tstring& original, _Inout_ SystemString& result)
    {
        return FromString(original.c_str(), result);
    }

    HRESULT SystemString::FromString(_In_ const std::string& original, _Inout_ SystemString& result)
    {
        result = original;
        result.m_hresult = S_OK;
        return result.m_hresult;
    }

    HRESULT SystemString::FromString(_In_ const WCHAR* lpzwStr, _Inout_ SystemString& result)
    {
        if (lpzwStr == nullptr)
        {
            result = "";
            result.m_hresult = E_INVALIDARG;
            return result.m_hresult;
        }

        size_t i = 0;
        const WCHAR* p = lpzwStr;
        // scan to a resonable length for the end of the string.
        for (i = 0; i < 10000; ++i, ++p)
        {
            if (*p == (WCHAR)0)
            {
                break;
            }
        }

        if (i > 10000)
        {
            result = "";
            return E_FAIL;
        }


        Ciconv icnv(iconv_open("UTF-8", "UTF-16LE"));

        // include the null terminator in the input string.
        size_t inbytes = (i+1)*2;
        size_t outbytes = (i+1)*4;
        unique_ptr<char[]> buffer(new char[outbytes]);
        char* input = (char*)lpzwStr;
        char* output = buffer.get();
        size_t count = iconv(icnv.m_iconv, &input, &inbytes, &output, &outbytes);
        if (count == (size_t)-1)
        {
            HRESULT hr = ErrnoToHResult(errno);
            result = "";
            result.m_hresult = hr;
            return result.m_hresult;
        }

        result = buffer.get();
        return result.m_hresult;
    }

    HRESULT SystemString::FromString(_In_ const CHAR* lpzStr, _Inout_ SystemString& result)
    {
        if (lpzStr == nullptr)
        {
            result = "";
            result.m_hresult = E_INVALIDARG;
        }
        else
        {
            result = SystemString(lpzStr);
        }

        return result.m_hresult;
    }
#else
    SystemString::SystemString(_In_ const string& original)
    {
        m_hresult = FromString(original.c_str(), *this);
    }

    SystemString::SystemString(_In_ const CHAR* lpzStr)
    {
        m_hresult = FromString(lpzStr, *this);
    }

    HRESULT SystemString::ToString(_Inout_ tstring& str)
    {
        str = *this;
        return S_OK;
    }

    HRESULT SystemString::ToString(_Inout_ string& str)
    {
        int required = WideCharToMultiByte(CP_UTF8, 0, c_str(), /*null terminated*/-1, nullptr, 0, 0, 0);
        if (required > 0)
        {
            unique_ptr<char[]> buffer(new char[required]);
            int written = WideCharToMultiByte(CP_UTF8, 0, c_str(), /*null terminated*/-1, buffer.get(), required, 0, 0);
            str = buffer.get();
            return S_OK;
        }

        DWORD errorCode = GetLastError();
        str = "";
        return HRESULT_FROM_WIN32(errorCode);
    }


    static HRESULT FromString(_In_ const tstring& original, _Inout_ SystemString& result)
    {
        return FromString(original.c_str(), result);
    }

    static HRESULT FromString(_In_ const std::string& original, _Inout_ SystemString& result)
    {
        return FromString(original.c_str(), result);
    }

    HRESULT SystemString::FromString(_In_ const WCHAR* lpzwStr, _Inout_ SystemString& result)
    {
        if (lpzwStr != nullptr)
        {
            result = SystemString(lpzwStr);
        }
        else
        {
            result = _T("");
            result.m_hresult = E_INVALIDARG;
        }

        return result.m_hresult;
    }

    HRESULT SystemString::FromString(_In_ const CHAR* lpzStr, _Inout_ SystemString& result)
    {
        if (lpzStr == nullptr)
        {
            result = _T("");
            result.m_hresult = E_INVALIDARG;
            return result.m_hresult;
        }

        int required = MultiByteToWideChar(CP_UTF8, 0, lpzStr, /*null terminated*/ -1, nullptr, 0);
        if (required > 0)
        {
            unique_ptr<WCHAR[]> buffer(new WCHAR[required]);
            int written = MultiByteToWideChar(CP_UTF8, 0, lpzStr, /*null terminated*/ -1, buffer.get(), required);
            result = buffer.get();
            result.m_hresult = S_OK;
            return result.m_hresult;
        }
        DWORD errorCode = GetLastError();
        result = L"";
        result.m_hresult = HRESULT_FROM_WIN32(errorCode);
        return result.m_hresult;
    }
#endif
}