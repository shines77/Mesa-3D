
#ifndef _D3D11_HELPERS_H_
#define _D3D11_HELPERS_H_

#ifdef DEBUG
extern struct D3D11InfoQueue *__d3d11_InfoQueue;

#define DBG(s)
#define Info(s) D3D11InfoQueue_AddMessage(infoQueue, D3D11_MESSAGE_CATEGORY_APPLICATION_DEFINED, D3D11_MESSAGE_SEVERITY_INFO, D3D11_MESSAGE_ID_UNKNOWN, s)
#define Warn(s) D3D11InfoQueue_AddMessage(infoQueue, D3D11_MESSAGE_CATEGORY_APPLICATION_DEFINED, D3D11_MESSAGE_SEVERITY_WARNING, D3D11_MESSAGE_ID_UNKNOWN, s)
#define Error(s)
#define FATAL(s)

#define ErrorID(d, s) D3D11InfoQueue_AddMessage(infoQueue, D3D11_MESSAGE_CATEGORY_APPLICATION_DEFINED, D3D11_MESSAGE_SEVERITY_ERROR, D3D11_MESSAGE_ID_##d, s)

static INLINE HRESULT Error(HRESULT hr, const char *desc, const char *file, int line)
{
    D3D11InfoQueue_AddMessage(__d3d11_InfoQueue, D3D11_MESSAGE_CATEGORY_APPLICATION_DEFINED, D3D11_MESSAGE_SEVERITY_ERROR, D3D11_MESSAGE_ID_UNKNOWN, desc);
    return hr;
}

#define STUB() do { \
        D3D11InfoQueue_AddMessage(__d3d11_InfoQueue, D3D11_MESSAGE_CATEGORY_APPLICATION_DEFINED, D3D11_MESSAGE_SEVERITY_WARNING, D3D11_MESSAGE_ID_UNKNOWN, "STUB"); \
    } while(0)

#define STUB_return(r) do { \
        D3D11InfoQueue_AddMessage(__d3d11_InfoQueue, D3D11_MESSAGE_CATEGORY_APPLICATION_DEFINED, D3D11_MESSAGE_SEVERITY_WARNING, D3D11_MESSAGE_ID_UNKNOWN, "STUB"); \
        return r; \
    } while(0)

#else /* !DEBUG */

static INLINE HRESULT Error(HRESULT hr, const char *desc, const char *file, int line)
{
    return hr;
}

#define STUB()
#define STUB_return(r) return r

#endif /* DEBUG */

#endif /* _D3D11_HELPERS_H_ */
