#include "fms.h"

signed char FMSFunc(int elementsCount, Transition *transitionTable, State currentState, Event eventToFire)
{
    while (elementsCount--) {
        Transition transition = transitionTable[elementsCount];
        if (transition.step == currentState && transition.event == eventToFire) {
            return transition.EventFunc();
            //break; // Target EventFunc founded and fired!
        }
    }
    return 0;    
}
