#include "framework.h"


namespace xml
{


   attr_array::attr_array(sp(::ca::application) papp) :
      ca(papp),
      ::ca::property_set(papp)
   {

      m_pnodeParent = ::null();

   }


} //namespace xml


