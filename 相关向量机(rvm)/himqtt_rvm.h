#ifndef HIMQTTRVM_H
#define HIMQTTRVM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>

#if defined(_WIN32)
#if defined(himqttrvm_EXPORTS)
#define HIMQTTRVM_API __declspec(dllexport)
#else
#define HIMQTTRVM_API __declspec(dllimport)
#endif
#elif defined(__GNUC__) && defined(himqttrvm_EXPORTS)
#define HIMQTTRVM_API __attribute__((visibility("default")))
#else
#define HIMQTTRVM_API
#endif

#define HIMQTTRVM_SUCCESS 0x00
#define HIMQTTRVM_INVALID_P1 0x11
#define HIMQTTRVM_INVALID_P2 0x12
#define HIMQTTRVM_INVALID_P3 0x13
#define HIMQTTRVM_INVALID_P4 0x14
#define HIMQTTRVM_INVALID_P5 0x15
#define HIMQTTRVM_INVALID_P6 0x16
#define HIMQTTRVM_INVALID_P7 0x17
#define HIMQTTRVM_LAPACK_ERROR 0x20
#define HIMQTTRVM_MATH_ERROR 0x40

typedef struct himqttrvm_cache himqttrvm_cache;
typedef struct himqttrvm_param himqttrvm_param;

HIMQTTRVM_API int himqttrvm_create_cache(himqttrvm_cache** cache, double* y, size_t count);
HIMQTTRVM_API int himqttrvm_destroy_cache(himqttrvm_cache* cache);

HIMQTTRVM_API int himqttrvm_create_param(himqttrvm_param** param, double alpha_init, double alpha_max, double alpha_tol, double beta_init, double basis_percent_min, size_t iter_max);
HIMQTTRVM_API int himqttrvm_destroy_param(himqttrvm_param* param);

HIMQTTRVM_API int himqttrvm_train(himqttrvm_cache* cache, himqttrvm_param* param1, himqttrvm_param* param2, double* phi, size_t* index, size_t count, size_t batch_size_max);

HIMQTTRVM_API int himqttrvm_get_training_stats(himqttrvm_cache* cache, size_t* basis_count, bool* bias_used);
HIMQTTRVM_API int himqttrvm_get_training_results(himqttrvm_cache* cache, size_t* index, double* mu);

HIMQTTRVM_API int himqttrvm_predict(double* phi, double* mu, size_t sample_count, size_t basis_count, double* y);

HIMQTTRVM_API int himqttrvm_get_version(int* major, int* minor, int* patch);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* HIMQTTRVM_H */
