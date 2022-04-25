#ifndef _TRITONT_HPP_
#define _TRITONT_HPP_

#include "triton.hpp"

class TritonTask:public StoreTask
{
	public:
		TritonTask(char *path)
		{
			SetPaths(path,
					 path,
					 NULL, NULL,
					 "pm3mgt");
			lockcomm(LMC_INTERNAL);

			m_triton.OpenMe();
		}
		triton m_triton;
};




#endif //_TRITONT_HPP_
