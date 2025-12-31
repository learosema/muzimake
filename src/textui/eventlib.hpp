class IEventListener {
public:
    virtual void onEvent(const int eventSource, const int eventId, const void *payload) = 0;
};

class EventEmitter 
{
public:
    EventEmitter(const int eventSource = 0);
    virtual ~EventEmitter();

    EventEmitter &Emit(const int eventId, const void *payload);
    EventEmitter &AddEventListener(const int eventId, IEventListener *pListener);
    inline const int EventSource() const { return m_eventSource; }

private:
    IEventListener **m_pListeners;
    int m_numListeners;
    int m_numReservedListeners;
    const int m_eventSource;
};
