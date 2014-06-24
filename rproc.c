#include <stdint.h>
#include <stddef.h>

/* ---------------------------------------------------------
   These definitions come from <linux/remoteproc.h> and
   may change as the remote processor interface is updated.
   Make sure they match the ones used by your current kernel
   source.
   ---------------------------------------------------------
*/
typedef uint32_t u32;
typedef uint64_t u64;
typedef uint8_t u8;

#define __packed __attribute__ ((packed))

struct resource_table {
  u32 ver;
  u32 num;
  u32 reserved[2];
  u32 offset[0];
} __packed;

struct fw_rsc_hdr {
  u32 type;
  u8 data[0];
} __packed;

struct fw_rsc_carveout {
  u32 da;
  u32 pa;
  u32 len;
  u32 flags;
  u32 reserved;
  u8 name[32];
} __packed;

struct fw_rsc_devmem {
  u32 da;
  u32 pa;
  u32 len;
  u32 flags;
  u32 reserved;
  u8 name[32];
} __packed;

struct fw_rsc_trace {
	u32 da;
	u32 len;
	u32 reserved;
	u8 name[32];
} __packed;

struct fw_rsc_vdev_vring {
	u32 da; /* device address */
	u32 align;
	u32 num;
	u32 notifyid;
	u32 reserved;
};

struct fw_rsc_vdev {
	u32 type;
	u32 id;
	u32 notifyid;
	u32 dfeatures;
	u32 gfeatures;
	u32 config_len;
	char status;
	char num_of_vrings;
	char reserved[2];
};

enum fw_resource_type {
	RSC_CARVEOUT	= 0,
	RSC_DEVMEM	= 1,
	RSC_TRACE	= 2,
	RSC_VDEV	= 3,
	RSC_LAST	= 4,
};

/* --------------------------------------------------------- */

/* Describe the resources required by this firmware image.  Currently
   we simply map 1M of pyhsical memory from PA 0xb0000000 to the device
   address 0x0 to hold our text/data sections.  We also map a 1M block
   containing the GPIO1 perihperal registers (PA 0x4a300000) to a device
   address 0xfff00000 so we can flash a LED. */

struct resources {
  struct resource_table resource_table;
  u32 offset[8];
  struct fw_rsc_hdr carve_out;
  struct fw_rsc_carveout carve_out_data;
  struct fw_rsc_hdr devmem;
  struct fw_rsc_devmem devmem_data;
  struct fw_rsc_hdr trace;
  struct fw_rsc_trace trace_data;
  struct fw_rsc_hdr uart;
  struct fw_rsc_devmem uart_data;
  struct fw_rsc_hdr l4;
  struct fw_rsc_devmem l4_data;
  struct fw_rsc_hdr ipcmem;
  struct fw_rsc_devmem ipcmem_data;
  struct fw_rsc_vdev console_vdev;
  struct fw_rsc_vdev_vring console_vring0;
  struct fw_rsc_vdev_vring console_vring1;
  struct fw_rsc_vdev rng_vdev;
  struct fw_rsc_vdev_vring rng_vring;
} __packed;

#define VIRTIO_ID_CONSOLE	3 /* virtio console */
#define VIRTIO_ID_RNG	4 /* virtio console */

extern char __log_buf[__LOG_BUF_LEN];

__attribute__ ((section(".resource_table")))
struct resources resources = {
  .resource_table = {1, 8, {0}},
  .offset = {offsetof(struct resources, carve_out),
	     offsetof(struct resources, devmem),
	     offsetof(struct resources, trace),
	     offsetof(struct resources, uart),
	     offsetof(struct resources, l4),
	     offsetof(struct resources, ipcmem),
	     offsetof(struct resources, console_vdev),
	     offsetof(struct resources, rng_vdev),
 	     },
  .carve_out = {RSC_CARVEOUT},
  .carve_out_data = {0, 0xb0000000, 0x800000, 0x0, 0, "text"},
  .devmem = {RSC_DEVMEM},
  .devmem_data = {0xfff00000, 0x4a300000, 0x20000, 0x0, 0, "gpio1"},
  .trace = {RSC_TRACE},
  .trace_data = {
	  // .da = 0x700000,
	  .da = (unsigned int) __log_buf,
	  .len = sizeof(__log_buf),
	  .name = "logbuf",
  },
  .uart = {RSC_DEVMEM},
  .uart_data = {0xfff20000, 0x48020000, 0x1000, 0x0, 0, "uart3"},
  .l4 = {RSC_DEVMEM},
  .l4_data = {0xaa000000, 0x4a000000, 16 * 1024 * 1024, 0x0, 0, "l4"},
  .ipcmem = {RSC_DEVMEM},
  .ipcmem_data = {0xa0000000, 0x99000000, 1 * 1024 * 1024, 0x0, 0, "ipc"},
   /* console vdev entry */
   {
   	RSC_VDEV, VIRTIO_ID_CONSOLE, 3,
   	0, 0, 0, 0, 2, { 0, 0 },
   	/* no config data */
   },
   /* the two vrings */
   { 0xa0000000, 4096, 4, 4, 0 },
   { 0xa0004000, 4096, 4, 5, 0 },
   /* rng vdev entry */
   {
   	RSC_VDEV, VIRTIO_ID_RNG + 100 /* HACK OUT RNG */, 6,
   	0, 0, 0, 0, 1, { 0, 0 },
   	/* no config data */
   },
   /* the two vrings */
   { 0xa0008000, 4096, 4, 7, 0 },
};
