
#include "stdafx.h"
#include "Globals.h"

CString ASCIITranslate(DWORD hex)
{
	unsigned int segLow;
	unsigned int segLowMiddle;
	unsigned int segHighMiddle;
	unsigned int segHigh;

	segLow = hex & 0xFF;
	segLowMiddle = (hex & 0xFF00) >> 8;
	segHighMiddle = (hex & 0xFF0000) >> 16;
	segHigh = (hex & 0xFF000000) >> 24;

	CString str;
	str.Format("%C", segLow);
	str.AppendFormat("%C", segLowMiddle);
	str.AppendFormat("%C", segHighMiddle);
	str.AppendFormat("%C", segHigh);

	return str;
}
