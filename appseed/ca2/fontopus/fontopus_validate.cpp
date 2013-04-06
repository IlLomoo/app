#include "framework.h"


namespace ca
{


   namespace fontopus
   {


      validate::validate(sp(::ca::application) papp, const char * pszForm, bool bAuth, bool bInteractive) :
         ca(papp),
         ::fontopus::validate(papp, pszForm, bAuth, bInteractive),
         m_netcfg(papp)
      {
         m_bInteractive          = bInteractive;
         m_bAuth          = bAuth;
         m_strForm               = pszForm;
         m_bDeferRegistration    = false;
         sp(::ca::application) pgenapp = (papp);
         if(pgenapp != ::null())
         {
            try
            {
               pgenapp->keep_alive();
            }
            catch(...)
            {
            }
         }
         //Sleep(15 * 1000);
         m_ptemplatePane   = new ::userbase::single_document_template(
            papp,
            "system/auth",
            System.type_info < form_document > (),
            System.type_info < simple_main_frame > (),
            System.type_info < userex::pane_tab_view > ());
         m_pauth           = ::null();
         m_pviewAuth       = ::null();
         m_pdocAuth        = ::null();
         m_pdoc            = ::null();
         m_ptabview        = ::null();
         m_pvOldWindow     = ::null();
      }

      validate::~validate()
      {
#if defined(LINUX) || defined(MACOS)
         //((HTHREAD) m_loginthread.get_os_data())->wait();
#else
         ::WaitForSingleObjectEx((HANDLE) m_loginthread.get_os_data(), INFINITE, FALSE);
#endif
      }

      void validate::close_all()
      {

         if(m_ptabview != ::null())
         {
            m_ptabview->get_wnd()->EndAllModalLoops(IDOK);
         }

         if(m_ptemplatePane != ::null())
         {
            m_ptemplatePane->close_all_documents(FALSE);
         }

      }

      void validate::show_and_request_auth()
      {

         if(m_loginthread.m_strLicense.is_empty())
         {
            m_pviewAuth->SetTimer(1984, 484, ::null());
         }
         ::ca::live_signal livesignal;
         livesignal.keep(get_app());
         m_ptabview->get_wnd()->RunModalLoop(MLF_NOIDLEMSG | MLF_NOKICKIDLE, &livesignal);
         m_ptemplatePane->close_all_documents(FALSE);

      }



      void validate::ensure_main_document()
      {

         if(m_pdoc != ::null())
            return;

         sp(::ca::create_context) createcontext(allocer());
         createcontext->m_bMakeVisible = false;
         createcontext->m_puiParent = Sys(get_app()).oprop("top_parent").ca < ::user::interaction > ();
         createcontext->m_bOuterPopupAlertLike = true;
         //Sleep(15 * 1000);
         m_pdoc = m_ptemplatePane->open_document_file(createcontext);
         userex::pane_tab_view * pview = m_pdoc->get_typed_view < userex::pane_tab_view >();
         pview->set_view_creator(this);
         m_ptabview = pview;
         pview->set_tab("ca2open", 1);
         pview->set_tab("network", 2);
         pview->set_image_tab("", Application.dir().matter("image/keyboard-h21.png"), 3);
         pview->set_cur_tab_by_id(1);

      }

      void validate::page1(const stringa & straMatter)
      {

         m_pdocAuth->get_html_data()->m_puser = m_loginthread.m_puser;

         string strPath;

         if(straMatter.get_count() <= 0)
         {

            if(m_strLicense.is_empty())
            {

               m_pdocAuth->get_html_data()->m_propertyset["reason"] = "Authenticating";

            }
            else
            {

               m_pdocAuth->get_html_data()->m_propertyset["reason"] = "Licensing";
               string strUrl;
               strUrl = "http://api.ca2.cc/spaignition/query?node=install_application&id=";
               strUrl += m_strLicense;
               strUrl += "&key=name";
               m_pdocAuth->get_html_data()->m_propertyset["project"] = System.http().get(strUrl);

               strUrl = "ext://https://account.ca2.cc/license?id="+ m_strLicense + "&lang=" + System.get_locale() + "&sessid=" + ApplicationUser.get_sessid(m_loginthread.m_strFontopusServer);
               m_pdocAuth->get_html_data()->m_propertyset["contribute_link"] = strUrl;

            }

            strPath = Application.dir().matter(m_strForm);

         }
         else
         {
            strPath = Application.dir().matter(straMatter);
         }

         if(!m_pdocAuth->on_open_document(strPath))
         {
            authentication_failed(::fontopus::result_fail, "Cannot open form for authentication!!");
            return;
         }

         display_main_frame();

         if(straMatter.get_count() <= 0)
         {

            sp(::user::interaction) pguie = m_pviewAuth->get_child_by_name("user");
            sp(text_interface) ptext =  (pguie.m_p);
            if(ptext != ::null())
            {
               ptext->_001SetText(m_loginthread.m_strUsername);
               if(m_loginthread.m_strUsername.is_empty())
                  Application.user()->set_keyboard_focus(pguie);
               else
               {
                  pguie = m_pviewAuth->get_child_by_name("password");
                  Application.user()->set_keyboard_focus(pguie);
               }
            }

         }

      }

      void validate::display_main_frame()
      {

         rect rectOpen;
         if(m_ptabview->GetParentFrame()->get_parent() == ::null())
         {

            System.get_screen_rect(rectOpen);
         }
         else
         {
            m_ptabview->GetParentFrame()->get_parent()->GetClientRect(rectOpen);
         }

#if defined(WINDOWSEX) || defined(LINUX)
         m_pvOldWindow = (void *) ::GetFocus();
#endif

         int32_t iWidth = rectOpen.width();
         int32_t iHeight = rectOpen.height();
         rectOpen.deflate(iWidth / 5, iHeight / 5);
         sp(simple_frame_window) pframe =  (m_pviewAuth->GetTopLevelParent().m_p);
         if(pframe != ::null())
         {
            pframe->m_bblur_Background = true;
         }
#ifdef WINDOWSEX
         int_ptr ui1 = GetCurrentThreadId();
         int_ptr ui2 = m_ptabview->GetTopLevelFrame()->m_pthread->m_pthread->get_os_int();
         if(::AttachThreadInput((uint32_t) ui1, (uint32_t) ui2, TRUE))
         {
            TRACE("AttachedThreadInput");
         }
#endif
   /*      if(System.m_puiInitialPlaceHolderContainer != ::null())
         {
            System.m_puiInitialPlaceHolderContainer->GetTopLevelParent()->ShowWindow(SW_SHOW);
         }
         else*/
         {
//            m_ptabview->GetTopLevelFrame()->ShowWindow(SW_SHOW);
         }
  //       m_ptabview->GetTopLevelFrame()->SetActiveWindow();
    //     ::SwitchToThisWindow(m_ptabview->GetTopLevelFrame()->get_wnd()->get_safe_handle(), FALSE);
         m_ptabview->GetTopLevelFrame()->SetWindowPos(
            ZORDER_TOP,
            rectOpen.left, rectOpen.top,
            rectOpen.width(), rectOpen.height(),
            SWP_SHOWWINDOW);
/*         if(m_ptabview->GetTopLevelFrame()->get_parent() != ::null())
         {
            try
            {
               m_ptabview->GetTopLevelFrame()->get_parent()->layout();
            }
            catch(...)
            {
            }
         }
         else
         {
         }*/
         if(m_ptabview->GetTopLevelFrame()->SetForegroundWindow())
         {
            TRACE("fontopus_validate tab_view top_level_frame set_foreground_window OK");
            if(m_ptabview->GetTopLevelFrame()->BringWindowToTop())
            {
               TRACE("fontopus_validate tab_view top_level_frame bring_window_to_top OK");
            }
         }

         m_ptabview->GetTopLevelFrame()->ActivateFrame();
         m_ptabview->GetTopLevelFrame()->SetFocus();

//         m_ptabview->GetTopLevelFrame()->layout();




         //m_pviewAuth->GetTopLevelParent()->SetForegroundWindow();
         //m_pviewAuth->GetTopLevelParent()->BringWindowToTop();

      }

      void validate::pageMessage(const stringa & straMatter, ::ca::property_set & set)
      {
         ensure_main_document();
         m_pdocAuth->get_html_data()->m_propertyset = set;
         page1(straMatter);
         if(m_ptabview->get_wnd()->m_iModalCount <= 0)
         {
            ::ca::live_signal livesignal;
            livesignal.keep(get_app());
            m_ptabview->get_wnd()->RunModalLoop(MLF_NOIDLEMSG | MLF_NOKICKIDLE, &livesignal);
            m_ptabview->get_wnd()->EndAllModalLoops(IDOK);
         }
      }

      void validate::on_create_view(::user::view_creator_data * pcreatordata)
      {
         switch(pcreatordata->m_id)
         {
         case 1:
            {
               m_pdocAuth = Cube.userex().create_child_form(this, pcreatordata->m_pholder);
               if(m_pdocAuth != ::null())
               {
                  m_pviewAuth = m_pdocAuth->get_typed_view < form_view > ();
                  m_pviewAuth->m_pcallback = this;
                  pcreatordata->m_pdoc = m_pdocAuth;
                  pcreatordata->m_pwnd = m_pviewAuth->GetParentFrame();
               }
            }
            break;
         case 2:
            {
               if(m_netcfg.initialize_child(pcreatordata->m_pholder))
               {
                  pcreatordata->m_pdoc = m_netcfg.m_pdoc;
                  pcreatordata->m_pwnd = m_netcfg.m_pview->GetParentFrame();
               }

            }
            break;
         case 3:
            {

               m_pkeyboardlayout = m_ptabview->create_view < ::ca2::keyboard_layout > ();
               m_pkeyboardlayout->CreateViews();
               pcreatordata->m_pdoc = m_pkeyboardlayout->m_pdoc;
               pcreatordata->m_pwnd = m_pkeyboardlayout;
            }
            break;
         }
         if(pcreatordata->m_pwnd != ::null())
         {
            pcreatordata->m_eflag.signalize(::user::view_creator_data::flag_hide_all_others_on_show);
         }

      }
      void validate::on_show_view()
      {
         switch(m_ptabview->get_view_id())
         {
         case 1:
            {
            }
            break;
         case 2:
            {
               m_netcfg.on_show();
            }
            break;
         }
      }






      void validate::get_mod(stringa & straHash, stringa & straSource)
      {
         straHash.remove_all();
         straSource.remove_all();
   /*#if !core_level_1 && !core_level_2
         SetDllDirectory(::null());
   #endif
         ::LoadLibraryA("salt.dll");
         uint32_t dwNeeded;
         if(!EnumProcessModules(::GetCurrentProcess(),  ::null(),  0,  &dwNeeded))
         {
            return;
         }
         uint32_t dwAlloc = (dwNeeded + sizeof(HMODULE)) * 2;
         HMODULE * pmodulea = new HMODULE[dwAlloc / sizeof(HMODULE)];
         if(pmodulea == ::null())
            return;
         if(!EnumProcessModules(::GetCurrentProcess(),  pmodulea,  dwAlloc,  &dwNeeded))
         {
            delete pmodulea;
            return;
         }
         string strModule;
         ::ca::file_system_sp fs(get_app());
         string strModuleFolder(System.get_ca2_module_folder());
         fs->FullPath(strModuleFolder, strModuleFolder);
         for(uint32_t dw = 0; dw < dwNeeded / (sizeof(HMODULE)); dw++)
         {
            strModule.Empty();
            GetModuleFileName(pmodulea[dw], strModule.GetBufferSetLength(4096), 4096);
            strModule.ReleaseBuffer();
            fs->FullPath(strModule, strModule);
            if(::ca::str::begins_ci(strModule, strModuleFolder))
            {
               straSource.add(strModule);
               straHash.add(System.file().md5(strModule));
            }
         }
         straHash.quick_sort();
         delete pmodulea;*/
      }

      string validate::calc_mod_hash()
      {
         if(m_loginthread.m_strModHash.has_char())
            return m_loginthread.m_strModHash;
         stringa straHash;
         stringa straSource;
         get_mod(straHash, straSource);
         m_loginthread.m_strModHash = System.crypt().md5(straHash.implode(";"));
         return m_loginthread.m_strModHash;
      }

      string validate::calc_key_hash()
      {
         if(m_loginthread.m_strKeyHash.has_char())
            return m_loginthread.m_strKeyHash;
   /*#if !core_level_1 && !core_level_2
         ::SetDllDirectoryA(System.get_ca2_module_folder());
   #endif
         HMODULE hmoduleSalt = ::LoadLibraryA("salt.dll");
         SALT salt = (SALT) ::GetProcAddress(hmoduleSalt, "salt");
         stringa straSource;
         if(m_loginthread.m_strUsername.has_char())
         {
            m_loginthread.m_strKeyHash = salt(get_app(), m_loginthread.m_strUsername, straSource);
            return m_loginthread.m_strKeyHash;
         }
         else */
         {
            m_loginthread.m_strKeyHash = "ca2t12n";
            return "ca2t12n";
         }
      }

      string validate::calc_ca2_hash()
      {
         if(m_loginthread.m_strCa2Hash.has_char())
            return m_loginthread.m_strCa2Hash;
         /*stringa straHash;
         stringa straSource;
         get_mod(straHash, straSource);
         straHash.insert_at(0, m_loginthread.m_strUsername);
   #if !core_level_1 && !core_level_2
         ::SetDllDirectoryA(System.get_ca2_module_folder());
   #endif*/
         /*HMODULE hmoduleSalt = ::LoadLibraryA("salt.dll");
         SALT salt = (SALT) ::GetProcAddress(hmoduleSalt, "salt");
         m_loginthread.m_strCa2Hash = salt(get_app(), straHash.implode(";"), straSource);*/
         m_loginthread.m_strCa2Hash = "ca2t12n";
         return m_loginthread.m_strCa2Hash;
      }


      // return hash and check if hash is valid
      bool validate::check_ca2_hash()
      {
         string strUrl("https://api.ca2.cc/account/check_hash");
         ::ca::property_set post;
         ::ca::property_set headers;
         ::ca::property_set set;
         string strResponse;
         stringa straHash;
         stringa straSource;
         get_mod(straHash, straSource);
         straHash.insert_at(0, m_loginthread.m_strPasshash);
         straHash.insert_at(0, m_loginthread.m_strUsername);

         post["hash"] = straHash.implode(";");
         post["source"] = straHash.implode(";");
         for(int32_t i = 0; i < 3; i++)
         {
            if(Application.http().get(strUrl, strResponse, post, headers, set))
               break;
         }
         if(strResponse == "OK")
            return true;
         else
            return false;
      }

      void validate::on_login_thread_response(::fontopus::e_result iAuth, const char * pszResponse)
      {
         if(iAuth == ::fontopus::result_auth)
         {
            authentication_succeeded();
         }
         else
         {
            authentication_failed(iAuth, pszResponse);
         }
      }


      bool validate::BaseOnControlEvent(::user::form * pview, ::user::control_event * pevent)
      {
         UNREFERENCED_PARAMETER(pview);
         if(pevent->m_eevent == ::user::event_button_clicked
            || pevent->m_eevent == ::user::event_enter_key)
         {
            if(pevent->m_puie->m_id == "submit" ||
               pevent->m_eevent == ::user::event_enter_key)
            {
               if(m_loginthread.get_os_data() != ::null())
                  return true;
               m_pviewAuth->KillTimer(1984);
               m_loginthread.oprop("defer_registration") = oprop("defer_registration");
               if(m_bAuth)
               {
                  m_ptabview->GetParentFrame()->ShowWindow(SW_HIDE);
                  sp(::user::interaction) pguie = m_pviewAuth->get_child_by_name("user");
                  sp(text_interface) ptext =  (pguie.m_p);
                  //m_loginthread.m_puser = dynamic_cast < ::fontopus::user * > (System.allocate_user());
                  ptext->_001GetText(m_loginthread.m_strUsername);
                  pguie = m_pviewAuth->get_child_by_name("password");
                  ptext =  (pguie.m_p);
                  ptext->_001GetText(m_loginthread.m_strPassword);
                  m_loginthread.m_pcallback = this;
                  m_loginthread.begin();
               }
               else
               {
                  m_pauth = new auth;
                  sp(::user::interaction) pguie = m_pviewAuth->get_child_by_name("user");
                  sp(text_interface) ptext =  (pguie.m_p);
                  ptext->_001GetText(m_pauth->m_strUsername);
                  pguie = m_pviewAuth->get_child_by_name("password");
                  ptext =  (pguie.m_p);
                  ptext->_001GetText(m_pauth->m_strPassword);
               }
               ::oswindow oswindowPrevious = (::oswindow) m_pvOldWindow;
               if(oswindowPrevious != ::null())
               {
                  sp(::user::interaction) puiPrevious = System.window_from_os_data(oswindowPrevious);
                  if(puiPrevious != ::null())
                  {
                     if(puiPrevious->SetForegroundWindow())
                     {
                        TRACE("fontopus_validate tab_view top_level_frame set_foreground_window OK");
                        if(puiPrevious->BringWindowToTop())
                        {
                           TRACE("fontopus_validate tab_view top_level_frame bring_window_to_top OK");
                        }
                     }
                     //puiPrevious->ActivateFrame();
                     puiPrevious->SetFocus();
                     puiPrevious->layout();
                  }
               }
               m_ptabview->GetParentFrame()->ShowWindow(SW_HIDE);
               return true;
            }
         }
         else if(pevent->m_eevent == ::user::event_timer
            && pevent->m_uiEvent == 1984)
         {

            string strDir;
            string strUsername;
            string strPasshash;

            string strPathUsername = ::dir::userappdata("license_auth/00001.data");
            string strPathPasshash = ::dir::userappdata("license_auth/00002.data");

            if(!Application.file().exists(strPathUsername) || !Application.file().exists(strPathPasshash))
               return true;

            System.crypt().file_get(strPathUsername, strUsername, "", get_app());
            m_loginthread.m_strUsername = strUsername;

            System.crypt().file_get(strPathPasshash, strPasshash, calc_key_hash(), get_app());

            if(strUsername.is_empty() || strPasshash.is_empty())
               return true;

            m_pviewAuth->KillTimer(1984);
            m_loginthread.m_strPassword.Empty();
            m_loginthread.m_strPasshash = strPasshash;
            m_loginthread.m_pcallback = this;
            m_loginthread.begin();

            return true;
         }
         return false;
      }

      void validate::authentication_failed(::fontopus::e_result iAuth, const char * pszResponse)
      {

         UNREFERENCED_PARAMETER(pszResponse);

         ::ca::property_set propertyset;

         string strUsername = m_loginthread.m_strUsername;

         m_bLicense = false;
         m_puser = ::null();

         if(m_pdocAuth != ::null())
         {

            m_pdocAuth->get_html_data()->m_puser = ::null();

         }

         if(m_strLicense.has_char())
         {
            if(m_bInteractive)
            {
               string strUrl;
               strUrl = "http://api.ca2.cc/spaignition/query?node=install_application&id=";
               strUrl += m_strLicense;
               strUrl += "&key=launch_name";
               string strName = Application.http().get(strUrl);
               if(strName.is_empty())
                  strName = m_strLicense;
               propertyset["project"] = strName;
               strUrl = "ext://https://"+ m_loginthread.m_strFontopusServer + "/license?id="+ m_strLicense + "&lang=" + System.get_locale() + "&sessid=" + ApplicationUser.get_sessid(m_loginthread.m_strFontopusServer);
               propertyset["contribute_link"] = strUrl;
               pageMessage("err\\user\\authentication\\not_licensed.xhtml", propertyset);
            }
         }
         else if(iAuth == ::fontopus::result_please_finish_registration)
         {
            if(m_bInteractive)
            {
               stringa stra;
               stra.add("err\\user\\authentication\\please_finish_registration.html");
               stra.add("err\\user\\authentication\\registration_deferred.html");
               pageMessage(stra, propertyset);
            }
         }
         else if(iAuth == ::fontopus::result_wrong_password_or_login)
         {
            if(m_bInteractive)
            {
               propertyset["register_link"] = "ext://http://"+ m_loginthread.m_strFontopusServer + "/register?email="+ System.url().url_encode(m_loginthread.m_strUsername);
               pageMessage("err\\user\\authentication\\wrong_fontopus_login.html", propertyset);
               try
               {
                  System.file().del(::dir::userappdata("license_auth/00001.data"));
               }
               catch(...)
               {
               }
               try
               {
                  System.file().del(::dir::userappdata("license_auth/00002.data"));
               }
               catch(...)
               {
               }
               m_loginthread.m_strModHash.Empty();
               m_loginthread.m_strKeyHash.Empty();
               m_loginthread.m_strCa2Hash.Empty();
            }
         }
         else if(iAuth == ::fontopus::result_time_out)
         {
            if(m_bInteractive)
            {
               propertyset["server"] = "account.ca2.cc";
               pageMessage("err\\user\\network\\connection_timed_out.html", propertyset);
            }
         }
         else if(iAuth == ::fontopus::result_registration_deferred)
         {
            if(m_bInteractive)
            {
               propertyset["server"] = "account.ca2.cc";
               propertyset["email"] = strUsername;
               pageMessage("err\\user\\authentication\\registration_deferred.html", propertyset);
            }
         }
         else if(iAuth == ::fontopus::result_no_login)
         {
            if(m_bInteractive)
            {
               pageMessage("err\\user\\authentication\\no_login.html", propertyset);
            }
         }
         else if(iAuth == ::fontopus::result_no_password)
         {
            if(m_bInteractive)
            {
               pageMessage("err\\user\\authentication\\no_password.html", propertyset);
            }
         }
         else
         {
            if(m_bInteractive)
            {
               pageMessage("err\\user\\authentication\\failed.html", propertyset);
            }
         }

         delete m_pauth;

      }

      void validate::authentication_succeeded()
      {

         ::fontopus::validate::authentication_succeeded();

         if(m_ptabview != ::null())
         {
            m_ptabview->get_wnd()->EndAllModalLoops(IDOK);
         }

      }

      validate::auth * validate::get_auth()
      {
         sp(::ca::create_context) createcontext(allocer());
         createcontext->m_bMakeVisible = true;
         sp(::form_document) pdoc = (m_ptemplatePane->open_document_file(createcontext));
         userex::pane_tab_view * pview = pdoc->get_typed_view < userex::pane_tab_view > ();
         pview->set_view_creator(this);
         rect rectOpen;
         System.get_screen_rect(rectOpen);
         int32_t iWidth = rectOpen.width();
         int32_t iHeight = rectOpen.width();
         rectOpen.deflate(iWidth / 5, iHeight / 5);
         m_ptabview->GetParentFrame()->SetWindowPos(ZORDER_TOP, rectOpen.left,
            rectOpen.top,
            rectOpen.width(), rectOpen.height(), SWP_SHOWWINDOW);
         m_ptabview = pview;
         pview->add_tab("login", 1);
         pview->set_cur_tab_by_id(1);
         pview->GetParentFrame()->RedrawWindow();
            ::ca::live_signal livesignal;
            livesignal.keep(get_app());
         pview->GetTopLevelFrame()->RunModalLoop(MLF_NOIDLEMSG | MLF_NOKICKIDLE, &livesignal);
         return m_pauth;
      }


   } // namespace fontopus


} // namespace ca


