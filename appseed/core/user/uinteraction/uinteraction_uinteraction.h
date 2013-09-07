#pragma once


namespace uinteraction
{


   class CLASS_DECL_ca2 uinteraction :
      virtual public ::base_departament
   {
   public:


      uinteraction(base_application * papp);
      virtual ~uinteraction();


      virtual sp(::uinteraction::interaction) get_new_uinteraction(const char * pszUinteractionLibrary);
      virtual sp(::uinteraction::interaction) get_uinteraction(const char * pszUinteractionLibrary);
      virtual sp(::uinteraction::frame::frame) get_frame_schema(const char * pszUinteractionLibrary, const char * pszFrameSchemaName);

   };


} // namespace uinteraction





