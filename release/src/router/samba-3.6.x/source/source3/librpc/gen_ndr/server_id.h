/* header auto-generated by pidl */

#ifndef _PIDL_HEADER_server_id
#define _PIDL_HEADER_server_id

#include <stdint.h>

#include "libcli/util/ntstatus.h"

#ifndef _HEADER_server_id
#define _HEADER_server_id

struct server_id {
	uint32_t pid;
	uint32_t vnn;
	uint64_t unique_id;
}/* [public] */;

#endif /* _HEADER_server_id */
#endif /* _PIDL_HEADER_server_id */