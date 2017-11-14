
#ifndef __SC_ADSL_H_
#  define __SC_ADSL_H_

#  define _ADSL_STATE_LEN_ 128

typedef struct tr098_adsl_stat_s
{
    unsigned long recvblocks;	/* Count of blocks received */
    unsigned long transblocks;	/* Count of blocks transmitted */
    unsigned long celldelin;	/* Delineated total cell count */
    unsigned long linkretrain;	/* Full Initializations and Short
				   Initializations */
    unsigned long initerr;	/* Initialization errors (LINIT failure) */
    unsigned long inittimeout;	/* Initialization timeout errors */
    unsigned long lof;		/* Count of the number of Loss of Framing
				   failures */
    unsigned long errsecs;	/* Errored Seconds. */
    unsigned long ses;		/* Severely Errored Seconds. */
    unsigned long fecerr;	/* Forward Error Corrections */
    unsigned long atucfecerr;	/* ATUC Forward Error Corrections */
    unsigned long hecerr;	/* HEC violation count */
    unsigned long atuchecerr;	/* ATUC HEC violation count */
    unsigned long crcerr;	/* CRC error count */
    unsigned long atuccrcerr;	/* ATUC CRC error count */

} tr098_adsl_stat_t;


typedef struct tr098_adsl_info_s
{
    char stat[_ADSL_STATE_LEN_];
    char modulationtype[32];	/* Adsl or Adsl2 or Adsl2+ */
    char firmware[128];		/* Firmware version */
	char status[128];
    char vendor[12];		/* CPE Provider */
    char cvendor[12];		/* DSL Provider */
    char country[12];		/* Country */
    char ccountry[12];		/* Country */
    unsigned int down_stream;	/* Downstream Actual Data Rate (bps) */
    unsigned int up_stream;	/* Upstream Actual Data Rate (bps) */
    unsigned int down_stream_max;	/* Downstream Max Data Rate (bps) */
    unsigned int up_stream_max;	/* Upstream Max Data Rate (bps) */
    unsigned short line_u_att;	/* Upstream Line Attenuation (0.1db) */
    unsigned short line_d_att;	/* Downstream Line Attenuation (0.1db) */
    unsigned short u_margin;	/* Upstream Signal-to-Noise Ratio Margin
				   (0.1db) */
    unsigned short d_margin;	/* Downtream Signal-to-Noise Ratio Margin
				   (0.1db) */
    unsigned short u_power;	/* Upstream Actual Aggregate Transmit Power */
    unsigned short d_power;	/* Downstream Actual Aggregate Transmit Power */
    unsigned short inter_leave_dep;	/* interleaving depth */
    unsigned short noisedown;	/* Downstream Signal-to-Noise Ratio (0.1db) */
    unsigned short noiseup;	/* Uptream Signal-to-Noise Ratio (0.1db) */
    char line_encode[8];	/* ADSL coding type used on this line */
    unsigned long totalstart;	/* Total Elapsed time */
    unsigned long showtimestart;	/* Elapsed time in the interval */
    unsigned short ACTPS_ds;
    unsigned short ACTPS_us;
    unsigned short ACTATPds;
    unsigned short ACTATPus;
    unsigned long HLIN_SCALE_ds;
    char DELT_HLIN_ds[8];
    char QLNpsds[8];
    char SNRpsds[8];
    char BITSpsds[8];
    char GAINSpsds[8];
    tr098_adsl_stat_t total;		/* Total Statistics from reboot */
    tr098_adsl_stat_t showtime;	/* Statistics from show time */
} tr098_adsl_info_t,adsl_info_t;

typedef struct tr098_atm_stats_s
{
    char link_state[16];
    char atmaal[16];
    unsigned long atm_tx_blks;
    unsigned long atm_rx_blks;
    unsigned long aal5_crc_err;
    unsigned long atm_hec_err;
    unsigned long atm_crc_err;
} tr098_atm_stats_t,atm_stats_t;



/*tr098:
int tr098_get_atm_stats(adsl_stats_t * p_atm_stats);
	Get the statistic of {atm}
Parameters:
	p_atm_stats: pointer of an adsl_atm_t struct
        vpi/vci : atm address
Return:
	return 1 if success, 0 if fail 
*/
int tr098_get_atm_stats(tr098_atm_stats_t * p_atm_stats, unsigned short vpi, unsigned short vci);


/*tr098:
int tr098_get_adsl_stats(adsl_stats_t * p_adsl_stats);
	Get the statistic of {adsl}
Parameters:
	p_adsl_stats: pointer of an adsl_stats_t struct
Return:
	return 1 if success, 0 if fail 
*/


/* dsl information */
int tr098_modem_status(tr098_adsl_info_t * adsl);
#define sc_get_AdslInfo tr098_modem_status
#define sc_get_atm_stats tr098_get_atm_stats

#define modem_status tr098_modem_status
#endif /*_API_ADSL_H_*/
