
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <errno.h>

typedef unsigned char UINT8;
typedef unsigned short UINT16;
typedef unsigned long UINT32;

#include "bcmadsl.h"
#include "adsldrv.h"
#include "AdslMibDef.h"
//#include "atmb_drv.h"
#include "DiagDef.h"
#include "atmapidrv.h"

#include "sc_info.h"

//static char *get_country_by_code(DSL_uint16_t country_code);

/*#define offsetof(s,m) (size_t)&(((s *)0)->m)*/



#define DEC_POINT(n)	(n)

static int
OpenBcmAdsl(void)
{
    int nFd = open("/dev/bcmadsl0", O_RDWR);

    return (nFd);
}

static char *
GetTrainingStatusStr(int status)
{
    switch (status)
    {
    case kAdslTrainingIdle:
	return "NoSignal";
    case kAdslTrainingG994:
    case kAdslTrainingG992Started:
    case kAdslTrainingG992ChanAnalysis:
    case kAdslTrainingG992Exchange:
	return "EstablishingLink";
    case kAdslTrainingConnected:
	return "Up";
    default:
	return "Disabled";
    }
}

static char *
GetModulationStr(int modType)
{
    switch (modType)
    {
    case kAdslModGdmt:
	return "ADSL_G.dmt";
    case kAdslModT1413:
	return "ADSL_ANSI_T1.413";
    case kAdslModGlite:
	return "ADSL_G.lite";
    case kAdslModAnnexI:
	return "IDSL";
    case kAdslModAdsl2:
	return "ADSL_G.dmt.bis";
    case kAdslModAdsl2p:
	return "ADSL_2plus";
    case kAdslModReAdsl2:
	return "ADSL_re-adsl2";
    default:
	return "Unknown";
    }
}

static int
AdjustRate(adslMibInfo * pMib, int rate)
{
    if (pMib->adslFramingMode & kAtmHeaderCompression)
	return (rate * 53) / 49;
    else
	return rate;
}

static int
GetRcvRate(adslMibInfo * pMib)
{
    int n;

    n = pMib->adsl2Info.rcvRate;
    if (n != 0)
	return n;

    n = pMib->adslConnection.rcvInfo.K;
    if (n != 0)
	return AdjustRate(pMib, (n - 1) * 8 * 4);
    else
	return 0;
}

static int
GetXmtRate(adslMibInfo * pMib)
{
    int n = pMib->adslConnection.xmtInfo.K;

    n = pMib->adsl2Info.xmtRate;
    if (n != 0)
	return n;

    n = pMib->adslConnection.xmtInfo.K;
    if (n != 0)
	return AdjustRate(pMib, (n - 1) * 8 * 4);
    else
	return 0;
}
static char *
GetLineCoding(adslMibInfo * pMib)
{
    switch (pMib->adslLine.adslLineCoding)
    {
    case kAdslLineCodingDMT:
	return "DMT";
    case kAdslLineCodingCAP:
	return "CAP";
    case kAdslLineCodingQAM:
	return "QAM";
    default:
    case kAdslLineCodingOther:
	return "Other";
    }
}

char *
GetVendorIdStr(char *id)
{
    static char returnedString[20];
    int version = (id[6] << 8) + id[7];

    sprintf(returnedString, "%c%c%c%c:0x%4x", id[2], id[3], id[4], id[5],
	    version);
    return returnedString;
}

int g_nAtmFd=-1;

/***************************************************************************
 * Function Name: OpenBcmAtm
 * Description  : Opens the bcmatm device.
 * Returns      : device handle if successsful or -1 if error
 ***************************************************************************/
static int OpenBcmAtm( void )
{
    int nFd = open( "/dev/bcmatm0", O_RDWR );

    return( nFd );
} /* OpenBcmAtm */

/***************************************************************************
 * Function Name: BcmAtm_GetInterfaceId
 * Description  : Returns the interface id for the specified ATM port.
 * Returns      : STS_SUCCESS if successful or error status.
 ***************************************************************************/

BCMATM_STATUS BcmAtm_GetInterfaceId( UINT8 ucPhyPort,
    UINT32 *pulInterfaceId )
{
    ATMDRV_INTERFACE_ID Arg = {ucPhyPort,{0,0,0}, 0, STS_ERROR};

    if( g_nAtmFd == -1  )
        g_nAtmFd = OpenBcmAtm();

    if( g_nAtmFd != -1  )
    {
        Arg.ulInterfaceId = *pulInterfaceId;
        ioctl( g_nAtmFd, ATMIOCTL_GET_INTERFACE_ID, &Arg );
        *pulInterfaceId = Arg.ulInterfaceId;
    }
    else
        Arg.baStatus = STS_STATE_ERROR;

    return( Arg.baStatus );
} // BcmAtm_GetInterfaceId

/***************************************************************************
 * Function Name: BcmAtm_GetVccAddrs
 * Description  : Returns the configured VCC addresses for an interface. 
 * Returns      : STS_SUCCESS if successful or error status.
 ***************************************************************************/
BCMATM_STATUS BcmAtm_GetVccAddrs( UINT32 ulInterfaceId, PATM_VCC_ADDR pVccAddrs,
    UINT32 ulNumVccs, UINT32 *pulNumReturned )
{
    ATMDRV_VCC_ADDRS Arg = {ulInterfaceId, pVccAddrs, ulNumVccs, 0, STS_ERROR};

    if( g_nAtmFd == -1  )
        g_nAtmFd = OpenBcmAtm();

    if( g_nAtmFd != -1  )
    {
        ioctl( g_nAtmFd, ATMIOCTL_GET_VCC_ADDRS, &Arg );
        *pulNumReturned = Arg.ulNumReturned;
    }
    else
        Arg.baStatus = STS_STATE_ERROR;

    return( Arg.baStatus );
} // BcmAtm_GetVccAddrs

/***************************************************************************
 * Function Name: BcmAtm_GetVccStatistics
 * Description  : Returns the VCC statistics record for the specified VCC
 *                address.
 * Returns      : STS_SUCCESS if successful or error status.
 ***************************************************************************/
BCMATM_STATUS BcmAtm_GetVccStatistics( PATM_VCC_ADDR pVccAddr,
    PATM_VCC_STATS pVccStatistics, UINT32 ulReset )
{
    ATMDRV_VCC_STATISTICS Arg = {{pVccAddr->ulInterfaceId, pVccAddr->usVpi,
        pVccAddr->usVci}, pVccStatistics, ulReset, STS_ERROR};

    if( g_nAtmFd == -1  )
        g_nAtmFd = OpenBcmAtm();

    if( g_nAtmFd != -1  )
        ioctl( g_nAtmFd, ATMIOCTL_GET_VCC_STATISTICS, &Arg );
    else
        Arg.baStatus = STS_STATE_ERROR;

    return( Arg.baStatus );
} // BcmAtm_GetVccStatistics

/***************************************************************************
 * Function Name: BcmAtm_GetInterfaceStatistics
 * Description  : Returns the statistics record for an interface.
 * Returns      : STS_SUCCESS if successful or error status.
 ***************************************************************************/
BCMATM_STATUS BcmAtm_GetInterfaceStatistics( UINT32 ulInterfaceId,
    PATM_INTERFACE_STATS pStatistics, UINT32 ulReset )
{
    ATMDRV_INTERFACE_STATISTICS Arg = {ulInterfaceId, pStatistics, ulReset,
        STS_ERROR};

    if( g_nAtmFd == -1  )
        g_nAtmFd = OpenBcmAtm();

    if( g_nAtmFd != -1  )
        ioctl( g_nAtmFd, ATMIOCTL_GET_INTERFACE_STATISTICS, &Arg );
    else
        Arg.baStatus = STS_STATE_ERROR;

    return( Arg.baStatus );
} // BcmAtm_GetInterfaceStatistics

/***************************************************************************
 * Function Name: BcmAtm_GetInterfaceCfg
 * Description  : Calls the interface object for the specified interface id
 *                to return the interface configuration record.
 * Returns      : STS_SUCCESS if successful or error status.
 ***************************************************************************/
BCMATM_STATUS BcmAtm_GetInterfaceCfg( UINT32 ulInterfaceId,
    PATM_INTERFACE_CFG pInterfaceCfg )
{
    ATMDRV_INTERFACE_CFG Arg = {ulInterfaceId, pInterfaceCfg, STS_ERROR};

    if( g_nAtmFd == -1  )
        g_nAtmFd = OpenBcmAtm();

    if( g_nAtmFd != -1  )
        ioctl( g_nAtmFd, ATMIOCTL_GET_INTERFACE_CFG, &Arg );
    else
        Arg.baStatus = STS_STATE_ERROR;

    return( Arg.baStatus );
} // BcmAtm_GetInterfaceCfg


int
tr098_modem_status(tr098_adsl_info_t * adsl)
{
    ADSLDRV_GET_OBJ arg_obj;

    adslChanPerfDataEntry *pChanPerfData;
    int fd;
    adslMibInfo adslMib;
    long size = sizeof(adslMib);
    // arg_obj.bvStatus = BCMADSL_STATUS_ERROR;

    if (adsl == NULL)
	return -1;

    fd = OpenBcmAdsl();

	{
		adslVersionInfo AdslVer;
		ADSLDRV_GET_VERSION     Arg;

		Arg.pAdslVer  = &AdslVer;
		Arg.bvStatus = BCMADSL_STATUS_ERROR;    
		if (ioctl(fd, ADSLIOCTL_GET_VERSION, &Arg))
		{
			goto moreinfo;
		}
		strcpy(adsl->firmware, AdslVer.phyVerStr);
	}
moreinfo:
    arg_obj.objId = NULL;
    arg_obj.objIdLen = 0;
    arg_obj.dataBuf = (char *) &adslMib;
    arg_obj.dataBufLen = size;
    if (ioctl(fd, ADSLIOCTL_GET_OBJ_VALUE, &arg_obj))
    {
	close(fd);
	return -1;
    }

    if (adslMib.adslConnection.chType == kAdslIntlChannel)
    {
	pChanPerfData = &adslMib.adslChanIntlPerfData;
	adsl->inter_leave_dep = 1;
    }
    else
    {
	pChanPerfData = &adslMib.adslChanFastPerfData;
	adsl->inter_leave_dep = 0;
    }

    strcpy(adsl->modulationtype,
	   GetModulationStr(adslMib.adslConnection.modType));
    strcpy(adsl->stat, GetTrainingStatusStr(adslMib.adslTrainingState));
    strcpy(adsl->status, GetTrainingStatusStr(adslMib.adslTrainingState));

    adsl->up_stream = GetXmtRate(&adslMib);
    adsl->down_stream = GetRcvRate(&adslMib);

    adsl->up_stream_max = (int)(adslMib.adslAtucPhys.adslCurrAttainableRate / 1000);
    adsl->down_stream_max = (int)(adslMib.adslPhys.adslCurrAttainableRate / 1000);

    adsl->d_margin = DEC_POINT(adslMib.adslPhys.adslCurrSnrMgn);
    adsl->u_margin = DEC_POINT(adslMib.adslAtucPhys.adslCurrSnrMgn);

    adsl->line_d_att = DEC_POINT(adslMib.adslPhys.adslCurrAtn);
    adsl->line_u_att = DEC_POINT(adslMib.adslAtucPhys.adslCurrAtn);

    adsl->noisedown = DEC_POINT(adslMib.adslPhys.adslSignalAttn);
    adsl->noiseup = DEC_POINT(adslMib.adslAtucPhys.adslSignalAttn);

    adsl->d_power = DEC_POINT(adslMib.adslAtucPhys.adslCurrOutputPwr);
    adsl->u_power = DEC_POINT(adslMib.adslPhys.adslCurrOutputPwr);



    adsl->totalstart =
	adslMib.adslPerfData.adslPerfCurr1DayTimeElapsed +
	adslMib.adslPerfData.adslAturPerfPrev1DayMoniSecs;
    adsl->showtimestart = adslMib.adslPerfData.adslPerfCurr1DayTimeElapsed;

    /* showtime things */
    adsl->showtime.recvblocks =
	pChanPerfData->perfCurr1Day.adslChanReceivedBlks;
    adsl->showtime.errsecs = adslMib.adslPerfData.perfSinceShowTime.adslESs;
    adsl->showtime.ses = adslMib.adslPerfData.perfSinceShowTime.adslSES;
    adsl->showtime.lof = adslMib.adslPerfData.perfSinceShowTime.adslLofs;
    adsl->showtime.fecerr = 0;
    adsl->showtime.atucfecerr = 0;
    adsl->showtime.crcerr = pChanPerfData->perfCurr1Day.adslChanUncorrectBlks;
    adsl->showtime.atuccrcerr = 0;
    adsl->showtime.hecerr = 0;
    adsl->showtime.atuchecerr = 0;
    adsl->showtime.celldelin = 0;
    adsl->showtime.linkretrain =
	adslMib.adslPerfData.failSinceShowTime.adslRetr;
    adsl->showtime.initerr =
	adslMib.adslPerfData.failSinceShowTime.adslInitErr;
    adsl->showtime.inittimeout =
	adslMib.adslPerfData.failSinceShowTime.adslInitTo;

    /* total things */
    adsl->total.recvblocks = pChanPerfData->perfTotal.adslChanReceivedBlks;
    adsl->total.errsecs = adslMib.adslPerfData.perfTotal.adslESs;
    adsl->total.ses = adslMib.adslPerfData.perfTotal.adslSES;
    adsl->total.lof = adslMib.adslPerfData.perfTotal.adslLofs;
    adsl->total.fecerr = 0;
    adsl->total.atucfecerr = 0;
    adsl->total.crcerr = pChanPerfData->perfTotal.adslChanUncorrectBlks;
    adsl->total.atuccrcerr = 0;
    adsl->total.hecerr = 0;
    adsl->total.atuchecerr = 0;
    adsl->total.celldelin = 0;
    adsl->total.linkretrain = adslMib.adslPerfData.failTotal.adslRetr;
    adsl->total.initerr = adslMib.adslPerfData.failTotal.adslInitErr;
    adsl->total.inittimeout = adslMib.adslPerfData.failTotal.adslInitTo;

    sprintf(adsl->line_encode, GetLineCoding(&adslMib));
    strcpy(adsl->country, "US");
    strcpy(adsl->ccountry, "US");

    strcpy(adsl->vendor, "Broadcom"/*adslMib.adslPhys.adslVendorID*/);
    strcpy(adsl->cvendor, "Broadcom"/*adslMib.adslAtucPhys.adslVendorID*/);
    
    close(fd);
    return 0;

}

int
tr098_get_atm_stats(tr098_atm_stats_t * p_atm_stats, unsigned short vpi, unsigned short vci)
{
    PATM_VCC_ADDR pAddrs = NULL;
    PATM_VCC_ADDR pAddrsMem = NULL;

    UINT32 i;
    UINT32 ulInterfaceId = 0;
    ATM_VCC_STATS vccStats;
    PATM_VCC_AAL5_STATS pAal5Stats = &vccStats.u.AtmVccAal5Stats;

    ATM_INTERFACE_CFG Cfg;
    ATM_INTERFACE_STATS Stats;
    PATM_INTF_ATM_STATS pAtmStats = &Stats.AtmIntfStats;

    if (!p_atm_stats) return -1;
    memset(p_atm_stats, 0, sizeof(*p_atm_stats));
    
    for( i = 0; i < PHY_NUM_PORTS; i++ )
    {
        int j;
        UINT32 ulAddrsReturned = 0;

        ulInterfaceId = (UINT32) i;
        BcmAtm_GetInterfaceId( (UINT8) i, &ulInterfaceId );
        BcmAtm_GetVccAddrs( ulInterfaceId, NULL, 0, &ulAddrsReturned );
        
        if (!ulAddrsReturned) continue;
        
        pAddrsMem = (PATM_VCC_ADDR)
            malloc( ulAddrsReturned * (sizeof(ATM_VCC_ADDR) + sizeof(UINT8)) );

        BcmAtm_GetVccAddrs(ulInterfaceId, pAddrsMem, ulAddrsReturned, &ulAddrsReturned);
        
        if (!ulAddrsReturned) continue;
        
        for(j = 0, pAddrs = pAddrsMem; j < ulAddrsReturned; pAddrs++,j++)
            if ((pAddrs->usVpi == vpi) && (pAddrs->usVci == vci))
                break;
        
        if (j == ulAddrsReturned) continue;
        
            /* Display VCC configuration. */
        memset( &Stats, 0x00, sizeof(Stats) );

        Cfg.ulStructureId = ID_ATM_INTERFACE_CFG;

        if( BcmAtm_GetInterfaceCfg( ulInterfaceId, &Cfg ) == STS_SUCCESS &&
            Cfg.ulIfOperStatus == OPRSTS_UP)
        {
            Stats.ulStructureId = ID_ATM_INTERFACE_STATS;
            if (BcmAtm_GetInterfaceStatistics(ulInterfaceId, &Stats,
                                          0) == STS_SUCCESS )
            {
                p_atm_stats->atm_tx_blks = pAtmStats->ulIfInOctets/48;
                p_atm_stats->atm_rx_blks = pAtmStats->ulIfOutOctets/48;
                p_atm_stats->atm_crc_err = pAtmStats->ulIfInErrors;
                p_atm_stats->atm_hec_err = pAtmStats->ulIfInHecErrors;
            }

            memset( &vccStats, 0x00, sizeof(vccStats) );
            vccStats.ulStructureId = ID_ATM_VCC_STATS;
            if (BcmAtm_GetVccStatistics( pAddrs, &vccStats, 0 ) == STS_SUCCESS)
                p_atm_stats->aal5_crc_err = pAal5Stats->ulAal5VccCrcErrors;
            strcpy(p_atm_stats->link_state, "Up");
            strcpy(p_atm_stats->atmaal, "AAL5");
        }
        else
            strcpy(p_atm_stats->link_state, "Down");
    }
    
    if( pAddrsMem )
        free( pAddrsMem );

    return 0;
}
