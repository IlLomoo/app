#pragma once


//#include <shellapi.h>
//#include <wincrypt.h>



//#include "windows_base_file.h"
//#include "windows_multithreading.h"
//#include "windows_utils.h"



//CLASS_DECL_AXIS void set_main_thread(HANDLE hThread);
//CLASS_DECL_AXIS void set_main_thread_id(UINT uiThread);


//CLASS_DECL_AXIS HANDLE get_main_thread();
//CLASS_DECL_AXIS UINT   get_main_thread_id();

//#ifdef __cplusplus
//
//CLASS_DECL_AXIS void attach_thread_input_to_main_thread(bool bAttach = true);
//
//#endif


//#ifdef __cplusplus
//
//CLASS_DECL_AXIS string key_to_char(WPARAM wparam, LPARAM lparam);
//CLASS_DECL_AXIS string read_resource_as_string_dup(HINSTANCE hinst, UINT nID, LPCTSTR lpcszType);
//
//#endif

//#define MESSAGE MSG
//#define LPMESSAGE LPMSG
//
//
//CLASS_DECL_AXIS int WinRegGetValueW(HKEY hkey, LPCWSTR lpSubKey, LPCWSTR lpValue, DWORD dwFlags, LPDWORD pdwType, PVOID pvData, LPDWORD pcbData);




#ifdef cplusplus


CLASS_DECL_AXIS bool vfxResolveShortcut(string & strTarget,const char * pszSource,sp(::user::interaction) puiMessageParentOptional);




#endif
















namespace axis
{


   template < class APP >
   static int32_t simple_app_main(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPTSTR lpCmdLine,int32_t nCmdShow)
   {

      if(!defer_axis_init())
      {
         return -1;
      }

      APP  * papp = new APP;
      ::windows::main_init_data * pmaininitdata = new ::windows::main_init_data;


      pmaininitdata->m_hInstance = hInstance;
      pmaininitdata->m_hPrevInstance = hPrevInstance;
      pmaininitdata->m_vssCommandLine = ::str::international::unicode_to_utf8(::GetCommandLineW());
      pmaininitdata->m_nCmdShow = nCmdShow;

      papp->init_main_data(pmaininitdata);

      int32_t iRet;

      iRet = papp->main();

      try
      {

         delete papp;

         papp = NULL;

      }
      catch(...)
      {
      }

      defer_axis_term();

      return iRet;

   }


} // namespace aura



