#include "framework.h"


namespace ca
{


   namespace user
   {


      user::user()
      {

         //::ca::user * papp = dynamic_cast <::ca::user *>(System.GetThread()->m_pAppThread);
         //::ca::connect(papp->m_signalAppLanguageChange, this, &user::VmsGuiiOnAppLanguage);

         m_pkeyboard = ::null();
         m_pwindowmap = ::null();

      }

      user::~user()
      {
      }



      bool user::initialize1()
      {

         if(!::user::user::initialize1())
            return false;

         return true;

      }


      bool user::initialize()
      {


         if(Application.is_cube())
         {
            System.factory().cloneable_small < int_biunique > ();
            System.factory().creatable_small < ::user::edit_plain_text > ();
            System.factory().cloneable_small < XfplayerViewLine > ();
            System.factory().creatable_small < ::user::place_holder > ();
            System.factory().creatable_small < ::user::place_holder_container > ();
         }

         if(!::user::user::initialize())
            return false;

         return true;
      }

      int32_t user::exit_instance()
      {
         if(Application.is_cube())
         {
            if(m_pwindowmap != ::null())
            {
               delete m_pwindowmap;
               m_pwindowmap = ::null();
            }
         }
         return 0;
      }




      int32_t user::GetVisibleFrameCountExcept(sp(::user::interaction) pwndExcept)
      {
         ::user::interaction_ptr_array wnda = Application.frames();
         int32_t iCount = 0;
         for(int32_t i = 0; i < wnda.get_size(); i++)
         {
              sp(::user::interaction) pwnd = wnda.element_at(i);
              if(pwnd != ::null() &&
                  pwnd != pwndExcept &&
                  pwnd->IsWindowVisible())
              {
                  iCount++;
              }
          }
          return iCount;
      }

      int32_t user::GetVisibleTopLevelFrameCountExcept(sp(::user::interaction) pwndExcept)
      {
         ::user::interaction_ptr_array wnda = Application.frames();
         int32_t iCount = 0;
         for(int32_t i = 0; i < wnda.get_size(); i++)
         {
            sp(::user::interaction) pwnd = wnda.element_at(i);
            if(pwnd != ::null() &&
               pwnd != pwndExcept &&
               pwnd->IsWindow() &&
               pwnd->IsWindowVisible() &&
               !(pwnd->GetStyle() & WS_CHILD))
            {
               iCount++;
            }
         }
         return iCount;
      }

      int32_t user::GetVisibleFrameCount()
      {
         ::user::interaction_ptr_array wnda = Application.frames();
         int32_t iCount = 0;
         for(int32_t i = 0; i < wnda.get_size(); i++)
         {
            sp(::user::interaction) pwnd = wnda.element_at(i);
            if(pwnd != ::null()
               && pwnd->IsWindow()
               && pwnd->IsWindowVisible())
            {
               iCount++;
            }
         }
         return iCount;
      }

      void user::VmsGuiiOnAppLanguage(::ca::signal_object * pobject)
      {
         SendMessageToWindows(::ca::application::APPM_LANGUAGE, 0, (LPARAM) pobject);
      }

      string user::message_box(const char * pszMatter, ::ca::property_set & propertyset)
      {

         class ::ca2::message_box box(get_app());

         box.show(pszMatter, &propertyset);

         return box.m_strResponse;

      }



   } // namespace user


} // namespace ca