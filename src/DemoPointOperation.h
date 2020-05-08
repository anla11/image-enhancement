#ifndef _DEMO_POINTOPERATION_
#define _DEMO_POINTOPERATION_

#include "PointOperation.h"

//-----------------------------------Quantization----------------------------------
Mat     quantization(const Mat &src);

//------------use Linear Equation to change brightness of image----------
Mat     LinearAdjust(const Mat &src);

//--------------------use gamma to change contrast of image--------------
Mat     powerAdjust(const Mat &src);

//-------------------------sigmoid adjust--------------------------------
Mat		sigmoidAdjust(const Mat &src);

//-------------------------Contrast-Strectching--------------------------
Mat		contrastStretching(const Mat &src);

//---------------------------Aplly Threshold-----------------------------
Mat		adaptiveThreshold(const Mat &src);
Mat     applyThreshold(const Mat &src);

#endif //_DEMO_POINTOPERATION_