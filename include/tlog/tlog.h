#ifndef _H_TLOG
#define _H_TLOG

#include "tlibc/platform/tlibc_platform.h"
#include "tlog/tlog_config_types.h"

#include <stdio.h>
#include "terrno.h"

#define TLOG_VERSION "0.0.1"


typedef struct _tlog_rolling_file_instance_t
{
	FILE *fout;
	tuint32 index;
}tlog_rolling_file_instance_t;

typedef struct _tlog_appender_instance_t
{
	tlog_appender_type_t type;
	tlog_rolling_file_instance_t rolling_file;	
}tlog_appender_instance_t;

typedef struct _tlog_instance_t
{
	tuint16 appender_instance_num;
	tlog_appender_instance_t appender_instance[TLOG_MAX_APPENDER_NUM];
}tlog_instance_t;



typedef struct _tlog_t tlog_t;
struct _tlog_t
{
	tlog_config_t config;
	tlog_instance_t instance;
};


TERROR_CODE tlog_init(tlog_t *self, const char *config_file);

void tlog_write(tlog_t *self, tlog_level_t level, const char *message, size_t message_size);


//�������������Զ�����־��ʽ
#include <sys/time.h>
#include <time.h>

#define TLOG_HELPER_MESSAGE_LENGTH 1024

#define TLOG_HELPER_LOG(inst, level, fmt, ...)\
{\
	struct timeval timestamp;\
	struct tm	tm;\
	char message[TLOG_HELPER_MESSAGE_LENGTH];\
	size_t message_len, _len;\
	const char* level_name = "";\
	switch(level)\
	{\
	case e_tlog_error:\
		level_name = "error";\
		break;\
	case e_tlog_warn:\
		level_name = "warn";\
		break;\
	case e_tlog_info:\
		level_name = "info";\
		break;\
	case e_tlog_debug:\
		level_name = "debug";\
		break;\
	}\
	gettimeofday(&timestamp, NULL);\
	localtime_r(&timestamp.tv_sec, &tm);\
	message_len = snprintf(message, TLOG_HELPER_MESSAGE_LENGTH,\
		"%04d-%02d-%02d %02d:%02d:%02d [%s] %s:%u : ",\
		tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,\
		tm.tm_hour, tm.tm_min, tm.tm_sec\
		,level_name, __FILE__, __LINE__\
		);\
	_len = snprintf(message + message_len, TLOG_HELPER_MESSAGE_LENGTH - message_len\
		, fmt, __VA_ARGS__);\
	message_len += _len;\
	if(message_len + 1 < TLOG_HELPER_MESSAGE_LENGTH)\
	{\
		message[message_len++] = '\n';\
		message[message_len] = 0;\
	}\
	message[TLOG_HELPER_MESSAGE_LENGTH - 1] = 0;\
	tlog_write(inst, level, message, message_len);\
}

#endif//_H_TLOG