#pragma once


namespace user
{


   class CLASS_DECL_BASE menu_base_item :
      virtual public ::object
   {
   public:


      menu_base_item(sp(::axis::application) papp);
      virtual ~menu_base_item();


      virtual bool create_menu(const stringa & straCommand, const stringa & straCommandTitle);
      virtual bool load_menu(sp(::xml::node) lpnode);


   };


} // namespace user



