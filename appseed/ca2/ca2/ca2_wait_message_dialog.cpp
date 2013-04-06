#include "framework.h"


namespace ca2 // ca8 + cube
{


   wait_message_dialog::wait_message_dialog(sp(::ca::application) papp) :
      ca(papp),
      userbase::view(papp),
      user::scroll_view(papp),
      userbase::scroll_view(papp),
      user::form(papp),
      userbase::form_view(papp),
      html_form(papp),
      html_form_view(papp),
      form_view(papp),
      dialog(papp)
   {
      m_dwStartTime = 0;
      m_dwDelay = 0;
   }

   wait_message_dialog::~wait_message_dialog()
   {
      /*if(m_pdocument != ::null())
      {
         m_pdocument->on_close_document();
         m_pdocument = ::null();
      }*/
   }

   void wait_message_dialog::on_show(const char * pszMatter, ::ca::property_set & propertyset)
   {
      UNREFERENCED_PARAMETER(pszMatter);
      UNREFERENCED_PARAMETER(propertyset);
      if(m_dwDelay > 0)
      {
         m_pdocument->get_html_data()->m_propertyset["wait_message_dialog_timeout"] = (int32_t) (m_dwDelay / 1000);
         m_pdocument->get_view()->SetTimer(5432175, 584, ::null());
      }
      m_dwStartTime = ::get_tick_count();
   }



   bool wait_message_dialog::BaseOnControlEvent(::user::control_event * pevent)
   {

      if(pevent->m_eevent == ::user::event_button_clicked)
      {

         m_strResponse = pevent->m_puie->m_id;

         EndModalLoop(IDOK);

      }
      else if(pevent->m_eevent == ::user::event_timer)
      {
         if(pevent->m_uiEvent == 5432175)
         {
            uint32_t dwTimeout = ::get_tick_count() - m_dwStartTime;
            if(dwTimeout > m_dwDelay)
            {
               if(on_timeout())
               {
                  EndModalLoop(IDOK);
               }
            }
            else
            {
               on_timer_soft_reload(dwTimeout);
            }
         
         }
      }
      return false;
   }


   bool wait_message_dialog::on_timeout()
   {
      m_strResponse = "timeout";
      return true;
   }

   void wait_message_dialog::on_timer_soft_reload(uint32_t dwTimeout)
   {
      string str;
      str.Format("%d", (int32_t) ((m_dwDelay - dwTimeout) / 1000));
      html::elemental * pelemental = m_pdocument->get_html_data()->get_element_by_id("timeout");
      if(pelemental != ::null())
      {
         pelemental->set_string(str);
         m_pframe->layout();
      }
      //m_pdocument->m_propertyset["wait_message_dialog_timeout"] = (int32_t) ((m_dwDelay - dwTimeout) / 1000);
      //m_pdocument->soft_reload();
   }


} // namespace ca2 // ca8 + cube


