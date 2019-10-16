#pragma once

#include <vector>
#include "../Object/Railing/AttrRailing.h"
#include "..\Tool\MarkupXml\Markup.h"


class CRailingWebData
{
public:
	bool GetRailingById(CString p_sId, AttrRailing& value)const;

	std::vector<AttrRailing> GetRailings(eRailingType type)const;
	std::vector<AttrRailing> GetAllRailings()const;

	std::vector<AttrRailing > ParseRailingsFromXML(CMarkup xml)const;//��XML����������Ϣ
protected:

};

