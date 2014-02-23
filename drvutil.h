#ifndef DRIVER_UTILS_INCLUDED
#define DRIVER_UTILS_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

BOOL	RegisterDriver		(LPCTSTR szDriverName, LPCTSTR szPathName);
BOOL	UnregisterDriver	(LPCTSTR szDriverName);

BOOL	StartDriver			(LPCTSTR szDriverName);
BOOL	StopDriver			(LPCTSTR szDriverName);

#ifdef __cplusplus
}
#endif

#endif