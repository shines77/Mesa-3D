
static struct D3D11InfoQueue queue;

static struct pipe_mutex queue_mutex;

HRESULT
D3D11PrintMsg(HRESULT hr,
              D3D11_MESSAGE_SEVERITY sev,
              D3D11_MESSAGE_CATEGORY cat,
              D3D11_MESSAGE_ID id,
              const char *file,
              const char *func,
              int line,
              const char *fmt, ...)
{
    char msg[1024];
    va_list ap;

    va_start(ap, fmt);
    vsnprintf(msg, sizeof(msg), fmt, ap);
    va_end(ap);

    D3D11InfoQueue_AddMessgae(&queue, cat, sev, id, msg);
    return hr;
}
