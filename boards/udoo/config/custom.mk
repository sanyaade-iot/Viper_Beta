#List of external drivers
BOARD_EXT_DRIVERS =

#Macro to be passed to compiler via -D directive
BOARD_DEFINES:= VM_UPLOAD_INTERFACE=0 VM_IRQS=8 VM_PRE_UPLOAD_WAIT=100 VM_UPLOAD_TIMEOUT=10000 VM_STDOUT=0 VM_STDERR=1 VM_STDIN=0 VM_BOARD=\"udoo\" VM_ARCH=\"arm.cortex-m3\" VM_RAM=98304


BOARD_DEFINES+= VM_UPLOADED_BYTECODE

BOARD_DEFINES+= $(BOARD_DEFS)