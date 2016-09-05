#ifndef __PLATFORM_CONFIG__
#define __PLATFORM_CONFIG__


#define PORT_PROVIDE_RANDOM TRUE
#define UID_BYTES   16

/*====== DRIVER MAPPING ================================================== */


#define EXT_SLOTS EXT_INTERRUPTS_NEEDED


/*====== PORT HOOKS ================================================ */

#define PORT_BLINK_FN() \
void _port_blink(int vpin,int n,int ms){			\
	int i;											\
	vhalPinSetMode(vpin,PINMODE_OUTPUT_PUSHPULL);	\
	for (i=0;i<n;i++){								\
		vhalPinWrite(vpin,1);						\
		vosThSleep(TIME_U(ms/2,MILLIS));			\
		vhalPinWrite(vpin,0);						\
		vosThSleep(TIME_U(ms/2,MILLIS));			\
	}												\
}

#define PORT_PRE_UPLOAD_HOOK() 
#define PORT_AFTER_UPLOAD_HOOK() vosThSleep(TIME_U(500,MILLIS))
#define PORT_LED_CONFIG() vhalPinSetMode(LED0,PINMODE_OUTPUT_PUSHPULL);
#define PORT_LED_ON() vhalPinWrite(LED0,1)
#define PORT_LED_OFF() vhalPinWrite(LED0,0)
#define PORT_BLINK_ONE()				_port_blink(LED0,1,10);
#define PORT_BLINK_PRE_UPLOAD()			_port_blink(LED0,1,100);
#define PORT_BLINK_PRE_UPLOAD_OK()		_port_blink(LED0,2,500);
#define PORT_BLINK_AFTER_UPLOAD_OK()	_port_blink(LED0,2,500);
#define PORT_BLINK_AFTER_UPLOAD_KO()	_port_blink(LED0,2,500);


#endif
