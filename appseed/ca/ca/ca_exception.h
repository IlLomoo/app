#pragma once


namespace ca
{


   class CLASS_DECL_ca exception :
      virtual public ::ca::ca
   {
   public:


      virtual ~exception();
      virtual void Delete();


   };


   void CLASS_DECL_ca rethrow(exception * pe);


} // namespace ca

