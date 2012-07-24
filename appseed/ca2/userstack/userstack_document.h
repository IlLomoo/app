#pragma once


namespace userstack
{


   class view;


   class CLASS_DECL_ca2 document :
      public ::userbase::document
   {
   public:


      document(::ca::application * papp); 
      virtual ~document();


      virtual bool on_new_document();
      
   #ifdef DEBUG
      virtual void assert_valid() const;
      virtual void dump(dump_context & dumpcontext) const;

#endif

   };


} // namespace userstack


