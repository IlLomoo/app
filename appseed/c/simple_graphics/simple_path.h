#pragma once


//
// CRoundRect.h : Version 1.0 - see article at CodeProject.com
//
// Author:  Darren Sessions
//
//
// Description:
//     CRoundRect Draws or Fills rounded rectangles for GDI+.  It was implemented
//	   to overcome the asymmetric issues associated with GDI+ round rectangles
//
// History
//     Version 1.0 - 2008 June 24
//     - Initial public release
//
// License:
//     This software is released under the Code Project Open License (CPOL),
//     which may be found here:  http://www.codeproject.com/info/eula.aspx
//     You are free to use this software in any way you like, except that you
//     may not sell this source code.
//
//     This software is provided "as is" with no expressed or implied warranty.
//     I accept no liability for any damage or loss of business that this
//     software may cause.
//
///////////////////////////////////////////////////////////////////////////////


//namespace graphics_round_rect
//{
class CLASS_DECL_c simple_path :
   public os_simple_path
{
public:

	//=============================================================================
	//
	// get_round_rect()
	//
	// Purpose:     Defines a Rounded Rectangle and places it in the GraphicsPath
	//
	// Parameters:  pPath		- [out] pointer to GraphicsPath that will recieve the
	//									path data
	//				r			- [in]	Rect that defines the round rectangle boundaries
	//				dia			- [in]	diameter of the rounded corners (2*radius)
	//
	// Returns:     None
	//
	bool add_round_rect(const RECT & rect, int dia);
   bool add_round_top_left(const RECT & rect, int dia);
   bool add_round_bottom_right(const RECT & rect, int dia);


   using os_simple_path::add_line;
   inline bool add_line(POINT p1, POINT p2);

};

//} // namespace graphics_round_rect




inline bool simple_path::add_line(POINT p1, POINT p2)
{
   return add_line(p1.x, p1.y, p2.x, p2.y);
}
