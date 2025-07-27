//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of NVIDIA CORPORATION nor the names of its
//    contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Copyright (c) 2008-2021 NVIDIA Corporation. All rights reserved.

#include "PxPhysXConfig.h"

#if PX_SUPPORT_GPU_PHYSX

#include "gpu/PxGpu.h"

#ifndef PX_PHYSX_GPU_STATIC
namespace grid
{
	class Server;
	class ClientContextPredictionManager;
}

namespace augphysx
{
	//forward declare stuff from PxPhysXGpuModuleLoader.cpp
	void PxLoadPhysxGPUModule(const char* appGUID);
	typedef augphysx::PxCudaContextManager* (PxCreateCudaContextManager_FUNC)(augphysx::PxFoundation& foundation, const augphysx::PxCudaContextManagerDesc& desc, augphysx::PxProfilerCallback* profilerCallback);
	typedef int (PxGetSuggestedCudaDeviceOrdinal_FUNC)(augphysx::PxErrorCallback& errc);
	typedef grid::ClientContextPredictionManager* (PxCreateClientContextManager_FUNC)(grid::Server* server, augphysx::PxU32 maxNbSleepMsg);
	extern PxCreateCudaContextManager_FUNC*  g_PxCreateCudaContextManager_Func;
	extern PxGetSuggestedCudaDeviceOrdinal_FUNC* g_PxGetSuggestedCudaDeviceOrdinal_Func;
	extern PxCreateClientContextManager_FUNC* g_CreateClientContextManager_Func;

} // end physx namespace



augphysx::PxCudaContextManager* PxCreateCudaContextManager(augphysx::PxFoundation& foundation, const augphysx::PxCudaContextManagerDesc& desc, augphysx::PxProfilerCallback* profilerCallback)
{
	if (!augphysx::g_PxCreateCudaContextManager_Func)
		augphysx::PxLoadPhysxGPUModule(desc.appGUID);

	if (augphysx::g_PxCreateCudaContextManager_Func)
		return augphysx::g_PxCreateCudaContextManager_Func(foundation, desc, profilerCallback);
	else
		return NULL;
}

int PxGetSuggestedCudaDeviceOrdinal(augphysx::PxErrorCallback& errc)
{
	if (!augphysx::g_PxGetSuggestedCudaDeviceOrdinal_Func)
		augphysx::PxLoadPhysxGPUModule(NULL);

	if (augphysx::g_PxGetSuggestedCudaDeviceOrdinal_Func)
		return augphysx::g_PxGetSuggestedCudaDeviceOrdinal_Func(errc);
	else
		return -1;
}

PX_C_EXPORT PX_PHYSX_CORE_API grid::ClientContextPredictionManager* PX_CALL_CONV PxCreateCudaClientContextManager(grid::Server* server, augphysx::PxU32 maxNbSleepMsg);

grid::ClientContextPredictionManager* PxCreateCudaClientContextManager(grid::Server* server, augphysx::PxU32 maxNbSleepMsg)
{
	if (!augphysx::g_CreateClientContextManager_Func)
		augphysx::PxLoadPhysxGPUModule(NULL);

	if (augphysx::g_CreateClientContextManager_Func)
		return augphysx::g_CreateClientContextManager_Func(server, maxNbSleepMsg);
	else
		return NULL;
}
#endif

#endif // PX_SUPPORT_GPU_PHYSX

