

#include <linux/init.h>
#include <linux/version.h>
#include <linux/module.h>
#if defined (CONFIG_MIPS)
	#include <linux/config.h>
#endif
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/pci.h>
#include <linux/sched.h>
#include <asm/uaccess.h>
#include  <linux/proc_fs.h>
#include <asm/rt2880/rt_mmap.h>
#include <linux/kthread.h>
#include "hs_dma.h"
extern struct HSdmaReqEntry HSDMA_Entry;

#define PATTERN_LEN  3000
#define PATTERN_BOUNDARY 16384 //for unalignment memcpy test
#define SHOW_INTERVAL	 100


#define ALL_LENGTH      1
//#define GDMA_FULL_TEST    1
//#define   TIMER_MODE  1


int len=8;
int mode=3;//mode=1:fix lenght  mode=2 interrupt mode, mode=3 polling mode all combination test, mode=4 scatter_gather test
int dot=0;
int run=0;
module_param(len, int, S_IRUGO);
module_param(mode, int, S_IRUGO);
module_param(dot, int, S_IRUGO);
module_param(run, int, S_IRUGO);
unsigned char *Src[16];
unsigned char *Dst[16];

dma_addr_t Src_phy[16];
dma_addr_t Dst_phy[16];


unsigned int Success=0;
unsigned int Fail=0;
unsigned int HSDMASuccess=0;
unsigned int BurstSize4DWsSuccess=0;
unsigned int BurstSize4DWsFail=0;
unsigned int BurstSize8DWsSuccess=0;
unsigned int BurstSize8DWsFail=0;
unsigned int BurstSize16DWsSuccess=0;
unsigned int BurstSize16DWsFail=0;
unsigned int BurstSize32DWsSuccess=0;
unsigned int BurstSize32DWsFail=0;
void central_periodic_timer_handler(unsigned long unused);
static DEFINE_TIMER(central_periodic_timer, central_periodic_timer_handler, 0, 0);

int check_result(unsigned char *src, unsigned char *dst, uint32_t length)
{
	
	int i;
	if(memcmp(src, dst, length)==0){
		//printk("check .. ok\n");
if (dot==1){		
		printk(".");
}
		Success++;
		if((Success % SHOW_INTERVAL)==0) {
			printk("HSDMA is still alive\n");
		}

	}else{
		printk("len=%d\n",length);
		printk("Success=%d\n",Success);
		printk("HSDMASuccess=%d\n",HSDMASuccess);
		Fail++;
		for(i=0;i<length;i++) {
			if(Dst[0][i]!=Src[0][i]) {
				printk("***index=%d**: ",i);
				printk("(Dst=%x Src=%x)\n", Dst[0][i], Src[0][i]);
			
			}
		}
		for(i=0;i<length;i++) {
			
				printk("(Dst=%x Src=%x)\n", Dst[0][i], Src[0][i]);
			
		}
#if	defined (TIMER_MODE)
		del_timer(&central_periodic_timer);
#endif
		return 1;
	}


	memset(dst, 0, length);
	return 0;
}


int Done(uint32_t transcount, uint32_t cpuIdx)
{
	int i=0;
	//udelay(10);
  //printk("transcount=%d, cpuIdx = %d\n", transcount, cpuIdx);
	if(memcmp(Src[cpuIdx], Dst[cpuIdx], transcount)==0){
		Success++;
		if (dot==1){		
			printk(".");
			printk("Success=%d\n", Success);
		}
   
		
		if((Success % SHOW_INTERVAL)==0) {
			printk("HSDMA is still alive\n");
		}
		
		if(mode==2 && run==0)
   	 HS_DmaMem2MemGenUse(Src_phy[(cpuIdx+1)%16], Dst_phy[(cpuIdx+1)%16], len, Done);
   	 
	}else{
		printk("check .. fail, transcount=%d\n", transcount);
		Fail++;
		for(i=0;i<transcount;i++) {
			if(Dst[cpuIdx][i]!=Src[cpuIdx][i]) {
				printk("***index=%d**: ",i);
				printk("(cpuIdx = %d Dst_address=%x Src_adress=%x)\n", cpuIdx, Dst[cpuIdx], Src[cpuIdx]);
				printk("(Dst=%x Src=%x)\n", Dst[cpuIdx][i], Src[cpuIdx][i]);
				return 1;
			}else {
        printk("HW update content so late\n");
    		Success++;
    	}
		}	

#if	defined (TIMER_MODE)
		del_timer(&central_periodic_timer);
#endif
		
	}
	//clear content of destination address
	memset(Dst[cpuIdx], 0, transcount);
	return 0;
}
void central_periodic_timer_handler(unsigned long unused)
{

	HS_DmaMem2MemGenUse(Src_phy[0], Dst_phy[0], PATTERN_LEN, NULL);
	check_result(&Src[0][0], &Dst[0][0], PATTERN_LEN);
	central_periodic_timer.expires = jiffies + HZ/10;
	add_timer(&central_periodic_timer);
}
int TimerMode(void *unused) 
{
	central_periodic_timer.expires = jiffies + HZ;
	add_timer(&central_periodic_timer);
	return 0;
}

int AllLenTest(void *unused) 
{
	int i=0;
	int checkReuslt;	
	int times=0;
	int j=0;
	int k=0;
	int HSDMA_glo_cfg=0;
	if (mode==0){	
		printk("All lenth test\n");
		while (1){
			for(i=8;i<16384;i++){
				for (times=0; times < (NUM_HSDMA_TX_DESC); times++ ){
					if(HS_DmaMem2Mem(Src_phy[times], Dst_phy[times], i, NULL)>=0) {
						checkReuslt = check_result(&Src[times][0], &Dst[times][0], i);
						if (checkReuslt==1) return 1;
					}	
					else {
						printk("len=%d\n",i);
						printk("****************stop test***************************\n");
						printk("HSDMASuccess=%d\n",HSDMASuccess);
						return 1;
					}
				}
			}
			HSDMASuccess++;
		printk("HSDMASuccess=%d\n",HSDMASuccess);
		}
	}	
	else if(mode==1){    
		printk("Fix Length mode \n");
		for (times=0; times < (run+1); times++ ){
			if (run==0){
				while (1){
					if(HS_DmaMem2MemGenUse(Src_phy[0], Dst_phy[0], len, NULL)>=0) {
						checkReuslt = check_result(&Src[0][0], &Dst[0][0], len);
						if (checkReuslt==1) return 1;
					} else {
			  	 		printk("stop test\n");
			  	 		printk("Success=%d\n",Success);  
			  	 		return 1;
			  	 	} 
				  
				}
			}else{
				if(HS_DmaMem2MemGenUse(Src_phy[0]+3, Dst_phy[0], len, NULL)>=0) {
					checkReuslt = check_result(&Src[0][3], &Dst[0][0], len);
					if (checkReuslt==1) return 1;
			  	} else {
					printk("stop test\n");
			  	 	printk("Success=%d\n",Success);  
			  	 	return 1;
			  	} 
	 		 }
	  
		}		      
	}
	else if( mode==2){
		printk("Interrupt Mode\n");
if (run==0){

		 HS_DmaMem2MemGenUse(Src_phy[0], Dst_phy[0], len, Done);
		  
}else{

    for (i=0; i<run; i++){
    	//printk("i=%d, run=%d\n", i, run);
		for (times=0; times < ADDRESS_VLAUE; times++ ){
			//printk("times=%d, Src_phy[times]=%x\n", times, Src_phy[times]);
	  	HS_DmaMem2MemGenUse(Src_phy[times], Dst_phy[times], len, Done);
		}
	
	
}
}
	}
	else if(mode==3){
		printk("All Combination Test\n");
		while (1){
			for (j=0; j<4; j++){
				if(j==0){
					printk("burst size=%d\n",4);
				}else{
					printk("burst size=%d\n",j*8);
				}
				HSDMA_glo_cfg = (HSDMA_TX_WB_DDONE | HSDMA_RX_DMA_EN | HSDMA_MUTI_ISSUE | HSDMA_TX_DMA_EN | ( j << 4 ));   //burst size= 4DWs~32DWs
				sysRegWrite(HSDMA_GLO_CFG, HSDMA_glo_cfg);
				printk("Golobal Comfig =%x\n", sysRegRead(HSDMA_GLO_CFG));
					for(k=0; k<4; k++){
						printk("Src address shift=%d\n", k);
						for(i=8;i<16384;i++){
						 //printk("i=%d\n",i);
							if(HS_DmaMem2MemGenUse(Src_phy[0]+k, Dst_phy[0], i, NULL)>=0) {
								checkReuslt = check_result(&Src[0][k], &Dst[0][0], i);
								if (j==0) BurstSize4DWsSuccess++;
								if (j==1) BurstSize8DWsSuccess++;
								if (j==2) BurstSize16DWsSuccess++;
								if (j==3) BurstSize32DWsSuccess++;
								if (checkReuslt==1) return 1;
							} else {
								printk("stop test no interrupt\n");
							}
						}
	
					}
				printk("BurstSize4DWsSuccess: %d\n",BurstSize4DWsSuccess);
				printk("BurstSize8DWsSuccess: %d\n",BurstSize8DWsSuccess);
				printk("BurstSize16DWsSuccess: %d\n",BurstSize16DWsSuccess);
				printk("BurstSize32DWsSuccess: %d\n",BurstSize32DWsSuccess);
			}
		 }

	}
	else if(mode==4){
		printk("len=15000, len=30000\n");
		while (1){
			if(HS_DmaMem2MemGenUse(Src_phy[0], Dst_phy[0], 15000, NULL)>=0) {
				checkReuslt = check_result(&Src[0][0], &Dst[0][0], 15000);
				if (checkReuslt==1) return 1;
			} else {
				printk("stop test\n");
				printk("Success=%d\n",Success);  
				return 1;
			}
				  	 	 
			if(HS_DmaMem2MemGenUse(Src_phy[0], Dst_phy[0], 30000, NULL)>=0) {
				checkReuslt = check_result(&Src[0][0], &Dst[0][0], 30000);
				if (checkReuslt==1) return 1;
			} else {
				printk("stop test\n");
				printk("Success=%d\n",Success);  
				return 1;
			} 
		}
	}	

	return 0;
}         


static int RalinkGdmaTestInit(void)
{
	int i=0,j=0;

	printk("Enable Ralink HSDMA Test Module\n");		
	//alloc phy memory for src
	for(i=0;i< ADDRESS_VLAUE; i++) {
		Src[i] = pci_alloc_consistent(NULL, PATTERN_BOUNDARY, &Src_phy[i]);
	  if(Src[i]==NULL) {
			printk("pci_alloc_consistent fail: Src[%d]==NULL\n", i);
			return 1;
	   }
	  }
	

	//fill out content
	for(i=0;i< ADDRESS_VLAUE; i++) {
	    for(j=0;j< PATTERN_BOUNDARY;j++) {
				Src[i][j]=i+j;
	    }
	}

	for(i=0;i< ADDRESS_VLAUE; i++) {
	//alloc phy memory for dst
	  Dst[i] = pci_alloc_consistent(NULL, PATTERN_BOUNDARY, &Dst_phy[i]);
		if(Dst[i]==NULL) {
			printk("pci_alloc_consistent fail: Dst[%d]==NULL\n", i);
			return 1;
	  }
	}


#if defined (ALL_LENGTH )
	printk("test transcount = 8bytes ~ 16383bytes  \n");
	//kernel_thread(AllLenTest, NULL, CLONE_VM);
	kthread_run(AllLenTest, NULL, "AllLenTest");
#elif defined (TIMER_MODE)
	printk("timer_mode  \n");
	//kernel_thread(TimerMode, NULL, CLONE_VM);
		kthread_run(TimerMode, NULL, "TimerMode");
#endif

	
	return 0;
}

static void __exit RalinkGdmaTestExit(void)
{

	int i=0;
	printk("Disable Ralink HSDMA Test Module\n");

	for(i=0;i<ADDRESS_VLAUE;i++) {
	    pci_free_consistent(NULL, PATTERN_BOUNDARY, Src[i], Src_phy[i]);
	    pci_free_consistent(NULL, PATTERN_BOUNDARY, Dst[i], Dst_phy[i]);
	}


	
#if defined (TIMER_MODE)
	del_timer(&central_periodic_timer);
#endif
}


module_init(RalinkGdmaTestInit);
module_exit(RalinkGdmaTestExit);

MODULE_DESCRIPTION("Ralink SoC DMA Test Module");
MODULE_AUTHOR("Harry");
MODULE_LICENSE("GPL");
