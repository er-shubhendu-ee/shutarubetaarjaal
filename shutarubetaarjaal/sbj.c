/* file: sbj.c */

#include "sbj.h"

//
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#define TAG "SBJ"

typedef struct sbj_tagBranchStruct {
    struct sbj_NodeBlock_t* pPrevSibling;
    struct sbj_NodeBlock_t* pNextSibling;
} sbj_BranchStruct_t;

/*!
 * @struct     typedef struct sbj_tagNodeBlock_t
 * @brief      a node
 * @details
 *
 **/
typedef struct sbj_tagNodeBlock_t {
    uint32_t id;
    struct sbj_NodeBlock_t* pHost;  //!< Single parent
    sbj_NodeParam_t* pParam;
    struct sbj_NodeBlock_t* pBranchs;  //!<
} sbj_NodeBlock_t;

static sbj_NodeBlock_t gPool_nodeBlock[sbj_MAX_NODE];
static sbj_NodeParam_t*
    gpPool_nodeParam[sbj_MAX_NODE];  //!< To facilitate user selection, either static or dynamic

int32_t sbj_init(void) {
    int32_t exeStatus = sbj_NO_ERROR;

#if sbj_LOG_LEVEL >= sbj_LOG_LEVEL_INFO
    sbj_LOGD(TAG, "SBJ initializing.");
#endif

    memset(gPool_nodeBlock, 0, sizeof(sbj_NodeBlock_t) * sbj_MAX_NODE);
    memset(gpPool_nodeParam, 0, sizeof(sbj_NodeParam_t*) * sbj_MAX_NODE);

#if sbj_LOG_LEVEL >= sbj_LOG_LEVEL_INFO
    sbj_LOGD(TAG, "SBJ initialization status: %d.", exeStatus);
#endif

    return exeStatus;
}


/*  Logging */

void sbj_log_write(
    int level,
    const char *pTag,
    const char *pFile,
    int line,
    const char *pFormat,
    ...)
{
    const char *pColor;
    const char *pLevel;
    FILE *pStream;
    va_list args;

    switch (level) {
    case sbj_LOG_LEVEL_ERROR:
        pColor = sbj_LOG_COLOR_RED;
        pLevel = "E";
        pStream = stderr;
        break;

    case sbj_LOG_LEVEL_INFO:
        pColor = sbj_LOG_COLOR_CYAN;
        pLevel = "I";
        pStream = stdout;
        break;

    case sbj_LOG_LEVEL_DEBUG:
    default:
        pColor = sbj_LOG_COLOR_YELLOW;
        pLevel = "D";
        pStream = stdout;
        break;
    }

    (void)fprintf(pStream, "%s[%s] %s (%s:%d): ",
                  pColor, pLevel, pTag, pFile, line);

    va_start(args, pFormat);
    (void)vfprintf(pStream, pFormat, args);
    va_end(args);

    (void)fprintf(pStream, "%s\n", sbj_LOG_COLOR_RESET);
}
