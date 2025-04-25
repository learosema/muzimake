#ifndef __DPMIUTIL_H__
#define __DPMIUTIL_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct dos_block_s {
    uint16_t segment;   // Real mode segment for ES
    uint16_t selector;  // Protected mode selector for direct access
} dos_block_t;

typedef struct pm_address_s {
	uint16_t selector;
	uint32_t offset;
} pm_address_t;

typedef struct rm_address_s {
	uint16_t segment;
	uint16_t offset;
} rm_address_t;

typedef struct rm_registers_s {
	uint32_t edi;
	uint32_t esi;
	uint32_t ebp;
	uint32_t reserved_by_system;
	uint32_t ebx;
	uint32_t edx;
	uint32_t ecx;
	uint32_t eax;
	uint32_t flags;
	uint16_t es,ds,fs,gs,ip,cs,sp,ss;
} rm_registers_t;


/**
 * Allocates a block of DOS memory using DPMI (int 0x31, ax=0x0100).
 * @param size Size in bytes.
 * @return A dos_block_t with segment and selector; segment==0 on failure.
 */
dos_block_t dpmi_alloc_dos_block(uint32_t size);

/**
 * Frees DOS memory allocated via DPMI_AllocDOSMemory.
 * @param dp DosPtr returned from allocation.
 */
void dpmi_free_dos_block(dos_block_t dp);


int dpmi_lock_linear_region(uint32_t linear_addr, uint32_t size);
int dpmi_unlock_linear_region(uint32_t linear_addr, uint32_t size);

#ifdef __cplusplus
}
#endif

#endif // DPMIUTIL_H
