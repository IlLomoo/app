#include "framework.h"


namespace command
{


   document::document(::ca::application * papp) :
      ca(papp),
      ::ca::data_container_base(papp),
      ::userbase::document(papp)
   {
   }

   bool document::on_new_document()
   {
	   if (!::userbase::document::on_new_document())
		   return FALSE;

      update_all_views(NULL, 0);


	   return TRUE;
   }

   document::~document()
   {
   }


 
   #ifdef DEBUG
   void document::assert_valid() const
   {
	   ::userbase::document::assert_valid();
   }

   void document::dump(dump_context & dumpcontext) const
   {
	   ::userbase::document::dump(dumpcontext);
   }
   #endif //DEBUG

   /////////////////////////////////////////////////////////////////////////////
   // document serialization

   /*
   void document::Serialize(CArchive& ar)
   {
	   if (ar.IsStoring())
	   {
		   // TODO: add storing code here
	   }
	   else
	   {
		   // TODO: add loading code here
	   }
   }
   */

   void document::data_on_after_change(::ca::signal_object * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
   }

   bool document::on_open_document(var varFile)
   {
      string str = Application.file().as_string(varFile);
   //  m_document.load(str);
      
      update_all_views(NULL, 123);

      return TRUE;

   }


} // namespace command



