#include "eventlib.hpp"
#include <stddef.h>
#include <string.h>

EventEmitter::EventEmitter(const int eventSource) : m_eventSource(eventSource), m_numListeners(0), m_numReservedListeners(1)
{
    m_pListeners = new IEventListener*[m_numReservedListeners];
}

EventEmitter::~EventEmitter()
{
    delete m_pListeners;
}

EventEmitter &EventEmitter::AddEventListener(const int eventId,  IEventListener *pListener)
{
    if (m_numListeners >= m_numReservedListeners) {
        const int increase = 1;
         IEventListener **pListenersNew = new IEventListener*[m_numReservedListeners + increase];
        memcpy(pListenersNew, m_pListeners, m_numReservedListeners * sizeof(IEventListener*));
        delete m_pListeners;
        m_pListeners = pListenersNew;
    }
    m_pListeners[m_numListeners] = pListener;
    m_numListeners++;
    return *this;
}

EventEmitter &EventEmitter::Emit(const int eventId, const void *payload)
{
    for (int i = 0; i < m_numListeners; i++) {
        m_pListeners[i]->onEvent(m_eventSource, eventId, payload);
    }
    return *this;
}
