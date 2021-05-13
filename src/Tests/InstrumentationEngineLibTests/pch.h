// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

// We use the deprecated experimental libraries until we can upgrade to c++17
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING



#ifdef PLATFORM_UNIX

#include "unix.h"
#include <ole.h>
#include <palrt.h>
#include <pal.h>
#endif

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include <windows.h>
#include <eh.h>

#include <sal.h>

#include <atlcomcli.h>
#ifndef PLATFORM_UNIX
#include <atlbase.h>
#include <atlcom.h>
#include <atlutil.h>
#endif
#include <atlcore.h>
//#ifndef PLATFORM_UNIX
#include <atlcoll.h>
//#endif
#include <atlsync.h>

#include <gtest/gtest.h>
#include "Common.Lib/refcount.h"

#if defined(PLATFORM_UNIX)
#define _WT(X) u ## X
#define _ST(X) u8 ## X
#else
#define _WT(X) L ## X
#define _ST(X) L ## X
#endif

// typedef mocks for clr types
/*
typedef UINT_PTR ProcessID;
typedef UINT_PTR AssemblyID;
typedef UINT_PTR AppDomainID;
typedef UINT_PTR ModuleID;
typedef UINT_PTR ClassID;
typedef UINT_PTR ThreadID;
typedef UINT_PTR ContextID;
typedef UINT_PTR FunctionID;
typedef UINT_PTR ObjectID;
typedef UINT_PTR GCHandleID;
typedef UINT_PTR COR_PRF_ELT_INFO;
typedef UINT_PTR ReJITID;

typedef ULONG   CorElementType;
typedef LPVOID  mdScope;                // Obsolete; not used in the runtime.
typedef ULONG32 mdToken;                // Generic token

typedef mdToken mdModule;               // Module token (roughly, a scope)
typedef mdToken mdTypeRef;              // TypeRef reference (this or other scope)
typedef mdToken mdTypeDef;              // TypeDef in this scope
typedef mdToken mdFieldDef;             // Field in this scope
typedef mdToken mdMethodDef;            // Method in this scope
typedef mdToken mdParamDef;             // param token
typedef mdToken mdInterfaceImpl;        // interface implementation token

typedef mdToken mdMemberRef;            // MemberRef (this or other scope)
typedef mdToken mdCustomAttribute;      // attribute token
typedef mdToken mdPermission;           // DeclSecurity

typedef mdToken mdSignature;            // Signature object
typedef mdToken mdEvent;                // event token
typedef mdToken mdProperty;             // property token

typedef mdToken mdModuleRef;            // Module reference (for the imported modules)

// Assembly tokens.
typedef mdToken mdAssembly;             // Assembly token.
typedef mdToken mdAssemblyRef;          // AssemblyRef token.
typedef mdToken mdFile;                 // File token.
typedef mdToken mdExportedType;         // ExportedType token.
typedef mdToken mdManifestResource;     // ManifestResource token.

typedef mdToken mdTypeSpec;             // TypeSpec object

typedef mdToken mdGenericParam;         // formal parameter to generic type or method
typedef mdToken mdMethodSpec;           // instantiation of a generic method
typedef mdToken mdGenericParamConstraint; // constraint on a formal generic parameter

// Application string.
typedef mdToken mdString;               // User literal string token.

typedef mdToken mdCPToken;              // constantpool token

typedef struct _ASSEMBLYMETADATA ASSEMBLYMETADATA;

typedef void * RPC_IF_HANDLE;

typedef WCHAR * BSTR;
*/
#include "cor.h"
#include "corprof.h"

using namespace std;

#define EXPECT_OK(X) EXPECT_EQ(S_OK, (X))
#define EXPECT_FAIL(X) EXPECT_TRUE(FAILED(X))
#define EXPECT_NULL(X) EXPECT_EQ(nullptr, (X))
#define EXPECT_NOT_NULL(X) EXPECT_NE(nullptr, (X))
