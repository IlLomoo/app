#include "framework.h"


namespace userex
{


   minute_list_view::minute_list_view(sp(::ca::application) papp) :
      ca(papp),
      userbase::view(papp),
      user::scroll_view(papp),
      user::form(papp),
      ::ca::user::form(papp),
      user::list(papp),
      user::form_list(papp),
      userbase::form_list(papp),
      simple_list_view(papp)
   {
      
      m_eview = ViewList;
      
      m_scrollinfo.m_rectMargin.null();

      m_etranslucency = TranslucencyPresent;


   }
   
   minute_list_view::~minute_list_view()
   {
   }



#ifdef DEBUG
   void minute_list_view::assert_valid() const
   {
      simple_list_view::assert_valid();
   }

   void minute_list_view::dump(dump_context & dumpcontext) const
   {
      simple_list_view::dump(dumpcontext);
   }
#endif //DEBUG



   void minute_list_view::install_message_handling(::ca::message::dispatch * pinterface)
   {
      simple_list_view::install_message_handling(pinterface);
      USER_MESSAGE_LINK(message_create, pinterface, this, &minute_list_view::_001OnCreate);
   }

   void minute_list_view::_001GetItemText(::user::list_item * pitem)
   {
      pitem->m_strText.Format("%02d", pitem->m_iItem);
      pitem->m_bOk = true;
   }

   ::count minute_list_view::_001GetItemCount()
   {
      return 60;
   }

   void minute_list_view::_001InsertColumns()
   {

      ::user::list_column column;

      column.m_iWidth               = 33;
      column.m_iSubItem             = 0;
      _001AddColumn(column);

   }

   void minute_list_view::_001OnCreate(::ca::signal_object * pobj)
   {
      pobj->previous();
      if(pobj->m_bRet)
         return;
      _001UpdateColumns();
      _001OnUpdateItemCount();
   }

   void minute_list_view::_001OnSelectionChange()
   {

      ::user::control_event ev;
      ev.m_puie         = this;
      ev.m_eevent       = ::user::event_after_change_text;
      ev.m_bUser        = true;
      get_parent()->BaseOnControlEvent(&ev);
      BaseOnControlEvent(&ev);

   }

   int32_t minute_list_view::set_minute(int32_t iMinute)
   {
      return (int32_t) set_cur_sel(iMinute);
   }

   int32_t minute_list_view::get_minute()
   {
      return (int32_t) get_cur_sel();
   }


} // namespace userex


