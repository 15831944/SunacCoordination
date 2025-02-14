
#pragma once
#pragma pack(push, 8)

#include <comdef.h>

//namespace COMKERNALLib
namespace TCH_TY
{
	struct __declspec(uuid("62d64041-adaa-4e25-b985-906354340c31"))
		/* LIBID */ __COMKERNALLib;

	struct __declspec(uuid("1b20a587-2e7f-490b-b212-d5b91aecb7d8"))
		/* dual interface */ IComOpening;


	struct /* coclass */ ComOpening;


	_COM_SMARTPTR_TYPEDEF(IComOpening, __uuidof(IComOpening));



	struct __declspec(uuid("1b20a587-2e7f-490b-b212-d5b91aecb7d8"))
	IComOpening : IAcadEntity
	{
		//
		// Property data
		//

		__declspec(property(get=GetWinSill,put=PutWinSill))
			double WinSill;
		__declspec(property(get=GetWidth,put=PutWidth))
			double Width;
		__declspec(property(get=GetGetKind))
			_bstr_t GetKind;
		__declspec(property(get=GetOutlineArea))
			_bstr_t OutlineArea;
		__declspec(property(get=GetOutlineLength))
			double OutlineLength;
		__declspec(property(get=GetDoorSill,put=PutDoorSill))
			double DoorSill;
		__declspec(property(get=GetSlot,put=PutSlot))
			_bstr_t Slot;
		__declspec(property(get=GetSlotExt,put=PutSlotExt))
			double SlotExt;
		__declspec(property(get=GetSlotWidth,put=PutSlotWidth))
			double SlotWidth;
		__declspec(property(get=GetHeight,put=PutHeight))
			double Height;
		__declspec(property(get=GetLabel,put=PutLabel))
			_bstr_t Label;
		__declspec(property(get=GetUpLevel,put=PutUpLevel))
			_bstr_t UpLevel;
		__declspec(property(get=GetTextHeight,put=PutTextHeight))
			double TextHeight;
		__declspec(property(get=GetDoorLine,put=PutDoorLine))
			short DoorLine;
		__declspec(property(get=GetHoleType,put=PutHoleType))
			_bstr_t HoleType;
		__declspec(property(get=GetStructure,put=PutStructure))
			_bstr_t Structure;
		__declspec(property(get=GetExtShading,put=PutExtShading))
			_bstr_t ExtShading;
		__declspec(property(get=GetAreaRatio))
			double AreaRatio;
		__declspec(property(get=GetTextStyle,put=PutTextStyle))
			_bstr_t TextStyle;
		__declspec(property(get=GetTextLayer,put=PutTextLayer))
			ACAD_LAYER TextLayer;
		__declspec(property(get=GetSlabThickness,put=PutSlabThickness))
			double SlabThickness;
		__declspec(property(get=GetGlassLayer,put=PutGlassLayer))
			ACAD_LAYER GlassLayer;
		__declspec(property(get=GetFrameLayer,put=PutFrameLayer))
			ACAD_LAYER FrameLayer;
		__declspec(property(get=GetSlabLayer,put=PutSlabLayer))
			ACAD_LAYER SlabLayer;
		__declspec(property(get=GetUFrame,put=PutUFrame))
			double UFrame;
		__declspec(property(get=GetVFrame,put=PutVFrame))
			double VFrame;
		__declspec(property(get=GetLayoutRotation,put=PutLayoutRotation))
			double LayoutRotation;
		__declspec(property(get=GetScale,put=PutScale))
			double Scale;
		__declspec(property(get=GetHideLabel,put=PutHideLabel))
			_bstr_t HideLabel;
		__declspec(property(get=GetSlotMaterial,put=PutSlotMaterial))
			_bstr_t SlotMaterial;
		__declspec(property(get=GetProStyle,put=PutProStyle))
			_bstr_t ProStyle;
		__declspec(property(get=GetLeftSlab,put=PutLeftSlab))
			_bstr_t LeftSlab;
		__declspec(property(get=GetRightSlab,put=PutRightSlab))
			_bstr_t RightSlab;
		__declspec(property(get=GetProWinSlabWidth,put=PutProWinSlabWidth))
			double ProWinSlabWidth;
		__declspec(property(get=GetSlotInsuThick,put=PutSlotInsuThick))
			double SlotInsuThick;
		__declspec(property(get=GetSlotInsuCap,put=PutSlotInsuCap))
			_bstr_t SlotInsuCap;
		__declspec(property(get=GetEvacuationType,put=PutEvacuationType))
			_bstr_t EvacuationType;
		__declspec(property(get=GetLineOffsetDist,put=PutLineOffsetDist))
			double LineOffsetDist;
		__declspec(property(get=GetProWinSlabInsuThick,put=PutProWinSlabInsuThick))
			double ProWinSlabInsuThick;
		__declspec(property(get=GetGetSubKind,put=PutGetSubKind))
			_bstr_t GetSubKind;
		__declspec(property(get=GetBayThick,put=PutBayThick))
			double BayThick;
		__declspec(property(get=GetHoleStyle,put=PutHoleStyle))
			_bstr_t HoleStyle;
		__declspec(property(get=GetConductivity))
			double Conductivity;
		__declspec(property(get=GetUsage,put=PutUsage))
			_bstr_t Usage;
		__declspec(property(get=GetShadowType,put=PutShadowType))
			_bstr_t ShadowType;
		__declspec(property(get=GetIsHigh,put=PutIsHigh))
			_bstr_t IsHigh;
		__declspec(property(get=GetStruWinSill,put=PutStruWinSill))
			double StruWinSill;

		//
		// Wrapper methods for error-handling
		//

		double GetScale ( );
		void PutScale (	double pVal );
		double GetLayoutRotation ( );
		void PutLayoutRotation (double pVal );
		_bstr_t GetHoleType ( );
		void PutHoleType (	_bstr_t pVal );
		double GetWinSill ( );
		void PutWinSill ( double pVal );
		double GetDoorSill ( );
		void PutDoorSill ( double pVal );
		double GetHeight ( );
		void PutHeight ( double pVal );
		double GetWidth ( );
		void PutWidth (	double pVal );
		double GetStruWinSill ( );
		void PutStruWinSill (	double pVal );
		double GetBayThick ( );
		void PutBayThick (	double pVal );
		double GetSlabThickness ( );
		void PutSlabThickness (	double pVal );
		double GetUFrame ( );
		void PutUFrame (	double pVal );
		double GetVFrame ( );
		void PutVFrame (	double pVal );
		_bstr_t GetOutlineArea ( );
		double GetOutlineLength ( );
		_bstr_t GetLabel ( );
		void PutLabel (	_bstr_t pVal );
		double GetTextHeight ( );
		void PutTextHeight (double pVal );
		_bstr_t GetTextStyle ( );
		void PutTextStyle (	_bstr_t pVal );
		_bstr_t GetHideLabel ( );
		void PutHideLabel (	_bstr_t pVal );
		ACAD_LAYER GetTextLayer ( );
		void PutTextLayer (	ACAD_LAYER pVal );
		ACAD_LAYER GetGlassLayer ( );
		void PutGlassLayer (ACAD_LAYER pVal );
		ACAD_LAYER GetFrameLayer ( );
		void PutFrameLayer (ACAD_LAYER pVal );
		ACAD_LAYER GetSlabLayer ( );
		void PutSlabLayer (	ACAD_LAYER pVal );
		_bstr_t GetProStyle ( );
		void PutProStyle (	_bstr_t pVal );
		_bstr_t GetLeftSlab ( );
		void PutLeftSlab (	_bstr_t pVal );
		_bstr_t GetRightSlab ( );
		void PutRightSlab (	_bstr_t pVal );
		double GetProWinSlabWidth ( );
		void PutProWinSlabWidth (	double pVal );
		double GetProWinSlabInsuThick ( );
		void PutProWinSlabInsuThick (	double pVal );
		_bstr_t GetSlot ( );
		void PutSlot (	_bstr_t pVal );
		_bstr_t GetSlotMaterial ( );
		void PutSlotMaterial (	_bstr_t pVal );
		double GetSlotExt ( );
		void PutSlotExt (	double pVal );
		double GetSlotWidth ( );
		void PutSlotWidth (	double pVal );
		double GetSlotInsuThick ( );
		void PutSlotInsuThick (	double pVal );
		_bstr_t GetSlotInsuCap ( );
		void PutSlotInsuCap ( _bstr_t pVal );
		_bstr_t GetGetKind ( );
		_bstr_t GetGetSubKind ( );
		void PutGetSubKind (	_bstr_t pVal );
		_bstr_t GetHoleStyle ( );
		void PutHoleStyle (	_bstr_t pVal );
		_bstr_t GetEvacuationType ( );
		void PutEvacuationType (_bstr_t pVal );
		_bstr_t GetUpLevel ( );
		void PutUpLevel ( _bstr_t pVal );
		short GetDoorLine ( );
		void PutDoorLine (	short pVal );
		double GetLineOffsetDist ( );
		void PutLineOffsetDist (	double pVal );
		_bstr_t GetStructure ( );
		void PutStructure (	_bstr_t pVal );
		_bstr_t GetExtShading ( );
		void PutExtShading (	_bstr_t pVal );
		double GetAreaRatio ( );
		double GetConductivity ( );
		_bstr_t GetUsage ( );
		void PutUsage (	_bstr_t pVal );
		_bstr_t GetShadowType ( );
		void PutShadowType ( _bstr_t pVal );
		_bstr_t GetIsHigh ( );
		void PutIsHigh ( _bstr_t pVal );

		//
		// Raw methods provided by interface
		//

		virtual HRESULT __stdcall get_Scale (
			/*[out,retval]*/ double * pVal ) = 0;
		virtual HRESULT __stdcall put_Scale (
			/*[in]*/ double pVal ) = 0;
		virtual HRESULT __stdcall get_LayoutRotation (
			/*[out,retval]*/ double * pVal ) = 0;
		virtual HRESULT __stdcall put_LayoutRotation (
			/*[in]*/ double pVal ) = 0;
		virtual HRESULT __stdcall get_HoleType (
			/*[out,retval]*/ BSTR * pVal ) = 0;
		virtual HRESULT __stdcall put_HoleType (
			/*[in]*/ BSTR pVal ) = 0;
		virtual HRESULT __stdcall get_WinSill (
			/*[out,retval]*/ double * pVal ) = 0;
		virtual HRESULT __stdcall put_WinSill (
			/*[in]*/ double pVal ) = 0;
		virtual HRESULT __stdcall get_DoorSill (
			/*[out,retval]*/ double * pVal ) = 0;
		virtual HRESULT __stdcall put_DoorSill (
			/*[in]*/ double pVal ) = 0;
		virtual HRESULT __stdcall get_Height (
			/*[out,retval]*/ double * pVal ) = 0;
		virtual HRESULT __stdcall put_Height (
			/*[in]*/ double pVal ) = 0;
		virtual HRESULT __stdcall get_Width (
			/*[out,retval]*/ double * pVal ) = 0;
		virtual HRESULT __stdcall put_Width (
			/*[in]*/ double pVal ) = 0;
		virtual HRESULT __stdcall get_StruWinSill (
			/*[out,retval]*/ double * pVal ) = 0;
		virtual HRESULT __stdcall put_StruWinSill (
			/*[in]*/ double pVal ) = 0;
		virtual HRESULT __stdcall get_BayThick (
			/*[out,retval]*/ double * pVal ) = 0;
		virtual HRESULT __stdcall put_BayThick (
			/*[in]*/ double pVal ) = 0;
		virtual HRESULT __stdcall get_SlabThickness (
			/*[out,retval]*/ double * pVal ) = 0;
		virtual HRESULT __stdcall put_SlabThickness (
			/*[in]*/ double pVal ) = 0;
		virtual HRESULT __stdcall get_UFrame (
			/*[out,retval]*/ double * pVal ) = 0;
		virtual HRESULT __stdcall put_UFrame (
			/*[in]*/ double pVal ) = 0;
		virtual HRESULT __stdcall get_VFrame (
			/*[out,retval]*/ double * pVal ) = 0;
		virtual HRESULT __stdcall put_VFrame (
			/*[in]*/ double pVal ) = 0;
		virtual HRESULT __stdcall get_OutlineArea (
			/*[out,retval]*/ BSTR * pVal ) = 0;
		virtual HRESULT __stdcall get_OutlineLength (
			/*[out,retval]*/ double * pVal ) = 0;
		virtual HRESULT __stdcall get_Label (
			/*[out,retval]*/ BSTR * pVal ) = 0;
		virtual HRESULT __stdcall put_Label (
			/*[in]*/ BSTR pVal ) = 0;
		virtual HRESULT __stdcall get_TextHeight (
			/*[out,retval]*/ double * pVal ) = 0;
		virtual HRESULT __stdcall put_TextHeight (
			/*[in]*/ double pVal ) = 0;
		virtual HRESULT __stdcall get_TextStyle (
			/*[out,retval]*/ BSTR * pVal ) = 0;
		virtual HRESULT __stdcall put_TextStyle (
			/*[in]*/ BSTR pVal ) = 0;
		virtual HRESULT __stdcall get_HideLabel (
			/*[out,retval]*/ BSTR * pVal ) = 0;
		virtual HRESULT __stdcall put_HideLabel (
			/*[in]*/ BSTR pVal ) = 0;
		virtual HRESULT __stdcall get_TextLayer (
			/*[out,retval]*/ ACAD_LAYER * pVal ) = 0;
		virtual HRESULT __stdcall put_TextLayer (
			/*[in]*/ ACAD_LAYER pVal ) = 0;
		virtual HRESULT __stdcall get_GlassLayer (
			/*[out,retval]*/ ACAD_LAYER * pVal ) = 0;
		virtual HRESULT __stdcall put_GlassLayer (
			/*[in]*/ ACAD_LAYER pVal ) = 0;
		virtual HRESULT __stdcall get_FrameLayer (
			/*[out,retval]*/ ACAD_LAYER * pVal ) = 0;
		virtual HRESULT __stdcall put_FrameLayer (
			/*[in]*/ ACAD_LAYER pVal ) = 0;
		virtual HRESULT __stdcall get_SlabLayer (
			/*[out,retval]*/ ACAD_LAYER * pVal ) = 0;
		virtual HRESULT __stdcall put_SlabLayer (
			/*[in]*/ ACAD_LAYER pVal ) = 0;
		virtual HRESULT __stdcall get_ProStyle (
			/*[out,retval]*/ BSTR * pVal ) = 0;
		virtual HRESULT __stdcall put_ProStyle (
			/*[in]*/ BSTR pVal ) = 0;
		virtual HRESULT __stdcall get_LeftSlab (
			/*[out,retval]*/ BSTR * pVal ) = 0;
		virtual HRESULT __stdcall put_LeftSlab (
			/*[in]*/ BSTR pVal ) = 0;
		virtual HRESULT __stdcall get_RightSlab (
			/*[out,retval]*/ BSTR * pVal ) = 0;
		virtual HRESULT __stdcall put_RightSlab (
			/*[in]*/ BSTR pVal ) = 0;
		virtual HRESULT __stdcall get_ProWinSlabWidth (
			/*[out,retval]*/ double * pVal ) = 0;
		virtual HRESULT __stdcall put_ProWinSlabWidth (
			/*[in]*/ double pVal ) = 0;
		virtual HRESULT __stdcall get_ProWinSlabInsuThick (
			/*[out,retval]*/ double * pVal ) = 0;
		virtual HRESULT __stdcall put_ProWinSlabInsuThick (
			/*[in]*/ double pVal ) = 0;
		virtual HRESULT __stdcall get_Slot (
			/*[out,retval]*/ BSTR * pVal ) = 0;
		virtual HRESULT __stdcall put_Slot (
			/*[in]*/ BSTR pVal ) = 0;
		virtual HRESULT __stdcall get_SlotMaterial (
			/*[out,retval]*/ BSTR * pVal ) = 0;
		virtual HRESULT __stdcall put_SlotMaterial (
			/*[in]*/ BSTR pVal ) = 0;
		virtual HRESULT __stdcall get_SlotExt (
			/*[out,retval]*/ double * pVal ) = 0;
		virtual HRESULT __stdcall put_SlotExt (
			/*[in]*/ double pVal ) = 0;
		virtual HRESULT __stdcall get_SlotWidth (
			/*[out,retval]*/ double * pVal ) = 0;
		virtual HRESULT __stdcall put_SlotWidth (
			/*[in]*/ double pVal ) = 0;
		virtual HRESULT __stdcall get_SlotInsuThick (
			/*[out,retval]*/ double * pVal ) = 0;
		virtual HRESULT __stdcall put_SlotInsuThick (
			/*[in]*/ double pVal ) = 0;
		virtual HRESULT __stdcall get_SlotInsuCap (
			/*[out,retval]*/ BSTR * pVal ) = 0;
		virtual HRESULT __stdcall put_SlotInsuCap (
			/*[in]*/ BSTR pVal ) = 0;
		virtual HRESULT __stdcall get_GetKind (
			/*[out,retval]*/ BSTR * pVal ) = 0;
		virtual HRESULT __stdcall get_GetSubKind (
			/*[out,retval]*/ BSTR * pVal ) = 0;
		virtual HRESULT __stdcall put_GetSubKind (
			/*[in]*/ BSTR pVal ) = 0;
		virtual HRESULT __stdcall get_HoleStyle (
			/*[out,retval]*/ BSTR * pVal ) = 0;
		virtual HRESULT __stdcall put_HoleStyle (
			/*[in]*/ BSTR pVal ) = 0;
		virtual HRESULT __stdcall get_EvacuationType (
			/*[out,retval]*/ BSTR * pVal ) = 0;
		virtual HRESULT __stdcall put_EvacuationType (
			/*[in]*/ BSTR pVal ) = 0;
		virtual HRESULT __stdcall get_UpLevel (
			/*[out,retval]*/ BSTR * pVal ) = 0;
		virtual HRESULT __stdcall put_UpLevel (
			/*[in]*/ BSTR pVal ) = 0;
		virtual HRESULT __stdcall get_DoorLine (
			/*[out,retval]*/ short * pVal ) = 0;
		virtual HRESULT __stdcall put_DoorLine (
			/*[in]*/ short pVal ) = 0;
		virtual HRESULT __stdcall get_LineOffsetDist (
			/*[out,retval]*/ double * pVal ) = 0;
		virtual HRESULT __stdcall put_LineOffsetDist (
			/*[in]*/ double pVal ) = 0;
		virtual HRESULT __stdcall get_Structure (
			/*[out,retval]*/ BSTR * pVal ) = 0;
		virtual HRESULT __stdcall put_Structure (
			/*[in]*/ BSTR pVal ) = 0;
		virtual HRESULT __stdcall get_ExtShading (
			/*[out,retval]*/ BSTR * pVal ) = 0;
		virtual HRESULT __stdcall put_ExtShading (
			/*[in]*/ BSTR pVal ) = 0;
		virtual HRESULT __stdcall get_AreaRatio (
			/*[out,retval]*/ double * pVal ) = 0;
		virtual HRESULT __stdcall get_Conductivity (
			/*[out,retval]*/ double * pVal ) = 0;
		virtual HRESULT __stdcall get_Usage (
			/*[out,retval]*/ BSTR * pVal ) = 0;
		virtual HRESULT __stdcall put_Usage (
			/*[in]*/ BSTR pVal ) = 0;
		virtual HRESULT __stdcall get_ShadowType (
			/*[out,retval]*/ BSTR * pVal ) = 0;
		virtual HRESULT __stdcall put_ShadowType (
			/*[in]*/ BSTR pVal ) = 0;
		virtual HRESULT __stdcall get_IsHigh (
			/*[out,retval]*/ BSTR * pVal ) = 0;
		virtual HRESULT __stdcall put_IsHigh (
			/*[in]*/ BSTR pVal ) = 0;
	};


	struct __declspec(uuid("fea237f0-693c-49d6-9548-f21b9f326237"))
		ComOpening;
	// [ default ] interface IComOpening
	// [ default, source ] interface IAcadObjectEvents



	#include "tch10_com19_opening.tli"


} // namespace TCH10_COM9_T20V5X64

#pragma pack(pop)