#include "StdAfx.h"


namespace plane
{


   session::session()
   {

      m_bDrawCursor              = true;
      m_pbergedge                = NULL;
/*      m_pnaturedocument          = NULL;
      m_pplatformdocument        = NULL;
      m_pbergedgedocument        = NULL;*/
      m_bShowPlatform            = false;
      m_pappCurrent              = NULL;
      m_psession                 = this;
      m_pbergedgeInterface       = NULL;

      ::fs::set * pset = new class ::fs::set(this);
      pset->m_spafsdata.add(new ::fs::native(this));
      m_spfsdata(pset);


      m_pifs                     = new ifs(this, "");

   }

   session::~session()
   {

      POSITION pos = m_mapApplication.get_start_position();

/*      if(m_pnaturedocument != NULL)
      {
         m_pnaturedocument->on_close_document();
      }

      if(m_pplatformdocument != NULL)
      {
         m_pplatformdocument->on_close_document();
      }

      if(m_pbergedgedocument != NULL)
      {
         m_pbergedgedocument->on_close_document();
      }
      */
      string strId;
      ::ca::application * pcaapp;

      while(pos != NULL)
      {

         strId.Empty();
         pcaapp = NULL;

         m_mapApplication.get_next_assoc(pos, strId, pcaapp);

         ::ca2::application * papp = dynamic_cast < ::ca2::application * > (pcaapp);

         papp->PostThreadMessageA(WM_QUIT, 0, 0);
      }

   }

   void session::construct()
   {
      m_strAppName         = "session";
      m_strBaseSupportId   = "votagus_ca2_bergedge";
      m_strInstallToken    = "session";
      m_strLicense         = "";
      m_eexclusiveinstance = ::radix::ExclusiveInstanceNone;
   }

   bool session::initialize_instance()
   {

      if(!::planebase::application::initialize_instance())
         return false;

      initialize_bergedge_application_interface();


      //GetStdFileManagerTemplate()->m_strLevelUp = "levelup";


      SetRegistryKey("ca2core");


      if(!InitializeLocalDataCentral())
      {
         simple_message_box(NULL, "Could not initialize Local data central");
         return false;
      }


      return true;
   }

   BOOL session::exit_instance()
   {
      try
      {
         ::planebase::application::exit_instance();
      }
      catch(...)
      {
      }
      return 0;
   }

   bool session::bergedge_start()
   {
      return true;
   }



   void session::_001OnFileNew()
   {
      //userbase::application::m_pdocmanager->_001OnFileNew();
   }


   bool session::_001OnCmdMsg(BaseCmdMsg * pcmdmsg)

   {
      return gen::application::_001OnCmdMsg(pcmdmsg);
   }

   ::ca::application * session::get_app() const
   {
      return ::planebase::application::get_app();
   }

   void session::load_string_table()
   {
      fontopus::application::load_string_table();
      fontopus::application::load_string_table("plane", "");
   }

/*   bool session::file_manager_open_file(
            ::filemanager::data * pdata,
            ::fs::item_array & itema)
   {
      UNREFERENCED_PARAMETER(pdata);
      if(itema.get_size() > 0)
      {
         return true;
      }
      return false;
   }*/


   void session::initialize_bergedge_application_interface()
   {
      int iCount = 32; // todo: get from session profile
/*      System.factory().creatable < ::session::document > (iCount);
      System.factory().creatable < ::session::view > (iCount);
      System.factory().creatable < ::session::pane_view > (iCount);
      System.factory().creatable < ::session::frame > (iCount);
      System.factory().creatable < plane::document > (iCount);
      System.factory().creatable < plane::view > (iCount);
      System.factory().creatable < plane::pane_view > (iCount);
      System.factory().creatable < plane::frame > (iCount);
      System.factory().creatable < nature::document > (iCount);
      System.factory().creatable < nature::view > (iCount);
      System.factory().creatable < nature::pane_view > (iCount);
      System.factory().creatable < nature::frame > (iCount);
      m_ptemplate_bergedge    = new ::userbase::single_document_template(
         this,
         "session/frame",
         System.template type_info < document > (),
         System.template type_info < frame > (),
         System.template type_info < view > ());
      m_ptemplate_platform    = new ::userbase::single_document_template(
         this,
         "session/frame",
         System.template type_info < plane::document > (),
         System.template type_info < plane::frame > (),
         System.template type_info < plane::pane_view > ());
      m_ptemplate_nature      = new ::userbase::single_document_template(
         this,
         "session/frame",
         System.template type_info < nature::document > (),
         System.template type_info < nature::frame > (),
         System.template type_info < nature::view > ());
      m_pnaturedocument = NULL;*/
   }

   bool session::create_bergedge(::ca::create_context * pcreatecontext)
   {
/*      if(m_pbergedgedocument == NULL)
      {

         ::ca::create_context_sp createcontextBergedge(get_app());
         createcontextBergedge.oattrib(pcreatecontext);
         createcontextBergedge->m_spCommandLine->m_varFile.set_type(var::type_empty);
         createcontextBergedge->m_bMakeVisible = false;

         m_pbergedgedocument = dynamic_cast < document * > (m_ptemplate_bergedge->open_document_file(createcontextBergedge));
         m_pbergedgedocument->m_pbergedge = this;
      
      }
      if(m_bShowPlatform)
      {
         if(m_pplatformdocument == NULL)
         {

            ::ca::create_context_sp createcontextPlatform;
            createcontextPlatform.oattrib(pcreatecontext);
            createcontextPlatform->m_spCommandLine->m_varFile.set_type(var::type_empty);
            createcontextPlatform->m_bMakeVisible = true;
            createcontextPlatform->m_puiParent = m_pbergedgedocument->get_bergedge_view();

            m_pplatformdocument  = dynamic_cast < plane::document * > (m_ptemplate_platform->open_document_file(createcontextPlatform));
            m_pplatformdocument->m_pbergedgedocument =  m_pbergedgedocument;
            //m_pnaturedocument    =
            // dynamic_cast < nature::document * > (
             //  papp->m_ptemplate_nature->open_document_file(NULL, false, m_pbergedgedocument->get_bergedge_view()));

            m_pbergedgedocument->set_platform(m_pplatformdocument);
            //m_pbergedgedocument->set_nature(m_pnaturedocument);
         }
      }
      return m_pbergedgedocument != NULL;*/
      return false;
   }

   void session::launch_app(const char * psz)
   {
      UNREFERENCED_PARAMETER(psz);
   }

   void session::install_app(const char * psz)
   {
      UNREFERENCED_PARAMETER(psz);
   }

   void session::on_request(::ca::create_context * pcreatecontext)
   {

      if(m_pbergedgeInterface != NULL)
      {

         m_pbergedgeInterface->on_request(pcreatecontext);
         return;

      }

      m_varCurrentViewFile = pcreatecontext->m_spCommandLine->m_varFile;


      string strApp;

      if((pcreatecontext->m_spCommandLine->m_varQuery["show_platform"] == 1 || command().m_varTopicQuery["show_platform"] == 1)
         && (!(bool)pcreatecontext->m_spCommandLine->m_varQuery["client_only"] && !(bool)command().m_varTopicQuery["client_only"])
         && (!pcreatecontext->m_spCommandLine->m_varQuery.has_property("client_only") && !command().m_varTopicQuery.has_property("client_only")))
      {
         m_bShowPlatform = true;
      }

      bool bCreate = true;
      if(pcreatecontext->m_spCommandLine->m_strApp.is_empty())
      {
         if(!pcreatecontext->m_spCommandLine->m_varFile.is_empty())
         {
            if(!open_by_file_extension(pcreatecontext))
            {
               if(m_pappCurrent != NULL)
               {
                  App(m_pappCurrent).request(pcreatecontext->m_spCommandLine);
               }
            }
            bCreate = false;
         }
         else if(m_bShowPlatform)
         {
            strApp = "bergedge";
//            create_bergedge(pcreatecontext);
  //          throw not_implemented_exception();
            /*if(get_document() != NULL && get_document()->get_typed_view < ::session::view >() != NULL)
            {
               ::simple_frame_window * pframe = dynamic_cast < ::simple_frame_window * > (get_document()->get_typed_view < ::session::view >()->GetParentFrame());
               if(pframe != NULL)
               {
                  pframe->ShowWindow(SW_SHOW);
                  pframe->InitialFramePosition();
               }
            }*/
            bCreate = true;
         }
         
      }
      if(bCreate)
      {

         if(strApp.is_empty())
         {
            if(pcreatecontext->m_spCommandLine->m_strApp == "session")
            {
               if(pcreatecontext->m_spCommandLine->m_varQuery.has_property("session_start"))
               {
                  strApp = pcreatecontext->m_spCommandLine->m_varQuery["session_start"];
               }
               else
               {
                  strApp = "session";
               }
            }
            else
            {
               strApp = pcreatecontext->m_spCommandLine->m_strApp;
            }
         }

         if(strApp.is_empty() || strApp == "session")
         {
            if(pcreatecontext->m_spCommandLine->m_varQuery.has_property("install")
            || pcreatecontext->m_spCommandLine->m_varQuery.has_property("uninstall"))
            {
               System.appptra().remove(this);
               return;
            }
            return;
         }
         
         ::plane::application * papp = dynamic_cast < ::plane::application * > (application_get(strApp, true, true, pcreatecontext->m_spCommandLine->m_pbiasCreate));
         if(papp == NULL)
            return;

         if(pcreatecontext->m_spCommandLine->m_varQuery.has_property("install")
         || pcreatecontext->m_spCommandLine->m_varQuery.has_property("uninstall"))
         {
            System.appptra().remove(papp);
            return;
         }

         pcreatecontext->m_spCommandLine->m_eventReady.ResetEvent();

         if(strApp != "session")
         {

            if(strApp == "bergedge")
            {
               m_pbergedge             = papp->get_bergedge();
               m_pbergedgeInterface    = papp;
            }


            UINT uiMessage = WM_APP + 2043;

            papp->PostThreadMessage(uiMessage, 2, (LPARAM) (::ca::create_context *) pcreatecontext);

            pcreatecontext->m_spCommandLine->m_eventReady.wait();


         }

         m_pappCurrent = papp;

      }



      
/*      if(m_bShowPlatform)
      {
         ::simple_frame_window * pframeApp = dynamic_cast < ::simple_frame_window * > (get_document()->get_typed_view < ::session::pane_view >()->get_view_uie());
         if(pframeApp != NULL)
         {
            pframeApp->WfiFullScreen(true, false);
         }
         ::simple_frame_window * pframe = dynamic_cast < ::simple_frame_window * > (get_document()->get_typed_view < ::session::pane_view >()->GetParentFrame());
         if(pframe != NULL)
         {
            pframe->ShowWindow(SW_SHOW);
         }
      }
      else
      {
         if(get_document() != NULL && get_document()->get_typed_view < ::session::view >() != NULL)
         {
            ::simple_frame_window * pframe = dynamic_cast < ::simple_frame_window * > (get_document()->get_typed_view < ::session::view >()->GetParentFrame());
            if(pframe != NULL)
            {
               pframe->ShowWindow(SW_SHOW);
               if(pframe->GetTypedParent < ::plugin::host_interaction > () != NULL)
               {
                  pframe->GetTypedParent < ::plugin::host_interaction > ()->layout();
               }
               else
               {
                  pframe->InitialFramePosition();
               }
            }
         }
      }

      try
      {
         get_view()->GetParentFrame()->SetWindowTextA(dynamic_cast < ::cube8::application * > (m_pappCurrent)->m_puser->m_strLogin);
      }
      catch(...)
      {
      }

      */


   }


   /*::session::document * session::get_document()
   {
      return m_pbergedgedocument;
   }

   ::session::view * session::get_view()
   {
      if(get_document() == NULL)
         return NULL;
      return get_document()->get_bergedge_view();
   }

   ::plane::document * session::get_platform()
   {
      return m_pplatformdocument;
   }

   ::nature::document * session::get_nature()
   {
      return m_pnaturedocument;
   }
   */
   bool session::open_by_file_extension(const char * pszPathName, ::ca::application_bias * pbiasCreate)
   {
      
      ::ca::create_context_sp cc(get_app());

      cc->m_spCommandLine->m_varFile = pszPathName;

      if(pbiasCreate != NULL)
      {
         cc->m_spApplicationBias->operator=(*pbiasCreate);
      }

      return open_by_file_extension(cc);

   }

   bool session::open_by_file_extension(::ca::create_context * pcreatecontext)
   {

      string strId;
      string strOriginalPathName(pcreatecontext->m_spCommandLine->m_varFile);
      string strPathName(strOriginalPathName);

      strPathName.trim();
      strPathName.trim("\"");

      strsize iFind = strPathName.find("?");
      if(iFind >= 0)
      {
         strPathName = strPathName.Left(iFind);
      }

      if(gen::str::ends_ci(strPathName, ".cgcl"))
      {
      }

      string strProtocol = System.url().get_protocol(strPathName);
      if(strProtocol == "app")
      {
         strId = System.url().get_server(strPathName);

         string str = System.url().get_object(strPathName);
         gen::str::begins_eat(str, "/");
         pcreatecontext->m_spCommandLine->m_varFile = str;
      }
      else
      {
         
         string strExtension = System.file().extension(strPathName);

         stringa straApp;

         //Cube.filehandler().get_extension_app(straApp, strExtension);
     

         if(straApp.get_count() == 1)
         {
            strId = straApp[0];
         }
         else
         {
            strId = "default_file_handler";
         }

      }


      /*|| gen::str::ends_ci(strPathName, ".cpp")
      || gen::str::ends_ci(strPathName, ".html"))
      {
         strId = "devedge";
      }
      else if(gen::str::ends_ci(strPathName, ".ca2"))
      {
         strId = Application.file().as_string(strOriginalPathName);
         if(gen::str::begins_eat(strId, "ca2prompt\r\n"))
         {
            strId.trim();
         }
         return false;
      }
      else if(gen::str::ends_ci(strPathName, ".txt")
      || gen::str::ends_ci(strOriginalPathName, ".pac"))
      {
         strId = "veriedit";
      }
      else if(gen::str::ends_ci(strPathName, ".ogv")
         || gen::str::ends_ci(strPathName, ".flv"))
      {
         strId = "verisimplevideo";
      }
      else if(gen::str::ends_ci(strPathName, ".kar")
      || gen::str::ends_ci(strPathName, ".mid")
      || gen::str::ends_ci(strPathName, ".mp3")
      || gen::str::ends_ci(strPathName, ".mk1"))
      {
         strId = "mplite";
      }
      else if(gen::str::ends_ci(strPathName, ".flv"))
      {
         strId = "verisimplevideo";
      }
      else if(gen::str::ends_ci(strPathName, ".bmp")
           || gen::str::ends_ci(strPathName, ".jpg")
           || gen::str::ends_ci(strPathName, ".jpeg")
           || gen::str::ends_ci(strPathName, ".png")
           || gen::str::ends_ci(strPathName, ".gif"))
      {
         strId = "eluce";
      }
      else if(gen::str::begins(strPathName, "rtprx://"))
      {
         strId = "rtprx";
      }
      else if(gen::str::begins(strPathName, "rtptx://"))
      {
         strId = "rtptx";
      }
      else
      {
#ifdef WINDOWS
         ::ShellExecuteW(
            NULL,
            L"open",
            gen::international::utf8_to_unicode(strPathName),
            NULL,
            NULL,
            SW_SHOW);
#endif
         return true;
      }*/
/*      ::user::application * papp = dynamic_cast < ::user::application * > (application_get(strId, true, true, pcreatecontext->m_spApplicationBias));
      if(papp == NULL)
         return false;
      papp->::ex1::request_interface::create(pcreatecontext);*/
      return true;
   }

   bool session::InitializeLocalDataCentral()
   {

/*      m_pdatabase = new nature::database(this);

      if(m_pdatabase == NULL)
      {
         TRACE("VmpLightApp::initialize_instance failed to instatiate LightDB\n");
         return false;
      }

      if(!m_pdatabase->Initialize())
      {
         TRACE("VmpLightApp::initialize_instance failed to initialize LightDB\n");
         return false;
      }*/

      return true;

   }

   session::run_application::run_application()
   {
      m_papp = NULL;
      m_puiParent = NULL;
   }

   void session::on_exclusive_instance_conflict(::radix::EExclusiveInstance eexclusive)
   {
      if(eexclusive == ::radix::ExclusiveInstanceLocalId)
      {
         gen::memory_file file(get_app());
         file.from_string(command().m_varTopicFile);
         COPYDATASTRUCT data;
         data.dwData = 1984;
         data.cbData = (DWORD) file.get_length();
         data.lpData = file.get_data();
         HWND hwnd = (HWND) ::FindWindowA(NULL, "ca2::fontopus::message_wnd::session::");

         ::SendMessage(hwnd, WM_COPYDATA, NULL, (LPARAM) &data);
      }
   }


   void session::request(::ca::create_context * pcreatecontext)
   {

      if(m_pbergedgeInterface != NULL)
      {

         m_pbergedgeInterface->request(pcreatecontext);
         return;

      }

      if(pcreatecontext->m_spCommandLine->m_varFile.get_type() == var::type_string)
      {
         if(gen::str::ends_ci(pcreatecontext->m_spCommandLine->m_varFile, ".ca2"))
         {
            string strCommand = Application.file().as_string(pcreatecontext->m_spCommandLine->m_varFile);
            if(gen::str::begins_eat(strCommand, "ca2prompt\r")
            || gen::str::begins_eat(strCommand, "ca2prompt\n"))
            {
               strCommand.trim();
               command().add_fork_uri(strCommand);
            }
            return;
         }
         else
         {
            on_request(pcreatecontext);
         }
      }
      else if(m_pappCurrent != NULL && m_pappCurrent != this 
         && (pcreatecontext->m_spCommandLine->m_strApp.is_empty() 
         ||App(m_pappCurrent).m_strAppName == pcreatecontext->m_spCommandLine->m_strApp))
      {
               

/*         if(get_document() != NULL && get_document()->get_typed_view < ::session::pane_view >() != NULL)
         {
            get_document()->get_typed_view < ::session::pane_view >()->set_cur_tab_by_id("app:" + App(m_pappCurrent).m_strAppName);
         }*/
         App(m_pappCurrent).request(pcreatecontext);
      }
      else
      {
         on_request(pcreatecontext);
      }
   }

   void session::request_topic_file(var & varQuery)
   {
      request(m_varTopicFile, varQuery);
   }

   void session::request_topic_file()
   {
      request(m_varTopicFile);
   }

   /*void session::request_application(const char * pszId, var varFile, var varQuery, ::ca::application_bias * pbiasCreate)
   {

      ::ca2::application_request request;

      request.m_iEdge         = m_iEdge;
      request.m_strApp        = pszId;
      request.m_varFile       = varFile;
      request.m_varQuery      = varQuery;
      request.m_pbiasCreate   = pbiasCreate;

      request_application(&request);

   }*/

   ::ca::application * session::application_get(const char * pszId, bool bCreate, bool bSynch, ::ca::application_bias * pbiasCreate)
   {
      ::ca::application * papp = NULL;

      if(m_mapApplication.Lookup(pszId, papp))
         return papp;
      else
      {
         if(!bCreate)
            return NULL;
         papp = NULL;
         try
         {
            
            papp = create_application(pszId, bSynch, pbiasCreate);
         }
         catch(...)
         {
            papp = NULL;
         }
         if(papp == NULL)
            return NULL;
         m_mapApplication.set_at(pszId, papp);
         return papp;
      }
   }

   bool session::is_session()
   {
      return true;
   }

   ::ca::application * session::get_current_application()
   {
      return m_pappCurrent;
   }


   void session::check_topic_file_change()
   {
      if(m_varCurrentViewFile != m_varTopicFile && !m_varTopicFile.is_empty())
      {
         m_varCurrentViewFile = m_varTopicFile;
         request_topic_file();
      }
   }

   ::user::interaction * session::get_request_parent_ui(::userbase::main_frame * pmainframe, ::ca::create_context * pcreatecontext)
   {

      return get_request_parent_ui((::user::interaction * ) pmainframe, pcreatecontext);

   }

   ::user::interaction * session::get_request_parent_ui(::user::interaction * pinteraction, ::ca::create_context * pcreatecontext)
   {


      ::user::interaction * puiParent = NULL;

      if(pcreatecontext->m_spCommandLine->m_varQuery["uicontainer"].ca2 < ::user::interaction >() != NULL)
         puiParent = pcreatecontext->m_spCommandLine->m_varQuery["uicontainer"].ca2 < ::user::interaction >();

      if(puiParent == NULL && pcreatecontext->m_puiParent != NULL)
      {
         puiParent = pcreatecontext->m_puiParent;
      }

      if(puiParent == NULL && pcreatecontext->m_spCommandLine->m_pbiasCreate != NULL)
      {
         puiParent = pcreatecontext->m_spCommandLine->m_pbiasCreate->m_puiParent;
      }

      if(puiParent == NULL && pcreatecontext->m_spApplicationBias.is_set())
      {
         puiParent = pcreatecontext->m_spApplicationBias->m_puiParent;
      }


/*      if(pui == NULL && m_puiInitialPlaceHolderContainer != NULL)
      {
         pui = m_puiInitialPlaceHolderContainer;
      }*/

/*      if(pui == NULL && m_bShowPlatform && m_pbergedge->get_document() != NULL)
      {
         pui = Bergedge.get_document()->get_bergedge_view();
      }

      return pui;

   }*/

      if(pinteraction->get_app()->is_bergedge() || pcreatecontext->m_bClientOnly ||
         Application.directrix().m_varTopicQuery["client_only"] != 0 ||
         pcreatecontext->m_bOuterPopupAlertLike)
      {
         return puiParent;
      }

      if(!create_bergedge(pcreatecontext))
      {
         return NULL;
      }




/*      cube8::application & app = App(pinteraction->get_app());

      string strAppName = app.m_strAppName;

      if(strAppName != "session")
      {

         if(get_document() != NULL)
         {

            if(get_document()->get_typed_view < ::session::pane_view >() != NULL)
            {

               get_document()->get_typed_view < ::session::pane_view >()->set_cur_tab_by_id("app:" + strAppName);

               puiParent = get_document()->get_typed_view < ::session::pane_view >()->get_tab_holder(get_document()->get_typed_view < ::session::pane_view >()->get_tab_by_id("app:" + strAppName));

            }
            else
            {

               puiParent = get_document()->get_typed_view < ::session::view >();

            }

         }

      }*/

      return puiParent;

   }

/*   ::user::place_holder_ptra session::get_place_holder(::userbase::main_frame * pmainframe, ::ca::create_context * pcreatecontext)
   {

      UNREFERENCED_PARAMETER(pcreatecontext);

      ::user::place_holder_ptra holderptra;


      cube8::application & app = App(pmainframe->get_app());

      string strAppName = app.m_strAppName;

      if(get_document()->get_typed_view < ::session::pane_view >() != NULL)
      {

         get_document()->get_typed_view < ::session::pane_view >()->set_cur_tab_by_id("app:" + strAppName);

         holderptra.add(get_document()->get_typed_view < ::session::pane_view >()->get_tab_holder(get_document()->get_typed_view < ::session::pane_view >()->get_tab_by_id("app:" + strAppName)));

      }
      else
      {

         holderptra.add(get_document()->get_typed_view < ::session::view >());

      }

      return holderptra;

   }*/

   /*
   bool session::place(::userbase::main_frame * pmainframe, ::ca::create_context * pcreatecontext)
   {

      get_place_holder(pmainframe, pcreatecontext).hold(pmainframe);

      return true;

   }
   */

   void session::get_screen_rect(LPRECT lprect)
   {
            

/*      if(get_document() != NULL && get_view() != NULL)
      {
         get_view()->GetWindowRect(lprect);
      }
      else 
      {
         System.get_screen_rect(lprect);
      }*/
   }

   bool session::on_install()
   {

      string strFormat;

      string strSentinelPath;

      strSentinelPath = System.dir().ca2("stage/x86/app-sentinel.exe");

      System.os().local_machine_set_run("ca2 app-sentinel", "\"" + strSentinelPath + "\"");


      System.os().defer_register_ca2_plugin_for_mozilla();



      return ::planebase::application::on_install();
   }


   void session::set_app_title(const char * pszAppId, const char * pszTitle)
   {

      ::ca::application * papp = NULL;

      if(m_mapApplication.Lookup(pszAppId, papp) && papp != NULL)
      {

/*         ::session::pane_view * ppaneview = get_document()->get_typed_view < ::session::pane_view >();

         if(ppaneview != NULL)
         {
         
            string strAppName(pszAppId);

            ::user::tab::pane * ppane = ppaneview->get_pane_by_id("app:" + strAppName);

            if(ppane != NULL)
            {

               ppane->m_strTitleEx = pszTitle;

               ppaneview->layout();

            }

         }*/

      }


   }

   //////////////////////////////////////////////////////////////////////////////////////////////////
   // Session/Bergedge
   //
   ::bergedge::view * session::get_view()
   {

      if(m_pbergedgeInterface != NULL)
      {
         return m_pbergedgeInterface->get_view();
      }

      return NULL;

   }

   ::bergedge::document * session::get_document()
   {

      if(m_pbergedgeInterface != NULL)
      {
         return m_pbergedgeInterface->get_document();
      }

      return NULL;

   }

   FileManagerTemplate * session::GetStdFileManagerTemplate()
   {
      
      if(m_pbergedgeInterface != NULL)
      {
         return m_pbergedgeInterface->GetStdFileManagerTemplate();
      }

      return NULL;

   }


} // namespace plane



