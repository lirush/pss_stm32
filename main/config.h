#ifndef CONFIG_H
#define CONFIG_H

#define control_command 0x04CB8461
#define telemetry_data  0x10CB8516
#define state_keys		  0x10CB8616
#define sep_data_1      0x10CBD711
#define sep_data_2      0x10CBD712

#define TSCH						(20)												//период TIM2, работающего на шедулер
#define QUANTITY_TASKS  (5)
#define TSYS            (TSCH*QUANTITY_TASKS)

#define QCANDEV					(17)							//it is quantity devices in can bus

#define ID_PMV           (0x04CB8461L)
#define ID_EBU           (0x0CF00400L)
#define ID_PKI1    			 (0x10CB491EL)
#define ID_PKI2          (0x10CB491CL)
#define ID_PKI3          (0x10CB491AL)
#define ID_PKI4          (0x10CB4918L)
#define ID_PKI5          (0x10CB4916L)
#define ID_PKI6          (0x10CB4919L)
#define ID_BRI1PN1AN1    (0x10CB5213L)                  //аналоговые сигналы от устройств БРИ
#define ID_BRI1PN1AN2    (0x10CB5313L)
#define ID_BRI1PN2AN1    (0x10CB5513L)
#define ID_BRI1PN2AN2    (0x10CB5613L)
#define ID_BRI2PN1AN1    (0x10CB5C14L)
#define ID_BRI2PN1AN2    (0x10CB5D14L)
#define ID_BRI2PN2AN1    (0x10CB5F14L)
#define ID_BRI2PN2AN2    (0x10CB6014L)
#define ID_PMV_PKI       (0x04CB8761L)
#define ID_PMV_PKI_RES   (0x04CB8716L)

#define PMV								(0)
#define EBU								(1)
#define PKI1n							(2)
#define PKI2n							(3)
#define PKI3n							(4)
#define PKI4n							(5)
#define PKI5n							(6)
#define PKI6n							(7)
#define BRI1PN1AN1		    (8)
#define BRI1PN1AN2		    (9)
#define BRI1PN2AN1		    (10)
#define BRI1PN2AN2		    (11)
#define BRI2PN1AN1		    (12)
#define BRI2PN1AN2		    (13)
#define BRI2PN2AN1		    (14)
#define BRI2PN2AN2		    (15)
#define PMV_PKI       		(16)

#endif  //CONFIG_H
