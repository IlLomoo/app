#include "framework.h"


#ifdef WINDOWS
#undef new
#include <gdiplus.h>
#endif

#ifdef LINUX
#include <sys/mman.h>
//#include <sys/stat.h>
//#include <fcntl.h>
#endif

void CLASS_DECL_ca __cdecl _ca2_purecall_();


void CLASS_DECL_ca __cdecl _ca2_purecall_()
{
    throw simple_exception(::ca::get_thread_app());
}

namespace ca
{

   extern HMODULE g_hmoduleOs;

}

namespace plugin
{

   void ms_get_dup_status_callback(void * p, int32_t i, uint_ptr dw);
   void my_se_translator_function(uint32_t, struct _EXCEPTION_POINTERS* );


   plugin::plugin()
   {

      m_papp                  = ::null();
      m_puiHost               = ::null();
      m_iHealingSurface       = 0;
      m_iEdge                 = -1;
      m_bAppStarted           = false;
      m_pbReady               = ::null();
      m_psystem               = ::null();
      m_bMainReady            = false;

      m_pbitmap               = ::null();
      m_pgraphics             = ::null();

#ifdef WINDOWS
      m_hfileBitmap           = INVALID_HANDLE_VALUE;
      m_hfilemapBitmap        = ::null();
      m_pcolorref             = ::null();
#else
      m_hfileBitmap           = -1;
      m_pcolorref             = (uint32_t *) MAP_FAILED;
#endif
      m_pmutexBitmap          = ::null();

   }

   bool plugin::hist(const char * pszUrl)
   {
      return open_url(pszUrl);
   }

   void plugin::run_start_install(const char * pszInstall)
   {
      UNREFERENCED_PARAMETER(pszInstall);
   }

   int32_t plugin::start_ca2_system()
   {

#ifdef METROWIN

      throw todo(get_app());

      return 0;

#else

      string strMutex = m_phost->m_vssChannel;

      ::ca::str::begins_eat_ci(strMutex, "\\ca2\\");

      m_pmutexBitmap = new simple_mutex("Global\\" + strMutex, false);

      bool bNew = false;

      //Sleep(15 * 1000);

      mutex_lock mlSystem(m_phost->m_mutexSystem, true);

      if(m_phost->get_system() == ::null())
      {

#ifdef WINDOWS

         _set_purecall_handler(_ca2_purecall_);

#endif

         ::plane::system * psystem = new ::plane::system();

         psystem->m_bDoNotExitIfNoApplications = true;

         m_phost->set_system((void *) (::plane::system *) psystem);

         mlSystem.unlock();

//         int32_t nReturnCode = 0;

/*         ::win::main_init_data * pinitmaindata  = new ::win::main_init_data;

         pinitmaindata->m_hInstance             = hInstance;
         pinitmaindata->m_hPrevInstance         = hPrevInstance;
         pinitmaindata->m_vssCommandLine        = ::ca::international::unicode_to_utf8(::GetCommandLineW());
         pinitmaindata->m_nCmdShow              = nCmdShow;


         psystem->init_main_data(pinitmaindata);*/

#ifdef WINDOWS
         psystem->m_hInstance = ::GetModuleHandle("ca.dll");
#endif

         if(!psystem->InitApplication())
            return 0;

         if(!psystem->process_initialize())
            return 0;

         psystem->start_application(true, ::null());



      }

      mlSystem.unlock();

      if(m_psystem == ::null())
      {

         m_bAppStarted = false;

         m_psystem = new ::plugin::system((::plane::system *) m_phost->get_system());

         m_psystem->m_pplugin = this;

#ifdef WINDOWS
         m_psystem->m_hInstance = ::GetModuleHandle("ca.dll");
#endif

         if(!m_psystem->InitApplication())
            return 0;

         if(!m_psystem->process_initialize())
            return 0;

         m_psystem->set_history(new history(m_psystem, this));

         m_psystem->m_prunstartinstaller = new run_start_installer(m_psystem, this);

         m_psystem->m_bInitializeProDevianMode = false;

         string strId;
         strId.Format("npca2::%08x", (uint_ptr) m_psystem);

         m_psystem->command()->m_varTopicQuery["local_mutex_id"] = strId;

         set_app(m_psystem);

      }

      m_puiHost = create_host_interaction();
      m_puiHost->m_pplugin = this;
      m_puiHost->install_message_handling(m_puiHost->m_pimpl);

      if(m_puiHost != ::null())
      {
         m_psystem->oprop("top_parent") = m_puiHost;
/*         m_puiHost->m_bRectOk = true;
         m_puiHost->m_pimpl->m_bRectOk = true;
         m_puiHost->m_rectParentClient = m_rect;
         m_puiHost->m_rectParentClient.offset(-point64(m_puiHost->m_rectParentClient.top_left()));
         m_puiHost->m_pimpl->m_rectParentClient = m_puiHost->m_rectParentClient;
         m_puiHost->SendMessageA(WM_SIZE);
         m_puiHost->SendMessageA(WM_MOVE);*/
//         class rect rect = m_phost->m_rect;
  //       set_window_rect(rect);
      }

///      m_psystem->m_puiInitialPlaceHolderContainer = m_puiHost;
      m_psystem->add_frame(m_puiHost);
      m_puiHost->layout();

      if(m_pbReady == ::null())
         m_pbReady = (bool *) ca2_alloc(sizeof(bool));


#ifdef WINDOWS
      // Create Message Queue
      MESSAGE msg;
	   PeekMessage(&msg, ::null(), 0, 0xffffffffu, FALSE);
#endif


      if(bNew || !m_bAppStarted)
      {
         m_bAppStarted = true;
         m_psystem->begin();
      }

      m_bInitialized = true;

      return 0;

#endif

   }


   void plugin::on_paint(simple_graphics & gWindow, LPCRECT lprectOut)
   {

      //class rect rect = m_phost->m_rect;

      //set_window_rect(rect);

      UNREFERENCED_PARAMETER(lprectOut);

      LPCRECT lprect = &m_rect;

      try
      {

         ensure_bitmap_data(lprect->right - lprect->left, lprect->bottom - lprect->top, true);

         if(m_puiHost == ::null())
            return;

         if(m_dib.is_null())
            m_dib.create(allocer());

         if(m_dib.is_null())
            return;

         m_dib->create(lprect->right - lprect->left, lprect->bottom - lprect->top);

         m_dib->Fill(0, 0, 0, 0);

//         ::ca::graphics_sp dc(get_app());

         //dc->CreateCompatibleDC(::null());

         //m_dib->defer_realize(dc);

         try
         {

            /*
            RECT rect;

            rect.left = 100;
            rect.right = 500;
            rect.top = 100;
            rect.bottom = 500;

            m_dib->get_graphics()->FillSolidRect(&rect, ARGB(128, 255, 255, 127));
            */

            m_puiHost->_000OnDraw(m_dib->get_graphics());

         }
         catch(...)
         {
         }

#ifdef WINDOWSEX

         ((Gdiplus::Graphics *) m_dib->get_graphics()->get_os_data())->Flush(Gdiplus::FlushIntentionSync);

         ::GdiFlush();

#endif

         m_sizeBitmap.cx = abs_dup(lprect->right - lprect->left);

         m_sizeBitmap.cy = abs_dup(lprect->bottom - lprect->top);

         {

            mutex_lock sl(*m_pmutexBitmap, true);

            memcpy(m_pcolorref, m_dib->get_data(), abs_dup(m_sizeBitmap.cy) * abs_dup(m_sizeBitmap.cx) * 4);

         }


      }
      catch(...)
      {
      }

      try
      {

         if(is_installation_lock_file_locked())
         {

            reload_plugin();

         }

      }
      catch(...)
      {
      }

   }

   void plugin::on_ready()
   {
      m_puiHost->PostMessage(host_interaction::message_check, 0, 1);
   }

   void plugin::start_ca2_login()
   {
      __begin_thread(m_puiHost->get_app(), &plugin::thread_proc_ca2_login, (LPVOID) this);
   }

   void plugin::start_ca2_logout()
   {
      __begin_thread(m_puiHost->get_app(), &plugin::thread_proc_ca2_logout, (LPVOID) this);
   }

   UINT c_cdecl plugin::thread_proc_ca2_login(LPVOID pvoid)
   {
      plugin * pplugin = (plugin *) pvoid;
      pplugin->ca2_login();
      return 0;
   }

   UINT c_cdecl plugin::thread_proc_ca2_logout(LPVOID pvoid)
   {
      plugin * pplugin = (plugin *) pvoid;
      pplugin->ca2_logout();
      return 0;
   }

   void plugin::ca2_login()
   {

      ::ca::property_set set(m_psystem);

      set.parse_url_query(m_strCa2LoginRuri);

      string strLocation = set["ruri"];

      if(strLocation.is_empty())
         strLocation = m_strCa2LoginRuri;

      ::ca::property_set setUri(m_psystem);

      setUri.parse_url_query(strLocation);

      if(Sys(m_psystem).url().get_server(strLocation).is_empty())
      {
         strLocation = Sys(m_psystem).url().override_if_empty(strLocation, get_host_location_url(), false);
      }

      string strSessId = set["sessid"];

      ::ca::property_set setLogin(get_app());

      ::fontopus::user * puser = ::null();

      //Sleep(15 * 1000);

      while(puser == ::null())
      {
         puser = Application.m_pfontopus->login(setLogin);
      }

      if(strSessId == puser->m_strFontopusServerSessId || puser->m_strFontopusServerSessId.get_length() < 16)
      {

         m_psystem->url().string_remove(strLocation, "sessid");

      }
      else
      {

         m_psystem->url().string_set(strLocation, "sessid", puser->m_strFontopusServerSessId);

      }

      m_psystem->url().string_remove(strLocation, "action");

      open_url(strLocation);

   }

   void plugin::ca2_logout()
   {

      m_psystem->m_pfontopus->logout();

      ::ca::property_set set(m_psystem);

      set.parse_url_query(m_strCa2LogoutRuri);

      string strLocation = set["ruri"];

      strLocation = m_psystem->url().string_remove(strLocation, "sessid");

      strLocation = m_psystem->url().string_remove(strLocation, "action");

      string strUrl;

      strUrl = "https://account.ca2.cc/sec?action=logout";

      m_psystem->url().string_set(strUrl, "ruri", strLocation);

      open_url(strUrl);

   }


   void ms_get_dup_status_callback(void * p, int32_t i, uint_ptr dw)
   {

      if(i == -3)
      {
         int32_t * pi = (int32_t *) p;
         simple_http_status * ps = (simple_http_status *) dw;
         *pi = ps->m_dwStatusCode;
      }

   }


   void plugin::ready_on_main_thread()
   {

      if(m_bMainReady)
         return;

      keeper < bool > keepMainReady(&m_bMainReady, true, false, true);

      xxdebug_box("ca plugin plugin", "ready_on_main_thread", 0);

      ::count iCount = get_memory_length();

      if(iCount > 0)
      {
         m_memory.allocate(iCount + 1);

         read_memory(m_memory, iCount);

         m_memory.get_data()[iCount] = '\0';

         open_ca2_string((const char *) m_memory.get_data());
      }
      else if(m_phost->m_strPluginUrl.get_length() > 0)
      {

         string strPluginUrl     = Sys(m_psystem).url().override_if_empty(m_phost->m_strPluginUrl, get_host_location_url());

         string strExtension     = System.file().extension(System.url().get_script(strPluginUrl));

         if(strExtension.is_empty() || strExtension.compare_no_case("ca2") == 0)
         {
            // remark alarm
            // STRESS : ms_get_dup
            // in ca library normally System or Application.http() is used
            string strPluginData;

            ::ca::http::e_status estatus = ::ca::http::status_failed;

            string strUrl = strPluginUrl;

            strUrl = System.url().string_set(strUrl, "authnone", 1);

            for(int32_t iAttempt = 0; iAttempt < 3; iAttempt++)
            {

               //strPluginData = ms_get_dup(strPluginUrl, false, &ms_get_dup_status_callback, (void *) &iStatusCode, false);

               ::ca::property_set post(get_app());
               ::ca::property_set headers(get_app());
               ::ca::property_set set(get_app());

               Application.http().get(strUrl, strPluginData, post, headers, set, ::null(), ::null(), ::null(), &estatus);

               if(estatus == ::ca::http::status_ok)
                  break;

            }

            if(estatus == ::ca::http::status_ok)
            {

               open_ca2_string(strPluginData);

            }

         }
         else
         {

            ::ca::application_bias * pbiasCreate = new ::ca::application_bias;
            pbiasCreate->m_puiParent = m_puiHost;
            pbiasCreate->m_set["NativeWindowFocus"] = false;
            m_psystem->get_session(0)->open_by_file_extension("\"" + strPluginUrl + "\"", pbiasCreate);

         }

      }

   }

   void plugin::open_ca2_string(const char * psz)
   {

      try
      {

         //Sleep(15 * 1000);

         LPSTR lpszAlloc = (LPSTR) (void *) psz;

         strsize iCount = strlen(psz);

         //Sleep(15 * 1000);

         string strPluginUrl;

         strPluginUrl = m_phost->m_strPluginUrl;

         Sys(m_psystem).url().override_if_empty(strPluginUrl, get_host_location_url());

         string strPluginScript = m_psystem->url().get_script(m_phost->m_strPluginUrl);


         ::ca::property_set headers(m_psystem);

         headers.parse_http_headers(m_phost->m_strPluginHeaders);

         string strContentType = headers["Content-Type"];

         string str1;

         // TODO |) : Should parse Content-type:
         // ALSO: this case only happens if all file has been downloaded before the plugin has initialized
         if(::ca::str::ends_ci(strPluginScript, ".mp3")
         || ::ca::str::ends_ci(strPluginScript, ".mid")
         || ::ca::str::ends_ci(strPluginScript, ".karaoke")
         || ::ca::str::ends_ci(strPluginScript, ".st3"))
         {
            //m_psystem->m_puiInitialPlaceHolderContainer = m_puiHost;
            ::ca::application_bias * pbiasCreate = new ::ca::application_bias;
            pbiasCreate->m_puiParent = m_puiHost;
            pbiasCreate->m_set["NativeWindowFocus"] = false;
            m_psystem->get_session(0)->open_by_file_extension("\"" + strPluginUrl + "\"", pbiasCreate);
         }
         else
         {
            LPSTR lpszStart = lpszAlloc;
            LPSTR lpszEnd = ::null();
            int32_t i = 0;
            for(; i < iCount; i++)
            {
               lpszEnd = &lpszStart[i];
               if(*lpszEnd == '\n')
               {
                  str1 = string(lpszStart, lpszEnd - lpszStart);
                  break;
               }
            }
            str1.trim();

            string str2;

            lpszStart = lpszEnd;
            for(; (lpszEnd - lpszAlloc) <= iCount; i++)
            {
               if(*lpszEnd == '\0' || !::ca::ch::is_whitespace(lpszEnd))
                  break;
               lpszEnd = (char *) ::ca::str::utf8_inc(lpszEnd);
            }
            lpszStart = lpszEnd;
            for(; (lpszEnd - lpszAlloc) <= iCount; i++)
            {
               if(*lpszEnd == '\0' || ::ca::ch::is_space_char(lpszEnd) || (lpszEnd - lpszAlloc) == iCount)
               {
                  str2 = string(lpszStart, lpszEnd - lpszStart);
                  break;
               }
               lpszEnd = (char *) ::ca::str::utf8_inc(lpszEnd);
            }

            string strId = str2;
            strsize iFind = strId.find("?");
            if(iFind >= 0)
            {
               strId = strId.Left(iFind);
            }
            ::ca::property_set set(get_app());
            set.parse_url_query(str2);

            string strBuildNumber =  set["build_number"];

            string strLocale = set["locale"];

            string strSchema = set["schema"];

            string strRuri = set["ruri"];

            if(strBuildNumber.is_empty())
            {

               strBuildNumber = "latest";

            }

                  //Sleep(15 * 1000);
            if(str1 == "ca2login")
            {
               // graphical - 2 - user interface for login - fontopus - through the plugin
               if(!m_psystem->install().is(::null(), strBuildNumber, "application", "app/ca2/fontopus", strLocale, strSchema))
               {
/*                  Sys(m_psystem).install().start(": app=session session_start=app/ca2/fontopus app_type=application install");
#ifdef WINDOWS
                  ::TerminateProcess(::GetCurrentProcess(), 0);
#else
                  kill(0, SIGSTOP);
#endif
                  m_bMainReady = false;*/

                  vsstring strCommandLine(": app=session session_start=app/ca2/fontopus app_type=application install ruri=\"" + strRuri + "\" locale=" + strLocale + " schema=" + strSchema);


#ifdef WINDOWSEX
                  PostMessage(m_phost->::small_ipc_tx_channel::m_oswindow, WM_USER + 100, 1, 1);
#else
                  throw not_implemented(get_app());
#endif
                  Sys(m_psystem).install().start(strCommandLine);

                  m_phost->m_bReload = true;

#ifdef WINDOWSEX
         //          ::TerminateProcess(::GetCurrentProcess(), 0);
#else
            //        kill(0, SIGSTOP);
#endif

                  m_bMainReady = false;



                  return;
               }
               m_strCa2LoginRuri = string(lpszEnd + 1, iCount - (lpszEnd - lpszStart) - 1);
               start_ca2_login();
            }
            else if(str1 == "ca2logout")
            {
               // graphical - 2 - user interface for logout - fontopus - through the plugin
               if(!m_psystem->install().is(::null(), strBuildNumber, "application", "app/ca2/fontopus", strLocale, strSchema))
               {
                  /*
                  Sys(m_psystem).install().start(": app=session session_start=app/ca2/fontopus app_type=application install");
#ifdef WINDOWS
                  ::TerminateProcess(::GetCurrentProcess(), 0);
#else
                  kill(0, SIGSTOP);
#endif
                  m_bMainReady = false;*/

                  vsstring strCommandLine(": app=session session_start=app/ca2/fontopus app_type=application install ruri=\"" + strRuri + "\" locale=" + strLocale + " schema=" + strSchema);

#ifdef WINDOWSEX
                  PostMessage(m_phost->::small_ipc_tx_channel::m_oswindow, WM_USER + 100, 1, 1);
#else
                  throw not_implemented(get_app());
#endif

                  Sys(m_psystem).install().start(strCommandLine);

                  m_phost->m_bReload = true;

#ifdef WINDOWSEX
         //          ::TerminateProcess(::GetCurrentProcess(), 0);
#else
            //        kill(0, SIGSTOP);
#endif

                  m_bMainReady = false;


                  return;
               }
               m_strCa2LogoutRuri = string(lpszEnd + 1, iCount - (lpszEnd - lpszStart) - 1);
               start_ca2_logout();
            }
            else if(str1 == "ca2prompt")
            {
               if(System.url().get_script(get_host_location_url()) == "/non_auth")
               {
                  m_strCa2LoginRuri = System.url().set_script(get_host_location_url(), "/");
                  start_ca2_login();
               }
               else
               {
                  m_puiHost->layout();
                  if(!m_bApp)
                  {
                     while(!*m_pbReady)
                     {
                        Sleep(284);
                     }
                     if(!m_bApp)
                     {
                        printf("Bergedge application is not initialized. Cannot start mplite.");
                        return;
                     }
                  }
                  if(str2.has_char())
                  {

                     string strType = set["app_type"];

                     if(strType.is_empty())
                        strType = "application";

                     if(strId.has_char() && !m_psystem->install().is(::null(), strBuildNumber, strType, strId, strLocale, strSchema))
                     {

                        string strCommandLine;

                        strCommandLine = ": app=session session_start=" + strId;

                        for(int32_t i = 0; i < set.m_propertya.get_count(); i++)
                        {

                           if(!set.m_propertya[i].get_string().has_char()
                           &&
                             (set.m_propertya[i].name() == "build_number"
                           || set.m_propertya[i].name() == "app_type"
                           || set.m_propertya[i].name() == "locale"
                           || set.m_propertya[i].name() == "schema"
                           || set.m_propertya[i].name() == "app"
                           || set.m_propertya[i].name() == "session_start"
                             )
                           )
                              continue;


                           strCommandLine += " ";

                           strCommandLine += set.m_propertya[i].name();

                           if(!set.m_propertya[i].get_string().has_char())
                              continue;

                           strCommandLine += "=";

                           strCommandLine += set.m_propertya[i].get_string();

                        }

                        if(set["app_type"].is_empty())
                           strCommandLine += " app_type=" + strType;

                        strCommandLine += " install";

                        //m_phost->m_pszReloadCommandLine = (const char *) HeapAlloc(GetProcessHeap(), 0, strCommandLine.get_length() + 1);
                        //strncpy((char *) m_phost->m_pszReloadCommandLine, strCommandLine, strCommandLine.get_length() + 1);


#ifdef WINDOWSEX
                        PostMessage(m_phost->::small_ipc_tx_channel::m_oswindow, WM_USER + 100, 1, 1);
#else
                        throw not_implemented(get_app());
#endif

                        Sys(m_psystem).install().start(strCommandLine);

                        m_phost->m_bReload = true;

#ifdef WINDOWSEX
              //          ::TerminateProcess(::GetCurrentProcess(), 0);
#else
                //        kill(0, SIGSTOP);
#endif

                        m_bMainReady = false;

                        return;
                        //m_puiHost->SetTimer(19841115, (1984 + 1977 )* 2, ::null());

                     }
                     else
                     {
                        m_puiHost->KillTimer(19841115);
                        //Sleep(15 * 1000);
   //                     m_psystem->m_puiInitialPlaceHolderContainer = m_puiHost;
                        xxdebug_box("plugin", "open_ca2_string", 0);
                        ::ca::application_bias * pbiasCreate = new ::ca::application_bias;
                        pbiasCreate->m_set["NativeWindowFocus"] = false;
                        pbiasCreate->m_puiParent = m_puiHost;
                        m_psystem->post_fork_uri(str2, pbiasCreate);
                     }
                  }
               }
            }
         }
      }
      catch(installing_exception &)
      {
         m_bReload = true;
/*#ifdef WINDOWS
         ::TerminateProcess(::GetCurrentProcess(), 0);
#else
         kill(0, SIGSTOP);
#endif*/
      }
   }

   void my_se_translator_function(uint32_t, struct _EXCEPTION_POINTERS* )
   {
   }

   bool plugin::finalize()
   {

      if(!m_bAppStarted)
         return true;


      g_bExiting = true;

#ifdef WINDOWS
      _set_se_translator(&my_se_translator_function);
#endif

      try
      {
         m_puiHost->DestroyWindow();
      }
      catch(...)
      {
      }


      try
      {
         System.os().post_to_all_threads(WM_QUIT, 0, 0);
      }
      catch(...)
      {
      }

      Sleep(1984);

      try
      {

         ::ca::thread * pthread = dynamic_cast < ::ca::thread * > (m_psystem->::ca::smart_pointer < ::ca::thread >::m_p);

         if(pthread->get_run())
         {
            if(m_pbReady != ::null())
            {
               *m_pbReady = false;
            }
            pthread->m_pbReady = m_pbReady;
            pthread->m_bRun = false;
            int32_t iRepeat = 0;
            while((m_pbReady != ::null() || !*m_pbReady) && iRepeat < 49)
            {
               Sleep(284);
               iRepeat++;
            }
         }


      }
      catch(...)
      {
      }

      try
      {
         ca2_free(m_pbReady);
      }
      catch(...)
      {
      }

      try
      {
         delete m_psystem;
      }
      catch(...)
      {
      }

#ifdef WINDOWS
      while(true)
      {
         try
         {
            if(!::FreeLibrary(::ca::g_hmoduleOs))
            {
               break;
            }

         }
         catch(...)
         {
         }

      }
#endif


      return true;

   }



#ifdef WINDOWSEX

   LRESULT plugin::message_handler(UINT uiMessage, WPARAM wparam, LPARAM lparam)
   {

      if(m_puiHost != ::null())
      {

         if(uiMessage >= WM_MOUSEFIRST && uiMessage <= WM_MOUSELAST)
         {

            point pt = point(lparam);

            //pt.x -= m_rect.left;

            //pt.y -= m_rect.top;

            lparam = pt.lparam();

         }

         if(uiMessage == WM_MOUSEMOVE)
         {

            sp(::ca::window) pwindow = m_puiHost->m_pimpl;

            pwindow->m_bMouseHover = true; // avoids tracking mouse leave;

         }

         sp(::ca::window) pwindow = m_puiHost->m_pimpl;

         oswindow oswindow = pwindow->get_handle();

         bool bIsWindow = ::IsWindow(oswindow) != FALSE;

         if(bIsWindow)
         {

            ::ca::smart_pointer < ::ca::message::base > spbase;

            spbase = m_puiHost->get_base(m_puiHost, uiMessage, wparam, lparam);

            m_puiHost->message_handler(spbase);

            return spbase->get_lresult();

         }
         else
         {

            return 0;

         }

      }

      return 0;

   }

#elif defined(METROWIN)

   LRESULT plugin::message_handler(UINT uiMessage, WPARAM wparam, LPARAM lparam)
   {

      return 0;

   }

#else

   int32_t plugin::message_handler(XEvent * pevent)
   {

      return 0;

   }

#endif

   bool plugin::os_native_bergedge_start()
   {

      m_bApp = true;

      set_ready();

      return true;

   }

   void plugin::set_window_rect(LPCRECT lpcrect)
   {

      m_rect = *lpcrect;

      if(m_puiHost != ::null())
      {

         m_puiHost->m_bRectOk = true;

         m_puiHost->m_pimpl->m_bRectOk = true;

         m_puiHost->m_rectParentClient =  m_rect;

         m_puiHost->m_rectParentClient.offset(-point64(m_puiHost->m_rectParentClient.top_left()));

         m_puiHost->m_pimpl->m_rectParentClient = m_puiHost->m_rectParentClient;

         rect64 rectWindow64;

         m_puiHost->GetWindowRect(rectWindow64);

         class rect rectWindow;

         rectWindow = rectWindow64;

         m_puiHost->SetWindowPos(ZORDER_TOP, rectWindow.left, rectWindow.top, rectWindow.width(), rectWindow.height(), 0);

         m_puiHost->send_message(WM_SIZE);

         m_puiHost->send_message(WM_MOVE);

      }

   }



} // namespace plugin




