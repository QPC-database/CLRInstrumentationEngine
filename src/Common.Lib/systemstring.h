// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
// Provides interfaces for working with cross-process strings.

#pragma once

#include "../Common.Headers/tstring.h"
#ifdef PLATFORM_UNIX
#include <iconv.h>
#define _WCHARBASE(wstr)

// On Linux, just call the base constructor for utf strings.
#define _CHARBASE(str) : osstring(str) {}
#else

// On Windows, just call the base constructor for wide strings.
#define _WCHARBASE(wstr) : osstring(wstr) {}
#define _CHARBASE(str)
#endif

namespace CommonLib
{
#ifdef PLATFORM_UNIX
    using osstring = std::string;
#else
    using osstring = std::wstring;
#endif

    /// <summary>
    /// Class for converting between operating system strings and standard strings
    /// or wide-character (Windows) strings. Useful for converting strings between
    /// Linux and Windows.
    /// </summary>
    class SystemString : public osstring
    {
    private:
        HRESULT m_hresult = S_OK;
    public:
        /// <summary>
        /// Construct a system string from a UTF16LE (Windows) string.
        /// </summary>
        /// <param name="original">The original string.</param>
        SystemString(_In_ const tstring& original) _WCHARBASE(original);

        /// <summary>
        /// Construct a system string from a UTF8 (Linux) string.
        /// </summary>
        /// <param name="original">The original string</param>
        SystemString(_In_ const std::string& original) _CHARBASE(original);

        /// <summary>
        /// Construct a system string from a UTF16LE (Windows) string.
        /// </summary>
        /// <param name="lpzwStr">The original string.</param>
        SystemString(_In_ const WCHAR* lpzwStr) _WCHARBASE(lpzwStr);

        /// <summary>
        /// Construct a system string from a UTF8 (Linux) string.
        /// </summary>
        /// <param name="lpzStr">The original string</param>
        SystemString(_In_ const CHAR* lpzStr) _CHARBASE(lpzStr);

        /// <summary>Returns true if an error occurred when constructing this object </summary>

        bool HasError() const { return m_hresult; >= 0; }

        HRESULT Error() const { return m_hresult; }

        /// <summary>
        /// Converts this string to a UTF8 (Linux) string
        /// </summary>
        /// <param name="converted">The converted string</param>
        /// <returns>S_OK for success. Error code otherwise.</returns>
        HRESULT ToString(_Inout_ std::string& converted);

        /// <summary>
        /// Converts this string to a UTF16LE (Windows) string
        /// </summary>
        /// <param name="converted">The converted string</param>
        /// <returns>S_OK for success. Error code otherwise</returns>
        HRESULT ToString(_Inout_ tstring& convertetd);

        /// <summary>
        /// Utility function to convert a UTF16LE (Windows) string to the local OS string
        /// </summary>
        /// <param name="original">The original string</param>
        /// <param name="result">The local OS string</param>
        /// <returns>S_OK for success. Error code otherwise</return>
        static HRESULT FromString(_In_ const tstring& origninal, _Inout_ SystemString& result);

        /// <summary>
        /// Utility function to convert a UTF8 (Linux) string to the local OS string
        /// </summary>
        /// <param name="original">The original string</param>
        /// <param name="result">The local OS string</param>
        /// <returns>S_OK for success. Error code otherwise.</return>
        static HRESULT FromString(_In_ const std::string& original, _Inout_ SystemString& result);

        /// <summary>
        /// Utility function to convert a UTF16LE (Windows) string to the local OS string
        /// </summary>
        /// <param name="original">The original string</param>
        /// <param name="result">The local OS string</param>
        /// <returns>S_OK for success, error code otherwise.</return>
        static HRESULT FromString(_In_ const WCHAR* lpzwStr, _Inout_ SystemString& result);

        /// <summary>
        /// Utility function to convert a UTF8 (Linux) string to the local OS string
        /// </summary>
        /// <param name="original">The original string</param>
        /// <param name="result">The local OS string</param>
        /// <returns>true for success, false otherwise</return>
        static HRESULT FromString(_In_ const CHAR* lpzStr, _Inout_ SystemString& result);
    };

}

#undef _WCHARBASE
#undef _CHARBASE

