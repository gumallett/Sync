#include "PortableDeviceCOM.h"

void COM_test() {
	HANDLE handle;
	HRESULT result;
	IPortableDeviceManager *mgr;
	DWORD ids;

	result = CoInitializeEx(0, COINIT_MULTITHREADED);

	if(result == S_OK) {
		result = CoCreateInstance(&CLSID_PortableDeviceManager, 0, CLSCTX_INPROC_SERVER, &IID_IPortableDeviceManager, &mgr);

		if(result == S_OK) {
			result = mgr->lpVtbl->GetDevices(mgr, 0, &ids);
			
		}
	}

	CoUninitialize();
}