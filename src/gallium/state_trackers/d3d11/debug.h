
#ifndef _D3D11_DEBUG_H_
#define _D3D11_DEBUG_H_

#ifdef DEBUG
HRESULT D3D11PrintMsg(HRESULT, D3D11_MESSAGE_SEVERITY, D3D11_MESSAGE_CATEGORY, D3D11_MESSAGE_ID, const char *file, const char *func, int line, const char *, ...);

#define DBG(r, s, args)   D3D11PrintMsg(r, D3D11_MESSAGE_SEVERITY_MESSAGE,    D3D11_MESSAGE_CATEGORY_APPLICATION_DEFINED, D3D11_MESSAGE_ID_UNKNOWN, __FILE__, __FUNCTION__, __LINE__, s, args)
#define INFO(r, s, args)  D3D11PrintMsg(r, D3D11_MESSAGE_SEVERITY_INFO,       D3D11_MESSAGE_CATEGORY_APPLICATION_DEFINED, D3D11_MESSAGE_ID_UNKNOWN, __FILE__, __FUNCTION__, __LINE__, s, args)
#define WARN(r, s, args)  D3D11PrintMsg(r, D3D11_MESSAGE_SEVERITY_WARNING,    D3D11_MESSAGE_CATEGORY_APPLICATION_DEFINED, D3D11_MESSAGE_ID_UNKNOWN, __FILE__, __FUNCTION__, __LINE__, s, args)
#define Error(r, s, args) D3D11PrintMsg(r, D3D11_MESSAGE_SEVERITY_ERROR,      D3D11_MESSAGE_CATEGORY_APPLICATION_DEFINED, D3D11_MESSAGE_ID_UNKNOWN, __FILE__, __FUNCTION__, __LINE__, s, args)
#define FATAL(r, s, args) D3D11PrintMsg(r, D3D11_MESSAGE_SEVERITY_CORRUPTION, D3D11_MESSAGE_CATEGORY_APPLICATION_DEFINED, D3D11_MESSAGE_ID_UNKNOWN, __FILE__, __FUNCTION__, __LINE__, s, args)

#define ErrorID(d, s, args) D3D11InfoQueue_AddMessage(infoQueue, D3D11_MESSAGE_CATEGORY_APPLICATION_DEFINED, D3D11_MESSAGE_SEVERITY_ERROR, D3D11_MESSAGE_ID_##d, __FILE__, __FUNCTION__, __LINE__, s)

#define STUB() do { \
        D3D11InfoQueue_AddMessage(__d3d11_InfoQueue, D3D11_MESSAGE_CATEGORY_APPLICATION_DEFINED, D3D11_MESSAGE_SEVERITY_WARNING, D3D11_MESSAGE_ID_UNKNOWN, "STUB"); \
    } while(0)

#define STUB_return(r) do { \
        D3D11InfoQueue_AddMessage(__d3d11_InfoQueue, D3D11_MESSAGE_CATEGORY_APPLICATION_DEFINED, D3D11_MESSAGE_SEVERITY_WARNING, D3D11_MESSAGE_ID_UNKNOWN, "STUB"); \
        return r; \
    } while(0)

#else /* !DEBUG */

static INLINE HRESULT DBG  (HRESULT hr, const char *str, ...) { return hr; }
static INLINE HRESULT INFO (HRESULT hr, const char *str, ...) { return hr; }
static INLINE HRESULT WARN (HRESULT hr, const char *str, ...) { return hr; }
static INLINE HRESULT Error(HRESULT hr, const char *str, ...) { return hr; }
static INLINE HRESULT FATAL(HRESULT hr, const char *str, ...) { return hr; }

#define STUB()
#define STUB_return(r) return r

#endif /* DEBUG */

#endif /* _D3D11_DEBUG_H_ */
