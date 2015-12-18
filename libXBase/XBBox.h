#pragma once
#include "libXBase/XPt3D.h"

//cette classe n'est pas une BBox et devrait plutot s'appeler XFrame3D
class XBBox
{
public:
	XPt3D min;
	XPt3D max;
public:
	XBBox(void);
	~XBBox(void);
};
