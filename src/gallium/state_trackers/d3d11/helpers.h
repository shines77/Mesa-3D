
#ifndef _D3D11_HELPERS_H_
#define _D3D11_HELPERS_H_

#define INFO(s) D3D11InfoQueue_AddMessage(infoQueue, D3D11_MESSAGE_CATEGORY_APPLICATION_DEFINED, D3D11_MESSAGE_SEVERITY_INFO, D3D11_MESSAGE_ID_UNKNOWN, s)
#define WARN(s) D3D11InfoQueue_AddMessage(infoQueue, D3D11_MESSAGE_CATEGORY_APPLICATION_DEFINED, D3D11_MESSAGE_SEVERITY_WARNING, D3D11_MESSAGE_ID_UNKNOWN, s)
#define ERROR(s) D3D11InfoQueue_AddMessage(infoQueue, D3D11_MESSAGE_CATEGORY_APPLICATION_DEFINED, D3D11_MESSAGE_SEVERITY_ERROR, D3D11_MESSAGE_ID_UNKNOWN, s)

#define ERROR_ID(d, s) D3D11InfoQueue_AddMessage(infoQueue, D3D11_MESSAGE_CATEGORY_APPLICATION_DEFINED, D3D11_MESSAGE_SEVERITY_ERROR, D3D11_MESSAGE_ID_##d, s)

#define return_error(r) do {                                           \
    ERROR(D3D11InfoQueue_AddMessage(infoQueue, CATEGORY, SEVERITY, ID, ""); \
        return r;
    } while(0)

#define STUB() do { \
        D3D11InfoQueue_AddMessage(); \
    } while(0)

#define STUB_return(r) do { \
        D3D11InfoQueue_AddMessage(); \
        return r;
    } while(0)

#endif /* _D3D11_HELPERS_H_ */
