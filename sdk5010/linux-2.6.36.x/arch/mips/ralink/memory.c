/**************************************************************************
 *
 *  BRIEF MODULE DESCRIPTION
 *     memory setup for Ralink RT2880 solution
 *
 *  Copyright 2007 Ralink Inc. (bruce_chang@ralinktech.com.tw)
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 *
 *  THIS  SOFTWARE  IS PROVIDED   ``AS  IS'' AND   ANY  EXPRESS OR IMPLIED
 *  WARRANTIES,   INCLUDING, BUT NOT  LIMITED  TO, THE IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN
 *  NO  EVENT  SHALL   THE AUTHOR  BE    LIABLE FOR ANY   DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 *  NOT LIMITED   TO, PROCUREMENT OF  SUBSTITUTE GOODS  OR SERVICES; LOSS OF
 *  USE, DATA,  OR PROFITS; OR  BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 *  ANY THEORY OF LIABILITY, WHETHER IN  CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 *  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  You should have received a copy of the  GNU General Public License along
 *  with this program; if not, write  to the Free Software Foundation, Inc.,
 *  675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *
 **************************************************************************
 * May 2007 Bruce Chang
 *
 * Initial Release
 *
 *
 *
 **************************************************************************
 */


#include <linux/init.h>
#include <linux/mm.h>
#include <linux/bootmem.h>
#include <linux/ioport.h>
#include <asm/bootinfo.h>
#include <asm/page.h>

#include <asm/mach-ralink/prom.h>
//#define DEBUG

enum surfboard_memtypes {
	surfboard_dontuse,
	surfboard_rom,
	surfboard_ram,
};

#ifdef DEBUG
static char *mtypes[3] = {
	"Dont use memory",
	"Used ROM memory",
	"Free RAM memory",
};
#endif

/* References to section boundaries */
extern char _end;



#define PFN_ALIGN(x)    (((unsigned long)(x) + (PAGE_SIZE - 1)) & PAGE_MASK)

#ifdef CONFIG_UBOOT_CMDLINE
__init int prom_get_ramsize(void)
{

        char *argptr;
	int  ramsize = 0;

        argptr = prom_getcmdline();

        if ((argptr = strstr(argptr, "ramsize=")) != NULL)
        {
                argptr += strlen("ramsize=");

                ramsize = simple_strtol(&argptr[0], NULL, 0);
        }

        return ramsize;
}
#endif

void __init prom_meminit()
{ 
	int ramsize = 0;

#ifdef CONFIG_UBOOT_CMDLINE
	ramsize = prom_get_ramsize();
#endif
	if(!ramsize) {
		ramsize = CONFIG_RALINK_RAM_SIZE;
	}

	if(ramsize >= 512) {
		add_memory_region(0x00000000, ramsize*1024*1024 + 64*1024*1024,  BOOT_MEM_RAM);
	} else {
		add_memory_region(0x00000000, ramsize*1024*1024,  BOOT_MEM_RAM);
	}
}

void __init prom_free_prom_memory(void)
{
        unsigned long addr;
        int i;

        for (i = 0; i < boot_mem_map.nr_map; i++) {
                if (boot_mem_map.map[i].type != BOOT_MEM_ROM_DATA)
                        continue;

                addr = boot_mem_map.map[i].addr;
                free_init_pages("prom memory",
                                addr, addr + boot_mem_map.map[i].size);
        }
}
#if 0
void __init
prom_free_prom_memory (void)
{
	int i;
	unsigned long freed = 0;
	unsigned long addr;

	for (i = 0; i < boot_mem_map.nr_map; i++) {
		if (boot_mem_map.map[i].type != BOOT_MEM_ROM_DATA)
			continue;

		addr = boot_mem_map.map[i].addr;
		while (addr < boot_mem_map.map[i].addr
			      + boot_mem_map.map[i].size) {
			ClearPageReserved(virt_to_page(__va(addr)));
			set_page_count(virt_to_page(__va(addr)), 1);
			free_page((unsigned long)__va(addr));
			addr += PAGE_SIZE;
			freed += PAGE_SIZE;
		}
	}
	printk("Freeing prom memory: %ldkb freed\n", freed >> 10);
}
#endif
