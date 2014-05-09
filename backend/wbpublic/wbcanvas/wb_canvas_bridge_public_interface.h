#ifndef _WB_CANVAS_BRIDGE_PUBLIC_INTERFACE_
#define _WB_CANVAS_BRIDGE_PUBLIC_INTERFACE_

#ifdef _WIN32

#ifdef MYSQLWBCANVASBRIDGE_EXPORTS
#define MYSQLWBCANVASBRIDGE_PUBLIC __declspec(dllexport)
#else
#define MYSQLWBCANVASBRIDGE_PUBLIC __declspec(dllimport)
#endif

#else

#define MYSQLWBCANVASBRIDGE_PUBLIC

#endif

#endif // _WB_CANVAS_BRIDGE_PUBLIC_INTERFACE_
