#List of external drivers
BOARD_EXT_DRIVERS =

#Macro to be passed to compiler via -D directive
BOARD_DEFINES:= VM_UPLOAD_INTERFACE=0 VM_IRQS=8 VM_PRE_UPLOAD_WAIT=2000 VM_UPLOAD_TIMEOUT=5000 VM_STDOUT=0 VM_STDERR=1 VM_STDIN=0 VM_BOARD=\"photon\" VM_ARCH=\"arm.cortex-m3\" VM_RAM=122280 VM_KICK_IWDG=1 VM_SERIAL_BAUD=115200


BOARD_DEFINES+= VM_UPLOADED_BYTECODE