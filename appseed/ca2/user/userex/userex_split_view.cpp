#include "framework.h"


namespace userex
{


   split_view::split_view(sp(base_application) papp) :
      element(papp),
      ::user::split_layout(papp),
      
      ::user::split_view(papp),
      place_holder_container(papp)
   {
      m_pviewcontainer = this;
   }
 
   split_view::~split_view()
   {
   }


   void split_view::on_new_view_creator_data(::user::view_creator_data * pcreatordata)
   {

      pcreatordata->m_pholder = get_new_place_holder();

      ::index iPane = get_pane_by_id(pcreatordata->m_id);

      if(iPane < 0)
         return;

      Pane * ppane = (Pane *) m_panea.element_at(iPane);

      ppane->m_pholder = pcreatordata->m_pholder;

      if(ppane->m_pholder == NULL)
         return;

      pcreatordata->m_pviewdata = (void *) ppane;

   }

   bool split_view::create_pane(int32_t iPane, bool bFixedSize, ::id id)
   {

      ASSERT(iPane >= 0);

      ASSERT(iPane < get_pane_count());

      Pane & pane = m_panea[iPane];

      pane.m_bFixedSize = bFixedSize;

      pane.m_id = id;

      return ::user::view_creator::create(id) != FALSE;

   }


   bool split_view::_001OnUpdateCmdUi(cmd_ui * pcmdui)
   {

      UNREFERENCED_PARAMETER(pcmdui);

      return false;

   }

   bool split_view::_001OnCommand(id id)
   {
      UNREFERENCED_PARAMETER(id);
      return false;
   }

} // namespace userex