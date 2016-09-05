#include "msgpack.h"


#define MSGPCK_NIL      0xc0
#define MSGPCK_TRUE     0xc2
#define MSGPCK_FALSE    0xc3


#define MSGPCK_UINT8    0xcc
#define MSGPCK_UINT16   0xcd
#define MSGPCK_UINT32   0xce

#define MSGPCK_INT8     0xd0
#define MSGPCK_INT16    0xd1
#define MSGPCK_INT32    0xd2

#define MSGPCK_FLOAT32  0xca
#define MSGPCK_FLOAT64  0xcb

#define MSGPCK_STRING   0xda
#define MSGPCK_BINARY   0xc5

#define MSGPCK_ARRAY    0xdc
#define MSGPCK_MAP      0xde


/*
void msgpack_write(Stream *ps, uint8_t type, ...) {
    va_list args;
    va_start (args, type );

    switch (type) {
        case MSGPCK_NIL:
        case MSGPCK_TRUE:
        case MSGPCK_FALSE:
            stream_write(ps, &type, 1);
            break;
        case MSGPCK_UINT8:
        case MSGPCK_UINT16:
        case MSGPCK_UINT32: {
                uint32_t tmp = va_arg(args, uint32_t);
                tmp = BSWAP32(tmp);
                type = MSGPCK_UINT32;
                stream_write(ps, &type, 1);
                stream_write(ps, &tmp, sizeof(uint32_t));
            }
            break;
        case MSGPCK_INT8:
        case MSGPCK_INT16:
        case MSGPCK_INT32: {
                int32_t tmp = va_arg(args, int32_t);
                type = MSGPCK_INT32;
                tmp = BSWAP32(tmp);
                stream_write(ps, &type, 1);
                stream_write(ps, &tmp, sizeof(int32_t));
            }
            break;
        case MSGPCK_FLOAT32:
        case MSGPCK_FLOAT64: {
                double tmpd = (va_arg(args, double));                
                uint8_t* tmp = &tmpd;
                type = MSGPCK_FLOAT64;
                //tmp = BSWAP64((uint64_t)tmp);
                stream_write(ps, &type, 1);
                //stream_write(ps, (uint8_t*)&tmp, sizeof(double));
                for(type = 8;type>0;type--)
                    stream_write(ps,tmp+type-1,1);
            }
            break;
        case MSGPCK_STRING:
        case MSGPCK_BINARY: {
                uint8_t *tmp = va_arg(args, uint8_t *);
                uint16_t len =  va_arg(args, uint32_t);
                len = BSWAP16(len);
                stream_write(ps, &type, 1);
                stream_write(ps, &len, sizeof(uint16_t));
                len = BSWAP16(len);
                stream_write(ps, tmp, len);
            }
            break;
        case MSGPCK_MAP:
        case MSGPCK_ARRAY: {
                uint16_t len =  va_arg(args, uint32_t);
                len = BSWAP16(len);
                stream_write(ps, &type, 1);
                stream_write(ps, &len, sizeof(uint16_t));
            }
            break;
        default:
            break;
    }

    va_end (args);
}
*/