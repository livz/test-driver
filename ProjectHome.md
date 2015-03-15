Wrapper application to test kernel drivers. Steps:

```
- Register driver as a Windows service
- Start the driver
- Call IOCTLs, do stuff
- Stop the service
- Delete the service
```