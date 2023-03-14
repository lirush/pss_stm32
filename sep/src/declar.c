#include "declar.h"

void InitSep(void)
{
    Sep.Valve[FIRST].Open = &ValvesOpen;
    Sep.Valve[FIRST].Close = &ValvesClose;
    Sep.Valve[FIRST].KeyOpenState = &VsKeyOpenState;
    Sep.Valve[FIRST].KeyCloseState = &VsKeyCloseState;
    Sep.Valve[SECOND].Open = &Valve2Open;
    Sep.Valve[SECOND].Close = &Valve2Close;
    Sep.Valve[SECOND].KeyOpenState = &V2KeyOpenState;
    Sep.Valve[SECOND].KeyCloseState = &V2KeyCloseState;
    Sep.Disel.Starter.On = &StarterON;
    Sep.Disel.Starter.Off = &StarterOFF;
    Sep.Disel.Starter.KeyState = &StarterKeyONState;
    Sep.Disel.Starter.Algorithm = &AlgStarter;
    Sep.Disel.Svechi.On = &SvechiON;
    Sep.Disel.Svechi.Off = &SvechiOFF;
    Sep.Disel.Ventilator.On = &VentOn;
    Sep.Disel.Ventilator.Off = &VentOff;
    Sep.Disel.Podogrev.On = &PodogrevON;
    Sep.Disel.Podogrev.Off = &PodogrevOFF;
    Sep.Disel.Podogrev.Algorithm = &AlgPodogrev;
    Sep.Disel.Engine.start_hh = &Engine_start;
    Sep.Disel.Engine.stop_hh = &Engine_hh_stop;
    Sep.Disel.Engine.start_1800 = &Engine_1800_start;
    Sep.Disel.Engine.stop_1800 = &Engine_1800_stop;
    Sep.Disel.AutoStart = &AutoStart;
    Sep.Disel.AutoStop = &AutoStop;
    Sep.ControlDisel = &StartStopDiselSep;
    InitLevelValve();
}
