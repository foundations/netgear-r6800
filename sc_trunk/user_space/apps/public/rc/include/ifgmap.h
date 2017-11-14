

#ifndef _IFGROUP_H_
#define _IFGROUP_H_

#include "if_type.h"

#define RT_TB_SRC  "/usr/etc/iproute2/rt_tables"
#define RT_TB_DST  "/etc/iproute2/rt_tables"

#define IFGROUP_TB_START 100
#define IFGROUP_TB_BASE  "rt"
#define RT_MAIN 254				// id of main routing table



//#define GID_NO 0   // if a wan port has this gid means it doesn't belong to a interface group
#define GID_ENABLE 1   
#define GID_DISABLE 0  

//#define WANID_NO 0   // if a wan port has this gid means it doesn't belong to a interface group
#define INVALID_ID 0

// we only have one phone set now
#define PHONE_PORT_ID 1


/*======================================================================
	routing table information
======================================================================*/

int create_rttab_map( void);
/*
	Get the id of routing table for a group

	Parameter: 
		gid: group id
		
	Return: 
		routing table id
	
*/
int get_rttab_id( int gid );
int init_rttab_group(int gid);




/* ===============================================================================
	ifgmap apis:
   =============================================================================== */


/* 
	free ifgmap memory
*/
void ifgmap_reset( void);


/* 
	get interface number for each type
*/
int ifgmap_get_if_num( IF_TYPE_enum if_type);

/*
	check if if_id is valid.

	if_type: type of interface
	if_id: id of interface

	Return:
		INVALID_ID : if_id is invalid
		if_id: if_id is valid
*/
int ifgmap_check_if_id( IF_TYPE_enum if_type, int if_id);

/*
	get interface group id by interface id

	Return:
		INVALID_ID : if_id is invalid
*/
int ifgmap_get_gid( IF_TYPE_enum if_type, int if_id);

/*
	get wan id by interface id

	Return:
		INVALID_ID : if_id is invalid
*/
int ifgmap_get_wanid( IF_TYPE_enum if_type, int if_id);


/*
	return GID_DISABLE if the interface group is disabled
*/
int ifgmap_is_ifg_enabled( int gid);


#endif 


