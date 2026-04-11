#ifndef _SRC_LAMINA_LMX_C_API_
#define _SRC_LAMINA_LMX_C_API_
#define LMX_CALL
#include <cstdint>
#ifdef __cplusplus
namespace lmx::runtime {
class Code;
}

extern "C" {
#endif

struct LMXEnv {
    lmx::runtime::Code* code;
    uint8_t* ip;

};
void lmx_global_init() noexcept;

int lmx_compile_do_file(const char* file_name);

#ifdef __cplusplus
}
#endif

#endif