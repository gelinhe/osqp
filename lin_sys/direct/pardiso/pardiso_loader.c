#include "lib_handler.h"
#include "pardiso_loader.h"

#ifdef IS_WINDOWS
#define PARDISOLIBNAME "mkl_rt." SHAREDLIBEXT
#else 
#define PARDISOLIBNAME "libmkl_rt." SHAREDLIBEXT
#endif

typedef void (*voidfun)(void);

voidfun lh_load_sym (soHandle_t h, const char *symName);


// Interfaces for Pardiso functions
typedef void (*pardiso_t)(void**, const c_int*, const c_int*, const c_int*,
                          const c_int*, const c_int*, const c_float*,
                          const c_int*, const c_int*, const c_int*,
                          const c_int*, c_int*, const c_int*, c_float*,
                          c_float*, const c_int*);
typedef int (*mkl_set_ifl_t)(int);


// Handlers are static variables
static soHandle_t Pardiso_handle = OSQP_NULL;
static pardiso_t func_pardiso = OSQP_NULL;
static mkl_set_ifl_t func_mkl_set_interface_layer = OSQP_NULL;


// Wrappers for loaded Pardiso function handlers
void pardiso(void** pt, const c_int* maxfct, const c_int* mnum,
                  const c_int* mtype, const c_int* phase, const c_int* n,
                  const c_float* a, const c_int* ia, const c_int* ja,
                  const c_int* perm, const c_int* nrhs, c_int* iparm,
                  const c_int* msglvl, c_float* b, c_float* x,
                  const c_int* error) {
    func_pardiso(pt, maxfct, mnum, mtype, phase, n, a, ia, ja,
                 perm, nrhs, iparm, msglvl, b, x, error);
}
c_int mkl_set_interface_layer(c_int code) {
    return (c_int)func_mkl_set_interface_layer((int)code);
}



c_int lh_load_pardiso(const char* libname) {
    // Load Pardiso library
    if (libname) {
        Pardiso_handle = lh_load_lib(libname);
    } else { /* try a default library name */
        Pardiso_handle = lh_load_lib(PARDISOLIBNAME);
    }
    if (!Pardiso_handle) return 1;

    // Load Pardiso functions
    func_pardiso = (pardiso_t)lh_load_sym(Pardiso_handle, "pardiso");
    if (!func_pardiso) return 1;

    func_mkl_set_interface_layer = (mkl_set_ifl_t)lh_load_sym(Pardiso_handle,
                                                    "MKL_Set_Interface_Layer");
    if (!func_mkl_set_interface_layer) return 1;

    return 0;
}

c_int lh_unload_pardiso() {
    int rc;

    if (Pardiso_handle == OSQP_NULL)
      return 0;

    rc = lh_unload_lib(Pardiso_handle);
    Pardiso_handle = OSQP_NULL;
    func_pardiso = OSQP_NULL;

    return rc;
}
