#pragma once


namespace windows
{


   class CLASS_DECL_BASE factory_exchange :
      virtual public ::object
   {
   public:


      factory_exchange(sp(::axis::application) papp);
      virtual ~factory_exchange();


   };


} // namespace windows




