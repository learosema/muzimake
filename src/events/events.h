#ifndef __EVENTS_H__
#define __EVENTS_H__

#include "my_mutex.h"

/* === Datenstrukturen für das Eventsystem === */

// Erweiterte Event-Struktur um Priorität (niedrigere Zahl = höhere Priorität)
typedef struct EventParam {
	unsigned int eventId;  // Eindeutige Kennung des Events
	void* wParam;          // Zusatzinformation 1
	void* lParam;          // Zusatzinformation 2
	int priority;          // Priorität: 1 = höchste, höhere Werte = niedrigere Priorität
} EventParam_t;

// Der Event Manager verwaltet alle Listener und die prioritätsbasierte Message Queue.
// Statt pthread-Mutexes werden hier unsere eigenen Mutexes verwendet.
typedef struct Listener Listener_t;
typedef struct QueueItem QueueItem_t;
typedef struct EventManager {
	Listener_t* listeners;
	QueueItem_t* queueHead;  // Kopf der Queue (höchste Priorität zuerst)
	my_mutex_t listenerMutex;
	my_mutex_t queueMutex;
} EventManager_t;

// Typdefinition eines Event-Handlers (vergleichbar mit WndProc)
typedef int (*EventHandler_t)(const EventParam_t* const evt); // const ... = data r/o, ... const = ptr r/o

/* === Initialisierung und Listener-Verwaltung === */
void InitEventManager(EventManager_t *manager);

void RegisterListener(EventManager_t *manager, EventHandler_t handler, unsigned int filter);

void UnregisterListener(EventManager_t *manager, EventHandler_t handler);

/* === Nachrichtensenden === */
int SendMessage(EventManager_t *manager, const EventParam_t *event);

void PostMessage(EventManager *manager, const EventParam_t *event);

/* === Verarbeitung der Message Queue (ohne Endlosschleife) === */
void ProcessMessageQueue(EventManager_t *manager);

/* === Cleanup-Funktion für den Event Manager === */
void DestroyEventManager(EventManager_t *manager);

#endif
