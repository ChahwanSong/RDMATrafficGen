#pragma once

namespace congweave {

/*************************************************************
***      T R A F F I C - G E N    P A R A M E T E R S      ***
**************************************************************/

/* DEBUGGING MODE, 0: Error, 1: Info, 2: Debug */
#define CONGWEAVE_DEBUG (0)


/* NIC bandwidth */
#define BW_GBPS (25)
/* number of persistent connections between client-server */
#define NUM_CONNECTIONS (2)


/* time to save records (seconds) */
#define TIME_TO_WARM_UP_MSG_GEN (10)
#define TIME_TO_WARM_UP_RESTING (10)
/* time to save records (seconds) */
#define TIME_TO_SIMULATE (30)




/*************************************************************
***           D E B U G G I N G   F U N C T I O N S        ***
**************************************************************/

/* Error Macro */
#if (CONGWEAVE_DEBUG >= 0)
#define rdma_error(msg, args...) \
    fprintf(stderr, "%s : %d : ERROR : " msg, __FILE__, __LINE__, ##args);
#else
#define rdma_error(msg, args...)
#endif

/* Debug Macro */
#if (CONGWEAVE_DEBUG >= 1)
#define rdma_info(msg, args...) \
    printf("INFO: " msg, ##args);
#else
#define rdma_info(msg, args...) 
#endif

/* Debug Macro */
#if (CONGWEAVE_DEBUG >= 2)
#define debug(msg, args...) \
    printf("DEBUG: " msg, ##args);
#else
#define debug(msg, args...) 
#endif


}