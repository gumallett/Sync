#include "PortableDeviceCOM.h"

TCHAR **getDeviceIds() {
	HRESULT result;
	IPortableDeviceManager *mgr;
	DWORD ids, namel = 0;
	TCHAR **deviceIDs = NULL;
	TCHAR **names = NULL;

	result = CoInitializeEx(0, COINIT_MULTITHREADED);

	if(result == S_OK) {
		result = CoCreateInstance(&CLSID_PortableDeviceManager, 0, CLSCTX_INPROC_SERVER, &IID_IPortableDeviceManager, &mgr);

		if(result == S_OK) {
			result = mgr->lpVtbl->GetDevices(mgr, 0, &ids);
			
			if(result == S_OK) {
				deviceIDs = malloc(ids * sizeof(TCHAR*));

				result = mgr->lpVtbl->GetDevices(mgr, deviceIDs, &ids);

				if(result == S_OK) {
					names = getDeviceNames(mgr, deviceIDs, ids);
				}
			}
		}
	}

	CoUninitialize();
	return deviceIDs;
}

TCHAR **getDeviceNames(IPortableDeviceManager *mgr, TCHAR **deviceIDs, DWORD numDevices) {
	HRESULT result;
	TCHAR **names;
	TCHAR *name;
	DWORD namel = 0, i;

	names = malloc(numDevices * sizeof(TCHAR *));

	for(i = 0; i< numDevices; i++) {
		result = mgr->lpVtbl->GetDeviceDescription(mgr, deviceIDs[i], NULL, &namel);
					
		if(result == S_OK) {
			name = malloc(namel * sizeof(TCHAR));
			result = mgr->lpVtbl->GetDeviceDescription(mgr, deviceIDs[0], name, &namel);

			if(result == S_OK) {
				names[i] = name;
			}
			else {
				names[i] = NULL;
			}
		}
	}

	return names;
}