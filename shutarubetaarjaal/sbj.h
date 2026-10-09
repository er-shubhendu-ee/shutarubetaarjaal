/* file: sbj.h */

#ifndef SBJ_H_
#define SBJ_H_

#include <stddef.h>
#include <stdint.h>

/*    Log configuration      */
#define sbj_LOG_LEVEL_NONE 0
#define sbj_LOG_LEVEL_ERROR 1
#define sbj_LOG_LEVEL_INFO 2
#define sbj_LOG_LEVEL_DEBUG 3

#define sbj_LOG_LEVEL sbj_LOG_LEVEL_DEBUG

/*  Protocol configuration  */
#define sbj_MAX_NODE 128  //!< max number of connections, including children and root

/*  Status/Error definitions*/
#define sbj_NO_ERROR 0

#define sbj_ERROR_NULL -1

typedef struct sbj_tagNodeParam {
    uint32_t size;
    float rssi;
    float bandwidth;
} sbj_NodeParam_t;

typedef struct sbj_NodeBlock_t* sbj_Node_t;

/* ANSI colors */
#define sbj_LOG_COLOR_RESET "\x1b[0m"
#define sbj_LOG_COLOR_RED "\x1b[31m"
#define sbj_LOG_COLOR_YELLOW "\x1b[33m"
#define sbj_LOG_COLOR_CYAN "\x1b[36m"

#ifdef __cplusplus
extern "C" {
#endif

int32_t sbj_init(void);

void sbj_log_write(int level, const char* pTag, const char* pFile, int line, const char* pFormat,
                   ...);

#ifdef __cplusplus
}
#endif

#define sbj_LOGE(TAG, ...)                                                            \
    do {                                                                              \
        if (sbj_LOG_LEVEL >= sbj_LOG_LEVEL_ERROR) {                                   \
            sbj_log_write(sbj_LOG_LEVEL_ERROR, TAG, __FILE__, __LINE__, __VA_ARGS__); \
        }                                                                             \
    } while (0)

#define sbj_LOGD(TAG, ...)                                                            \
    do {                                                                              \
        if (sbj_LOG_LEVEL >= sbj_LOG_LEVEL_DEBUG) {                                   \
            sbj_log_write(sbj_LOG_LEVEL_DEBUG, TAG, __FILE__, __LINE__, __VA_ARGS__); \
        }                                                                             \
    } while (0)

#define sbj_LOGI(TAG, ...)                                                           \
    do {                                                                             \
        if (sbj_LOG_LEVEL >= sbj_LOG_LEVEL_INFO) {                                   \
            sbj_log_write(sbj_LOG_LEVEL_INFO, TAG, __FILE__, __LINE__, __VA_ARGS__); \
        }                                                                            \
    } while (0)

#endif /* @end  SBJ_H_*/