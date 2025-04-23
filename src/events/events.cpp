#include <stdio.h>
#include <stdlib.h>

#include "events.h"

/* === Datenstrukturen für das Eventsystem === */

// Struktur eines registrierten Listeners
typedef struct Listener Listener_t;
struct Listener {
    EventHandler_t handler;
    unsigned int filter;   // Filter: z. B. Event-ID, für die dieser Listener zuständig ist
    Listener_t* next;
};

// Element der Message Queue (verkettete Liste)
typedef struct QueueItem QueueItem_t;
struct QueueItem {
    EventParam_t event;
    QueueItem_t* next;
};

/* === Initialisierung und Listener-Verwaltung === */

void InitEventManager(EventManager_t* manager) {
    manager->listeners = NULL;
    manager->queueHead = NULL;
    my_mutex_init(&manager->listenerMutex);
    my_mutex_init(&manager->queueMutex);
}

void RegisterListener(EventManager_t* manager, EventHandler_t handler, unsigned int filter) {
    Listener_t* newListener = (Listener_t*) malloc(sizeof(Listener_t));
    if (newListener == NULL) {
        return; // Fehlerbehandlung: Speicherfehler
    }
    newListener->handler = handler;
    newListener->filter = filter;

    my_mutex_lock(&manager->listenerMutex);
    newListener->next = manager->listeners;
    manager->listeners = newListener;
    my_mutex_unlock(&manager->listenerMutex);
}

void UnregisterListener(EventManager_t* manager, EventHandler_t handler) {
    my_mutex_lock(&manager->listenerMutex);
    Listener_t** current = &manager->listeners;
    while (*current != NULL) {
        if ((*current)->handler == handler) {
            Listener_t* temp = *current;
            *current = (*current)->next;
            free(temp);
            break;
        }
        current = &(*current)->next;
    }
    my_mutex_unlock(&manager->listenerMutex);
}

/* === Nachrichtensenden === */

// SendMessage: Sende das Event synchron an alle passenden Listener.
int SendMessage(EventManager_t* manager, const EventParam_t* event) {
    int result = 0;

    my_mutex_lock(&manager->listenerMutex);
    Listener_t* current = manager->listeners;
    while (current != NULL) {
        if (current->filter == event->eventId) {
            // Aufruf des registrierten Handlers
            result = current->handler((EventParam_t*)event);
            if (result != 0) { // Bei nicht-null Rückgabewert eventuell abbrechen
                break;
            }
        }
        current = current->next;
    }
    my_mutex_unlock(&manager->listenerMutex);

    return result;
}

// PostMessage: Fügt ein Event (mit Priorität) in die Message Queue ein.
void PostMessage(EventManager_t* manager, const EventParam_t* event) {
    QueueItem_t* newItem = (QueueItem_t*) malloc(sizeof(QueueItem_t));
    if (newItem == NULL) {
        return; // Fehlerbehandlung: Speicherfehler
    }
    newItem->event = *event;
    newItem->next = NULL;

    my_mutex_lock(&manager->queueMutex);
    // Sortierte Einfügung: Das Event mit der niedrigsten Prioritätszahl wird zuerst abgearbeitet.
    if (manager->queueHead == NULL || newItem->event.priority < manager->queueHead->event.priority) {
        newItem->next = manager->queueHead;
        manager->queueHead = newItem;
    } else {
        QueueItem_t* current = manager->queueHead;
        while (current->next != NULL && current->next->event.priority <= newItem->event.priority) {
            current = current->next;
        }
        newItem->next = current->next;
        current->next = newItem;
    }
    my_mutex_unlock(&manager->queueMutex);
}

/* === Verarbeitung der Message Queue (ohne Endlosschleife) === */

// ProcessMessageQueue: Abarbeiten aller gepufferten Events.
void ProcessMessageQueue(EventManager_t* manager) {
    while (1) {
        my_mutex_lock(&manager->queueMutex);
        if (manager->queueHead == NULL) {
            my_mutex_unlock(&manager->queueMutex);
            break;
        }

        // Das höchstpriorisierte Event aus der Queue entnehmen
        QueueItem* item = manager->queueHead;
        manager->queueHead = item->next;
        my_mutex_unlock(&manager->queueMutex);

        // Das Event an die passenden Listener senden (synchron via SendMessage)
        SendMessage(manager, &item->event);

        free(item);
    }
}

/* === Cleanup-Funktion für den Event Manager === */

void DestroyEventManager(EventManager_t* manager) {
    // Listenerliste freigeben
    my_mutex_lock(&manager->listenerMutex);
    while (manager->listeners != NULL) {
        Listener *temp = manager->listeners;
        manager->listeners = manager->listeners->next;
        free(temp);
    }
    my_mutex_unlock(&manager->listenerMutex);

    // Message Queue leeren
    my_mutex_lock(&manager->queueMutex);
    while (manager->queueHead != NULL) {
        QueueItem* temp = manager->queueHead;
        manager->queueHead = manager->queueHead->next;
        free(temp);
    }
    my_mutex_unlock(&manager->queueMutex);

    // Unsere Mutexes zerstören (bei unserem Spinlock ist dies unkritisch)
    my_mutex_destroy(&manager->listenerMutex);
    my_mutex_destroy(&manager->queueMutex);
}
