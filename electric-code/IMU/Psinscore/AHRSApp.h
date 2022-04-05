/* CVGApp c++ hearder file VGApp.h */
/*
	By     : Yan Gongmin @ NWPU
	Date   : 2018-02-05
	From   : College of Automation, 
	         Northwestern Polytechnical University, 
			 Xi'an 710072, China
*/

#ifndef _AHRSAPP_H
#define _AHRSAPP_H

#include "PSINS.h"


class CAHRSApp:public CSINSTDKF
{
public:
	CAHRSApp(double Yaw0=0.0);
	virtual void SetMeas(void) {};
	void Update(const CVect3 &wm, const CVect3 &vm, const CVect3 &vnGPS, const CVect3 &posGPS, double ts);
};

#endif

