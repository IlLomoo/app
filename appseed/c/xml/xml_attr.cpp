#include "framework.h"


namespace xml
{


   attr_array::attr_array(sp(base_application) papp) :
      element(papp),
      property_set(papp)
   {

      m_pnodeParent = NULL;

   }


} //namespace xml


