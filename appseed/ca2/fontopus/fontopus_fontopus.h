#pragma once


namespace ca
{


   namespace fontopus
   {


      class CLASS_DECL_ca2 fontopus :
         virtual public ::fontopus::fontopus
      {
      public:


         fontopus();
         virtual ~fontopus();


         virtual void construct(sp(::ca::application) papp);


         virtual ::fontopus::user * login(::ca::property_set & set);
         virtual bool get_auth(const char * psz, string & strUsername, string & strPassword);


         virtual bool initialize_instance();
         virtual int32_t exit_instance();

         virtual bool check_license(const char * pszId, bool bInteractive);

         virtual void on_request(sp(::ca::create_context) pcreatecontext);


      };


   } // namespace fontopus


} // namespace ca



