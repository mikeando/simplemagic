#ifndef SIMPLEMAGIC_H
#define SIMPLEMAGIC_H

#ifndef SMC_NO_MAGIC
#include <stdint.h>
#define SMC_ADD_MAGIC() uint64_t smc__magic
#ifndef SMC_NO_MAGIC_CHECKS
#define SMC_MAGIC(T,V) static const uint64_t smc__magic_##T = V
#define smc_check_type(type,data) do { \
	type* vv = data; \
	smc__check_struct_type(#type, #data, smc__magic##type, (vv==NULL)?0:((type*)(data))->smc_magic, vv, __LINE__,__FILE__); \
	} while(0)
void smc__check_struct_type( const char * type, const char* expr, uint64_t expected_magic, uint64_t magic, void * p, int line, const char * file);
#define smc_init_magic(type,data) do { \
	data->smc__magic = smc__magic_##type ; \
	} while(0)
#else
#define SMC_MAGIC(T,V)
#define smc_check_type(type,data)
#define smc_init_magic(type,data)
#endif
#else
#define SMC_ADD_MAGIC()
#define SMC_MAGIC(T,V)
#define smc_check_type(type,data)
#define smc_init_magic(type,data)
#endif

#endif
