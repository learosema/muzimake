#include "ielement.hpp"
#include "eventlib.hpp"

class Button : IElement, IEventListener {
    
    void render() override;
    void onEvent(const int eventSource, const int eventId, const void *payload) override;


private:
};