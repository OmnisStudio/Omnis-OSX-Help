/* $Header:   J:\vcs\studio60\o7exe\indy\omnisres.inv   1.0   13 Oct 2010 14:08:56   bmitchel  $ */

/**************** Changes ******************
Date			Edit				Bug					Description
24-Feb-10	rmm6905b								Added ability to vertically align labels - applied to our internal resources.
02-Jul-08	rmm_rfmenu							Context menus for remote forms.
11-Aug-05 MHRES                   Mach-o resource changes
17-Jun-03	rmm4556			ST/DB/420		Added font configuration for method editor.
28-Mar-01	AE											As QBORD_CUSTOM changed, so must QTOOLBAR_EDGE_xxx/_MENU_xxx
20 OCT 00 mpmCarbon								OSX Changes NOT MARKED
07 Jan 98 mpm4417									Implements color/pattern/line-style picker from resources
16-Jan-97	rmm2183			PR/HI/505		Rationalised tooltip preferences.
14-Jun-96	mtToolBorder						Toolbar Borders
10 JUN 96  PK4063									Custom NC border support
24-May-96 mtToolbar								More flags for toolbars
23-May-96 PK4010i									More flags for icon array
20-May-96 PK4010h									Icon array as part of resources

--------------------------------------------------
New system should use format :

CONTROL "text",id,"classname",style,top,left,width,height

*/

#define ctqfList												2000
#define ctqfComboBox										2001
#define	ctqfPopmenu											2002
#define	ctqfColorpalette								2003
#define ctqfToolbar											2004
#define ctqfMenubar											2005
#define ctqfToolgroup										2006
#define ctqfRuler												2007
#define ctqfPopupColorpalette						2008
#define ctqfPopupPatternpalette 				2009
#define ctqfPopupLinepalette 						2010
#define ctqfDroplist										2011
#define ctqfStatusBar										2012
#define	ctqfButtonArea									2013
#define ctqfCheck												2014
#define ctqfTabStrip										2015
#define ctqfRadio												2016
#define ctqfPush												2017
#define ctqfGroupBox										2018
#define ctqfCompList										2019
#define ctqfStatic											2020
#define ctqfTreeList										2021
#define ctqfStringGrid									2022
#define ctqfEdit												2023
#define ctqfEditMul											2024
#define ctqfExtcomp											2025
#define ctqfEndGroupBox									2026
#define ctqfTabPane											2027
#define ctqfIconArray										2028

#define ctqfDebug												2098
#define ctqfStub												2099
#define ctqfNibParent										3000 // MHRES

// control styles 32 bit value nibbles 5 to 0
// if the control is a component, the component part is specified
// in the 1 nibble. if not, the floating attributes use the 1 nibble

// mpm4417 many changes to flags
// general styles nibble 0 - border styles
#define QBORD_NONE											0x00000000L
#define QBORD_PLAIN											0x00000001L
#define QBORD_INSET											0x00000002L
#define QBORD_EMBOSSED									0x00000003L
#define QBORD_BEVEL											0x00000004L
#define QBORD_INSETBEVEL								0x00000005L
#define QBORD_CHISEL										0x00000006L
#define QBOAD_EMBOSSEDCHISEL						0x00000007L
#define QBORD_SHADOW										0x00000008L
#define QBORD_SINGLE_INSET							0x00000009L						// rmm2361
#define QBORD_SINGLE_EMBOSSED						0x0000000aL					// rmm2361
#define QBORD_3DFACE										0x0000000bL					// rmm2361
#define QBORD_3DHILITE									0x0000000cL					// rmm2361
#define QBORD_CTRL_EDIT									0x00000020L					// mpmCarbon begins
#define QBORD_CTRL_LIST									0x00000021L
#define QBORD_CTRL_SHADOW								0x00000022L
#define QBORD_CTRL_SHADOW_EX						0x00000023L					// mpmCarbon ends
#define QBORD_CUSTOM										0x000000FFL			// PK4063
#define QBORD_MASK											0x000000FFL

// general styles nibble 1 - floating attributes
#define QFLOAT_NONE											0x00000000L
#define QFLOAT_LEFT											0x00000100L
#define QFLOAT_RIGHT										0x00000200L
#define QFLOAT_LEFT_RIGHT								0x00000300L
#define QFLOAT_TOP											0x00000400L
#define QFLOAT_BOTTOM										0x00000800L
#define QFLOAT_TOP_BOTTOM								0x00000C00L

// general styles nibble 1 - component type ( QCOMPONENT must be also specified )
#define QWND_WC_FRAME										0x00000100L
#define QWND_WC_MENUBAR									0x00000200L
#define QWND_WC_TOOLBAR_TOP							0x00000300L
#define QWND_WC_STATUSBAR								0x00000400L
#define QWND_WC_TOOLBAR_BOTTOM					0x00000500L
#define QWND_WC_TOOLBAR_LEFT						0x00000600L
#define QWND_WC_TOOLBAR_RIGHT						0x00000700L
#define QWND_WC_MAIN_HEADER							0x00000800L
#define QWND_WC_HORZ_HEADER							0x00000900L
#define	QWND_WC_VERT_HEADER							0x00000a00L
#define QWND_WC_HEADER_BUTTON						0x00000b00L
#define QWND_WC_CLIENT									0x00000c00L
#define QWND_WC_MASK										0x00000F00L

// general styles nibble 2
#define QHSCROLLBAR											0x00001000L
#define QVSCROLLBAR											0x00002000L
#define QDISABLED												0x00004000L
#define QCOMPONENT											0x00008000L

// general styles nibble 3
#define QDRAGBORDER											0x00010000L
#define QVERTCENTERTEXT									0x00020000L	// rmm6905b
#define QDROPCOMBOLABEL									0x00040000L	// rmm6905b

// nibble 7-5 ( 0xFFF00000L are field specific )

//////////////////////////////////////////////////////////////////////////////
// control specific styles follow
//
// NOTE: USE NAME IN FIELD STYLE   e.g. QSTATUSBAR_xxxxx
//
// NOTE: YOU CANNOT USE BITS 0x10000000L or 0x40000000, since these are set
//			by default, by the Windows resource compiler.
//
//////////////////////////////////////////////////////////////////////////////

// ctqfComboBox
#define QFONTS													0x80000000L
#define QFONTSIZES											0x20000000L
#define QCOMBOAUTOHSCROLL								0x08000000L
// ctqfDroplist

// ctqfEdit & ctqfEditMul
#define QEDITAUTOHSCROLL								0x80000000L
#define QSTATIC													0x20000000L
#define QEDITNOHIDESELECTION						0x08000000L

// ctqfStatic & ctqfEdit & ctqfEditMul
#define QLEFT														0x01000000L
#define QCENTER													0x02000000L
#define QRIGHT													0x04000000L

// ctqfList
#define QLISTSORT												0x01000000L
#define QLISTMULTISELECT								0x02000000L
#define QLISTTABCOLUMNS									0x04000000L

// ctqfButtonArea
#define QDONOTFLASH											0x01000000L

// ctqfCheck

// ctqfRadio
#define QRADIOGROUPSTART								0x01000000L

// ctqfPush // mpm4417 various new flags
#define QBUTTSYSTEM											0x00000001L
#define QBUTT3DSYS											0x00000002L
#define QBUTT3D													0x00000003L
#define QBUTTHEADER											0x00000004L
#define QBUTTCOMBO											0x00000005L
#define QBUTT97													0x00000006L
#define QBUTTMASK												0x0000000fL

#define QDEFAULT												0x80000000L

#define QCOLORPICKER										0x00C00000L
#define QLINEPICKER											0x00D00000L
#define QPATTERNPICKER									0x00E00000L
#define QBUTTMODEMASK										0x00F00000L

// ctqfCheck & ctqfRadio & ctqfPush
#define QGRAYWHENDISABLED								0x20000000L

// ctqfGroupBox

// ctqfTreeList

// ctqfStringGrid

// ctqfToolbar	// mtToolBorder
// QBORD_NONE has been added in
#define QTOOLBAR_TOP										0x00000000L
#define QTOOLBAR_BOTTOM									0x01000000L
#define QTOOLBAR_LEFT										0x02000000L
#define QTOOLBAR_RIGHT									0x03000000L
#define QTOOLBAR_MASK										0x03000000L

// QBORD_CUSTOM has been added in
#define QTOOLBAR_EDGE_TOP								0x040000FFL
#define QTOOLBAR_EDGE_BOTTOM						0x080000FFL
#define QTOOLBAR_EDGE_LEFT							0x0C0000FFL
#define QTOOLBAR_EDGE_RIGHT							0x200000FFL
#define QTOOLBAR_EDGE_MASK							0x2C0000FFL
#define QTOOLBAR_MENU_EDGE							0x800000FFL
#define QTOOLBAR_MENU_MASK							0x80000000L

// mpmToolbars5
#define QTOOLBAR_MODE_DEFAULT						0x00000000L
#define QTOOLBAR_MODE_SMALL_ICN					0x00100000L
#define QTOOLBAR_MODE_LARGE_ICN					0x00200000L
#define QTOOLBAR_MODE_TEXT							0x00400000L
#define QTOOLBAR_MODE_VAR_TEXT					0x00800000L
#define QTOOLBAR_MODE_MASK							0x00F00000L

// ctqfStatusBar
#define QSTATUSBAR_DIVIDER							0x01000000L

// ctqfRuler
#define QRULERHORIZONTAL								0x01000000L
#define QRULERVERTICAL									0x02000000L

// ctqfPopmenu

// ctqfMenubar

// ctqfDebug

// ctqfTabStrip

// ctqfStub

// ctqfPopupLinepalette

// ctqfPopupPatternpalette

// ctqfPopupColorpalette

// ctqfColorpalette


// ctqfToolgroup

// ctqfExtcomp

// ctqfTabPane
#define QTABPANE_LEFT										0x01000000L
#define QTABPANE_RIGHT									0x02000000L
#define QTABPANE_BOTTOM									0x03000000L
#define QTABPANE_BUTTON									0x04000000L
#define QTABPANE_TRIANGLE								0x08000000L
#define QTABPANE_ROUNDED								0x0c000000L
#define QTABPANE_SINGLELINE							0x80000000L

// ctqfIconArray
#define QICONARRAY_SINGLEROW						0x01000000L
#define QICONARRAY_NODRAGDROP						0x02000000L
#define QICONARRAY_NOEDITTEXT						0x04000000L
#define QICONARRAY_NODELETEITEM					0x08000000L

// eof
