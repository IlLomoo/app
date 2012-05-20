#pragma once

#ifdef DEBUG
   void CLASS_DECL_ca TRACELASTERROR();
#else
   #define TRACELASTERROR() ((void)0)
#endif


CLASS_DECL_ca string FormatMessageFromSystem(DWORD dwError);



// This is ca2 API library.
// 
// 
//
// 
// 
// 
// 
// 


#ifndef __DEBUGAPI_H__
#define __DEBUGAPI_H__

#pragma once

#ifdef __cplusplus

namespace gen
{
extern "C" {
#endif
#define TRACE_MAX_NAME_SIZE 64


dword_ptr __stdcall gen_TraceOpenProcess( DWORD idProcess );
void __stdcall gen_TraceCloseProcess( dword_ptr dwProcess );
void __stdcall gen_TraceSnapshotProcess( dword_ptr dwProcess );

/*dword_ptr __stdcall gen_TraceRegister(HINSTANCE hInst,
   int (__cdecl *fnCrtDbgReport)(int,const char *,int,const char *,const char *,va_list));
bool __stdcall gen_TraceUnregister(dword_ptr dwModule);*/

/*dword_ptr __stdcall gen_TraceRegisterCategoryA(dword_ptr dwModule, const CHAR szCategoryName[TRACE_MAX_NAME_SIZE]);
dword_ptr __stdcall gen_TraceRegisterCategoryU(dword_ptr dwModule, const WCHAR szCategoryName[TRACE_MAX_NAME_SIZE]);*/

/*bool __stdcall gen_TraceModifyProcess(dword_ptr dwProcess, UINT nLevel, bool bEnabled, bool bFuncAndCategoryNames, bool bFileNameAndLineNo);
bool __stdcall gen_TraceModifyModule(dword_ptr dwProcess, dword_ptr dwModule, UINT nLevel, trace::e_status eStatus);
bool __stdcall gen_TraceModifyCategory(dword_ptr dwProcess, dword_ptr dwCategory, UINT nLevel, TRACESTATUS eStatus);
bool __stdcall gen_TraceGetProcess(dword_ptr dwProcess, UINT *pnLevel, bool *pbEnabled, bool *pbFuncAndCategoryNames, bool *pbFileNameAndLineNo);
bool __stdcall gen_TraceGetModule(dword_ptr dwProcess, dword_ptr dwModule, UINT *pnLevel, TRACESTATUS *pStatus);
bool __stdcall gen_TraceGetCategory(dword_ptr dwProcess, dword_ptr dwCategory, UINT *pnLevel, TRACESTATUS *pStatus);

_INSECURE_DEPRECATE("gen_TraceGetUpdateEventNameA is unsafe. Instead use AtlTraceGetUpdateEventNameA_s") 
void __stdcall gen_TraceGetUpdateEventNameA(CHAR *pszEventName);
_INSECURE_DEPRECATE("gen_TraceGetUpdateEventNameU is unsafe. Instead use AtlTraceGetUpdateEventNameU_s") 
void __stdcall gen_TraceGetUpdateEventNameU(WCHAR *pszEventName);

void __stdcall gen_TraceGetUpdateEventNameA_s(CHAR *pszEventName, size_t cchEventName);
void __stdcall gen_TraceGetUpdateEventNameU_s(WCHAR *pszEventName, size_t cchEventName);*/

void __cdecl gen_TraceVA(dword_ptr dwModule, const char *pszFileName, int nLineNo,
                  dword_ptr dwCategory, UINT nLevel, const CHAR *pszFormat, va_list ptr);
void __cdecl gen_TraceVU(dword_ptr dwModule,const char *pszFileName, int nLineNo,
                  dword_ptr dwCategory, UINT nLevel, const WCHAR *pszFormat, va_list ptr);

bool __stdcall gen_TraceLoadSettingsA(const CHAR *pszFileName, dword_ptr dwProcess = 0);
bool __stdcall gen_TraceLoadSettingsU(const WCHAR *pszFileName, dword_ptr dwProcess = 0);
bool __stdcall gen_TraceSaveSettingsA(const CHAR *pszFileName, dword_ptr dwProcess = 0);
bool __stdcall gen_TraceSaveSettingsU(const WCHAR *pszFileName, dword_ptr dwProcess = 0);

/*typedef struct TRACESETTINGS
{
   UINT nLevel;
   TRACESTATUS eStatus;
} TRACESETTINGS;

typedef struct TRACEPROCESSSETTINGS
{
   UINT nLevel;
   bool bEnabled, bFuncAndCategoryNames, bFileNameAndLineNo;
} TRACEPROCESSSETTINGS;

typedef struct TRACEPROCESSINFO
{
   WCHAR szName[TRACE_MAX_NAME_SIZE], szPath[MAX_PATH];
   DWORD dwId;
   TRACEPROCESSSETTINGS settings;
   int nModules;
} TRACEPROCESSINFO;

typedef struct TRACEMODULEINFO
{
   WCHAR szName[TRACE_MAX_NAME_SIZE], szPath[MAX_PATH];
   TRACESETTINGS settings;
   dword_ptr dwModule;
   int nCategories;
} TRACEMODULEINFO;

typedef struct TRACECATEGORYINFO
{
   WCHAR szName[TRACE_MAX_NAME_SIZE];
   TRACESETTINGS settings;
   dword_ptr dwCategory;
} TRACECATEGORYINFO;

bool __stdcall gen_TraceGetProcessInfo(dword_ptr dwProcess, TRACEPROCESSINFO* pProcessInfo);
void __stdcall gen_TraceGetModuleInfo(dword_ptr dwProcess, int iModule, TRACEMODULEINFO* pModuleInfo);
void __stdcall gen_TraceGetCategoryInfo(dword_ptr dwProcess, dword_ptr dwModule, int iCategory, TRACECATEGORYINFO* pAtlTraceCategoryInfo);
*/

#ifdef UNICODE
#define gen_TraceRegisterCategory AtlTraceRegisterCategoryU
#define gen_TraceGetUpdateEventName AtlTraceGetUpdateEventNameU
#define gen_TraceGetUpdateEventName_s AtlTraceGetUpdateEventNameU_s
#define gen_Trace AtlTraceU
#define gen_TraceV AtlTraceVU
#define gen_TraceLoadSettings AtlTraceLoadSettingsU
#define gen_TraceSaveSettings AtlTraceSaveSettingsU

#else
#define gen_TraceRegisterCategory AtlTraceRegisterCategoryA
#define gen_TraceGetUpdateEventName AtlTraceGetUpdateEventNameA
#define gen_TraceGetUpdateEventName_s AtlTraceGetUpdateEventNameA_s
#define gen_Trace AtlTraceA
#define gen_TraceV AtlTraceVA
#define gen_TraceLoadSettings AtlTraceLoadSettingsA
#define gen_TraceSaveSettings AtlTraceSaveSettingsA

#endif

#ifdef __cplusplus
};

};  // namespace gen
#endif

#endif  // __DEBUGAPI_H__
