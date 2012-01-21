#include "StdAfx.h"


namespace cube2
{


   application::application()
   {


   }

   application::~application()
   {


   }

   bool application::initialize()
   {

      m_dwAlive = ::GetTickCount();

      if(!cube1::application::initialize())
         return false;

      ensure_app_interest();

      return true;
   }

   BOOL CALLBACK application::GetAppsEnumWindowsProc(
       HWND hwnd,
       LPARAM lParam)
   {
      application * papp = (application *) lParam;
      DWORD dwptr;
      if(!::SendMessageTimeout(hwnd, WM_APP + 2000, 1, 0, SMTO_BLOCK, 10, (PDWORD_PTR) &dwptr) || dwptr != 2)
      {
         return TRUE;
      }
      if(!::SendMessageTimeout(hwnd, WM_APP + 2000, 2, 0, SMTO_BLOCK, 10, (PDWORD_PTR) &dwptr) || dwptr != 4)
      {
         return TRUE;
      }
      if(!::SendMessageTimeout(hwnd, WM_APP + 2000, 4, 0, SMTO_BLOCK, 10, (PDWORD_PTR) &dwptr) || dwptr != 5)
      {
         return TRUE;
      }
      if(!::SendMessageTimeout(hwnd, WM_APP + 2000, 5, 0, SMTO_BLOCK, 10, (PDWORD_PTR) &dwptr) || dwptr != 8)
      {
         return TRUE;
      }
      if(!::SendMessageTimeout(hwnd, WM_APP + 2000, 8, 0, SMTO_BLOCK, 10, (PDWORD_PTR) &dwptr) || dwptr != 11)
      {
         return TRUE;
      }
      if(!::SendMessageTimeout(hwnd, WM_APP + 2000, 11, 0, SMTO_BLOCK, 10, (PDWORD_PTR) &dwptr) || dwptr != 23)
      {
         return TRUE;
      }
      if(!::SendMessageTimeout(hwnd, WM_APP + 2000, 23, 0, SMTO_BLOCK, 10, (PDWORD_PTR) &dwptr) || dwptr != 33)
      {
         return TRUE;
      }
      if(::SendMessageTimeout(hwnd, WM_APP + 2000, 33, 0, SMTO_BLOCK, 10, (PDWORD_PTR) &dwptr))
      {
         if(dwptr == 1)
         {
            if(papp->m_straAppInterest.find_first("command") >= 0)
               papp->m_mapAppInterest["command"] = hwnd;
         }
         else if(dwptr == 67)
         {
            if(papp->m_straAppInterest.find_first("winactionarea") >= 0)
               papp->m_mapAppInterest["winactionarea"] = hwnd;
         }
         else if(dwptr == 68)
         {
            if(papp->m_straAppInterest.find_first("winutil") >= 0)
               papp->m_mapAppInterest["winutil"] = hwnd;
         }
         else if(dwptr == 69)
         {
            if(papp->m_straAppInterest.find_first("windesk") >= 0)
               papp->m_mapAppInterest["windesk"] = hwnd;
         }
      }
      return TRUE;
   }



   void application::update_app_interest()
   {
      EnumWindows(GetAppsEnumWindowsProc, (LPARAM) (application *) (this));
   }

   void application::ensure_app_interest()
   {
      for(int i = 0; i < m_straAppInterest.get_count(); i++)
      {
         if(m_straAppInterest[i] != m_strAppName
            && !::IsWindow(m_mapAppInterest[m_straAppInterest[i]]))
         {
            System.assert_running_local(m_straAppInterest[i]);
         }
      }
   }

   HWND application::get_ca2_app_wnd(const char * psz)
   {
      return m_mapAppInterest[psz];
   }

   bool application::initialize_instance()
   {
      if(!::cube1::application::initialize_instance())
         return false;
      if(m_psession != NULL && m_psession->m_pbergedge != NULL)
      {
         Session.m_pappCurrent = this;
      }
      return true;
   }

   bool application::on_install()
   {
      try
      {
         string strRoot;
         string strDomain;
         if(is_system())
         {
            strRoot     = "app";
            strDomain   = "main";
         }
         else if(is_bergedge())
         {
            strRoot     = "app";
            strDomain   = "bergedge";
         }
         else
         {
            stringa stra;
            stra.add_tokens(App(this).m_strAppName, "_", FALSE);
         
            string strLibrary;
         
            strLibrary = App(this).m_strLibraryName;

            if(strLibrary.has_char() && strLibrary.CompareNoCase("app_" + App(this).m_strAppName) == 0)
               strLibrary.Empty();

            if(strLibrary.has_char())
            {
          
               ::ca2::library library;

               if(library.open(this, strLibrary))
               {
                  stringa straAppList;
                  library.get_app_list(straAppList);
                  if(straAppList.get_count() <= 1)
                     strLibrary.Empty();
               }
               else
               {
                  strLibrary.Empty();
               }

            }

            for(int i = 1; i < stra.get_upper_bound(); i++)
            {
               stra[i] == "_" + stra[i];
            }
            if(stra.get_size() > 1)
            {
               strRoot = "app-" + stra[0];
               stra.remove_at(0);
               if(strLibrary.has_char())
                  stra.insert_at(stra.get_upper_bound(), strLibrary);
               strDomain += stra.implode("/");
            }
            else
            {
               strRoot = "app";
               if(strLibrary.has_char())
                  strDomain = strLibrary + "/";
               strDomain += App(this).m_strAppName;
            }
         }

         update_appmatter(strRoot, strDomain);
      }
      catch(...)
      {
         return false;
      }


      return true;

   }


   bool application::on_uninstall()
   {
      return true;
   }

   void application::on_exclusive_instance_local_conflict()
   {
      m_straAppInterest.add(m_strAppName);
      update_app_interest();
      HWND hwnd = get_ca2_app_wnd(m_strAppName);
      if(hwnd != NULL)
      {
         ::ShowWindow(hwnd, SW_RESTORE);
      }
   }

   int application::send_simple_command(const char * psz, void * osdataSender)
   {
      string strApp;
      stringa stra;
      stra.add_tokens(psz, "::", true);
      if(stra.get_size() > 0)
      {
         strApp = stra[0];
         HWND hwnd = get_ca2_app_wnd(strApp);
         if(hwnd != NULL)
         {
            return send_simple_command((void *) hwnd, psz, osdataSender);
         }
      }
      return -1;
   }

   int application::send_simple_command(void * osdata, const char * psz, void * osdataSender)
   {
      HWND hwnd = (HWND) osdata;
      if(!::IsWindow(hwnd))
         return -1;
      COPYDATASTRUCT cds;
      memset(&cds, 0, sizeof(cds));
      cds.dwData = 198477;
      cds.cbData = strlen(psz);
      cds.lpData = (PVOID) psz;
      return SendMessage(hwnd, WM_COPYDATA, (WPARAM) (HWND) osdataSender, (LPARAM) &cds);
   }

   void application::request(::ca::create_context * pcreatecontext)
   {
      ::cube1::application::request(pcreatecontext);
   }

   bool application::update_appmatter(const char * pszRoot, const char * pszRelative)
   {
      
      gen::international::locale_style localestyle(this);


      
      
      string strLocale;
      string strStyle;

      
      if(Application.directrix().m_varTopicQuery["locale"].has_char() && Application.directrix().m_varTopicQuery["locale"].get_string().CompareNoCase("_std") != 0)
      {
         if(Application.directrix().m_varTopicQuery["style"].has_char() && Application.directrix().m_varTopicQuery["style"].get_string().CompareNoCase("_std") != 0)
         {
            localestyle.add_locale_variant(Application.directrix().m_varTopicQuery["locale"], Application.directrix().m_varTopicQuery["style"]);
         }
         localestyle.add_locale_variant(Application.directrix().m_varTopicQuery["locale"], Application.directrix().m_varTopicQuery["locale"]);
      }
      
      
      if(Application.directrix().m_varTopicQuery["lang"].has_char() && Application.directrix().m_varTopicQuery["lang"].get_string().CompareNoCase("_std") != 0)
      {
         if(Application.directrix().m_varTopicQuery["style"].has_char() && Application.directrix().m_varTopicQuery["style"].get_string().CompareNoCase("_std") != 0)
         {
            localestyle.add_locale_variant(Application.directrix().m_varTopicQuery["lang"], Application.directrix().m_varTopicQuery["style"]);
         }
         localestyle.add_locale_variant(Application.directrix().m_varTopicQuery["lang"], Application.directrix().m_varTopicQuery["lang"]);
      }
      
      
      if(Application.directrix().m_varTopicQuery["style"].has_char() && Application.directrix().m_varTopicQuery["style"].get_string().CompareNoCase("_std") != 0)
      {
         localestyle.add_locale_variant(get_locale(), Application.directrix().m_varTopicQuery["style"]);
      }
      if(get_style().has_char() && get_style().CompareNoCase("_std") != 0 && get_style().CompareNoCase(get_locale()) != 0)
      {
         localestyle.add_locale_variant(get_locale(), Application.directrix().m_varTopicQuery["style"]);
      }
      localestyle.add_locale_variant(get_locale(), get_locale());

      
      localestyle.finalize();

      
      for(int i = 0; i < localestyle.m_straLocale.get_count(); i++)
      {
         update_appmatter(pszRoot, pszRelative, localestyle.m_straLocale[i], localestyle.m_straStyle[i]);
      }


      return true;

   }
   
   bool application::update_appmatter(const char * pszRoot, const char * pszRelative, const char * pszLocale, const char * pszStyle)
   {

      string strLocale;
      string strStyle;
      string strRelative = System.dir().path(System.dir().path(pszRoot, "appmatter", pszRelative), App(this).get_locale_style_dir(pszLocale, pszStyle)) + ".zip";
      string strFile = System.dir().ca2(strRelative);
      string strUrl;
      if(_ca_is_basis())
      {
         strUrl = "http://basis.spaignition.api.laborserver.net/download?authnone&version=basis&stage=";
      }
      else
      {
         strUrl = "http://stage.spaignition.api.laborserver.net/download?authnone&version=stage&stage=";
      }

      strUrl += System.url().url_encode(strRelative);

      Application.http().download(strUrl, strFile);

      System.compress().extract_all(strFile, this);

      return true;

   }




} //namespace cube2
