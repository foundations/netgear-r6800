#include <linux/init.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/config.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/pci.h>
#include <linux/sched.h>
#include <asm/uaccess.h>

#include "ralink_gdma.h"
#include "hs_dma.h"
extern struct HSdmaReqEntry HSDMA_Entry;
#define PATTERN_LEN     2000
#define TIME_TEST      1
unsigned char *Src[256];
unsigned char *Dst[256];
unsigned char *SrcMem[256];
unsigned char *DstMem[256];
dma_addr_t Src_phy[256];
dma_addr_t Dst_phy[256];


unsigned char DoneBit[256];
unsigned int Success=0;
unsigned int Fail=0;
spinlock_t	mem_page_lock; 
void check_result(unsigned char *src, unsigned char *dst, uint32_t len)
{
	int i;
	//printk("Src=%p Dst=%p Len=%x\n", src, dst, len);
	if(memcmp(src, dst, len)==0){
		//printk("check .. ok\n");
		Success++;
	}else{
		printk("check .. fail\n");
		for (i=0;i<len;i++){
			printk("Src=%x,Dst=%x\n", Src[0][i],Dst[0][i]);
		}
		Fail++;
	}
	memset(dst, 0, PATTERN_LEN);
}


int Time_test(void *unused) 
{
	unsigned long time1, time2, temp;
	unsigned long final =0;
	unsigned long final1=0;
	unsigned long final2=0;
	unsigned long final3=0;
	unsigned long final4=0;
	unsigned long flags;
	int i,j;
	unsigned long updateCRX =0;
	printk("HZ = %d\n", HZ);
	spin_lock_irqsave(&mem_page_lock, flags);
	//===================================================================
	for (j=0;j<10000;j++){
		time1 = read_c0_count();
		HS_DmaMem2Mem(Src_phy[0], Dst_phy[0], PATTERN_LEN, NULL);
		time2 = read_c0_count();
		check_result(&Src[0][0], &Dst[0][0], PATTERN_LEN);
		for (i=0;i<NUM_HSDMA_RX_DESC;i++){
			if (HSDMA_Entry.HSDMA_rx_ring0[i].hsdma_rxd_info2.DDONE_bit == 1) { 
				 updateCRX = i; 
				HSDMA_Entry.HSDMA_rx_ring0[i].hsdma_rxd_info2.DDONE_bit = 0; // RX_Done_bit=1->0
			}	
		}
		sysRegWrite(HSDMA_RX_CALC_IDX0, cpu_to_le32((u32)updateCRX)); //update RX CPU IDX
		sysRegWrite(HSDMA_INT_STATUS, HSDMA_FE_INT_ALL);  //Write one clear INT_status
		temp = time2-time1;
		final=final+temp;
	}	
	printk("HS_DMA spend = %lu /440000 ms\n", final);
  //==================================================================
	for (j=0;j<10000;j++){
		time1 = read_c0_count();
		GdmaMem2Mem(Src_phy[0], Dst_phy[0], PATTERN_LEN, NULL);
		time2 = read_c0_count();
		temp = time2-time1;
		final3 = final3 + temp;
	}
	printk("GDMA spend = %lu /25000 ms\n", final3);
	check_result(&Src[0][0], &Dst[0][0], PATTERN_LEN);
//=================================================================
	for (j=0;j<10000;j++){
		dma_cache_sync(NULL,&SrcMem[0][0] , PATTERN_LEN, DMA_FROM_DEVICE);
		time1 = read_c0_count();
	 	memcpy(&DstMem[0][0],&SrcMem[0][0],PATTERN_LEN);
	 	//dma_cache_sync(NULL,&DstMem[0][0] , PATTERN_LEN, DMA_TO_DEVICE);
	 	time2 = read_c0_count();
	 	check_result(&SrcMem[0][0], &DstMem[0][0], PATTERN_LEN);
	 	temp = time2 - time1;
		final1= final1 + temp;
	}
	printk("memcpy spend (cache->memory)= %lu /440000 ms\n", final1);
//================================================================= 
	for (j=0;j<10000;j++){ 
		time1 = read_c0_count();
		memcpy(&DstMem[0][0],&SrcMem[0][0],PATTERN_LEN);
		time2 = read_c0_count();
		temp = time2 - time1;
		final4= final4 + temp;
	}
	printk("memcpy spend cache= %lu /440000 ms\n", final4);
	check_result(&SrcMem[0][0], &DstMem[0][0], PATTERN_LEN); 
//===================================================================
	 for (j=0;j<10000;j++){ 
		time1 = read_c0_count();
	 	memcpy(&Dst[0][0],&Src[0][0],PATTERN_LEN);
		time2 = read_c0_count();
		temp = time2 - time1;
	final2= final2 + temp;
	}
	printk("memcpy spend noncache= %lu /440000 ms\n", final2);
	check_result(&Src[0][0], &Dst[0][0], PATTERN_LEN);
	spin_unlock_irqrestore(&mem_page_lock, flags);
	return 0;
}


static int RalinkGdmaTestInit(void)
{
	int i=0,j=0;
	printk("Enable MeM Test Module\n");		
	//alloc phy memory for src
	for(i=0;i< MAX_GDMA_CHANNEL; i++) {
		Src[i] = pci_alloc_consistent(NULL, PATTERN_LEN, &Src_phy[i]);
		if(Src[i]==NULL) {
			printk("pci_alloc_consistent fail: Src[%d]==NULL\n", i);
			return 1;
		}
	}
	SrcMem[0]=kmalloc(PATTERN_LEN, GFP_KERNEL);
	//fill out content
	for(i=0;i< MAX_GDMA_CHANNEL; i++) {
		for(j=0;j< PATTERN_LEN;j++) {
			Src[i][j]=i+j;
		}
	}
	for(j=0;j< PATTERN_LEN;j++) {
		SrcMem[0][j]=j;
	}
	//alloc phy memory for dst
	for(i=0;i< MAX_GDMA_CHANNEL; i++) {
		Dst[i] = pci_alloc_consistent(NULL, PATTERN_LEN, &Dst_phy[i]);
		if(Dst[i]==NULL) {
			printk("pci_alloc_consistent fail: Dst[%d]==NULL\n", i);
			return 1;
		}
	}
	DstMem[0]=kmalloc(PATTERN_LEN, GFP_KERNEL);
	spin_lock_init(&mem_page_lock);
	printk("Src_phy[0]=%x\n", Src_phy[0]);
	printk("Dst_phy[0]=%x\n", Dst_phy[0]);
#if defined (TIME_TEST)
	printk("Mem2Mem Performance Test \n");
	kernel_thread(Time_test, NULL, CLONE_VM);
#endif
	
	return 0;
}

static void __exit RalinkGdmaTestExit(void)
{
	int i=0;
	printk("Disable Ralink GDMA Test Module\n");
	for(i=0;i<MAX_GDMA_CHANNEL;i++) {
	    pci_free_consistent(NULL, PATTERN_LEN, Src[i], Src_phy[i]);
	    pci_free_consistent(NULL, PATTERN_LEN, Dst[i], Dst_phy[i]);
	}

}


module_init(RalinkGdmaTestInit);
module_exit(RalinkGdmaTestExit);

MODULE_DESCRIPTION("Ralink SoC DMA Test Module");
MODULE_AUTHOR("Harry Huang");
MODULE_LICENSE("GPL");
