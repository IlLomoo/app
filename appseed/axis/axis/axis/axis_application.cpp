#include "framework.h"


namespace axis
{


   application::application() :
      m_simpledb(this)
   {

      m_peventReady = NULL;


#ifdef WINDOWS

      m_hinstance                = NULL;

#endif

      m_paxisapp              = this;

      if(m_pauraapp != NULL)
      {

         m_paxissystem           = m_pauraapp->m_paxissystem;

         if(m_pauraapp->m_paxissession == NULL && m_paxissystem != NULL)
         {

            m_paxissession       = m_paxissystem->m_paxissession;

         }
         else
         {

            m_paxissession       = m_pauraapp->m_paxissession;

         }

#ifdef WINDOWS

         m_hinstance             = m_pauraapp->m_hinstance;

#endif

      }
      else
      {

         m_paxissystem           = NULL;

      }

//      m_pcoreapp                 = NULL;

      
      m_http.set_app(this);



      //m_psignal                  = canew(class signal());





      m_bAxisProcessInitialize         = false;
      m_bAxisProcessInitializeResult   = false;

      m_bAxisInitialize1               = false;
      m_bAxisInitialize1Result         = false;

      m_bAxisInitialize                = false;
      m_bAxisInitializeResult          = false;

      m_bAxisInitializeInstance        = false;
      m_bAxisInitializeInstanceResult  = false;






      
      // initialize wait cursor state
      m_iWaitCursorCount         = 0;

      m_bShouldInitializeGTwf = true;
      m_bEnableOnDemandDrawing = true;
      m_bInitializeProDevianMode = true;

      m_pinitmaindata = NULL;



   }

   application::~application()
   {
   }


   void application::assert_valid() const
   {

      thread::assert_valid();


   }

   void application::dump(dump_context & dumpcontext) const
   {

      thread::dump(dumpcontext);

#ifdef WINDOWS

      dumpcontext << "m_hinstance = " << (void *)m_hinstance;

#endif

      //dumpcontext << "\nm_strCmdLine = " << m_strCmdLine;
      //dumpcontext << "\nm_nCmdShow = " << m_nCmdShow;
      dumpcontext << "\nm_bHelpMode = " << m_strAppName;

      dumpcontext << "\n";

   }



   string application::load_string(id id)
   {
      string str;
      if(!load_string(str,id))
      {
         return (const string &)id;
      }
      return str;
   }

   bool application::load_string(string & str,id id)
   {
      if(!load_cached_string(str,id,true))
      {
         return false;
      }
      return true;
   }

   bool application::load_cached_string(string & str,id id,bool bLoadStringTable)
   {
      ::xml::document doc(this);
      if(!doc.load(id))
      {
         return load_cached_string_by_id(str,id,"",bLoadStringTable);
      }
      sp(::xml::node) pnodeRoot = doc.get_root();
      if(pnodeRoot->get_name() == "string")
      {
         string strId = pnodeRoot->attr("id");
         string strValue = pnodeRoot->get_value();
         return load_cached_string_by_id(str,strId,strValue,bLoadStringTable);
      }
      str = doc.get_name();
      return true;
   }

   bool application::load_cached_string_by_id(string & str,id id,const string & pszFallbackValue,bool bLoadStringTable)
   {

      synch_lock sl(&m_mutexStr);

      string strId(id.str());
      string strTable;
      string strString;
      string_to_string * pmap = NULL;
      index iFind = 0;
      if((iFind = strId.find(':')) <= 0)
      {
         strTable = "";
         strString = strId;
      }
      else
      {
         strTable = strId.Mid(0,iFind);
         strString = strId.Mid(iFind + 1);
      }
      if(m_stringtableStd.Lookup(strTable,pmap))
      {
         if(pmap->Lookup(strString,str))
         {
            return true;
         }
      }
      else if(m_stringtable.Lookup(strTable,pmap))
      {
         if(pmap->Lookup(strString,str))
         {
            return true;
         }
      }
      else if(bLoadStringTable)
      {
         load_string_table(strTable,"");
         return load_cached_string_by_id(str,id,pszFallbackValue,false);
      }
      if(pszFallbackValue.is_empty())
         str = strId;
      else
         str = pszFallbackValue;
      return true;
   }

   void application::load_string_table(const string & pszApp,const string & pszId)
   {

      synch_lock sl(&m_mutexStr);

      string strApp(pszApp);
      string strMatter;
      string strLocator;

      if(strApp.is_empty())
      {
         strLocator = System.dir().appmatter_locator(this);
      }
      else
      {
         strLocator = System.dir().appmatter_locator(strApp);
      }

      if(pszId.is_empty())
      {
         strMatter = "stringtable.xml";
      }
      else 
      {
         strMatter = pszId + ".xml";
      }

      string strTableId = strApp;

      if(pszId.has_char() && *pszId != '\0')
      {
         strTableId += "\\";
         strTableId += pszId;
      }

      ::xml::document doc(get_app());
      string strFilePath = System.dir().matter_from_locator(Sess(this).str_context(),strLocator,strMatter);
      if(!System.file().exists(strFilePath,this))
      {
         try
         {
            if(m_stringtable[pszId] != NULL)
               delete m_stringtable[pszId];
         }
         catch(...)
         {
         }
         m_stringtable.set_at(pszId,new string_to_string);
         return;
      }
      string strFile = Application.file().as_string(strFilePath);
      if(!doc.load(strFile))
         return;
      string_to_string * pmapNew = new string_to_string;
      for(int32_t i = 0; i < doc.get_root()->children().get_count(); i++)
      {
         string strId = doc.get_root()->child_at(i)->attr("id");
         string strValue = doc.get_root()->child_at(i)->get_value();
         pmapNew->set_at(strId,strValue);
      }

      string_to_string * pmapOld = m_stringtable[strTableId];

      m_stringtable[strTableId] = NULL;

      if(pmapOld != NULL)
      {

         try
         {

            delete pmapOld;

         }
         catch(...)
         {

         }

      }

      m_stringtable[strTableId] = pmapNew;
      ASSERT(m_stringtable[strTableId] == pmapNew);
   }




   void application::load_string_table()
   {
      load_string_table("","");
   }


   //sp(object) application::alloc(sp(type) info)
   //{
   //   return System.alloc(this,info);
   //}

   //sp(object) application::alloc(const  id & idType)
   //{
   //   return System.alloc(this,idType);
   //}

   bool application::is_system()
   {

      return false;

   }

   bool application::is_session()
   {

      return false;

   }


   bool application::is_serviceable()
   {

      return false;

   }
   

   //bool application::app_map_lookup(const char * psz,void * & p)
   //{
   //   return m_appmap.Lookup(psz,p) != FALSE;
   //}

   //void application::app_map_set(const char * psz,void * p)
   //{
   //   m_appmap.set_at(psz,p);
   //}


   //sp(::command_thread) application::command_central()
   //{
   //   return m_pcommandthread;
   //}

   //sp(::command_thread) application::command_thread()
   //{
   //   return m_pcommandthread;
   //}

   //sp(::command_thread) application::command()
   //{
   //   return m_pcommandthread;
   //}

   //sp(::command_thread) application::guideline()
   //{
   //   return m_pcommandthread;
   //}

   //sp(::command_thread) application::directrix()
   //{
   //   return m_pcommandthread;
   //}

   //sp(::command_thread) application::axiom()
   //{
   //   return m_pcommandthread;
   //}

   bool application::verb()
   {
      axiom()->run();
      return true;
   }

   //sp(::command_thread) application::creation()
   //{
   //   return m_pcommandthread;
   //}

//   class open_url
//   {
//   public:
//      string m_strLink;
//      string m_strTarget;
//      open_url(const string & strLink,const string & pszTarget);
//      bool open();
//   };
//
//
//   open_url::open_url(const string & strLink,const string & pszTarget)
//   {
//      m_strLink = strLink;
//      m_strTarget = pszTarget;
//   }
//
//
//   uint32_t c_cdecl thread_proc_open_url(void * p)
//   {
//
//      open_url * popenurl = (open_url *) p;
//
//      if(!popenurl->open())
//         return -1;
//
//      return 0;
//   }
//
//   bool open_url::open()
//   {
//      string strLink = m_strLink;
//      string pszTarget = m_strTarget;
//#ifdef WINDOWSEX
//      string strUrl = strLink;
//      if(!::str::begins_ci(strUrl,"http://")
//         && !::str::begins_ci(strUrl,"https://"))
//      {
//         strUrl = "http://" + strUrl;
//      }
//      ::ShellExecuteA(NULL,"open",strUrl,NULL,NULL,SW_SHOW);
//      return true;
//#elif defined METROWIN
//#pragma push_macro("System")
//#undef System
//      ::Windows::Foundation::Uri ^ uri = ref new ::Windows::Foundation::Uri(strLink);
//      ::Windows::System::LauncherOptions ^ options = ref new ::Windows::System::LauncherOptions();
//      options->TreatAsUntrusted = false;
//      bool success = ::wait(::Windows::System::Launcher::LaunchUriAsync(uri,options));
//
//      return success;
//
//#pragma pop_macro("System")
//#elif defined(LINUX)
//      ::system("xdg-open " + strLink);
//      return true;
//#elif defined(APPLEOS)
//      openURL(strLink);
//      return true;
//#else
//      throw not_implemented(get_thread_app());
//#endif
//
//   }


   //bool application::open_link(const string & strLink,const string & pszTarget)
   //{
   //   if(is_system())
   //   {
   //      return __begin_thread(this,thread_proc_open_url,new open_url(strLink, pszTarget)) != FALSE;
   //   }
   //   else
   //   {
   //      return System.open_link(strLink,pszTarget);
   //   }

   //   return false;

   //}


   //ptr_array < ::user::interaction > application::frames()
   //{

   //   return m_framea.axis_ptra();

   //}


   //void application::add_frame(sp(::user::interaction) pwnd)
   //{

   //   synch_lock sl(&m_framea.m_mutex); // recursive lock (on m_framea.add(pwnd)) but m_puiMain is "cared" by m_frame.m_mutex

   //   m_framea.add(pwnd);

   //   if(m_puiMain == NULL)
   //   {

   //      m_puiMain = pwnd;

   //   }

   //}


   //void application::remove_frame(sp(::user::interaction) pwnd)
   //{

   //   synch_lock sl(&m_framea.m_mutex); // recursive lock (on m_framea.remove(pwnd)) but m_puiMain is "cared" by m_frame.m_mutex

   //   if(m_puiMain == pwnd)
   //   {

   //      m_puiMain = NULL;

   //   }

   //   m_framea.remove(pwnd);

   //}




//#if defined(METROWIN) || defined(APPLE_IOS)
//
//   sp(::user::interaction) application::window_from_os_data(void * pdata)
//   {
//
//      return window_from_handle((oswindow)pdata);
//
//   }
//
//#else
//
//   sp(::user::interaction) application::window_from_os_data(void * pdata)
//   {
//
//      return window_from_handle((oswindow)pdata);
//
//   }
//
//#endif



   //void application::DoWaitCursor(int32_t nCode)
   //{

   //   if(nCode < 0)
   //   {

   //      m_iWaitCursorCount = 0;
   //      ShowWaitCursor(false);

   //   }
   //   else if(nCode == 0)
   //   {

   //      if(m_iWaitCursorCount > 0)
   //      {
   //         m_iWaitCursorCount--;
   //      }

   //      if(m_iWaitCursorCount > 0)
   //      {

   //         ShowWaitCursor(true);

   //      }

   //      m_iWaitCursorCount = 0;
   //      ShowWaitCursor(false);

   //   }
   //   else
   //   {

   //      if(m_iWaitCursorCount < 0)
   //      {
   //         m_iWaitCursorCount = 0;
   //      }

   //      m_iWaitCursorCount++;

   //      ShowWaitCursor(true);

   //   }



   //}

   //void application::ShowWaitCursor(bool bShow)
   //{

   //   if(m_pimpl == NULL)
   //      return;

   //   m_pimpl->ShowWaitCursor(bShow);

   //}



   void application::construct(const char * pszAppId)
   {

   }



   //void application::TermThread(HINSTANCE hInstTerm)
   //{

   //   ::exception::throw_interface_only(this);

   //}



   //sp(::user::interaction) application::FindWindow(const char * lpszClassName,const char * lpszWindowName)
   //{

   //   ::exception::throw_interface_only(this);

   //}


   //sp(::user::interaction) application::FindWindowEx(oswindow oswindowParent,oswindow oswindowChildAfter,const char * lpszClass,const char * lpszWindow)
   //{

   //   ::exception::throw_interface_only(this);

   //}


   //string application::get_version()
   //{

   //   ::exception::throw_interface_only(this);

   //}


   void application::SetCurrentHandles()
   {

      ::aura::application::SetCurrentHandles();


      if(is_installing() || is_uninstalling())
      {

         if(is_system())
         {

            System.install().trace().initialize();

            System.install().m_progressApp.m_scalar                           = int_scalar(&System.install(),scalar_app_install_progress);

            System.install().::int_scalar_source::m_plistener                 = &System.install().m_progressApp;

            System.install().m_progressApp.m_plistener                        = &System.install().trace();

            System.install().m_progressApp.m_dProgressStart                   = 0.84;

            System.install().m_progressApp.m_dProgressEnd                     = 0.998;

            System.install().m_iProgressAppInstallStart                       = 0;

            if(directrix()->m_varTopicQuery["session_start"] == "session")
            {

               System.install().m_iProgressAppInstallEnd = 2 * 5;

            }
            else
            {

               System.install().m_iProgressAppInstallEnd = 3 * 5;

            }

         }

      }

   }


   //void application::set_env_var(const string & var,const string & value)
   //{

   //   ::exception::throw_interface_only(this);

   //}




   //bool application::_001OnDDECommand(const char * lpcsz)
   //{

   //   ::exception::throw_interface_only(this);

   //   return false;

   //}


   //void application::_001EnableShellOpen()
   //{

   //   ::exception::throw_interface_only(this);

   //}


   //::aura::document * application::_001OpenDocumentFile(var varFile)
   //{

   //   ::exception::throw_interface_only(this);

   //   return NULL;

   //}


   //void application::_001OnFileNew(signal_details * pobj)
   //{

   //   ::exception::throw_interface_only(this);

   //}


   //sp(::aura::printer) application::get_printer(const char * pszDeviceName)
   //{

   //   ::exception::throw_interface_only(this);

   //}


   //bool application::update_module_paths()
   //{


   //   if(is_system())
   //   {

   //      if(!m_pimpl->update_module_paths())
   //         return false;


   //   }

   //   return true;


   //}



   string application::veriwell_multimedia_music_midi_get_default_library_name()
   {

      if(m_pimpl == NULL)
         return "";

      return m_pimpl->veriwell_multimedia_music_midi_get_default_library_name();

   }



   string application::multimedia_audio_mixer_get_default_library_name()
   {

      if(m_pimpl == NULL)
         return "";

      return m_pimpl->multimedia_audio_mixer_get_default_library_name();

   }



   string application::multimedia_audio_get_default_library_name()
   {

      if(m_pimpl == NULL)
         return "";

      return m_pimpl->multimedia_audio_get_default_library_name();

   }



   string application::draw2d_get_default_library_name()
   {

      if(m_pimpl == NULL)
         return "draw2d_cairo";

      return m_pimpl->draw2d_get_default_library_name();

   }









/*
   void application::process_machine_event_data(machine_event_data * pdata)
   {
      if(pdata->m_fixed.m_bRequestCloseApplication)
      {
         _001CloseApplication();
      }
   }
   */

   void application::_001CloseApplication()
   {

      throw todo(get_app());

   }





//   sp(::user::interaction) application::release_capture_uie()
//   {
//
//#if defined(LINUX)
//
//      oswindow oswindowCapture = ::GetCapture();
//      if (oswindowCapture == NULL)
//         return NULL;
//      return oswindowCapture->get_user_interaction()->ReleaseCapture();
//
//#elif defined(WINDOWS)
//
//      oswindow oswindowCapture = ::GetCapture();
//      if(oswindowCapture == NULL)
//         return NULL;
//      return System.window_from_os_data(oswindowCapture)->ReleaseCapture();
//
//#elif defined(APPLEOS)
//
//      oswindow oswindowCapture = ::GetCapture();
//      if (oswindowCapture == NULL)
//         return NULL;
//      return oswindowCapture->get_user_interaction()->ReleaseCapture();
//
//#else
//
//      ::exception::throw_not_implemented(get_app());
//
//#endif
//
//   }
//
//
//   sp(::user::interaction) application::get_capture_uie()
//   {
//
//#if defined(METROWIN)
//
//      oswindow oswindowCapture = ::GetCapture();
//
//      if (oswindowCapture == NULL)
//         return NULL;
//
//      ::user::interaction * pui = oswindowCapture->window();
//
//      if (pui == NULL)
//         return NULL;
//
//      return pui->GetCapture();
//
//#elif defined(WINDOWS) || defined(APPLE_IOS)
//
//      oswindow oswindowCapture = ::GetCapture();
//
//      if(oswindowCapture == NULL)
//         return NULL;
//
//      sp(::user::interaction) pui = System.window_from_os_data(oswindowCapture);
//
//      if(pui == NULL)
//         return NULL;
//
//      return pui->GetCapture();
//
//#else
//
//      //      ::exception::throw_not_implemented(get_app());
//
//      oswindow oswindowCapture = ::GetCapture();
//
//      if (oswindowCapture == NULL)
//         return NULL;
//
//      return ::GetCapture()->get_user_interaction()->m_pimpl->GetCapture();
//
//#endif
//
//   }
//
//
//
//
//
//
//
//   uint32_t application::get_thread_id()
//   {
//      return m_pimpl->get_thread_id();
//   }
//
//
//

//
//#ifndef METROWIN
//
//   void application::get_time(timeval *p)
//   {
//      m_pimpl->get_time(p);
//   }
//
//#endif
//

   //bool application::do_prompt_file_name(var & varFile,UINT nIDSTitle,uint32_t lFlags,bool bOpenFileDialog,sp(::aura::impact_system) ptemplate, ::aura::document * pdocument)
   //{

   //   UNREFERENCED_PARAMETER(varFile);
   //   UNREFERENCED_PARAMETER(nIDSTitle);

   //   return false;

   //}

   //string CLASS_DECL_AXIS application::get_cred(const string & strRequestUrl,const RECT & rect,string & strUsername,string & strPassword,string strToken,string strTitle,bool bInteractive)
   //{

   //   return ::fontopus::get_cred(this,strRequestUrl, rect,strUsername,strPassword,strToken,strTitle,bInteractive);

   //}




   bool application::get_temp_file_name_template(string & strRet,const char * pszName,const char * pszExtension,const char * pszTemplate)
   {

#ifdef METROWIN

      string str(::Windows::Storage::ApplicationData::Current->TemporaryFolder->Path);

#else

      char lpPathBuffer[MAX_PATH * 4];

      uint32_t dwRetVal = GetTempPath(sizeof(lpPathBuffer),lpPathBuffer);

      if(dwRetVal > sizeof(lpPathBuffer) || (dwRetVal == 0))
      {

         return FALSE;

      }

      string str(lpPathBuffer);

#endif

      char bufItem[64];

      string strRelative;

      SYSTEMTIME st;

      memset_dup(&st,0,sizeof(st));

      GetSystemTime(&st);

      itoa_dup(bufItem,st.wYear,10);
      zero_pad(bufItem,4);
      strRelative += bufItem;

      itoa_dup(bufItem,st.wMonth,10);
      zero_pad(bufItem,2);
      strRelative += "-";
      strRelative += bufItem;

      itoa_dup(bufItem,st.wDay,10);
      zero_pad(bufItem,2);
      strRelative += "-";
      strRelative += bufItem;

      itoa_dup(bufItem,st.wHour,10);
      zero_pad(bufItem,2);
      strRelative += " ";
      strRelative += bufItem;

      itoa_dup(bufItem,st.wMinute,10);
      zero_pad(bufItem,2);
      strRelative += "-";
      strRelative += bufItem;

      itoa_dup(bufItem,st.wSecond,10);
      zero_pad(bufItem,2);
      strRelative += "-";
      strRelative += bufItem;

      for(int32_t i = 0; i < (1024 * 1024); i++)
      {
         strRet = ::file::path(str) / (strRelative + "-" + hex::lower_from(i + 1)) / (string(pszName) + string(".") + pszExtension);
         if(pszTemplate != NULL)
         {
            if(System.install().is_file_ok(strRet,pszTemplate,""))
               return true;
         }

         if(file_exists_dup(strRet))
         {

            try
            {

               Application.file().del(strRet);

            }
            catch(...)
            {

               continue;

            }

            return true;

         }
         else
         {
            return true;
         }
      }
      return false;

   }


   bool application::get_temp_file_name(string & strRet,const char * pszName,const char * pszExtension)
   {

      return get_temp_file_name_template(strRet,pszName,pszExtension,NULL);

   }






/*

   ::visual::icon * application::set_icon(object * pobject,::visual::icon * picon,bool bBigIcon)
   {

      ::visual::icon * piconOld = get_icon(pobject,bBigIcon);

      if(bBigIcon)
      {

         pobject->oprop("big_icon").operator =((sp(object)) picon);

      }
      else
      {

         pobject->oprop("small_icon").operator =((sp(object)) picon);

      }

      return piconOld;

   }


   ::visual::icon * application::get_icon(object * pobject,bool bBigIcon) const
   {

      if(bBigIcon)
      {

         return const_cast <object *> (pobject)->oprop("big_icon").cast < ::visual::icon >();

      }
      else
      {

         return const_cast <object *> (pobject)->oprop("small_icon").cast < ::visual::icon >();

      }

   }
*/


   bool application::final_handle_exception(::exception::exception & e)
   {
      UNREFERENCED_PARAMETER(e);
      //linux      exit(-1);

      if(!is_system())
      {

         // get_app() may be it self, it is ok...
         if(Sys(get_app()).final_handle_exception((::exception::exception &) e))
            return true;


      }

      return false;
   }





   int32_t application::main()
   {

      TRACE(string(typeid(*this).name()) + " main");;

      dappy(string(typeid(*this).name()) + " : application::main 1");

      try
      {

         TRACE(string(typeid(*this).name()) + " on_run");;
         dappy(string(typeid(*this).name()) + " : going to on_run : " + ::str::from(m_iReturnCode));
         m_iReturnCode = 0;
         m_bReady = true;
         m_bRun = true;
         m_iReturnCode = on_run();
         if(m_iReturnCode != 0)
         {
            dappy(string(typeid(*this).name()) + " : on_run failure : " + ::str::from(m_iReturnCode));
            ::OutputDebugStringW(L"application::main on_run termination failure");
         }

      }
      catch(::exit_exception &)
      {

         dappy(string(typeid(*this).name()) + " : on_run exit_exception");

         System.post_thread_message(WM_QUIT,0,0);

         goto exit_application;

      }
      catch(...)
      {

         dappy(string(typeid(*this).name()) + " : on_run general exception");

         goto exit_application;

      }

      try
      {

         if(is_system())
         {

            dappy(string(typeid(*this).name()) + " : quiting main");

            //post_to_all_threads(WM_QUIT,0,0);

            //Sleep(5000);

         }

      }
      catch(...)
      {

      }

   exit_application:

      try
      {

         m_iReturnCode = exit();

      }
      catch(::exit_exception &)
      {

         post_to_all_threads(WM_QUIT,0,0);

         m_iReturnCode = -1;

      }
      catch(...)
      {

         m_iReturnCode = -1;

      }

      return m_iReturnCode;

   }




   bool application::pre_run()
   {

      TRACE(string(typeid(*this).name()) + " main_start");;
      try
      {

         m_dwAlive = ::get_tick_count();
         TRACE(string(typeid(*this).name()) + "application_pre_run");;
         int32_t m_iReturnCode = application_pre_run();
         if(m_iReturnCode != 0)
         {
            dappy(string(typeid(*this).name()) + " : applicationpre_run failure : " + ::str::from(m_iReturnCode));
            m_bReady = true;
            TRACE("application::main application_pre_run failure");
            return false;
         }

         xxdebug_box("pre_runnned","pre_runnned",MB_ICONINFORMATION);
         dappy(string(typeid(*this).name()) + " : pre_runned : " + ::str::from(m_iReturnCode));
         TRACE(string(typeid(*this).name()) + " initial_check_directrix");;
         if(!initial_check_directrix())
         {
            dappy(string(typeid(*this).name()) + " : initial_check_directrix failure");
            m_iReturnCode = -1;
            exit();
            m_bReady = true;
            ::OutputDebugStringW(L"exiting on check directrix");
            return false;
         }


         TRACE(string(typeid(*this).name()) + " os_native_bergedge_start");;
         m_dwAlive = ::get_tick_count();
         if(!os_native_bergedge_start())
         {
            dappy(string(typeid(*this).name()) + " : os_native_bergedge_start failure");
            exit();
            m_iReturnCode = -1;
            m_bReady = true;
            ::OutputDebugStringW(L"application::main os_native_bergedge_start failure");
            return false;
         }

         return true;
      }
      catch(::exit_exception &)
      {

         dappy(string(typeid(*this).name()) + " : main_start exit_exception");

         post_to_all_threads(WM_QUIT,0,0);

      }

      return false;

   }




   int32_t application::on_run()
   {
      int32_t m_iReturnCode = 0;

      try
      {
         ::aura::application_signal_details signal(m_psignal,::aura::application_signal_start);
         m_psignal->emit(&signal);
      }
      catch(...)
      {
      }

      dappy(string(typeid(*this).name()) + " : starting on_run : " + ::str::from(m_iReturnCode));

      thread * pthread = ::get_thread();

      install_message_handling(pthread->m_pthreadimpl);

      dappy(string(typeid(*this).name()) + " : starting on_run 2 : " + ::str::from(m_iReturnCode));

      try
      {
         try
         {
            m_bReady = true;
            if(m_peventReady != NULL)
               m_peventReady->SetEvent();
         }
         catch(...)
         {
         }
      run:
         try
         {
            m_iReturnCode = run();

         }
         catch(::exit_exception & e)
         {

            throw e;

         }
         catch(const ::exception::exception & e)
         {
            if(on_run_exception((::exception::exception &) e))
               goto run;
            if(final_handle_exception((::exception::exception &) e))
               goto run;
            try
            {
               m_iReturnCode = exit();
            }
            catch(::exit_exception & e)
            {

               throw e;

            }
            catch(...)
            {
               m_iReturnCode = -1;
            }
            goto InitFailure;
         }
      }
      catch(::exit_exception & e)
      {

         throw e;

      }
      catch(...)
      {
         // linux-like exit style on crash, differently from general windows error message approach
         // to prevent or correct from crash, should:
         // - look at dumps - to do;
         // - look at trace and log - always doing;
         // - look at debugger with the visual or other tool atashed - to doing;
         // - fill other possibilities;
         // - restart and send information in the holy back, and stateful or self-heal as feedback from below;
         // - ...
         // - ..
         // - .
         // - .
         // - .
         // - .
         // -  .
         // - ...
         // - ...
         // - ...
         // to pro-activia and overall benefits workaround:
         // - stateful applications:
         //      - browser urls, tabs, entire history, in the ca2computing cloud;
         //      - uint16_t - html document to simplify here - with all history of undo and redos per document optimized by cvs, svn, syllomatter;
         //           - not directly related but use date and title to name document;
         //      - save forms after every key press in .undo.redo.form file parallel to appmatter / form/undo.redo.file parity;
         //      - last ::ikaraoke::karaoke song and scoring, a little less motivated at time of writing;
         //
         // - ex-new-revolut-dynamic-news-self-healing
         //      - pre-history, antecendentes
         //            - sometimes we can't recover from the last state
         //            - to start from the beggining can be too heavy, waity, worky, bory(ing)
         //      - try to creativetily under-auto-domain with constrained-learning, or heuristcally recover from restart, shutdown, login, logoff;
         //           - reification :
         //           - if the document is corrupted, try to open the most of it
         //           - if can only detect that the document cannot be opened or damaged, should creatively workarounds as it comes, as could it be
         //              done, for example search in the web for a proper solution?
         //           - ::ikaraoke::karaoke file does not open? can open next? do it... may animate with a temporary icon...
         //           - import a little as pepper for the meal, prodevian technology into estamira, so gaming experience relativity can open ligh
         //               speed into cartesian dimensions of
         //               core, estamira and prodevian. Take care not to flood prodevian brand black ink over the floor of the estamira office...
         //               black letters, or colorful and pink are accepted and sometimes desired, for example, hello kity prodevian, pirarucu games,
         //               I think no one likes to be boring, but a entire background in black... I don't know... only for your personal office, may be...
         //           - could an online colaborator investigate crashes promptly in a funny way, and make news and jokes? Like terra and UOL for the real world?
         //               - new crash, two documents lost, weathers fault, too hot, can't think, my mother was angry with me, lead to buggy code;
         //               - new version with bug fixes;
         //      - new versions
         //      - automatic updates
         //      - upgrades
         //      - rearrangemntes
         //      - downgrade in the form of retro
         // - ...
         // - ..
         // - .
         // - .
         // - .
         // - .
         // -  .
         // - ...
         // - ...
         // - ...

      }
   InitFailure:
      try
      {
         if(m_peventReady != NULL)
            m_peventReady->SetEvent();
      }
      catch(...)
      {
      }
      try
      {
         thread * pthread = this;
         if(pthread != NULL && pthread->m_pbReady != NULL)
         {
            *pthread->m_pbReady = true;
         }
      }
      catch(...)
      {
      }
      /*try
      {
      thread * pthread = dynamic_cast < thread * > (this);
      ::SetEvent((HANDLE) pthread->m_peventReady);
      }
      catch(...)
      {
      }*/

      // let translator run undefinetely
      /*if(is_system())
      {
      translator::detach();
      }*/

      return m_iReturnCode;
   }




   int32_t application::application_pre_run()
   {

      return ::aura::application::application_pre_run();

   }



   bool application::InitApplication()
   {

      return true;

   }



   bool application::initial_check_directrix()
   {

      if(directrix()->m_varTopicQuery.has_property("install"))
      {

         if(!on_install())
         {
            System.m_iReturnCode = -1;
            return false;
         }

         string strId = m_strAppId;

         xxdebug_box("on_install1",strId,0);

         if(strId.is_empty())
            strId = m_strAppName;

         if(strId.has_char() && command()->m_varTopicQuery.has_property("app") && strId == command()->m_varTopicQuery["app"])
         {

            system_add_app_install(strId);

         }
         else if(strId.has_char() && command()->m_varTopicQuery.has_property("session_start") && strId == command()->m_varTopicQuery["session_start"])
         {

            system_add_app_install(strId);

         }
         else if(m_strInstallToken.has_char())
         {

            system_add_app_install(m_strInstallToken);

         }

      }
      else if(directrix()->m_varTopicQuery.has_property("uninstall"))
      {

         if(!on_uninstall())
            return false;

         System.install().remove_spa_start(m_strInstallType,m_strInstallToken);

      }

      string strLicense = get_license_id();

      var & varTopicQuey = System.directrix()->m_varTopicQuery;

      bool bHasInstall = varTopicQuey.has_property("install");

      bool bHasUninstall = varTopicQuey.has_property("uninstall");

      if(!(bHasInstall || bHasUninstall)
         && m_bLicense
         && strLicense.has_char())
      {

         if(!Session.assert_user_logged_in())
         {
            return false;
         }

         // call application's is_licensed function
         // because if delay is needed for authentication -
         // or either asking for authentication -
         // current application startup won't be
         // exited by timeout.

         int32_t iRetry = 1;

      retry_license:

         iRetry--;

         if(!Session.is_licensed(strLicense))
         {

            if(iRetry > 0)
               goto retry_license;

            return false;

         }

      }


      return true;

   }


   bool application::on_install()
   {

      if(!aura::application::on_install())
         return false;

      return true;

   }


   bool application::on_uninstall()
   {

      bool bOk = aura::application::on_uninstall();

      return bOk;

   }


   bool application::system_add_app_install(const char * pszId)
   {

      synch_lock sl(System.m_spmutexSystemAppData);

      string strId(pszId);
      string strSystemLocale = System.m_strLocale;
      string strSystemSchema = System.m_strSchema;
      stringa straLocale = command()->m_varTopicQuery["locale"].stra();
      stringa straSchema = command()->m_varTopicQuery["schema"].stra();

      System.install().remove_spa_start(m_strInstallType,strId);
      System.install().add_app_install(System.command()->m_varTopicQuery["build_number"],m_strInstallType,strId,strSystemLocale,m_strSchema);
      System.install().add_app_install(System.command()->m_varTopicQuery["build_number"],m_strInstallType,strId,strSystemLocale,strSystemSchema);
      System.install().add_app_install(System.command()->m_varTopicQuery["build_number"],m_strInstallType,strId,m_strLocale,m_strSchema);

      for(index iLocale = 0; iLocale < straLocale.get_count(); iLocale++)
      {

         System.install().add_app_install(System.command()->m_varTopicQuery["build_number"],m_strInstallType,strId,straLocale[iLocale],m_strSchema);

      }

      for(index iSchema = 0; iSchema < straSchema.get_count(); iSchema++)
      {

         System.install().add_app_install(System.command()->m_varTopicQuery["build_number"],m_strInstallType,strId,m_strLocale,straSchema[iSchema]);

      }

      for(index iLocale = 0; iLocale < straLocale.get_count(); iLocale++)
      {

         for(index iSchema = 0; iSchema < straSchema.get_count(); iSchema++)
         {

            System.install().add_app_install(System.command()->m_varTopicQuery["build_number"],m_strInstallType,strId,straLocale[iLocale],straSchema[iSchema]);

         }

      }

      System.install().add_app_install(System.command()->m_varTopicQuery["build_number"],m_strInstallType,strId,strSystemLocale,"");
      System.install().add_app_install(System.command()->m_varTopicQuery["build_number"],m_strInstallType,strId,m_strLocale,"");

      for(index iLocale = 0; iLocale < straLocale.get_count(); iLocale++)
      {

         System.install().add_app_install(System.command()->m_varTopicQuery["build_number"],m_strInstallType,strId,straLocale[iLocale],"");

      }

      System.install().add_app_install(System.command()->m_varTopicQuery["build_number"],m_strInstallType,strId,"",m_strSchema);
      System.install().add_app_install(System.command()->m_varTopicQuery["build_number"],m_strInstallType,strId,"",strSystemSchema);

      for(index iSchema = 0; iSchema < straSchema.get_count(); iSchema++)
      {

         System.install().add_app_install(System.command()->m_varTopicQuery["build_number"],m_strInstallType,strId,"",straSchema[iSchema]);

      }


      System.install().add_app_install(System.command()->m_varTopicQuery["build_number"],m_strInstallType,strId,"","");


      return true;

   }



   bool application::os_native_bergedge_start()
   {

      return true;

   }











   void application::on_request(sp(::create) pcreatecontext)
   {

      ::object::on_request(pcreatecontext);

      command()->consolidate(pcreatecontext);

   }


   int32_t application::run()
   {

      return ::aura::application::run();

   }




   sp(::aura::application) application::assert_running(const char * pszAppId)
   {

      sp(::aura::application) papp;

      papp = Session.m_appptra.find_running_defer_try_quit_damaged(pszAppId);

      if(papp.is_null())
      {

         sp(::create) spcreatecontext(allocer());

         papp = Session.start_application("application",pszAppId,spcreatecontext);

      }

      return papp;

   }




/*
   typedef  void(*PFN_ca2_factory_exchange)(sp(application) papp);

*/


   /*::file::binary_buffer_sp application::friendly_get_file(var varFile, UINT nOpenFlags)
   {

   try
   {

   return m_file.get_file(varFile, nOpenFlags);

   }
   catch (::file::exception & e)
   {

   string strMessage = e.get_message();

   App(this).simple_message_box(NULL, strMessage, MB_OK);

   return NULL;

   }

   }
   */




   //bool application::is_installing()
   //{

   //   return directrix()->has_property("install");

   //}


   //bool application::is_uninstalling()
   //{

   //   return directrix()->has_property("uninstall");

   //}


   //bool application::create_new_service()
   //{

   //   if(m_pservice != NULL)
   //      return false;

   //   m_pservice = allocate_new_service();

   //   if(m_pservice == NULL)
   //      return false;

   //   return true;

   //}



   //bool application::create_service()
   //{

   //   return System.os().create_service(this);

   //}

   //bool application::remove_service()
   //{

   //   return System.os().remove_service(this);

   //}

   //bool application::start_service()
   //{

   //   return System.os().start_service(this);

   //}

   //bool application::stop_service()
   //{

   //   return System.os().stop_service(this);

   //}


   //void application::on_service_request(sp(::create) pcreatecontext)
   //{

   //   if(!is_serviceable())
   //      return;

   //   if(pcreatecontext->m_spCommandLine->m_varQuery.has_property("create_service"))
   //   {
   //      create_service();
   //   }
   //   else if(pcreatecontext->m_spCommandLine->m_varQuery.has_property("start_service"))
   //   {
   //      start_service();
   //   }
   //   else if(pcreatecontext->m_spCommandLine->m_varQuery.has_property("stop_service"))
   //   {
   //      stop_service();
   //   }
   //   else if(pcreatecontext->m_spCommandLine->m_varQuery.has_property("remove_service"))
   //   {
   //      remove_service();
   //   }


   //}







   bool application::process_initialize()
   {

      if(m_bAxisProcessInitialize)
         return m_bAxisProcessInitializeResult;

      m_bAxisProcessInitialize = true;
      m_bAxisProcessInitializeResult = false;




      if(!::aura::application::process_initialize())
         return false;
      

      //if(m_pthreadimpl == NULL)
      //{

      //   m_pthreadimpl.alloc(allocer());

      //   m_pthreadimpl->m_pthread = this;

      //}

      //m_pimpl.alloc(allocer());

      //m_pimpl->construct(NULL);

      //m_pimpl->m_pimpl = this;

      //if(::get_thread() == NULL)
      //{
      //   set_thread(dynamic_cast <thread *> (this));
      //}

      //if(is_system())
      //{

      //   if(!update_module_paths())
      //      return false;

      //}


      //if(!ca_process_initialize())
      //   return false;

      if(is_system())
      {

         draw2d_factory_exchange();

      }

      //if(!m_pimpl->process_initialize())
      //   return false;


      m_bAxisProcessInitializeResult = true;

      return true;

   }


   bool application::initialize_instance()
   {

      if(m_bAxisInitializeInstance)
         return m_bAxisInitializeInstanceResult;

      m_bAxisInitializeInstance = true;
      m_bAxisInitializeInstanceResult = false;

      if(!::aura::application::initialize_instance())
      {

         return false;

      }

      m_bAxisInitializeInstanceResult = true;

      return true;

   }


   bool application::initialize1()
   {

      if(m_bAxisInitialize1)
         return m_bAxisInitialize1Result;

      m_simpledb.construct(this);

      if(!m_simpledb.initialize())
         return false;

      if(!m_simpledb.initialize2())
         return false;

      set_data_server(simpledb().get_data_server());

      m_bAxisInitialize1 = true;

      m_bAxisInitialize1Result = false;

      m_dwAlive = ::get_tick_count();

      if(!::aura::application::initialize1())
         return false;

      m_bAxisInitialize1Result = true;

      return true;

   }


   bool application::initialize2()
   {

      if(!m_pimpl->initialize2())
         return false;

      if(!ca_initialize2())
         return false;

      return true;

   }


   bool application::initialize3()
   {

      if(!m_pimpl->initialize3())
         return false;

      if(!ca_initialize3())
         return false;

      return true;

   }


   bool application::initialize()
   {

      if(m_bAxisInitialize)
         return m_bAxisInitializeResult;

      m_bAxisInitialize = true;
      m_bAxisInitializeResult = false;

      ::aura::application_signal_details signal(m_psignal,::aura::application_signal_initialize);

      m_psignal->emit(&signal);

      if(!signal.m_bOk)
         return false;

      m_dwAlive = ::get_tick_count();

      if(is_system())
      {
         if(guideline()->m_varTopicQuery.propset().has_property("save_processing"))
         {
            Session.savings().save(::aura::resource_processing);
         }
         if(guideline()->m_varTopicQuery.propset().has_property("save_blur_back"))
         {
            Session.savings().save(::aura::resource_blur_background);
         }
         if(guideline()->m_varTopicQuery.propset().has_property("save_transparent_back"))
         {
            Session.savings().save(::aura::resource_translucent_background);
         }
      }

      if(directrix()->m_varTopicQuery.propset().has_property("install"))
      {
         // core level app install
         if(!Ex2OnAppInstall())
            return false;
      }
      else if(directrix()->m_varTopicQuery.propset().has_property("uninstall"))
      {
         // core level app uninstall
         if(!Ex2OnAppUninstall())
            return false;
      }
      else
      {
#ifdef WINDOWSEX
         // when this process is started in the context of a system account,
         // for example, this code ensure that the process will
         // impersonate a loggen on ::fontopus::user
         HANDLE hprocess;
         HANDLE htoken;

         hprocess = ::GetCurrentProcess();

         if(!OpenProcessToken(
            hprocess,
            TOKEN_ALL_ACCESS,
            &htoken))
            return false;

         if(!ImpersonateLoggedOnUser(htoken))
         {
            TRACELASTERROR();
            return false;
         }
#endif
      }

      m_dwAlive = ::get_tick_count();

      if(is_system()
         && command_thread()->m_varTopicQuery["app"] != "app-core/netnodelite"
         && command_thread()->m_varTopicQuery["app"] != "app-core/netnode_dynamic_web_server"
         && command_thread()->m_varTopicQuery["app"] != "app-gtech/alarm"
         && command_thread()->m_varTopicQuery["app"] != "app-gtech/sensible_service")
      {
         System.http().defer_auto_initialize_proxy_configuration();
      }

      m_dwAlive = ::get_tick_count();

      m_bAxisInitializeResult = true;

      dappy(string(typeid(*this).name()) + " : initialize ok : " + ::str::from(m_iReturnCode));

      return true;

   }


   int32_t application::exit_instance()
   {


      try
      {

         
         //destroy_message_queue();

      }
      catch(...)
      {

         m_iReturnCode = -1;

      }




      try
      {


         /*      try
         {
         if(m_plemonarray != NULL)
         {
         delete m_plemonarray;
         }
         }
         catch(...)
         {
         }
         m_plemonarray = NULL;
         */


         m_pcommandthread.release();

         release_exclusive();

//         if(m_spuiMessage.is_set())
         {

            //if(!destroy_message_queue())
            {

               // TRACE("Could not finalize message interaction_impl");

            }

         }


         if(m_psignal != NULL)
         {

            ::aura::application_signal_details signal(m_psignal,::aura::application_signal_exit_instance);

            try
            {

               m_psignal->emit(&signal);

            }
            catch(...)
            {

            }

            m_psignal.release();

         }

         //try
         //{
         //   if (!is_system())
         //   {
         //      System.unregister_bergedge_application(this);
         //   }
         //}
         //catch (...)
         //{
         //}

         /*try
         {
         ::release(smart_pointer <thread>::m_p);
         }
         catch(...)
         {
         }*/


         if(is_system())
         {

            //         try
            //       {
            //        if(m_spfilesystem.m_p != NULL)
            //      {
            //       ::core::del(m_spfilesystem.m_p);
            //  }
            //         }
            //       catch(...)
            //     {
            //   }
         }



         try
         {

            sp(thread_impl) pthread = m_pthreadimpl;

            if(pthread != NULL)
            {

               try
               {
                  // avoid calling CloseHandle() on our own thread handle
                  // during the thread destructor
                  // avoid thread object data auto deletion on thread termination,
                  // letting thread function terminate
                  m_bAutoDelete = false;

                  set_run(false);

                  pthread->exit_instance();

               }
               catch(...)
               {

               }

            }

         }
         catch(...)
         {

         }

         /*try
         {

            m_pthreadimpl.release();

         }
         catch(...)
         {

         }
         */


         try
         {

            ::aura::application * papp = m_pimpl.detach();

            if(papp != NULL && papp != this && !papp->is_system())
            {

               try
               {

                  papp->exit_instance();

               }
               catch(...)
               {

               }

            }

         }
         catch(...)
         {

         }

      }
      catch(...)
      {

         m_iReturnCode = -1;

      }

      //try
      //{

      //   if(Session.appptra().get_count() <= 1)
      //   {

      //      if(System.thread::get_os_data() != NULL)
      //      {
      //         System.post_thread_message(WM_QUIT);

      //      }

      //   }

      //}
      //catch(...)
      //{

      //   m_iReturnCode = -1;

      //}

      return m_iReturnCode;

   }


   bool application::finalize()
   {

      bool bOk = false;

      try
      {

         bOk = thread::finalize();

      }
      catch(...)
      {

         bOk = false;

      }

      return bOk;

   }






   bool application::is_running()
   {
      return is_alive();
   }

   //service_base * application::allocate_new_service()
   //{

   //   return NULL;

   //}


//   bool application::check_exclusive()
//   {
//
//#ifdef METROWIN
//
//      return true;
//
//#endif
//
//      bool bSetOk;
//
//      LPSECURITY_ATTRIBUTES lpsa = NULL;
//
//      bool bResourceException = false;
//
//#ifdef WINDOWSEX
//
//      bSetOk = false;
//
//      SECURITY_ATTRIBUTES MutexAttributes;
//      ZeroMemory(&MutexAttributes,sizeof(MutexAttributes));
//      MutexAttributes.nLength = sizeof(MutexAttributes);
//      MutexAttributes.bInheritHandle = FALSE; // object uninheritable
//      // declare and initialize a security descriptor
//      SECURITY_DESCRIPTOR SD;
//      bool bInitOk = InitializeSecurityDescriptor(&SD,SECURITY_DESCRIPTOR_REVISION) != FALSE;
//      if(bInitOk)
//      {
//         // give the security descriptor a Null Dacl
//         // done using the  "TRUE, (PACL)NULL" here
//         bSetOk = SetSecurityDescriptorDacl(&SD,
//            TRUE,
//            (PACL)NULL,
//            FALSE) != FALSE;
//      }
//
//      if(bSetOk)
//      {
//
//         MutexAttributes.lpSecurityDescriptor = &SD;
//
//         lpsa = &MutexAttributes;
//
//      }
//
//#else
//
//      bSetOk = true;
//
//#endif
//
//      if(bSetOk)
//      {
//         // Make the security attributes point
//         // to the security descriptor
//         bResourceException = false;
//         try
//         {
//            m_pmutexGlobal = new mutex(this,FALSE,get_global_mutex_name(),lpsa);
//         }
//         catch(resource_exception &)
//         {
//            try
//            {
//               m_pmutexGlobal = new mutex(this,FALSE,get_global_mutex_name());
//            }
//            catch(resource_exception &)
//            {
//               bResourceException = true;
//            }
//         }
//
//         if(m_eexclusiveinstance == ExclusiveInstanceGlobal
//            && (::GetLastError() == ERROR_ALREADY_EXISTS || bResourceException))
//         {
//            // Should in some way activate the other instance, but this is global, what to do? do not know yet.
//            //System.simple_message_box("A instance of the application:<br><br>           - " + string(m_strAppName) + "<br><br>seems to be already running at the same machine<br>Only one instance of this application can run globally: at the same machine.<br><br>Exiting this new instance.");
//            TRACE("A instance of the application:<br><br>           - " + string(m_strAppName) + "<br><br>seems to be already running at the same machine<br>Only one instance of this application can run globally: at the same machine.<br><br>Exiting this new instance.");
//            on_exclusive_instance_conflict(ExclusiveInstanceGlobal);
//            return false;
//         }
//         if(m_eexclusiveinstance == ExclusiveInstanceGlobalId)
//         {
//            bResourceException = false;
//            try
//            {
//               m_pmutexGlobalId = new mutex(this,FALSE,get_global_id_mutex_name(),lpsa);
//            }
//            catch(resource_exception &)
//            {
//               try
//               {
//                  m_pmutexGlobalId = new mutex(this,FALSE,get_global_id_mutex_name());
//               }
//               catch(resource_exception &)
//               {
//                  bResourceException = true;
//               }
//            }
//            if(::GetLastError() == ERROR_ALREADY_EXISTS || bResourceException)
//            {
//               // Should in some way activate the other instance
//               //System.simple_message_box("A instance of the application:<br><br>           - " + string(m_strAppName) + "<br><br>seems to be already running at the same account.<br>Only one instance of this application can run locally: at the same account.<br><br>Exiting this new instance.");
//               TRACE("A instance of the application:<br><br>           - " + string(m_strAppName) + "with the id \"" + get_local_mutex_id() + "\" <br><br>seems to be already running at the same machine<br>Only one instance of this application can run globally: at the same machine with the same id.<br><br>Exiting this new instance.");
//               on_exclusive_instance_conflict(ExclusiveInstanceGlobalId);
//               return false;
//            }
//         }
//         bResourceException = false;
//         try
//         {
//            m_pmutexLocal = new mutex(this,FALSE,get_local_mutex_name(),lpsa);
//         }
//         catch(resource_exception &)
//         {
//            try
//            {
//               m_pmutexLocal = new mutex(this,FALSE,get_local_mutex_name());
//            }
//            catch(resource_exception &)
//            {
//               bResourceException = true;
//            }
//         }
//         if(m_eexclusiveinstance == ExclusiveInstanceLocal && (::GetLastError() == ERROR_ALREADY_EXISTS || bResourceException))
//         {
//            // Should in some way activate the other instance
//            //System.simple_message_box("A instance of the application:<br><br>           - " + string(m_strAppName) + "<br><br>seems to be already running at the same account.<br>Only one instance of this application can run locally: at the same account.<br><br>Exiting this new instance.");
//            TRACE("A instance of the application:<br><br>           - " + string(m_strAppName) + "<br><br>seems to be already running at the same account.<br>Only one instance of this application can run locally: at the same account.<br><br>Exiting this new instance.");
//            on_exclusive_instance_conflict(ExclusiveInstanceLocal);
//            return false;
//         }
//         if(m_eexclusiveinstance == ExclusiveInstanceLocalId)
//         {
//            bResourceException = false;
//            try
//            {
//               m_pmutexLocalId = new mutex(this,FALSE,get_local_id_mutex_name(),lpsa);
//            }
//            catch(resource_exception &)
//            {
//               try
//               {
//                  m_pmutexLocalId = new mutex(this,FALSE,get_local_id_mutex_name());
//               }
//               catch(resource_exception &)
//               {
//                  bResourceException = true;
//               }
//            }
//            if(::GetLastError() == ERROR_ALREADY_EXISTS || bResourceException)
//            {
//               // Should in some way activate the other instance
//               //System.simple_message_box("A instance of the application:<br><br>           - " + string(m_strAppName) + "<br><br>seems to be already running at the same account.<br>Only one instance of this application can run locally: at the same account.<br><br>Exiting this new instance.");
//               TRACE("A instance of the application:<br><br>           - " + string(m_strAppName) + "with the id \"" + get_local_mutex_id() + "\" <br><br>seems to be already running at the same account.<br>Only one instance of this application can run locally: at the same ac::count with the same id.<br><br>Exiting this new instance.");
//               on_exclusive_instance_conflict(ExclusiveInstanceLocalId);
//               return false;
//            }
//         }
//      }
//      else
//      {
//         return false;
//      }
//
//      return true;
//
//   }
//
//   bool application::release_exclusive()
//   {
//      if(m_pmutexGlobal.is_set())
//      {
//         m_pmutexGlobal.release();
//      }
//      if(m_pmutexGlobalId.is_set())
//      {
//         m_pmutexGlobalId.release();
//      }
//      if(m_pmutexLocal.is_set())
//      {
//         m_pmutexLocal.release();
//      }
//      if(m_pmutexLocalId.is_set())
//      {
//         m_pmutexLocalId.release();
//      }
//      return true;
//   }




   //string application::get_local_mutex_name(const char * pszAppName)
   //{
   //   string strMutex;
   //   strMutex.Format("Local\\ca2_application_local_mutex:%s",pszAppName);
   //   return strMutex;
   //}

   //string application::get_local_id_mutex_name(const char * pszAppName,const char * pszId)
   //{
   //   string strId(pszId);
   //   string strMutex;
   //   strMutex.Format("Local\\ca2_application_local_mutex:%s, id:%s",pszAppName,strId.c_str());
   //   return strMutex;
   //}

   //string application::get_global_mutex_name(const char * pszAppName)
   //{
   //   string strMutex;
   //   strMutex.Format("Global\\ca2_application_global_mutex:%s",pszAppName);
   //   return strMutex;
   //}

   //string application::get_global_id_mutex_name(const char * pszAppName,const char * pszId)
   //{
   //   string strId(pszId);
   //   string strMutex;
   //   strMutex.Format("Global\\ca2_application_global_mutex:%s, id:%s",pszAppName,strId.c_str());
   //   return strMutex;
   //}

   //string application::get_local_mutex_name()
   //{
   //   return get_local_mutex_name(get_mutex_name_gen());
   //}

   //string application::get_local_id_mutex_name()
   //{
   //   return get_local_id_mutex_name(get_mutex_name_gen(),get_local_mutex_id());
   //}

   //string application::get_global_mutex_name()
   //{
   //   return get_global_mutex_name(get_mutex_name_gen());
   //}

   //string application::get_global_id_mutex_name()
   //{
   //   return get_global_id_mutex_name(get_mutex_name_gen(),get_global_mutex_id());
   //}




   //void application::on_exclusive_instance_conflict(EExclusiveInstance eexclusive)
   //{
   //   if(eexclusive == ExclusiveInstanceLocal)
   //   {
   //      on_exclusive_instance_local_conflict();
   //   }
   //}

   //void application::on_exclusive_instance_local_conflict()
   //{
   //}


   //string application::get_mutex_name_gen()
   //{
   //   return m_strAppName;
   //}

   //string application::get_local_mutex_id()
   //{
   //   return command()->m_varTopicQuery["local_mutex_id"];
   //}

   //string application::get_global_mutex_id()
   //{
   //   return command()->m_varTopicQuery["global_mutex_id"];
   //}

   //::mutex * application::get_local_mutex()
   //{
   //   return m_pmutexLocal;
   //}

   //::mutex * application::get_global_mutex()
   //{
   //   return m_pmutexGlobal;
   //}



   bool application::Ex2OnAppInstall()
   {
      return true;
   }

   bool application::Ex2OnAppUninstall()
   {
      return true;
   }




   bool application::update_appmatter(::sockets::http_session * & psession,const ::file::path & pszRoot,const string & pszRelative)
   {

      ::str::international::locale_schema localeschema(this);

      Session.fill_locale_schema(localeschema);

      bool bIgnoreStdStd = string(pszRoot) == "app" && (string(pszRelative) == "main" || string(pszRelative) == "bergedge");

      //update_appmatter(h, psession, pszRoot, pszRelative, localeschema.m_idLocale, localeschema.m_idSchema);

      ::count iCount = localeschema.m_idaLocale.get_count();

      for(index i = 0; i < iCount; i++)
      {
         if(localeschema.m_idaLocale[i] == __id(std) && localeschema.m_idaSchema[i] == __id(std) && bIgnoreStdStd)
            continue;
         update_appmatter(psession,pszRoot,pszRelative,localeschema.m_idaLocale[i],localeschema.m_idaSchema[i]);
         System.install().m_progressApp()++;
      }


      return true;

   }

   bool application::update_appmatter(::sockets::http_session * & psession,const ::file::path & pszRoot,const string & pszRelative,const string & pszLocale,const string & pszStyle)
   {

      string strLocale;
      string strSchema;
      TRACE("update_appmatter(root=%s, relative=%s, locale=%s, style=%s)",pszRoot,pszRelative,pszLocale,pszStyle);
      ::file::path strRelative = ::file::path(pszRoot) / "appmatter" / pszRelative  / Sess(this).get_locale_schema_dir(pszLocale,pszStyle) + ".zip";
      ::file::path strFile = System.dir().element() / strRelative;
      ::file::path strUrl(::file::path_url);
      if(_ca_is_basis())
      {
         strUrl = "http://basis-server.ca2.cc/api/spaignition/download?authnone&version=basis&stage=";
      }
      else
      {
         strUrl = "http://stage-server.ca2.cc/api/spaignition/download?authnone&version=stage&stage=";
      }

      strUrl += System.url().url_encode(strRelative);


      if(psession == NULL)
      {

         while(true)
         {

            property_set setEmpty(get_app());

            psession = System.http().open(System.url().get_server(strUrl),System.url().get_protocol(strUrl),setEmpty,NULL,NULL);

            if(psession != NULL)
               break;

            Sleep(184);

         }

      }

      property_set set;

      set["get_memory"] = "";

      psession = System.http().request(psession,strUrl,set);

      ::file::memory_buffer file(get_app(),set["get_memory"].cast < primitive::memory >());

      if(set["get_memory"].cast < primitive::memory >() != NULL && set["get_memory"].cast < primitive::memory >()->get_size() > 0)
      {

         zip::Util util;

         string strDir = strFile;

         ::str::ends_eat_ci(strDir,".zip");

         try
         {

            util.extract_all(this, strDir,&file);

         }
         catch(...)
         {

            // spa app.install.exe would recover by retrying or someone would fix the resource packaging problem and then zip extraction at least should work.

            return false;

         }

         //System.compress().extract_all(strFile, this);

      }

      return true;

   }







   //void application::on_set_scalar(e_scalar escalar,int64_t iValue, int iFlags)
   //{

   //   //if (escalar == scalar_app_install_progress)
   //   //{

   //   //   m_iProgressInstallStep = iValue;

   //   //}
   //   //else if (escalar == scalar_app_install_progress_min)
   //   //{

   //   //   m_iProgressInstallStart = iValue;

   //   //}
   //   //else if (escalar == scalar_app_install_progress_max)
   //   //{

   //   //   m_iProgressInstallEnd = iValue;

   //   //}
   //   //else
   //   {

   //      return ::int_scalar_source::on_set_scalar(escalar,iValue, int iFlags);

   //   }

   //}


   //void application::get_scalar_minimum(e_scalar escalar,int64_t & i)
   //{

   //   //if (escalar == scalar_app_install_progress)
   //   //{

   //   //   i = m_iProgressInstallStart;

   //   //}
   //   //else if (escalar == scalar_app_install_progress_min)
   //   //{

   //   //   i = 0;

   //   //}
   //   //else if (escalar == scalar_app_install_progress_max)
   //   //{

   //   //   i = 0;

   //   //}
   //   //else
   //   {

   //      ::int_scalar_source::get_scalar_minimum(escalar,i);

   //   }

   //}

   //void application::get_scalar(e_scalar escalar,int64_t & i)
   //{

   //   //if (escalar == scalar_app_install_progress)
   //   //{

   //   //   i = m_iProgressInstallStep;

   //   //}
   //   //else if (escalar == scalar_app_install_progress_min)
   //   //{

   //   //   i = m_iProgressInstallStart;

   //   //}
   //   //else if (escalar == scalar_app_install_progress_max)
   //   //{

   //   //   i = m_iProgressInstallEnd;

   //   //}
   //   //else
   //   {

   //      ::int_scalar_source::get_scalar(escalar,i);

   //   }

   //}

   //void application::get_scalar_maximum(e_scalar escalar,int64_t & i)
   //{

   //   //if (escalar == scalar_download_size)
   //   //{

   //   //   i = m_iProgressInstallEnd;

   //   //}
   //   //else if (escalar == scalar_app_install_progress_min)
   //   //{

   //   //   i = 0x7fffffff;

   //   //}
   //   //else if (escalar == scalar_app_install_progress_max)
   //   //{

   //   //   i = 0x7fffffff;

   //   //}
   //   //else
   //   {

   //      ::int_scalar_source::get_scalar_minimum(escalar,i);

   //   }

   //}




   //service_base * application::get_service()
   //{

   //   return m_pservice;

   //}





   //string application::get_license_id()
   //{

   //   if(m_strLicense.is_empty())
   //   {
   //      
   //      m_strLicense = m_strAppId;

   //      if(m_strLicense.is_empty())
   //      {

   //         m_strLicense = m_strAppName;

   //      }

   //   }



   //   return m_strLicense;

   //}







   bool application::assert_user_logged_in()
   {

      const char * pszRequestUrl = NULL;

      string strRequestUrl;

      if(file_as_string_dup("C:\\ca2\\config\\system\\ignition_server.txt").has_char())
      {

         strRequestUrl = "https://" + file_as_string_dup("C:\\ca2\\config\\system\\ignition_server.txt") + "/api/spaignition";

         pszRequestUrl = strRequestUrl;

      }

      if(m_paxissession->fontopus()->get_user(true,pszRequestUrl) == NULL)
         //   throw exit_exception(get_app(),"You have not logged in!! db_str_set::load");
         return false;

      return true;

   }


   //::user::interaction * application::get_active_guie()
   //{

   //   return Session.get_active_guie();

   //}


   //::user::interaction * application::get_focus_guie()
   //{

   //   return Session.get_focus_guie();

   //}



   string application::matter_as_string(const char * pszMatter,const char * pszMatter2)
   {

      var varQuery;

      varQuery["disable_ca2_sessid"] = true;

      return file().as_string(dir().matter(::file::path(pszMatter) / pszMatter2),varQuery);

   }

   //string application::dir().matter(const char * pszMatter,const char * pszMatter2)
   //{

   //   return dir().matter(pszMatter,pszMatter2);

   //}

   //bool application::is_inside_time_dir(const char * pszPath)
   //{
   //   return dir().is_inside_time(pszPath);
   //}

   bool application::file_is_read_only(const char * pszPath)
   {
//      return file().is_read_only(pszPath);
      throw not_implemented(this);
      return false;
      
   }

//   string application::file_as_string(var varFile)
//   {
//
//      //if(::str::begins_ci(varFile.get_string(),"http://")
//      //   || ::str::begins_ci(varFile.get_string(),"https://"))
//      //{
//
//      //   ::property_set set(get_app());
//
//      //   return http().get(varFile.get_string(),set);
//
//      //}
//      //else if(varFile.has_property("url") &&
//      //   (::str::begins_ci(varFile["url"].get_string(),"http://")
//      //   || ::str::begins_ci(varFile["url"].get_string(),"https://")))
//      //{
//
//      //   ::property_set set(get_app());
//
//      //   return http().get(varFile["url"].get_string(),set);
//
//      //}
//      //else
//      //{
//         return file().as_string(varFile.get_string());
////      }
//
//   }
//
//   //string application::dir_path(const char * psz1,const char * psz2,const char * psz3)
//   //{
//   //   return psz1,psz2,psz3);
//   //}
//
//   string application::dir_name(const char * psz)
//   {
//      return System.dir().name(psz);
//   }
//
//   bool application::dir_mk(const char * psz)
//   {
//      return dir().mk(psz);
//   }
//
//   string application::file_title(const char * psz)
//   {
//      return System.file().title_(psz);
//   }
//   string application::file_name(const char * psz)
//   {
//      return System.file().name_(psz);
//   }
//
//
//   ::file::buffer_sp application::file_get_file(var varFile,uint32_t uiFlags)
//   {
//
//      return file().get_file(varFile,uiFlags);
//
//   }

   //void application::dir_matter_ls_file(const string & str,stringa & stra)
   //{

   //   return dir().matter_ls_file(str,stra);

   //}

//#ifdef APPLEOS
//
//   string application::dir_pathfind(const char * pszEnv, const char * pszTopic, const char * pszMode)
//   {
//      
//      return dir().pathfind(pszEnv, pszTopic, pszMode);
//      
//   }
//
//#endif
//
//   string application::file_as_string(var varFile,var & varQuery)
//   {
//
//      return file().as_string(varFile);
//
//   }
//
//
//
//   bool application::file().exists(const char * pszPath)
//   {
//
//      return file().exists(pszPath) != FALSE;
//
//   }
//
//
//   bool application::file_is_equal_path(const char * pszPath1,const char * pszPath2)
//   {
//
//      return System.file().path().is_equal(pszPath1,pszPath2) != FALSE;
//
//   }
//
//
//   bool application::dir_is(const char * psz)
//   {
//
//      return dir().is(psz);
//
//   }


   //bool application::file_del(const char * psz)
   //{

   //   try
   //   {

   //      System.file().del(psz);

   //   }
   //   catch(...)
   //   {

   //      return false;

   //   }

   //   return true;

   //}

   //string application::file_extension(const char * pszPath)
   //{

   //   return file_extension_dup(pszPath);

   //}


   //string application::dir_userappdata(const char * lpcsz,const char * lpcsz2)
   //{

   //   return dir().userappdata(lpcsz,lpcsz2);

   //}

   //string application::dir_appdata(const char * lpcsz,const char * lpcsz2)
   //{

   //   return System.dir().appdata(lpcsz,lpcsz2);

   //}


   //string application::dir_simple_path(const string & str1,const string & str2)
   //{

   //   return System.dir().simple_path(str1,str2);

   //}


   //string application::dir_element(const char * psz)
   //{

   //   return System.dir().element(psz);

   //}


   //string application::dir_ca2module(const char * psz)
   //{

   //   return System.dir().ca2module(psz);

   //}


   //void application::dir_ls_dir(const char * lpcsz,::file::patha & patha)
   //{

   //   dir().ls_dir(lpcsz,ppatha,ppathaName);

   //}


   //void application::dir_rls(const char * lpcsz,::file::patha & patha)
   //{

   //   dir().rls(lpcsz,ppatha,ppathaName,ppathaRelative);

   //}




   //string application::file_time_square()
   //{

   //   return file().time_square();

   //}


   //string application::http_get_locale_schema(const char * pszUrl,const char * pszLocale,const char * pszSchema)
   //{

   //   return http().get_locale_schema(pszUrl,pszLocale,pszSchema);

   //}


   


} // namespace axis







//#include "framework.h" // from "axis/user/user.h"
//#include "base/user/user.h"

namespace axis
{


   UINT application::APPM_LANGUAGE = WM_APP + 117;
   WPARAM application::WPARAM_LANGUAGE_UPDATE = 1;






   void application::process_message_filter(int32_t code,signal_details * pobj)
   {

      if(pobj == NULL)
         return;   // not handled

      SCAST_PTR(::message::base,pbase,pobj);

      sp(::user::frame_window) pTopFrameWnd;
      //::user::interaction * pMainWnd;
      ::user::interaction * pMsgWnd;
      switch(code)
      {
         //case MSGF_DDEMGR:
         // Unlike other WH_MSGFILTER codes, MSGF_DDEMGR should
         //  never call the next hook.
         // By returning FALSE, the message will be dispatched
         //  instead (the default behavior).
         //return;

      case MSGF_MENU:
         pMsgWnd = dynamic_cast < ::user::interaction * > (pbase->m_pwnd);
         if(pMsgWnd != NULL)
         {
            pTopFrameWnd = pMsgWnd->GetTopLevelFrame();
            if(pTopFrameWnd != NULL && pTopFrameWnd->IsTracking() &&
               pTopFrameWnd->m_bHelpMode)
            {
               //pMainWnd = __get_main_window();
               //if((m_puiMain != NULL) && (IsEnterKey(pbase) || IsButtonUp(pbase)))
               //{
               //   //                  pMainWnd->SendMessage(WM_COMMAND, ID_HELP);
               //   pbase->m_bRet = true;
               //   return;
               //}
            }
         }
         // fall through...

      case MSGF_DIALOGBOX:    // handles message boxes as well.
         //pMainWnd = __get_main_window();
         if(code == MSGF_DIALOGBOX && m_puiActive != NULL &&
            pbase->m_uiMessage >= WM_KEYFIRST && pbase->m_uiMessage <= WM_KEYLAST)
         {
            //// need to translate messages for the in-place container
            //___THREAD_STATE* pThreadState = __get_thread_state();
            //ENSURE(pThreadState);

            //if (pThreadState->m_bInMsgFilter)
            //   return;
            //pThreadState->m_bInMsgFilter = TRUE;    // avoid reentering this code
            //if (m_puiActive->is_window_enabled())
            //{
            //   pre_translate_message(pobj);
            //   if(pobj->m_bRet)
            //   {
            //      pThreadState->m_bInMsgFilter = FALSE;
            //      return;
            //   }
            //}
            //pThreadState->m_bInMsgFilter = FALSE;    // ok again
         }
         break;
      }
      // default to not handled

   }




   bool application::get_frame(::user::interaction * & pui)
   {

      synch_lock sl(&m_mutexFrame);

      if(m_uiptraFrame.get_count() <= 0)
      {

         return false;

      }

      if(pui == NULL)
      {

         pui = m_uiptraFrame[0];

         return true;

      }
      else
      {

         for(index i = m_uiptraFrame.get_upper_bound(); i >= 0; i--)
         {

            if(m_uiptraFrame[i] == pui)
            {

               i++;

               if(i < m_uiptraFrame.get_count())
               {

                  pui = m_uiptraFrame[i];

                  return true;

               }
               else
               {

                  return false;

               }

            }

         }

      }

      return false;

   }


   void application::add_frame(sp(::user::interaction) pwnd)
   {

      synch_lock sl(&m_mutexFrame); // recursive lock (on m_framea.add(pwnd)) but m_puiMain is "cared" by m_frame.m_mutex

      if(m_uiptraFrame.add_unique(pwnd))
      {

         System.defer_create_system_frame_window();

         Session.on_create_frame_window();

         if(m_puiMain == NULL)
         {

            m_puiMain = pwnd;

         }

      }

   }


   void application::remove_frame(sp(::user::interaction) pwnd)
   {

      synch_lock sl(&m_mutexFrame); // recursive lock (on m_framea.remove(pwnd)) but m_puiMain is "cared" by m_frame.m_mutex

      if(m_puiMain == pwnd)
      {

         m_puiMain = NULL;

      }

      m_uiptraFrame.remove(pwnd);


   }




   ::user::interaction * application::window_from_os_data(void * pdata)
   {

      if(pdata == NULL)
      {

         return NULL;

      }

      return window_from_handle((oswindow)pdata);

   }


   void application::DoWaitCursor(int32_t nCode)
   {

      if(nCode < 0)
      {

         m_iWaitCursorCount = 0;
         ShowWaitCursor(false);

      }
      else if(nCode == 0)
      {

         if(m_iWaitCursorCount > 0)
         {
            m_iWaitCursorCount--;
         }

         if(m_iWaitCursorCount > 0)
         {

            ShowWaitCursor(true);

         }

         m_iWaitCursorCount = 0;
         ShowWaitCursor(false);

      }
      else
      {

         if(m_iWaitCursorCount < 0)
         {
            m_iWaitCursorCount = 0;
         }

         m_iWaitCursorCount++;

         ShowWaitCursor(true);

      }



   }

   void application::ShowWaitCursor(bool bShow)
   {

      if(m_pimpl == NULL)
         return;

      m_pimpl->ShowWaitCursor(bShow);

   }



   void application::TermThread(HINSTANCE hInstTerm)
   {

      ::exception::throw_interface_only(this);

   }



   sp(::user::interaction) application::FindWindow(const char * lpszClassName,const char * lpszWindowName)
   {

      ::exception::throw_interface_only(this);

      return NULL;

   }


   sp(::user::interaction) application::FindWindowEx(oswindow oswindowParent,oswindow oswindowChildAfter,const char * lpszClass,const char * lpszWindow)
   {

      ::exception::throw_interface_only(this);

      return NULL;

   }


   string application::get_version()
   {

      ::exception::throw_interface_only(this);

      return "";

   }




   void application::set_env_var(const string & var,const string & value)
   {

      ::exception::throw_interface_only(this);

   }



   sp(::aura::printer) application::get_printer(const char * pszDeviceName)
   {

      ::exception::throw_interface_only(this);

      return NULL;

   }


   //bool application::update_module_paths()
   //{


   //   if(is_system())
   //   {

   //      if(!m_pimpl->update_module_paths())
   //         return false;


   //   }

   //   return true;


   //}



   sp(::user::interaction) application::release_capture_uie()
   {

#if defined(LINUX)

      oswindow oswindowCapture = ::GetCapture();
      if(oswindowCapture == NULL)
         return NULL;
      return oswindowCapture->get_user_interaction()->ReleaseCapture();

#elif defined(WINDOWS)

      oswindow oswindowCapture = ::GetCapture();
      if(oswindowCapture == NULL)
         return NULL;
      return System.window_from_os_data(oswindowCapture)->ReleaseCapture();

#elif defined(APPLEOS)

      oswindow oswindowCapture = ::GetCapture();
      if(oswindowCapture == NULL)
         return NULL;
      return oswindowCapture->get_user_interaction()->ReleaseCapture();

#else

      ::exception::throw_not_implemented(get_app());

#endif

   }


   sp(::user::interaction) application::get_capture_uie()
   {

#if defined(METROWIN)

      oswindow oswindowCapture = ::GetCapture();

      if(oswindowCapture == NULL)
         return NULL;

      ::user::interaction * pui = oswindowCapture->window();

      if(pui == NULL)
         return NULL;

      return pui->GetCapture();

#elif defined(WINDOWS) || defined(APPLE_IOS)

      oswindow oswindowCapture = ::GetCapture();

      if(oswindowCapture == NULL)
         return NULL;

      sp(::user::interaction) pui = System.window_from_os_data(oswindowCapture);

      if(pui == NULL)
         return NULL;

      return pui->GetCapture();

#else

      //      ::exception::throw_not_implemented(get_app());

      oswindow oswindowCapture = ::GetCapture();

      if(oswindowCapture == NULL)
         return NULL;

      return window_from_handle(::GetCapture());

#endif

   }







   uint32_t application::get_thread_id()
   {
      return m_pimpl->get_thread_id();
   }





#ifndef METROWIN

   void application::get_time(timeval *p)
   {
      m_pimpl->get_time(p);
   }

#endif


   bool application::do_prompt_file_name(var & varFile,UINT nIDSTitle,uint32_t lFlags,bool bOpenFileDialog,::aura::impact_system * ptemplate,::aura::document * pdocument)
   {

      UNREFERENCED_PARAMETER(varFile);
      UNREFERENCED_PARAMETER(nIDSTitle);

      return false;

   }


   string application::get_cred(const string & strRequestUrlParam,const RECT & rect,string & strUsername,string & strPassword,string strToken,string strTitle,bool bInteractive)
   {

      string str = ::fontopus::get_cred(this,strUsername,strPassword,strToken);

      if(str == "ok")
         return "ok";

      if(!bInteractive)
         return "failed";

      sp(::fontopus::simple_ui) pui;

      string strRequestUrl(strRequestUrlParam);

      if(strRequestUrl.is_empty())
      {

         string strIgnitionServer = file_as_string_dup("C:\\ca2\\config\\system\\ignition_server.txt");

         if(::str::ends_ci(strIgnitionServer,".ca2.cc"))
         {

            strRequestUrl = "https://" + strIgnitionServer + "/";

         }
         else
         {

            strRequestUrl = "https://account.ca2.cc/";

         }

      }

      pui = canew(::fontopus::simple_ui(this,strRequestUrl));

      pui->m_login.m_peditUser->SetWindowText(strUsername);

      pui->m_login.m_ppassword->SetWindowText("");

      string strResult = pui->get_cred(rect,strUsername,strPassword,strToken,strTitle);

      pui->DestroyWindow();

      return strResult;

   }




   //bool application::get_temp_file_name_template(string & strRet,const char * pszName,const char * pszExtension,const char * pszTemplate)
   //{

   //   throw not_implemented(this);

   //   return false;

   //}


   //bool application::get_temp_file_name(string & strRet,const char * pszName,const char * pszExtension)
   //{

   //   return get_temp_file_name_template(strRet,pszName,pszExtension,NULL);

   //}








   ::visual::icon * application::set_icon(object * pobject,::visual::icon * picon,bool bBigIcon)
   {

      ::visual::icon * piconOld = get_icon(pobject,bBigIcon);

      if(bBigIcon)
      {

         pobject->oprop("big_icon").operator =((sp(object)) picon);

      }
      else
      {

         pobject->oprop("small_icon").operator =((sp(object)) picon);

      }

      return piconOld;

   }


   ::visual::icon * application::get_icon(object * pobject,bool bBigIcon) const
   {

      if(bBigIcon)
      {

         return const_cast <object *> (pobject)->oprop("big_icon").cast < ::visual::icon >();

      }
      else
      {

         return const_cast <object *> (pobject)->oprop("small_icon").cast < ::visual::icon >();

      }

   }



   //bool application::final_handle_exception(::exception::exception & e)
   //{
   //   UNREFERENCED_PARAMETER(e);
   //   //linux      exit(-1);

   //   if(!is_system())
   //   {

   //      // get_app() may be it self, it is ok...
   //      if(Sys(get_app()).final_handle_exception((::exception::exception &) e))
   //         return true;


   //   }

   //   return false;
   //}





   //int32_t application::main()
   //{

   //   TRACE(string(typeid(*this).name()) + " main");;

   //   dappy(string(typeid(*this).name()) + " : application::main 1");

   //   try
   //   {

   //      TRACE(string(typeid(*this).name()) + " on_run");;
   //      dappy(string(typeid(*this).name()) + " : going to on_run : " + ::str::from(m_iReturnCode));
   //      m_iReturnCode = 0;
   //      m_bReady = true;
   //      m_bRun = true;
   //      m_iReturnCode = on_run();
   //      if(m_iReturnCode != 0)
   //      {
   //         dappy(string(typeid(*this).name()) + " : on_run failure : " + ::str::from(m_iReturnCode));
   //         ::OutputDebugStringW(L"application::main on_run termination failure");
   //      }

   //   }
   //   catch(::exit_exception &)
   //   {

   //      dappy(string(typeid(*this).name()) + " : on_run exit_exception");

   //      System.post_thread_message(WM_QUIT,0,0);

   //      goto exit_application;

   //   }
   //   catch(...)
   //   {

   //      dappy(string(typeid(*this).name()) + " : on_run general exception");

   //      goto exit_application;

   //   }

   //   try
   //   {

   //      if(is_system())
   //      {

   //         dappy(string(typeid(*this).name()) + " : quiting main");

   //         //post_to_all_threads(WM_QUIT,0,0);

   //         //Sleep(5000);

   //      }

   //   }
   //   catch(...)
   //   {

   //   }

   //exit_application:

   //   try
   //   {

   //      m_iReturnCode = exit();

   //   }
   //   catch(::exit_exception &)
   //   {

   //      post_to_all_threads(WM_QUIT,0,0);

   //      m_iReturnCode = -1;

   //   }
   //   catch(...)
   //   {

   //      m_iReturnCode = -1;

   //   }

   //   return m_iReturnCode;

   //}




   //bool application::pre_run()
   //{

   //   TRACE(string(typeid(*this).name()) + " main_start");;
   //   try
   //   {

   //      m_dwAlive = ::get_tick_count();
   //      TRACE(string(typeid(*this).name()) + "application_pre_run");;
   //      int32_t m_iReturnCode = application_pre_run();
   //      if(m_iReturnCode != 0)
   //      {
   //         dappy(string(typeid(*this).name()) + " : applicationpre_run failure : " + ::str::from(m_iReturnCode));
   //         m_bReady = true;
   //         TRACE("application::main application_pre_run failure");
   //         return false;
   //      }

   //      xxdebug_box("pre_runnned","pre_runnned",MB_ICONINFORMATION);
   //      dappy(string(typeid(*this).name()) + " : pre_runned : " + ::str::from(m_iReturnCode));
   //      TRACE(string(typeid(*this).name()) + " initial_check_directrix");;
   //      if(!initial_check_directrix())
   //      {
   //         dappy(string(typeid(*this).name()) + " : initial_check_directrix failure");
   //         m_iReturnCode = -1;
   //         exit();
   //         m_bReady = true;
   //         ::OutputDebugStringW(L"exiting on check directrix");
   //         return false;
   //      }


   //      TRACE(string(typeid(*this).name()) + " os_native_bergedge_start");;
   //      m_dwAlive = ::get_tick_count();
   //      if(!os_native_bergedge_start())
   //      {
   //         dappy(string(typeid(*this).name()) + " : os_native_bergedge_start failure");
   //         exit();
   //         m_iReturnCode = -1;
   //         m_bReady = true;
   //         ::OutputDebugStringW(L"application::main os_native_bergedge_start failure");
   //         return false;
   //      }

   //      return true;
   //   }
   //   catch(::exit_exception &)
   //   {

   //      dappy(string(typeid(*this).name()) + " : main_start exit_exception");

   //      post_to_all_threads(WM_QUIT,0,0);

   //   }

   //   return false;

   //}




   //int32_t application::on_run()
   //{
   //   int32_t m_iReturnCode = 0;

   //   try
   //   {
   //      ::aura::application_signal_details signal(this,m_psignal,::aura::application_signal_start);
   //      m_psignal->emit(&signal);
   //   }
   //   catch(...)
   //   {
   //   }

   //   dappy(string(typeid(*this).name()) + " : starting on_run : " + ::str::from(m_iReturnCode));

   //   thread * pthread = ::get_thread();

   //   install_message_handling(pthread->m_pthreadimpl);

   //   dappy(string(typeid(*this).name()) + " : starting on_run 2 : " + ::str::from(m_iReturnCode));

   //   try
   //   {
   //      try
   //      {
   //         m_bReady = true;
   //         if(m_peventReady != NULL)
   //            m_peventReady->SetEvent();
   //      }
   //      catch(...)
   //      {
   //      }
   //   run:
   //      try
   //      {
   //         m_iReturnCode = run();

   //      }
   //      catch(::exit_exception & e)
   //      {

   //         throw e;

   //      }
   //      catch(const ::exception::exception & e)
   //      {
   //         if(on_run_exception((::exception::exception &) e))
   //            goto run;
   //         if(final_handle_exception((::exception::exception &) e))
   //            goto run;
   //         try
   //         {
   //            m_iReturnCode = exit();
   //         }
   //         catch(::exit_exception & e)
   //         {

   //            throw e;

   //         }
   //         catch(...)
   //         {
   //            m_iReturnCode = -1;
   //         }
   //         goto InitFailure;
   //      }
   //   }
   //   catch(::exit_exception & e)
   //   {

   //      throw e;

   //   }
   //   catch(...)
   //   {
   //      // linux-like exit style on crash, differently from general windows error message approach
   //      // to prevent or correct from crash, should:
   //      // - look at dumps - to do;
   //      // - look at trace and log - always doing;
   //      // - look at debugger with the visual or other tool atashed - to doing;
   //      // - fill other possibilities;
   //      // - restart and send information in the holy back, and stateful or self-heal as feedback from below;
   //      // - ...
   //      // - ..
   //      // - .
   //      // - .
   //      // - .
   //      // - .
   //      // -  .
   //      // - ...
   //      // - ...
   //      // - ...
   //      // to pro-activia and overall benefits workaround:
   //      // - stateful applications:
   //      //      - browser urls, tabs, entire history, in the ca2computing cloud;
   //      //      - uint16_t - html document to simplify here - with all history of undo and redos per document optimized by cvs, svn, syllomatter;
   //      //           - not directly related but use date and title to name document;
   //      //      - save forms after every key press in .undo.redo.form file parallel to appmatter / form/undo.redo.file parity;
   //      //      - last ::ikaraoke::karaoke song and scoring, a little less motivated at time of writing;
   //      //
   //      // - ex-new-revolut-dynamic-news-self-healing
   //      //      - pre-history, antecendentes
   //      //            - sometimes we can't recover from the last state
   //      //            - to start from the beggining can be too heavy, waity, worky, bory(ing)
   //      //      - try to creativetily under-auto-domain with constrained-learning, or heuristcally recover from restart, shutdown, login, logoff;
   //      //           - reification :
   //      //           - if the document is corrupted, try to open the most of it
   //      //           - if can only detect that the document cannot be opened or damaged, should creatively workarounds as it comes, as could it be
   //      //              done, for example search in the web for a proper solution?
   //      //           - ::ikaraoke::karaoke file does not open? can open next? do it... may animate with a temporary icon...
   //      //           - import a little as pepper for the meal, prodevian technology into estamira, so gaming experience relativity can open ligh
   //      //               speed into cartesian dimensions of
   //      //               core, estamira and prodevian. Take care not to flood prodevian brand black ink over the floor of the estamira office...
   //      //               black letters, or colorful and pink are accepted and sometimes desired, for example, hello kity prodevian, pirarucu games,
   //      //               I think no one likes to be boring, but a entire background in black... I don't know... only for your personal office, may be...
   //      //           - could an online colaborator investigate crashes promptly in a funny way, and make news and jokes? Like terra and UOL for the real world?
   //      //               - new crash, two documents lost, weathers fault, too hot, can't think, my mother was angry with me, lead to buggy code;
   //      //               - new version with bug fixes;
   //      //      - new versions
   //      //      - automatic updates
   //      //      - upgrades
   //      //      - rearrangemntes
   //      //      - downgrade in the form of retro
   //      // - ...
   //      // - ..
   //      // - .
   //      // - .
   //      // - .
   //      // - .
   //      // -  .
   //      // - ...
   //      // - ...
   //      // - ...

   //   }
   //InitFailure:
   //   try
   //   {
   //      if(m_peventReady != NULL)
   //         m_peventReady->SetEvent();
   //   }
   //   catch(...)
   //   {
   //   }
   //   try
   //   {
   //      thread * pthread = this;
   //      if(pthread != NULL && pthread->m_pbReady != NULL)
   //      {
   //         *pthread->m_pbReady = true;
   //      }
   //   }
   //   catch(...)
   //   {
   //   }
   //   /*try
   //   {
   //   thread * pthread = dynamic_cast < thread * > (this);
   //   ::SetEvent((HANDLE) pthread->m_peventReady);
   //   }
   //   catch(...)
   //   {
   //   }*/

   //   // let translator run undefinetely
   //   /*if(is_system())
   //   {
   //   translator::detach();
   //   }*/

   //   return m_iReturnCode;
   //}




   //int32_t application::application_pre_run()
   //{

   //   return ::axis::application::application_pre_run();

   //}



   //bool application::InitApplication()
   //{

   //   return true;

   //}



   //bool application::initial_check_directrix()
   //{

   //   return axis::application::initial_check_directrix();

   //}


   //bool application::on_install()
   //{

   //   if(!axis::application::on_install())
   //      return false;



   //   return true;

   //}


   //bool application::on_uninstall()
   //{

   //   bool bOk = axis::application::on_uninstall();


   //   if(is_serviceable())
   //   {

   //      if(!remove_service())
   //         bOk = false;

   //   }

   //   return bOk;

   //}


   //bool application::system_add_app_install(const char * pszId)
   //{



   //   return axis::application::system_add_app_install(pszId);

   //}



   //bool application::os_native_bergedge_start()
   //{

   //   return axis::application::os_native_bergedge_start();

   //}











   //   void application::construct(const char *pszId)
   //   {
   //      //if(m_strAppName.has_char())
   //      //   return;
   //      //m_strAppName.Empty();
   //      //m_strId.Empty();
   //      if (pszId == NULL)
   //      {
   //#ifdef WINDOWSEX
   //         wstring wstr = ::GetCommandLineW();
   //         string str = ::str::international::unicode_to_utf8(wstr);
   //         strsize iFind = str.find(" : ");
   //         if (iFind >= 0)
   //         {
   //            iFind = str.find("app=", iFind);
   //            if (iFind >= 0)
   //            {
   //               strsize iEnd = str.find(" ", iFind);
   //               if (iEnd < 0)
   //               {
   //                  m_strId = str.Mid(iFind + 4);
   //               }
   //               else
   //               {
   //                  m_strId = str.Mid(iFind + 4, iEnd - iFind - 4);
   //               }
   //               ::str::begins_eat(m_strId, "\"");
   //               ::str::ends_eat(m_strId, "\"");
   //            }
   //         }
   //#endif
   //      }
   //      else
   //      {
   //         m_strId = pszId;
   //      }
   //      if (m_strId.is_empty())
   //         m_strId = "mplite";
   //      construct();
   //      if (m_strAppName.is_empty())
   //      {
   //         if (m_strAppId.has_char())
   //            m_strAppName = m_strAppId;
   //         else if (m_strInstallToken.has_char())
   //            m_strAppName = m_strInstallToken;
   //      }
   //   }


   //void application::construct()
   //{
   //   string strId = m_strId;
   //   char chFirst = '\0';
   //   if (strId.get_length() > 0)
   //   {
   //      chFirst = strId[0];
   //   }

   //   ::core::application::construct();

   //}




   //void application::_001OnFileNew()
   //{
   //   string strId = m_strId;
   //   char chFirst = '\0';
   //   if (strId.get_length() > 0)
   //   {
   //      chFirst = strId[0];
   //   }
   //   ::core::application::_001OnFileNew(NULL);
   //}


   //bool application::bergedge_start()
   //{
   //   string strId = m_strId;
   //   char chFirst = '\0';
   //   if (strId.get_length() > 0)
   //   {
   //      chFirst = strId[0];
   //   }
   //   return ::core::application::bergedge_start();
   //}



   //bool application::on_install()
   //{
   //   string strId = m_strId;
   //   char chFirst = '\0';
   //   if (strId.get_length() > 0)
   //   {
   //      chFirst = strId[0];
   //   }
   //   return ::core::application::on_install();
   //}

   //bool application::on_uninstall()
   //{
   //   string strId = m_strId;
   //   char chFirst = '\0';
   //   if (strId.get_length() > 0)
   //   {
   //      chFirst = strId[0];
   //   }
   //   return ::core::application::on_uninstall();
   //}


   //void application::on_request(sp(::create) pcreatecontext)
   //{

   //   ::object::on_request(pcreatecontext);

   //   command()->consolidate(pcreatecontext);

   //}


   //bool application::is_serviceable()
   //{


   //   string strId = m_strId;
   //   char chFirst = '\0';
   //   if (strId.get_length() > 0)
   //   {
   //      chFirst = strId[0];
   //   }
   //   return ::core::application::is_serviceable();
   //}

   //service_base * application::allocate_new_service()
   //{

   //   return NULL;

   //}


   //sp(::aura::document) application::_001OpenDocumentFile(var varFile)
   //{
   //   string strId = m_strId;
   //   char chFirst = '\0';
   //   if (strId.get_length() > 0)
   //   {
   //      chFirst = strId[0];
   //   }
   //   return ::core::application::_001OpenDocumentFile(varFile);

   //}


   //int32_t application::run()
   //{

   //   return ::axis::application::run();


   //}











   void application::on_service_request(sp(::create) pcreatecontext)
   {

      if(!is_serviceable())
         return;

      if(pcreatecontext->m_spCommandLine->m_varQuery.has_property("create_service"))
      {
         create_service();
      }
      else if(pcreatecontext->m_spCommandLine->m_varQuery.has_property("start_service"))
      {
         start_service();
      }
      else if(pcreatecontext->m_spCommandLine->m_varQuery.has_property("stop_service"))
      {
         stop_service();
      }
      else if(pcreatecontext->m_spCommandLine->m_varQuery.has_property("remove_service"))
      {
         remove_service();
      }


   }



   //bool application::process_initialize()
   //{

   //   if(m_bAxisProcessInitialize)
   //      return m_bAxisProcessInitializeResult;

   //   m_bAxisProcessInitialize = true;
   //   m_bAxisProcessInitializeResult = false;


   //   if(!::axis::application::process_initialize())
   //      return false;


   //   if(is_system())
   //   {

   //      draw2d_factory_exchange();

   //   }

   //   m_bAxisProcessInitializeResult = true;

   //   return true;

   //}


   //bool application::initialize_instance()
   //{

   //   if(!::axis::application::initialize_instance())
   //      return false;

   //   return true;

   //}


   //bool application::initialize1()
   //{

   //   if(m_bAxisInitialize1)
   //      return m_bAxisInitialize1Result;

   //   m_bAxisInitialize1 = true;

   //   m_bAxisInitialize1Result = false;

   //   if(!::axis::application::initialize1())
   //      return false;

   //   m_dwAlive = ::get_tick_count();

   //   m_bAxisInitialize1Result = true;

   //   return true;

   //}


   //bool application::initialize2()
   //{

   //   if(!::axis::application::initialize2())
   //      return false;

   //   return true;

   //}


   //bool application::initialize3()
   //{

   //   if(!::axis::application::initialize3())
   //      return false;

   //   return true;

   //}


   //bool application::initialize()
   //{

   //   if(!::axis::application::initialize())
   //      return false;



   //   return true;

   //}


   //int32_t application::exit_instance()
   //{


   //   try
   //   {

   //      m_iReturnCode = ::axis::application::exit_instance();

   //   }
   //   catch(...)
   //   {

   //      m_iReturnCode = -1;

   //   }


   //   return m_iReturnCode;

   //}


   //bool application::finalize()
   //{

   //   bool bOk = false;

   //   try
   //   {

   //      bOk = ::axis::application::finalize();

   //   }
   //   catch(...)
   //   {

   //      bOk = false;

   //   }

   //   return bOk;

   //}


   void application::on_exclusive_instance_conflict(EExclusiveInstance eexclusive)
   {

      if(eexclusive == ExclusiveInstanceLocal)
      {

         on_exclusive_instance_local_conflict();

      }

   }


   void application::on_exclusive_instance_local_conflict()
   {

   }


   string application::get_mutex_name_gen()
   {
      return m_strAppName;
   }



   //bool application::Ex2OnAppInstall()
   //{
   //   return true;
   //}

   //bool application::Ex2OnAppUninstall()
   //{
   //   return true;
   //}


   void application::draw2d_factory_exchange()
   {

#ifdef CUBE

      ::draw2d_factory_exchange(this);

#else

      string strLibrary = draw2d_get_default_library_name();

      if(strLibrary.is_empty())
         strLibrary = "draw2d_cairo";

      ::aura::library & library = System.m_libraryDraw2d;

      if(library.is_opened())
         return;

      if(!library.open(strLibrary))
      {

         if(strLibrary != "draw2d_cairo")
         {
            if(!library.open("draw2d_cairo"))
            {
               throw "failed to do draw2d factory exchange";
            }
         }
         else
         {
            throw "failed to do draw2d factory exchange";
         }
      }


      PFN_ca2_factory_exchange pfn_ca2_factory_exchange = library.get < PFN_ca2_factory_exchange >("ca2_factory_exchange");

      pfn_ca2_factory_exchange(this);

#endif


   }












   int32_t application::simple_message_box_timeout(::aura::interaction * pwndOwner,const char * pszMessage,::duration durationTimeOut,UINT fuStyle)
   {
      UNREFERENCED_PARAMETER(durationTimeOut);
      return simple_message_box(pwndOwner,pszMessage,fuStyle);
   }


   //   void application::tellme_destroyed(::user::interaction * pui, bool * pDestroyed, bool bTell)
   //   {
   //
   //      if(pui == NULL)
   //         return;
   //
   //      try
   //      {
   //
   //#ifdef LINUX
   //
   //         synch_lock sl(get_ui_destroyed_mutex());
   //
   //         if(bTell)
   //         {
   //
   //            if(pui->m_bDestroying)
   //            {
   //
   //               *pDestroyed = true;
   //
   //            }
   //            else
   //            {
   //
   //               pui->m_bptraTellMeDestroyed.add(pDestroyed);
   //
   //            }
   //
   //         }
   //         else
   //         {
   //
   //            pui->m_bptraTellMeDestroyed.remove(pDestroyed);
   //
   //         }
   //
   //#endif
   //
   //      }
   //      catch(...)
   //      {
   //
   //      }
   //
   //   }


   string application::get_license_id()
   {

      if(m_strLicense.has_char())
         return m_strLicense;

      if(m_strInstallToken.has_char())
         return m_strInstallToken;

      return m_strAppId;

   }




   ::user::user * application::create_user()
   {

      return canew(::user::user(this));

   }


   void application::on_create_keyboard()
   {

      Session.user()->set_keyboard_layout(NULL,::action::source::database());

   }


   bool application::defer_initialize_twf()
   {

      if(System.m_ptwf == NULL && (System.m_bShouldInitializeGTwf && m_bShouldInitializeGTwf && m_bInitializeProDevianMode))
      {

         if(!System.initialize_twf())
            return false;

      }

      return true;

   }


   //bool application::assert_user_logged_in()
   //{

   //   return axis::application::assert_user_logged_in();

   //}


   ::user::interaction * application::get_active_guie()
   {

      return Session.get_active_guie();

   }


   ::user::interaction * application::get_focus_guie()
   {

      return Session.get_focus_guie();

   }




   bool application::on_thread_on_idle(::thread_impl * pimpl,LONG lCount)
   {

      //      ASSERT_VALID(this);
      //
      //#if defined(WINDOWS) && defined(DEBUG) && !defined(___NO_DEBUG_CRT)
      //      // check core API's allocator (before idle)
      //      if(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) & _CRTDBG_CHECK_ALWAYS_DF)
      //         ASSERT(__check_memory());
      //#endif

      single_lock sl(&pimpl->m_mutexUiPtra,TRUE);

      ::user::interaction * pui = NULL;


      if(lCount <= 0)
      {
         while(get_frame(pui))
         {
            //::user::interaction * pui = (::user::interaction *) pimpl->m_spuiptra->element_at(i)->m_pvoidUserInteraction;
            bool bOk = false;
            try
            {

               bOk = pui != NULL && pui->IsWindowVisible();
            }
            catch(...)
            {
            }
            //if(!bOk)
            //{
            //   try
            //   {
            //      Application.remove_frame(pui);
            //   }
            //   catch(...)
            //   {
            //   }
            //   try
            //   {
            //      Session.remove_frame(pui);
            //   }
            //   catch(...)
            //   {
            //   }
            //   try
            //   {
            //      System.remove_frame(pui);
            //   }
            //   catch(...)
            //   {
            //   }
            //}
            //else
            //{
            //   sl.unlock();
            //   try
            //   {
            //      //pui->send_message(WM_IDLEUPDATECMDUI,(WPARAM)TRUE);
            //   }
            //   catch(...)
            //   {

            //   }
            //   sl.lock();
            //}
         }


      }
      else if(lCount >= 0)
      {
      }

      //#if defined(WINDOWS) && defined(DEBUG) && !defined(___NO_DEBUG_CRT)
      //      // check core API's allocator (after idle)
      //      if(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) & _CRTDBG_CHECK_ALWAYS_DF)
      //         ASSERT(__check_memory());
      //#endif
      //


      return lCount < 0;  // nothing more to do if lCount >= 0

   }


   bool application::post_user_message(::thread_impl * pimpl,::aura::interaction * pui,UINT message,WPARAM wparam,lparam lparam)
   {

      if(pimpl->m_hthread == NULL)
         return false;

      ::user::message * pmessage    = new ::user::message;
      pmessage->m_pui               = (::user::interaction *) pui->m_pvoidUserInteraction;
      pmessage->m_uiMessage         = message;
      pmessage->m_wparam            = wparam;
      pmessage->m_lparam            = lparam;

      return pimpl->post_thread_message(WM_APP + 1984,77,(LPARAM)pmessage) != FALSE;

   }



   bool application::is_window(::aura::interaction * pui)
   {

      return ((::user::interaction *)pui->m_pvoidUserInteraction)->IsWindow();

   }

   LRESULT application::send_message(::aura::interaction * pui,UINT message,WPARAM wparam,lparam lparam)
   {

      return ((::user::interaction *)pui->m_pvoidUserInteraction)->send_message(message,wparam,lparam);

   }

   oswindow application::get_safe_handle(::aura::interaction * pui)
   {

      return ((::user::interaction *)pui->m_pvoidUserInteraction)->get_safe_handle();

   }

   void application::dispatch_user_message(::signal_details * pobj)
   {

      smart_pointer < ::user::message > spmessage(pobj->m_lparam);
      spmessage->send();
      pobj->m_uiMessage   = 0;    // ssshhhh.... - self-healing - sh...
      pobj->m_wparam      = 0;    // ssshhhh.... - self-healing - sh...
      pobj->m_bRet        = true;
      return;

   }


   ::user::interaction * application::get_parent(::user::interaction * pui)
   {

      return pui->GetParent();

   }


   bool application::enable_window(::aura::interaction * pui,bool bEnable)
   {

      // if control has the focus, move the focus before disabling
      if(!bEnable)
      {

         if(System.get_focus_guie() == ((::user::interaction *)pui->m_pvoidUserInteraction))
         {

            Application.send_message(Application.get_parent(((::user::interaction *)pui->m_pvoidUserInteraction)),WM_NEXTDLGCTL,0,(LPARAM)FALSE);

         }

      }

      return ((::user::interaction *)pui->m_pvoidUserInteraction)->enable_window(bEnable);

   }


   bool application::set_window_text(::user::interaction * pui,const string & strText)
   {

      pui->SetWindowText(strText);

      return true;

   }


   ptr_array < ::thread > application::get_thread(::aura::interaction * pui)
   {

      return ((::user::interaction *)pui->m_pvoidUserInteraction)->m_threadptra;

   }


   void application::add_thread(::aura::interaction * pui,::thread * pthread)
   {

      ((::user::interaction *)pui->m_pvoidUserInteraction)->m_threadptra.add_unique(pthread);

   }

   void application::remove_thread(::aura::interaction * pui,::thread * pthread)
   {

      ((::user::interaction *)pui->m_pvoidUserInteraction)->m_threadptra.remove(pthread);

   }

   void application::window_graphics_update_window(window_graphics * & pdata,oswindow interaction_impl,COLORREF * pOsBitmapData,const RECT & rect,int cxParam,int cyParam,int iStride,bool bTransferBuffer)
   {

      window_graphics::update_window(pdata,interaction_impl,pOsBitmapData,rect,cxParam,cyParam,iStride, bTransferBuffer);

   }



   sp(::message::base) application::get_message_base(LPMESSAGE lpmsg)
   {

      ::user::interaction * pwnd = NULL;

      if(pwnd == NULL && lpmsg->hwnd != NULL)
      {

         if(lpmsg->message == 126)
         {

            TRACE0("WM_DISPLAYCHANGE");

         }

         ::user::interaction * pwindow = System.window_from_os_data(lpmsg->hwnd);

         if(pwindow != NULL)
         {
            try
            {
               pwnd = pwindow;
            }
            catch(...)
            {
               pwnd = NULL;
            }
         }

         if(pwnd == NULL)
            return NULL;

      }


      if(pwnd != NULL)
         return pwnd->get_base(lpmsg->message,lpmsg->wParam,lpmsg->lParam);

      ::thread * pthread = ::get_thread();

      if(pthread != NULL)
         return pthread->get_base(lpmsg->message,lpmsg->wParam,lpmsg->lParam);

      return get_base(lpmsg->message,lpmsg->wParam,lpmsg->lParam);

   }


   void application::process_message(signal_details * pobj)
   {

      sp(::message::base) pbase = pobj;

      if(pbase.is_null() || pbase->m_pwnd == NULL)
      {

         try
         {

            message_handler(pobj);

         }
         catch(const ::exception::exception & e)
         {

            TRACE("application::process_message : error processing application thread message (const ::exception::exception & )");

            if(App(this).on_run_exception((::exception::exception &) e))
               goto run;

            if(App(this).final_handle_exception((::exception::exception &) e))
               goto run;

            __post_quit_message(-1);

            pbase->set_lresult(-1);

            return;

         }
         catch(::exception::base * pe)
         {

            process_window_procedure_exception(pe,pbase);

            TRACE(::aura::trace::category_AppMsg,0,"Warning: Uncaught exception in message_handler (returning %ld) (application::process_message : error processing application thread message).\n",(int_ptr)pbase->get_lresult());

            pe->Delete();

         }
         catch(...)
         {

            TRACE("application::process_message : error processing application thread message (...)");

         }

         return;

      }

      try
      {

         ((::user::interaction *)pbase->m_pwnd->m_pvoidUserInteraction)->message_handler(pobj);


      }
      catch(const ::exception::exception & e)
      {

         TRACE("application::process_message : error processing window message (const ::exception::exception & )");

         if(App(this).on_run_exception((::exception::exception &) e))
            goto run;

         if(App(this).final_handle_exception((::exception::exception &) e))
            goto run;

         __post_quit_message(-1);

         pbase->set_lresult(-1);

         return;

      }
      catch(::exception::base * pe)
      {

         process_window_procedure_exception(pe,pbase);

         TRACE(::aura::trace::category_AppMsg,0,"Warning: Uncaught exception in message_handler (returning %ld) (application::process_message : error processing window message).\n",(int_ptr)pbase->get_lresult());

         pe->Delete();

      }
      catch(...)
      {

         TRACE("application::process_message : error processing window message (...)");

      }

   run:;

   }

   int32_t application::simple_message_box(::aura::interaction * puiOwner,const char * pszMessage,UINT fuStyle)
   {

#if defined(WINDOWSEX)

      return ::simple_message_box(puiOwner->get_safe_handle(),pszMessage,m_strAppName,fuStyle);
      //return MessageBoxW(puiOwner->get_safe_handle(),wstring(pszMessage),wstring(m_strAppName),fuStyle);
      //return MessageBoxW((puiOwner == NULL ? NULL : (puiOwner->get_wnd() == NULL ? NULL : puiOwner->get_handle())),
      //   wstring(pszMessage), wstring(m_strAppName), fuStyle);

#elif  defined(LINUX) || defined(APPLEOS) || defined(ANDROID)

      return ::simple_message_box(puiOwner->get_safe_handle(),pszMessage,m_strAppName,fuStyle);
      //   return simple_message_box((puiOwner == NULL ? NULL : (puiOwner->get_wnd() == NULL ? NULL : puiOwner->get_handle())), pszMessage, m_strAppName, fuStyle);

#else

      return ::simple_message_box(puiOwner->get_safe_handle(),pszMessage,m_strAppName,fuStyle);

#endif

   }

   /*
   int32_t application::simple_message_box(sp(::user::interaction) puiOwner, const char * pszMessage, UINT fuStyle)
   {

   #if defined(WINDOWSEX)

   return MessageBoxW((puiOwner == NULL ? NULL : (puiOwner->get_wnd() == NULL ? NULL : puiOwner->get_handle())),
   wstring(pszMessage), wstring(m_strAppName), fuStyle);

   #elif  defined(LINUX) || defined(APPLEOS) || defined(ANDROID)

   return simple_message_box((puiOwner == NULL ? NULL : (puiOwner->get_wnd() == NULL ? NULL : puiOwner->get_handle())), pszMessage, m_strAppName, fuStyle);

   #else

   return simple_message_box(m_psystem->m_posdata->m_pui->get_handle(), pszMessage, m_strAppName, fuStyle);

   #endif

   }
   */

   int32_t application::simple_message_box(const char * pszMessage,UINT fuStyle)
   {

#if defined(WINDOWSEX)

      return ::simple_message_box(NULL,pszMessage,m_strAppName,fuStyle);
      //return MessageBoxW(NULL,wstring(pszMessage),wstring(m_strAppName),fuStyle);
      //return MessageBoxW((puiOwner == NULL ? NULL : (puiOwner->get_wnd() == NULL ? NULL : puiOwner->get_handle())),
      //   wstring(pszMessage), wstring(m_strAppName), fuStyle);

#elif  defined(LINUX) || defined(APPLEOS) || defined(ANDROID)

      return ::simple_message_box(NULL,pszMessage,m_strAppName,fuStyle);
      //   return simple_message_box((puiOwner == NULL ? NULL : (puiOwner->get_wnd() == NULL ? NULL : puiOwner->get_handle())), pszMessage, m_strAppName, fuStyle);

#else

      return ::simple_message_box(NULL,pszMessage,m_strAppName,fuStyle);

#endif

   }

   string application::message_box(const string & pszMatter,property_set & propertyset)
   {

      simple_message_box(pszMatter,0);

      return "";

   }


   //int32_t application::simple_message_box_timeout(sp(::user::interaction) pwndOwner,const char * pszMessage,::duration durationTimeOut,UINT fuStyle)
   //{
   //   UNREFERENCED_PARAMETER(durationTimeOut);
   //   return simple_message_box(pwndOwner,pszMessage,fuStyle);
   //}


   //::user::user * application::create_user()
   //{

   //   return canew(::user::user(this));

   //}

   bool application::set_keyboard_layout(const char * pszPath,::action::context actioncontext)
   {

      return Session.user()->set_keyboard_layout(pszPath,actioncontext);

   }


   ::user::interaction * application::main_window()
   {

      if(m_puiMain == NULL)
         return NULL;

      return (::user::interaction *) m_puiMain->m_pvoidUserInteraction;

   }


   sp(type) application::user_default_controltype_to_typeinfo(::user::e_control_type e_type)
   {

      return sp(type)();

   }


   int32_t application::hotplugin_host_starter_start_sync(const char * pszCommandLine,::aura::application * papp,hotplugin::host * phost,hotplugin::plugin * pplugin)
   {

      string strValue;

      if(get_command_line_param(strValue,pszCommandLine,"enable_desktop_launch"))
      {

#ifdef METROWIN

         return -1;

#else

         return ::call_sync(::path::a_spa(),pszCommandLine,::dir::a_spa(),SW_SHOWNORMAL,840,84,NULL,0);

#endif

      }
      else
      {

         return hotplugin::host::host_starter_start_sync(pszCommandLine,get_app(),NULL);

      }

   }


   bool application::BaseOnControlEvent(::user::control_event * pevent)
   {

      return false;

   }

   
   bool application::on_open_document(::aura::document * pdocument,var varFile)
   {

      UNREFERENCED_PARAMETER(pdocument);
      UNREFERENCED_PARAMETER(varFile);
      return true;

   }


   bool application::on_save_document(::aura::document * pdocument,var varFile)
   {
      
      UNREFERENCED_PARAMETER(pdocument);
      UNREFERENCED_PARAMETER(varFile);
      return true;

   }

   
   void application::on_update_view(::aura::impact * pview,::aura::impact * pviewSender,LPARAM lHint,object* pHint)
   {


   }


   bool application::keyboard_focus_is_focusable(::user::elemental * pue)
   {

      return false;

   }


   bool application::keyboard_focus_OnSetFocus(::user::elemental * pue)
   {

      return true;

   }


} // namespace axis







