
#include <Python.h>
#include <stddef.h>

/* this block of #ifs should be kept exactly identical between
   c/_cffi_backend.c, cffi/vengine_cpy.py, cffi/vengine_gen.py
   and cffi/_cffi_include.h */
#if defined(_MSC_VER)
# include <malloc.h>   /* for alloca() */
# if _MSC_VER < 1600   /* MSVC < 2010 */
   typedef __int8 int8_t;
   typedef __int16 int16_t;
   typedef __int32 int32_t;
   typedef __int64 int64_t;
   typedef unsigned __int8 uint8_t;
   typedef unsigned __int16 uint16_t;
   typedef unsigned __int32 uint32_t;
   typedef unsigned __int64 uint64_t;
   typedef __int8 int_least8_t;
   typedef __int16 int_least16_t;
   typedef __int32 int_least32_t;
   typedef __int64 int_least64_t;
   typedef unsigned __int8 uint_least8_t;
   typedef unsigned __int16 uint_least16_t;
   typedef unsigned __int32 uint_least32_t;
   typedef unsigned __int64 uint_least64_t;
   typedef __int8 int_fast8_t;
   typedef __int16 int_fast16_t;
   typedef __int32 int_fast32_t;
   typedef __int64 int_fast64_t;
   typedef unsigned __int8 uint_fast8_t;
   typedef unsigned __int16 uint_fast16_t;
   typedef unsigned __int32 uint_fast32_t;
   typedef unsigned __int64 uint_fast64_t;
   typedef __int64 intmax_t;
   typedef unsigned __int64 uintmax_t;
# else
#  include <stdint.h>
# endif
# if _MSC_VER < 1800   /* MSVC < 2013 */
#  ifndef __cplusplus
    typedef unsigned char _Bool;
#  endif
# endif
#else
# include <stdint.h>
# if (defined (__SVR4) && defined (__sun)) || defined(_AIX) || defined(__hpux)
#  include <alloca.h>
# endif
#endif

#if PY_MAJOR_VERSION < 3
# undef PyCapsule_CheckExact
# undef PyCapsule_GetPointer
# define PyCapsule_CheckExact(capsule) (PyCObject_Check(capsule))
# define PyCapsule_GetPointer(capsule, name) \
    (PyCObject_AsVoidPtr(capsule))
#endif

#if PY_MAJOR_VERSION >= 3
# define PyInt_FromLong PyLong_FromLong
#endif

#define _cffi_from_c_double PyFloat_FromDouble
#define _cffi_from_c_float PyFloat_FromDouble
#define _cffi_from_c_long PyInt_FromLong
#define _cffi_from_c_ulong PyLong_FromUnsignedLong
#define _cffi_from_c_longlong PyLong_FromLongLong
#define _cffi_from_c_ulonglong PyLong_FromUnsignedLongLong
#define _cffi_from_c__Bool PyBool_FromLong

#define _cffi_to_c_double PyFloat_AsDouble
#define _cffi_to_c_float PyFloat_AsDouble

#define _cffi_from_c_int_const(x)                                        \
    (((x) > 0) ?                                                         \
        ((unsigned long long)(x) <= (unsigned long long)LONG_MAX) ?      \
            PyInt_FromLong((long)(x)) :                                  \
            PyLong_FromUnsignedLongLong((unsigned long long)(x)) :       \
        ((long long)(x) >= (long long)LONG_MIN) ?                        \
            PyInt_FromLong((long)(x)) :                                  \
            PyLong_FromLongLong((long long)(x)))

#define _cffi_from_c_int(x, type)                                        \
    (((type)-1) > 0 ? /* unsigned */                                     \
        (sizeof(type) < sizeof(long) ?                                   \
            PyInt_FromLong((long)x) :                                    \
         sizeof(type) == sizeof(long) ?                                  \
            PyLong_FromUnsignedLong((unsigned long)x) :                  \
            PyLong_FromUnsignedLongLong((unsigned long long)x)) :        \
        (sizeof(type) <= sizeof(long) ?                                  \
            PyInt_FromLong((long)x) :                                    \
            PyLong_FromLongLong((long long)x)))

#define _cffi_to_c_int(o, type)                                          \
    ((type)(                                                             \
     sizeof(type) == 1 ? (((type)-1) > 0 ? (type)_cffi_to_c_u8(o)        \
                                         : (type)_cffi_to_c_i8(o)) :     \
     sizeof(type) == 2 ? (((type)-1) > 0 ? (type)_cffi_to_c_u16(o)       \
                                         : (type)_cffi_to_c_i16(o)) :    \
     sizeof(type) == 4 ? (((type)-1) > 0 ? (type)_cffi_to_c_u32(o)       \
                                         : (type)_cffi_to_c_i32(o)) :    \
     sizeof(type) == 8 ? (((type)-1) > 0 ? (type)_cffi_to_c_u64(o)       \
                                         : (type)_cffi_to_c_i64(o)) :    \
     (Py_FatalError("unsupported size for type " #type), (type)0)))

#define _cffi_to_c_i8                                                    \
                 ((int(*)(PyObject *))_cffi_exports[1])
#define _cffi_to_c_u8                                                    \
                 ((int(*)(PyObject *))_cffi_exports[2])
#define _cffi_to_c_i16                                                   \
                 ((int(*)(PyObject *))_cffi_exports[3])
#define _cffi_to_c_u16                                                   \
                 ((int(*)(PyObject *))_cffi_exports[4])
#define _cffi_to_c_i32                                                   \
                 ((int(*)(PyObject *))_cffi_exports[5])
#define _cffi_to_c_u32                                                   \
                 ((unsigned int(*)(PyObject *))_cffi_exports[6])
#define _cffi_to_c_i64                                                   \
                 ((long long(*)(PyObject *))_cffi_exports[7])
#define _cffi_to_c_u64                                                   \
                 ((unsigned long long(*)(PyObject *))_cffi_exports[8])
#define _cffi_to_c_char                                                  \
                 ((int(*)(PyObject *))_cffi_exports[9])
#define _cffi_from_c_pointer                                             \
    ((PyObject *(*)(char *, CTypeDescrObject *))_cffi_exports[10])
#define _cffi_to_c_pointer                                               \
    ((char *(*)(PyObject *, CTypeDescrObject *))_cffi_exports[11])
#define _cffi_get_struct_layout                                          \
    ((PyObject *(*)(Py_ssize_t[]))_cffi_exports[12])
#define _cffi_restore_errno                                              \
    ((void(*)(void))_cffi_exports[13])
#define _cffi_save_errno                                                 \
    ((void(*)(void))_cffi_exports[14])
#define _cffi_from_c_char                                                \
    ((PyObject *(*)(char))_cffi_exports[15])
#define _cffi_from_c_deref                                               \
    ((PyObject *(*)(char *, CTypeDescrObject *))_cffi_exports[16])
#define _cffi_to_c                                                       \
    ((int(*)(char *, CTypeDescrObject *, PyObject *))_cffi_exports[17])
#define _cffi_from_c_struct                                              \
    ((PyObject *(*)(char *, CTypeDescrObject *))_cffi_exports[18])
#define _cffi_to_c_wchar_t                                               \
    ((wchar_t(*)(PyObject *))_cffi_exports[19])
#define _cffi_from_c_wchar_t                                             \
    ((PyObject *(*)(wchar_t))_cffi_exports[20])
#define _cffi_to_c_long_double                                           \
    ((long double(*)(PyObject *))_cffi_exports[21])
#define _cffi_to_c__Bool                                                 \
    ((_Bool(*)(PyObject *))_cffi_exports[22])
#define _cffi_prepare_pointer_call_argument                              \
    ((Py_ssize_t(*)(CTypeDescrObject *, PyObject *, char **))_cffi_exports[23])
#define _cffi_convert_array_from_object                                  \
    ((int(*)(char *, CTypeDescrObject *, PyObject *))_cffi_exports[24])
#define _CFFI_NUM_EXPORTS 25

typedef struct _ctypedescr CTypeDescrObject;

static void *_cffi_exports[_CFFI_NUM_EXPORTS];
static PyObject *_cffi_types, *_cffi_VerificationError;

static int _cffi_setup_custom(PyObject *lib);   /* forward */

static PyObject *_cffi_setup(PyObject *self, PyObject *args)
{
    PyObject *library;
    int was_alive = (_cffi_types != NULL);
    (void)self; /* unused */
    if (!PyArg_ParseTuple(args, "OOO", &_cffi_types, &_cffi_VerificationError,
                                       &library))
        return NULL;
    Py_INCREF(_cffi_types);
    Py_INCREF(_cffi_VerificationError);
    if (_cffi_setup_custom(library) < 0)
        return NULL;
    return PyBool_FromLong(was_alive);
}

union _cffi_union_alignment_u {
    unsigned char m_char;
    unsigned short m_short;
    unsigned int m_int;
    unsigned long m_long;
    unsigned long long m_longlong;
    float m_float;
    double m_double;
    long double m_longdouble;
};

struct _cffi_freeme_s {
    struct _cffi_freeme_s *next;
    union _cffi_union_alignment_u alignment;
};

#ifdef __GNUC__
  __attribute__((unused))
#endif
static int _cffi_convert_array_argument(CTypeDescrObject *ctptr, PyObject *arg,
                                        char **output_data, Py_ssize_t datasize,
                                        struct _cffi_freeme_s **freeme)
{
    char *p;
    if (datasize < 0)
        return -1;

    p = *output_data;
    if (p == NULL) {
        struct _cffi_freeme_s *fp = (struct _cffi_freeme_s *)PyObject_Malloc(
            offsetof(struct _cffi_freeme_s, alignment) + (size_t)datasize);
        if (fp == NULL)
            return -1;
        fp->next = *freeme;
        *freeme = fp;
        p = *output_data = (char *)&fp->alignment;
    }
    memset((void *)p, 0, (size_t)datasize);
    return _cffi_convert_array_from_object(p, ctptr, arg);
}

#ifdef __GNUC__
  __attribute__((unused))
#endif
static void _cffi_free_array_arguments(struct _cffi_freeme_s *freeme)
{
    do {
        void *p = (void *)freeme;
        freeme = freeme->next;
        PyObject_Free(p);
    } while (freeme != NULL);
}

static int _cffi_init(void)
{
    PyObject *module, *c_api_object = NULL;

    module = PyImport_ImportModule("_cffi_backend");
    if (module == NULL)
        goto failure;

    c_api_object = PyObject_GetAttrString(module, "_C_API");
    if (c_api_object == NULL)
        goto failure;
    if (!PyCapsule_CheckExact(c_api_object)) {
        PyErr_SetNone(PyExc_ImportError);
        goto failure;
    }
    memcpy(_cffi_exports, PyCapsule_GetPointer(c_api_object, "cffi"),
           _CFFI_NUM_EXPORTS * sizeof(void *));

    Py_DECREF(module);
    Py_DECREF(c_api_object);
    return 0;

  failure:
    Py_XDECREF(module);
    Py_XDECREF(c_api_object);
    return -1;
}

#define _cffi_type(num) ((CTypeDescrObject *)PyList_GET_ITEM(_cffi_types, num))

/**********/



#include <stdbool.h>                            /* bool */
#include <stdint.h>                             /* uintxx_t */
#include <stdio.h>                              /* printf, sprintf */
#include <sys/socket.h>                         /* AF_INET[6] */
#include <netinet/in.h>                         /* struct in[6]_addr */
#include <arpa/inet.h>                         /* inet_pton */

#include <libipset/linux_ip_set.h>              /* enum ipset_cmd */
#include <libipset/data.h>    /* enum ipset_data */
#include <libipset/errcode.h>    /* error handling */
#include <libipset/session.h>
#include <libipset/parse.h>   /* ipset_parse_* */
#include <libipset/types.h>   /* struct ipset_type */
#include <libipset/ui.h>    /* core options, commands */
#include <libipset/utils.h>   /* STREQ */

/* Report and output buffer sizes */
#define IPSET_ERRORBUFLEN               1024
#define IPSET_OUTBUFLEN                 8192



static int _cffi_e_____D_enum____D_1(PyObject *lib)
{
  if ((NFPROTO_UNSPEC) > 0 || (long)(NFPROTO_UNSPEC) != 0L) {
    char buf[64];
    if ((NFPROTO_UNSPEC) <= 0)
        snprintf(buf, 63, "%ld", (long)(NFPROTO_UNSPEC));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(NFPROTO_UNSPEC));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum $enum_$1: ", "NFPROTO_UNSPEC", buf, "0");
    return -1;
  }
  if ((NFPROTO_IPV4) <= 0 || (unsigned long)(NFPROTO_IPV4) != 2UL) {
    char buf[64];
    if ((NFPROTO_IPV4) <= 0)
        snprintf(buf, 63, "%ld", (long)(NFPROTO_IPV4));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(NFPROTO_IPV4));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum $enum_$1: ", "NFPROTO_IPV4", buf, "2");
    return -1;
  }
  if ((NFPROTO_ARP) <= 0 || (unsigned long)(NFPROTO_ARP) != 3UL) {
    char buf[64];
    if ((NFPROTO_ARP) <= 0)
        snprintf(buf, 63, "%ld", (long)(NFPROTO_ARP));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(NFPROTO_ARP));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum $enum_$1: ", "NFPROTO_ARP", buf, "3");
    return -1;
  }
  if ((NFPROTO_BRIDGE) <= 0 || (unsigned long)(NFPROTO_BRIDGE) != 7UL) {
    char buf[64];
    if ((NFPROTO_BRIDGE) <= 0)
        snprintf(buf, 63, "%ld", (long)(NFPROTO_BRIDGE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(NFPROTO_BRIDGE));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum $enum_$1: ", "NFPROTO_BRIDGE", buf, "7");
    return -1;
  }
  if ((NFPROTO_IPV6) <= 0 || (unsigned long)(NFPROTO_IPV6) != 10UL) {
    char buf[64];
    if ((NFPROTO_IPV6) <= 0)
        snprintf(buf, 63, "%ld", (long)(NFPROTO_IPV6));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(NFPROTO_IPV6));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum $enum_$1: ", "NFPROTO_IPV6", buf, "10");
    return -1;
  }
  if ((NFPROTO_DECNET) <= 0 || (unsigned long)(NFPROTO_DECNET) != 12UL) {
    char buf[64];
    if ((NFPROTO_DECNET) <= 0)
        snprintf(buf, 63, "%ld", (long)(NFPROTO_DECNET));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(NFPROTO_DECNET));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum $enum_$1: ", "NFPROTO_DECNET", buf, "12");
    return -1;
  }
  if ((NFPROTO_NUMPROTO) <= 0 || (unsigned long)(NFPROTO_NUMPROTO) != 13UL) {
    char buf[64];
    if ((NFPROTO_NUMPROTO) <= 0)
        snprintf(buf, 63, "%ld", (long)(NFPROTO_NUMPROTO));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(NFPROTO_NUMPROTO));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum $enum_$1: ", "NFPROTO_NUMPROTO", buf, "13");
    return -1;
  }
  return ((void)lib,0);
}

static int _cffi_e_enum_ipset_cmd(PyObject *lib)
{
  if ((IPSET_CMD_NONE) > 0 || (long)(IPSET_CMD_NONE) != 0L) {
    char buf[64];
    if ((IPSET_CMD_NONE) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_CMD_NONE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_CMD_NONE));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_cmd: ", "IPSET_CMD_NONE", buf, "0");
    return -1;
  }
  if ((IPSET_CMD_PROTOCOL) <= 0 || (unsigned long)(IPSET_CMD_PROTOCOL) != 1UL) {
    char buf[64];
    if ((IPSET_CMD_PROTOCOL) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_CMD_PROTOCOL));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_CMD_PROTOCOL));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_cmd: ", "IPSET_CMD_PROTOCOL", buf, "1");
    return -1;
  }
  if ((IPSET_CMD_CREATE) <= 0 || (unsigned long)(IPSET_CMD_CREATE) != 2UL) {
    char buf[64];
    if ((IPSET_CMD_CREATE) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_CMD_CREATE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_CMD_CREATE));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_cmd: ", "IPSET_CMD_CREATE", buf, "2");
    return -1;
  }
  if ((IPSET_CMD_DESTROY) <= 0 || (unsigned long)(IPSET_CMD_DESTROY) != 3UL) {
    char buf[64];
    if ((IPSET_CMD_DESTROY) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_CMD_DESTROY));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_CMD_DESTROY));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_cmd: ", "IPSET_CMD_DESTROY", buf, "3");
    return -1;
  }
  if ((IPSET_CMD_FLUSH) <= 0 || (unsigned long)(IPSET_CMD_FLUSH) != 4UL) {
    char buf[64];
    if ((IPSET_CMD_FLUSH) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_CMD_FLUSH));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_CMD_FLUSH));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_cmd: ", "IPSET_CMD_FLUSH", buf, "4");
    return -1;
  }
  if ((IPSET_CMD_RENAME) <= 0 || (unsigned long)(IPSET_CMD_RENAME) != 5UL) {
    char buf[64];
    if ((IPSET_CMD_RENAME) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_CMD_RENAME));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_CMD_RENAME));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_cmd: ", "IPSET_CMD_RENAME", buf, "5");
    return -1;
  }
  if ((IPSET_CMD_SWAP) <= 0 || (unsigned long)(IPSET_CMD_SWAP) != 6UL) {
    char buf[64];
    if ((IPSET_CMD_SWAP) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_CMD_SWAP));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_CMD_SWAP));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_cmd: ", "IPSET_CMD_SWAP", buf, "6");
    return -1;
  }
  if ((IPSET_CMD_LIST) <= 0 || (unsigned long)(IPSET_CMD_LIST) != 7UL) {
    char buf[64];
    if ((IPSET_CMD_LIST) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_CMD_LIST));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_CMD_LIST));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_cmd: ", "IPSET_CMD_LIST", buf, "7");
    return -1;
  }
  if ((IPSET_CMD_SAVE) <= 0 || (unsigned long)(IPSET_CMD_SAVE) != 8UL) {
    char buf[64];
    if ((IPSET_CMD_SAVE) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_CMD_SAVE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_CMD_SAVE));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_cmd: ", "IPSET_CMD_SAVE", buf, "8");
    return -1;
  }
  if ((IPSET_CMD_ADD) <= 0 || (unsigned long)(IPSET_CMD_ADD) != 9UL) {
    char buf[64];
    if ((IPSET_CMD_ADD) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_CMD_ADD));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_CMD_ADD));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_cmd: ", "IPSET_CMD_ADD", buf, "9");
    return -1;
  }
  if ((IPSET_CMD_DEL) <= 0 || (unsigned long)(IPSET_CMD_DEL) != 10UL) {
    char buf[64];
    if ((IPSET_CMD_DEL) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_CMD_DEL));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_CMD_DEL));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_cmd: ", "IPSET_CMD_DEL", buf, "10");
    return -1;
  }
  if ((IPSET_CMD_TEST) <= 0 || (unsigned long)(IPSET_CMD_TEST) != 11UL) {
    char buf[64];
    if ((IPSET_CMD_TEST) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_CMD_TEST));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_CMD_TEST));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_cmd: ", "IPSET_CMD_TEST", buf, "11");
    return -1;
  }
  if ((IPSET_CMD_HEADER) <= 0 || (unsigned long)(IPSET_CMD_HEADER) != 12UL) {
    char buf[64];
    if ((IPSET_CMD_HEADER) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_CMD_HEADER));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_CMD_HEADER));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_cmd: ", "IPSET_CMD_HEADER", buf, "12");
    return -1;
  }
  if ((IPSET_CMD_TYPE) <= 0 || (unsigned long)(IPSET_CMD_TYPE) != 13UL) {
    char buf[64];
    if ((IPSET_CMD_TYPE) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_CMD_TYPE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_CMD_TYPE));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_cmd: ", "IPSET_CMD_TYPE", buf, "13");
    return -1;
  }
  if ((IPSET_MSG_MAX) <= 0 || (unsigned long)(IPSET_MSG_MAX) != 14UL) {
    char buf[64];
    if ((IPSET_MSG_MAX) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_MSG_MAX));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_MSG_MAX));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_cmd: ", "IPSET_MSG_MAX", buf, "14");
    return -1;
  }
  if ((IPSET_CMD_RESTORE) <= 0 || (unsigned long)(IPSET_CMD_RESTORE) != 14UL) {
    char buf[64];
    if ((IPSET_CMD_RESTORE) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_CMD_RESTORE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_CMD_RESTORE));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_cmd: ", "IPSET_CMD_RESTORE", buf, "14");
    return -1;
  }
  if ((IPSET_CMD_HELP) <= 0 || (unsigned long)(IPSET_CMD_HELP) != 15UL) {
    char buf[64];
    if ((IPSET_CMD_HELP) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_CMD_HELP));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_CMD_HELP));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_cmd: ", "IPSET_CMD_HELP", buf, "15");
    return -1;
  }
  if ((IPSET_CMD_VERSION) <= 0 || (unsigned long)(IPSET_CMD_VERSION) != 16UL) {
    char buf[64];
    if ((IPSET_CMD_VERSION) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_CMD_VERSION));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_CMD_VERSION));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_cmd: ", "IPSET_CMD_VERSION", buf, "16");
    return -1;
  }
  if ((IPSET_CMD_QUIT) <= 0 || (unsigned long)(IPSET_CMD_QUIT) != 17UL) {
    char buf[64];
    if ((IPSET_CMD_QUIT) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_CMD_QUIT));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_CMD_QUIT));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_cmd: ", "IPSET_CMD_QUIT", buf, "17");
    return -1;
  }
  if ((IPSET_CMD_MAX) <= 0 || (unsigned long)(IPSET_CMD_MAX) != 18UL) {
    char buf[64];
    if ((IPSET_CMD_MAX) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_CMD_MAX));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_CMD_MAX));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_cmd: ", "IPSET_CMD_MAX", buf, "18");
    return -1;
  }
  if ((IPSET_CMD_COMMIT) <= 0 || (unsigned long)(IPSET_CMD_COMMIT) != 18UL) {
    char buf[64];
    if ((IPSET_CMD_COMMIT) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_CMD_COMMIT));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_CMD_COMMIT));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_cmd: ", "IPSET_CMD_COMMIT", buf, "18");
    return -1;
  }
  return _cffi_e_____D_enum____D_1(lib);
}

static int _cffi_e_enum_ipset_envopt(PyObject *lib)
{
  if ((IPSET_ENV_SORTED) <= 0 || (unsigned long)(IPSET_ENV_SORTED) != 1UL) {
    char buf[64];
    if ((IPSET_ENV_SORTED) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_ENV_SORTED));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_ENV_SORTED));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_envopt: ", "IPSET_ENV_SORTED", buf, "1");
    return -1;
  }
  if ((IPSET_ENV_QUIET) <= 0 || (unsigned long)(IPSET_ENV_QUIET) != 2UL) {
    char buf[64];
    if ((IPSET_ENV_QUIET) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_ENV_QUIET));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_ENV_QUIET));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_envopt: ", "IPSET_ENV_QUIET", buf, "2");
    return -1;
  }
  if ((IPSET_ENV_RESOLVE) <= 0 || (unsigned long)(IPSET_ENV_RESOLVE) != 4UL) {
    char buf[64];
    if ((IPSET_ENV_RESOLVE) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_ENV_RESOLVE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_ENV_RESOLVE));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_envopt: ", "IPSET_ENV_RESOLVE", buf, "4");
    return -1;
  }
  if ((IPSET_ENV_EXIST) <= 0 || (unsigned long)(IPSET_ENV_EXIST) != 8UL) {
    char buf[64];
    if ((IPSET_ENV_EXIST) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_ENV_EXIST));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_ENV_EXIST));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_envopt: ", "IPSET_ENV_EXIST", buf, "8");
    return -1;
  }
  if ((IPSET_ENV_LIST_SETNAME) <= 0 || (unsigned long)(IPSET_ENV_LIST_SETNAME) != 16UL) {
    char buf[64];
    if ((IPSET_ENV_LIST_SETNAME) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_ENV_LIST_SETNAME));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_ENV_LIST_SETNAME));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_envopt: ", "IPSET_ENV_LIST_SETNAME", buf, "16");
    return -1;
  }
  if ((IPSET_ENV_LIST_HEADER) <= 0 || (unsigned long)(IPSET_ENV_LIST_HEADER) != 32UL) {
    char buf[64];
    if ((IPSET_ENV_LIST_HEADER) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_ENV_LIST_HEADER));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_ENV_LIST_HEADER));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_envopt: ", "IPSET_ENV_LIST_HEADER", buf, "32");
    return -1;
  }
  return _cffi_e_enum_ipset_cmd(lib);
}

static int _cffi_e_enum_ipset_err_type(PyObject *lib)
{
  if ((IPSET_ERROR) > 0 || (long)(IPSET_ERROR) != 0L) {
    char buf[64];
    if ((IPSET_ERROR) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_ERROR));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_ERROR));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_err_type: ", "IPSET_ERROR", buf, "0");
    return -1;
  }
  if ((IPSET_WARNING) <= 0 || (unsigned long)(IPSET_WARNING) != 1UL) {
    char buf[64];
    if ((IPSET_WARNING) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_WARNING));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_WARNING));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_err_type: ", "IPSET_WARNING", buf, "1");
    return -1;
  }
  return _cffi_e_enum_ipset_envopt(lib);
}

static int _cffi_e_enum_ipset_opt(PyObject *lib)
{
  if ((IPSET_OPT_NONE) > 0 || (long)(IPSET_OPT_NONE) != 0L) {
    char buf[64];
    if ((IPSET_OPT_NONE) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_OPT_NONE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_OPT_NONE));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_opt: ", "IPSET_OPT_NONE", buf, "0");
    return -1;
  }
  if ((IPSET_SETNAME) <= 0 || (unsigned long)(IPSET_SETNAME) != 1UL) {
    char buf[64];
    if ((IPSET_SETNAME) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_SETNAME));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_SETNAME));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_opt: ", "IPSET_SETNAME", buf, "1");
    return -1;
  }
  if ((IPSET_OPT_TYPENAME) <= 0 || (unsigned long)(IPSET_OPT_TYPENAME) != 2UL) {
    char buf[64];
    if ((IPSET_OPT_TYPENAME) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_OPT_TYPENAME));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_OPT_TYPENAME));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_opt: ", "IPSET_OPT_TYPENAME", buf, "2");
    return -1;
  }
  if ((IPSET_OPT_FAMILY) <= 0 || (unsigned long)(IPSET_OPT_FAMILY) != 3UL) {
    char buf[64];
    if ((IPSET_OPT_FAMILY) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_OPT_FAMILY));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_OPT_FAMILY));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_opt: ", "IPSET_OPT_FAMILY", buf, "3");
    return -1;
  }
  if ((IPSET_OPT_IP) <= 0 || (unsigned long)(IPSET_OPT_IP) != 4UL) {
    char buf[64];
    if ((IPSET_OPT_IP) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_OPT_IP));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_OPT_IP));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_opt: ", "IPSET_OPT_IP", buf, "4");
    return -1;
  }
  if ((IPSET_OPT_IP_FROM) <= 0 || (unsigned long)(IPSET_OPT_IP_FROM) != 4UL) {
    char buf[64];
    if ((IPSET_OPT_IP_FROM) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_OPT_IP_FROM));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_OPT_IP_FROM));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_opt: ", "IPSET_OPT_IP_FROM", buf, "4");
    return -1;
  }
  if ((IPSET_OPT_IP_TO) <= 0 || (unsigned long)(IPSET_OPT_IP_TO) != 5UL) {
    char buf[64];
    if ((IPSET_OPT_IP_TO) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_OPT_IP_TO));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_OPT_IP_TO));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_opt: ", "IPSET_OPT_IP_TO", buf, "5");
    return -1;
  }
  if ((IPSET_OPT_CIDR) <= 0 || (unsigned long)(IPSET_OPT_CIDR) != 6UL) {
    char buf[64];
    if ((IPSET_OPT_CIDR) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_OPT_CIDR));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_OPT_CIDR));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_opt: ", "IPSET_OPT_CIDR", buf, "6");
    return -1;
  }
  if ((IPSET_OPT_MARK) <= 0 || (unsigned long)(IPSET_OPT_MARK) != 7UL) {
    char buf[64];
    if ((IPSET_OPT_MARK) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_OPT_MARK));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_OPT_MARK));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_opt: ", "IPSET_OPT_MARK", buf, "7");
    return -1;
  }
  if ((IPSET_OPT_PORT) <= 0 || (unsigned long)(IPSET_OPT_PORT) != 8UL) {
    char buf[64];
    if ((IPSET_OPT_PORT) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_OPT_PORT));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_OPT_PORT));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_opt: ", "IPSET_OPT_PORT", buf, "8");
    return -1;
  }
  if ((IPSET_OPT_PORT_FROM) <= 0 || (unsigned long)(IPSET_OPT_PORT_FROM) != 8UL) {
    char buf[64];
    if ((IPSET_OPT_PORT_FROM) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_OPT_PORT_FROM));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_OPT_PORT_FROM));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_opt: ", "IPSET_OPT_PORT_FROM", buf, "8");
    return -1;
  }
  if ((IPSET_OPT_PORT_TO) <= 0 || (unsigned long)(IPSET_OPT_PORT_TO) != 9UL) {
    char buf[64];
    if ((IPSET_OPT_PORT_TO) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_OPT_PORT_TO));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_OPT_PORT_TO));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_opt: ", "IPSET_OPT_PORT_TO", buf, "9");
    return -1;
  }
  if ((IPSET_OPT_TIMEOUT) <= 0 || (unsigned long)(IPSET_OPT_TIMEOUT) != 10UL) {
    char buf[64];
    if ((IPSET_OPT_TIMEOUT) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_OPT_TIMEOUT));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_OPT_TIMEOUT));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_opt: ", "IPSET_OPT_TIMEOUT", buf, "10");
    return -1;
  }
  if ((IPSET_OPT_GC) <= 0 || (unsigned long)(IPSET_OPT_GC) != 11UL) {
    char buf[64];
    if ((IPSET_OPT_GC) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_OPT_GC));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_OPT_GC));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_opt: ", "IPSET_OPT_GC", buf, "11");
    return -1;
  }
  if ((IPSET_OPT_HASHSIZE) <= 0 || (unsigned long)(IPSET_OPT_HASHSIZE) != 12UL) {
    char buf[64];
    if ((IPSET_OPT_HASHSIZE) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_OPT_HASHSIZE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_OPT_HASHSIZE));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_opt: ", "IPSET_OPT_HASHSIZE", buf, "12");
    return -1;
  }
  if ((IPSET_OPT_MAXELEM) <= 0 || (unsigned long)(IPSET_OPT_MAXELEM) != 13UL) {
    char buf[64];
    if ((IPSET_OPT_MAXELEM) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_OPT_MAXELEM));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_OPT_MAXELEM));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_opt: ", "IPSET_OPT_MAXELEM", buf, "13");
    return -1;
  }
  if ((IPSET_OPT_MARKMASK) <= 0 || (unsigned long)(IPSET_OPT_MARKMASK) != 14UL) {
    char buf[64];
    if ((IPSET_OPT_MARKMASK) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_OPT_MARKMASK));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_OPT_MARKMASK));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_opt: ", "IPSET_OPT_MARKMASK", buf, "14");
    return -1;
  }
  if ((IPSET_OPT_NETMASK) <= 0 || (unsigned long)(IPSET_OPT_NETMASK) != 15UL) {
    char buf[64];
    if ((IPSET_OPT_NETMASK) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_OPT_NETMASK));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_OPT_NETMASK));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_opt: ", "IPSET_OPT_NETMASK", buf, "15");
    return -1;
  }
  if ((IPSET_OPT_PROBES) <= 0 || (unsigned long)(IPSET_OPT_PROBES) != 16UL) {
    char buf[64];
    if ((IPSET_OPT_PROBES) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_OPT_PROBES));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_OPT_PROBES));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_opt: ", "IPSET_OPT_PROBES", buf, "16");
    return -1;
  }
  if ((IPSET_OPT_RESIZE) <= 0 || (unsigned long)(IPSET_OPT_RESIZE) != 17UL) {
    char buf[64];
    if ((IPSET_OPT_RESIZE) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_OPT_RESIZE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_OPT_RESIZE));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_opt: ", "IPSET_OPT_RESIZE", buf, "17");
    return -1;
  }
  if ((IPSET_OPT_SIZE) <= 0 || (unsigned long)(IPSET_OPT_SIZE) != 18UL) {
    char buf[64];
    if ((IPSET_OPT_SIZE) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_OPT_SIZE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_OPT_SIZE));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_opt: ", "IPSET_OPT_SIZE", buf, "18");
    return -1;
  }
  if ((IPSET_OPT_FORCEADD) <= 0 || (unsigned long)(IPSET_OPT_FORCEADD) != 19UL) {
    char buf[64];
    if ((IPSET_OPT_FORCEADD) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_OPT_FORCEADD));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_OPT_FORCEADD));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_opt: ", "IPSET_OPT_FORCEADD", buf, "19");
    return -1;
  }
  if ((IPSET_OPT_ELEMENTS) <= 0 || (unsigned long)(IPSET_OPT_ELEMENTS) != 20UL) {
    char buf[64];
    if ((IPSET_OPT_ELEMENTS) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_OPT_ELEMENTS));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_OPT_ELEMENTS));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_opt: ", "IPSET_OPT_ELEMENTS", buf, "20");
    return -1;
  }
  if ((IPSET_OPT_REFERENCES) <= 0 || (unsigned long)(IPSET_OPT_REFERENCES) != 21UL) {
    char buf[64];
    if ((IPSET_OPT_REFERENCES) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_OPT_REFERENCES));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_OPT_REFERENCES));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_opt: ", "IPSET_OPT_REFERENCES", buf, "21");
    return -1;
  }
  if ((IPSET_OPT_MEMSIZE) <= 0 || (unsigned long)(IPSET_OPT_MEMSIZE) != 22UL) {
    char buf[64];
    if ((IPSET_OPT_MEMSIZE) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_OPT_MEMSIZE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_OPT_MEMSIZE));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_opt: ", "IPSET_OPT_MEMSIZE", buf, "22");
    return -1;
  }
  if ((IPSET_OPT_ETHER) <= 0 || (unsigned long)(IPSET_OPT_ETHER) != 23UL) {
    char buf[64];
    if ((IPSET_OPT_ETHER) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_OPT_ETHER));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_OPT_ETHER));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_opt: ", "IPSET_OPT_ETHER", buf, "23");
    return -1;
  }
  if ((IPSET_OPT_NAME) <= 0 || (unsigned long)(IPSET_OPT_NAME) != 24UL) {
    char buf[64];
    if ((IPSET_OPT_NAME) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_OPT_NAME));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_OPT_NAME));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_opt: ", "IPSET_OPT_NAME", buf, "24");
    return -1;
  }
  if ((IPSET_OPT_NAMEREF) <= 0 || (unsigned long)(IPSET_OPT_NAMEREF) != 25UL) {
    char buf[64];
    if ((IPSET_OPT_NAMEREF) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_OPT_NAMEREF));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_OPT_NAMEREF));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_opt: ", "IPSET_OPT_NAMEREF", buf, "25");
    return -1;
  }
  if ((IPSET_OPT_IP2) <= 0 || (unsigned long)(IPSET_OPT_IP2) != 26UL) {
    char buf[64];
    if ((IPSET_OPT_IP2) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_OPT_IP2));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_OPT_IP2));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_opt: ", "IPSET_OPT_IP2", buf, "26");
    return -1;
  }
  if ((IPSET_OPT_CIDR2) <= 0 || (unsigned long)(IPSET_OPT_CIDR2) != 27UL) {
    char buf[64];
    if ((IPSET_OPT_CIDR2) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_OPT_CIDR2));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_OPT_CIDR2));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_opt: ", "IPSET_OPT_CIDR2", buf, "27");
    return -1;
  }
  if ((IPSET_OPT_IP2_TO) <= 0 || (unsigned long)(IPSET_OPT_IP2_TO) != 28UL) {
    char buf[64];
    if ((IPSET_OPT_IP2_TO) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_OPT_IP2_TO));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_OPT_IP2_TO));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_opt: ", "IPSET_OPT_IP2_TO", buf, "28");
    return -1;
  }
  if ((IPSET_OPT_PROTO) <= 0 || (unsigned long)(IPSET_OPT_PROTO) != 29UL) {
    char buf[64];
    if ((IPSET_OPT_PROTO) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_OPT_PROTO));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_OPT_PROTO));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_opt: ", "IPSET_OPT_PROTO", buf, "29");
    return -1;
  }
  if ((IPSET_OPT_IFACE) <= 0 || (unsigned long)(IPSET_OPT_IFACE) != 30UL) {
    char buf[64];
    if ((IPSET_OPT_IFACE) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_OPT_IFACE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_OPT_IFACE));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_opt: ", "IPSET_OPT_IFACE", buf, "30");
    return -1;
  }
  if ((IPSET_OPT_SETNAME2) <= 0 || (unsigned long)(IPSET_OPT_SETNAME2) != 31UL) {
    char buf[64];
    if ((IPSET_OPT_SETNAME2) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_OPT_SETNAME2));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_OPT_SETNAME2));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_opt: ", "IPSET_OPT_SETNAME2", buf, "31");
    return -1;
  }
  if ((IPSET_OPT_EXIST) <= 0 || (unsigned long)(IPSET_OPT_EXIST) != 32UL) {
    char buf[64];
    if ((IPSET_OPT_EXIST) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_OPT_EXIST));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_OPT_EXIST));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_opt: ", "IPSET_OPT_EXIST", buf, "32");
    return -1;
  }
  if ((IPSET_OPT_BEFORE) <= 0 || (unsigned long)(IPSET_OPT_BEFORE) != 33UL) {
    char buf[64];
    if ((IPSET_OPT_BEFORE) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_OPT_BEFORE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_OPT_BEFORE));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_opt: ", "IPSET_OPT_BEFORE", buf, "33");
    return -1;
  }
  if ((IPSET_OPT_PHYSDEV) <= 0 || (unsigned long)(IPSET_OPT_PHYSDEV) != 34UL) {
    char buf[64];
    if ((IPSET_OPT_PHYSDEV) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_OPT_PHYSDEV));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_OPT_PHYSDEV));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_opt: ", "IPSET_OPT_PHYSDEV", buf, "34");
    return -1;
  }
  if ((IPSET_OPT_NOMATCH) <= 0 || (unsigned long)(IPSET_OPT_NOMATCH) != 35UL) {
    char buf[64];
    if ((IPSET_OPT_NOMATCH) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_OPT_NOMATCH));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_OPT_NOMATCH));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_opt: ", "IPSET_OPT_NOMATCH", buf, "35");
    return -1;
  }
  if ((IPSET_OPT_COUNTERS) <= 0 || (unsigned long)(IPSET_OPT_COUNTERS) != 36UL) {
    char buf[64];
    if ((IPSET_OPT_COUNTERS) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_OPT_COUNTERS));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_OPT_COUNTERS));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_opt: ", "IPSET_OPT_COUNTERS", buf, "36");
    return -1;
  }
  if ((IPSET_OPT_PACKETS) <= 0 || (unsigned long)(IPSET_OPT_PACKETS) != 37UL) {
    char buf[64];
    if ((IPSET_OPT_PACKETS) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_OPT_PACKETS));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_OPT_PACKETS));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_opt: ", "IPSET_OPT_PACKETS", buf, "37");
    return -1;
  }
  if ((IPSET_OPT_BYTES) <= 0 || (unsigned long)(IPSET_OPT_BYTES) != 38UL) {
    char buf[64];
    if ((IPSET_OPT_BYTES) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_OPT_BYTES));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_OPT_BYTES));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_opt: ", "IPSET_OPT_BYTES", buf, "38");
    return -1;
  }
  if ((IPSET_OPT_CREATE_COMMENT) <= 0 || (unsigned long)(IPSET_OPT_CREATE_COMMENT) != 39UL) {
    char buf[64];
    if ((IPSET_OPT_CREATE_COMMENT) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_OPT_CREATE_COMMENT));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_OPT_CREATE_COMMENT));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_opt: ", "IPSET_OPT_CREATE_COMMENT", buf, "39");
    return -1;
  }
  if ((IPSET_OPT_ADT_COMMENT) <= 0 || (unsigned long)(IPSET_OPT_ADT_COMMENT) != 40UL) {
    char buf[64];
    if ((IPSET_OPT_ADT_COMMENT) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_OPT_ADT_COMMENT));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_OPT_ADT_COMMENT));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_opt: ", "IPSET_OPT_ADT_COMMENT", buf, "40");
    return -1;
  }
  if ((IPSET_OPT_SKBINFO) <= 0 || (unsigned long)(IPSET_OPT_SKBINFO) != 41UL) {
    char buf[64];
    if ((IPSET_OPT_SKBINFO) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_OPT_SKBINFO));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_OPT_SKBINFO));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_opt: ", "IPSET_OPT_SKBINFO", buf, "41");
    return -1;
  }
  if ((IPSET_OPT_SKBMARK) <= 0 || (unsigned long)(IPSET_OPT_SKBMARK) != 42UL) {
    char buf[64];
    if ((IPSET_OPT_SKBMARK) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_OPT_SKBMARK));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_OPT_SKBMARK));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_opt: ", "IPSET_OPT_SKBMARK", buf, "42");
    return -1;
  }
  if ((IPSET_OPT_SKBPRIO) <= 0 || (unsigned long)(IPSET_OPT_SKBPRIO) != 43UL) {
    char buf[64];
    if ((IPSET_OPT_SKBPRIO) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_OPT_SKBPRIO));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_OPT_SKBPRIO));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_opt: ", "IPSET_OPT_SKBPRIO", buf, "43");
    return -1;
  }
  if ((IPSET_OPT_SKBQUEUE) <= 0 || (unsigned long)(IPSET_OPT_SKBQUEUE) != 44UL) {
    char buf[64];
    if ((IPSET_OPT_SKBQUEUE) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_OPT_SKBQUEUE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_OPT_SKBQUEUE));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_opt: ", "IPSET_OPT_SKBQUEUE", buf, "44");
    return -1;
  }
  if ((IPSET_OPT_FLAGS) <= 0 || (unsigned long)(IPSET_OPT_FLAGS) != 48UL) {
    char buf[64];
    if ((IPSET_OPT_FLAGS) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_OPT_FLAGS));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_OPT_FLAGS));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_opt: ", "IPSET_OPT_FLAGS", buf, "48");
    return -1;
  }
  if ((IPSET_OPT_CADT_FLAGS) <= 0 || (unsigned long)(IPSET_OPT_CADT_FLAGS) != 49UL) {
    char buf[64];
    if ((IPSET_OPT_CADT_FLAGS) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_OPT_CADT_FLAGS));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_OPT_CADT_FLAGS));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_opt: ", "IPSET_OPT_CADT_FLAGS", buf, "49");
    return -1;
  }
  if ((IPSET_OPT_ELEM) <= 0 || (unsigned long)(IPSET_OPT_ELEM) != 50UL) {
    char buf[64];
    if ((IPSET_OPT_ELEM) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_OPT_ELEM));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_OPT_ELEM));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_opt: ", "IPSET_OPT_ELEM", buf, "50");
    return -1;
  }
  if ((IPSET_OPT_TYPE) <= 0 || (unsigned long)(IPSET_OPT_TYPE) != 51UL) {
    char buf[64];
    if ((IPSET_OPT_TYPE) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_OPT_TYPE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_OPT_TYPE));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_opt: ", "IPSET_OPT_TYPE", buf, "51");
    return -1;
  }
  if ((IPSET_OPT_LINENO) <= 0 || (unsigned long)(IPSET_OPT_LINENO) != 52UL) {
    char buf[64];
    if ((IPSET_OPT_LINENO) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_OPT_LINENO));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_OPT_LINENO));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_opt: ", "IPSET_OPT_LINENO", buf, "52");
    return -1;
  }
  if ((IPSET_OPT_REVISION) <= 0 || (unsigned long)(IPSET_OPT_REVISION) != 53UL) {
    char buf[64];
    if ((IPSET_OPT_REVISION) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_OPT_REVISION));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_OPT_REVISION));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_opt: ", "IPSET_OPT_REVISION", buf, "53");
    return -1;
  }
  if ((IPSET_OPT_REVISION_MIN) <= 0 || (unsigned long)(IPSET_OPT_REVISION_MIN) != 54UL) {
    char buf[64];
    if ((IPSET_OPT_REVISION_MIN) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_OPT_REVISION_MIN));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_OPT_REVISION_MIN));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_opt: ", "IPSET_OPT_REVISION_MIN", buf, "54");
    return -1;
  }
  if ((IPSET_OPT_MAX) <= 0 || (unsigned long)(IPSET_OPT_MAX) != 55UL) {
    char buf[64];
    if ((IPSET_OPT_MAX) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_OPT_MAX));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_OPT_MAX));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_opt: ", "IPSET_OPT_MAX", buf, "55");
    return -1;
  }
  return _cffi_e_enum_ipset_err_type(lib);
}

static int _cffi_e_enum_ipset_output_mode(PyObject *lib)
{
  if ((IPSET_LIST_NONE) > 0 || (long)(IPSET_LIST_NONE) != 0L) {
    char buf[64];
    if ((IPSET_LIST_NONE) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_LIST_NONE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_LIST_NONE));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_output_mode: ", "IPSET_LIST_NONE", buf, "0");
    return -1;
  }
  if ((IPSET_LIST_PLAIN) <= 0 || (unsigned long)(IPSET_LIST_PLAIN) != 1UL) {
    char buf[64];
    if ((IPSET_LIST_PLAIN) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_LIST_PLAIN));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_LIST_PLAIN));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_output_mode: ", "IPSET_LIST_PLAIN", buf, "1");
    return -1;
  }
  if ((IPSET_LIST_SAVE) <= 0 || (unsigned long)(IPSET_LIST_SAVE) != 2UL) {
    char buf[64];
    if ((IPSET_LIST_SAVE) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_LIST_SAVE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_LIST_SAVE));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_output_mode: ", "IPSET_LIST_SAVE", buf, "2");
    return -1;
  }
  if ((IPSET_LIST_XML) <= 0 || (unsigned long)(IPSET_LIST_XML) != 3UL) {
    char buf[64];
    if ((IPSET_LIST_XML) <= 0)
        snprintf(buf, 63, "%ld", (long)(IPSET_LIST_XML));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(IPSET_LIST_XML));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum ipset_output_mode: ", "IPSET_LIST_XML", buf, "3");
    return -1;
  }
  return _cffi_e_enum_ipset_opt(lib);
}

static PyObject *
_cffi_f_inet_pton(PyObject *self, PyObject *args)
{
  int x0;
  char const * x1;
  void * x2;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int result;
  PyObject *pyresult;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:inet_pton", &arg0, &arg1, &arg2))
    return NULL;

  x0 = _cffi_to_c_int(arg0, int);
  if (x0 == (int)-1 && PyErr_Occurred())
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(0), arg1, (char **)&x1);
  if (datasize != 0) {
    x1 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(0), arg1, (char **)&x1,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(1), arg2, (char **)&x2);
  if (datasize != 0) {
    x2 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(1), arg2, (char **)&x2,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = inet_pton(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_ipset_cmd(PyObject *self, PyObject *args)
{
  struct ipset_session * x0;
  enum ipset_cmd x1;
  uint32_t x2;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int result;
  PyObject *pyresult;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:ipset_cmd", &arg0, &arg1, &arg2))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(2), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(2), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  if (_cffi_to_c((char *)&x1, _cffi_type(3), arg1) < 0)
    return NULL;

  x2 = _cffi_to_c_int(arg2, uint32_t);
  if (x2 == (uint32_t)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = ipset_cmd(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_ipset_commit(PyObject *self, PyObject *arg0)
{
  struct ipset_session * x0;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int result;
  PyObject *pyresult;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(2), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(2), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = ipset_commit(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_ipset_data_get(PyObject *self, PyObject *args)
{
  struct ipset_data const * x0;
  enum ipset_opt x1;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  void const * result;
  PyObject *pyresult;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:ipset_data_get", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(4), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(4), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  if (_cffi_to_c((char *)&x1, _cffi_type(5), arg1) < 0)
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = ipset_data_get(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_pointer((char *)result, _cffi_type(6));
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_ipset_data_set(PyObject *self, PyObject *args)
{
  struct ipset_data * x0;
  enum ipset_opt x1;
  void const * x2;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int result;
  PyObject *pyresult;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:ipset_data_set", &arg0, &arg1, &arg2))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(7), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(7), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  if (_cffi_to_c((char *)&x1, _cffi_type(5), arg1) < 0)
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(6), arg2, (char **)&x2);
  if (datasize != 0) {
    x2 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(6), arg2, (char **)&x2,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = ipset_data_set(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_ipset_envopt_parse(PyObject *self, PyObject *args)
{
  struct ipset_session * x0;
  int x1;
  char const * x2;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int result;
  PyObject *pyresult;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:ipset_envopt_parse", &arg0, &arg1, &arg2))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(2), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(2), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, int);
  if (x1 == (int)-1 && PyErr_Occurred())
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(0), arg2, (char **)&x2);
  if (datasize != 0) {
    x2 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(0), arg2, (char **)&x2,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = ipset_envopt_parse(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_ipset_envopt_test(PyObject *self, PyObject *args)
{
  struct ipset_session * x0;
  enum ipset_envopt x1;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  _Bool result;
  PyObject *pyresult;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:ipset_envopt_test", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(2), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(2), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  if (_cffi_to_c((char *)&x1, _cffi_type(8), arg1) < 0)
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = ipset_envopt_test(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c__Bool(result);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_ipset_errcode(PyObject *self, PyObject *args)
{
  struct ipset_session * x0;
  enum ipset_cmd x1;
  int x2;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int result;
  PyObject *pyresult;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:ipset_errcode", &arg0, &arg1, &arg2))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(2), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(2), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  if (_cffi_to_c((char *)&x1, _cffi_type(3), arg1) < 0)
    return NULL;

  x2 = _cffi_to_c_int(arg2, int);
  if (x2 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = ipset_errcode(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_ipset_load_types(PyObject *self, PyObject *noarg)
{

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { ipset_load_types(); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  (void)noarg; /* unused */
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_ipset_saved_type(PyObject *self, PyObject *arg0)
{
  struct ipset_session const * x0;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  struct ipset_type const * result;
  PyObject *pyresult;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(10), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(10), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = ipset_saved_type(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_pointer((char *)result, _cffi_type(11));
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_ipset_session_data(PyObject *self, PyObject *arg0)
{
  struct ipset_session const * x0;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  struct ipset_data * result;
  PyObject *pyresult;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(10), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(10), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = ipset_session_data(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_pointer((char *)result, _cffi_type(7));
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_ipset_session_error(PyObject *self, PyObject *arg0)
{
  struct ipset_session const * x0;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  char const * result;
  PyObject *pyresult;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(10), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(10), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = ipset_session_error(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_pointer((char *)result, _cffi_type(0));
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_ipset_session_fini(PyObject *self, PyObject *arg0)
{
  struct ipset_session * x0;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int result;
  PyObject *pyresult;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(2), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(2), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = ipset_session_fini(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_ipset_session_handle(PyObject *self, PyObject *arg0)
{
  struct ipset_session const * x0;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  struct ipset_handle * result;
  PyObject *pyresult;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(10), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(10), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = ipset_session_handle(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_pointer((char *)result, _cffi_type(12));
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_ipset_session_init(PyObject *self, PyObject *arg0)
{
  int(* x0)(char const *, ...);
  struct ipset_session * result;
  PyObject *pyresult;

  x0 = (int(*)(char const *, ...))_cffi_to_c_pointer(arg0, _cffi_type(13));
  if (x0 == (int(*)(char const *, ...))NULL && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = ipset_session_init(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_pointer((char *)result, _cffi_type(2));
  return pyresult;
}

static PyObject *
_cffi_f_ipset_session_lineno(PyObject *self, PyObject *args)
{
  struct ipset_session * x0;
  uint32_t x1;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:ipset_session_lineno", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(2), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(2), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, uint32_t);
  if (x1 == (uint32_t)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { ipset_session_lineno(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_ipset_session_outfn(PyObject *self, PyObject *args)
{
  struct ipset_session * x0;
  int(* x1)(char const *, ...);
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int result;
  PyObject *pyresult;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:ipset_session_outfn", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(2), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(2), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  x1 = (int(*)(char const *, ...))_cffi_to_c_pointer(arg1, _cffi_type(13));
  if (x1 == (int(*)(char const *, ...))NULL && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = ipset_session_outfn(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_ipset_session_output(PyObject *self, PyObject *args)
{
  struct ipset_session * x0;
  enum ipset_output_mode x1;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int result;
  PyObject *pyresult;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:ipset_session_output", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(2), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(2), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  if (_cffi_to_c((char *)&x1, _cffi_type(14), arg1) < 0)
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = ipset_session_output(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static int _cffi_const_ipset_session_report(PyObject *lib)
{
  PyObject *o;
  int res;
  int(* i)(struct ipset_session *, enum ipset_err_type, char const *, ...);
  i = (ipset_session_report);
  o = _cffi_from_c_pointer((char *)i, _cffi_type(15));
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "ipset_session_report", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_e_enum_ipset_output_mode(lib);
}

static PyObject *
_cffi_f_ipset_session_report_reset(PyObject *self, PyObject *arg0)
{
  struct ipset_session * x0;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(2), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(2), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { ipset_session_report_reset(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_ipset_session_warning(PyObject *self, PyObject *arg0)
{
  struct ipset_session const * x0;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  char const * result;
  PyObject *pyresult;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(10), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(10), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = ipset_session_warning(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_pointer((char *)result, _cffi_type(0));
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_ipset_type_check(PyObject *self, PyObject *arg0)
{
  struct ipset_session * x0;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  struct ipset_type const * result;
  PyObject *pyresult;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(2), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(2), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = ipset_type_check(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_pointer((char *)result, _cffi_type(11));
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_ipset_type_get(PyObject *self, PyObject *args)
{
  struct ipset_session * x0;
  enum ipset_cmd x1;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  struct ipset_type const * result;
  PyObject *pyresult;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:ipset_type_get", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(2), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(2), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  if (_cffi_to_c((char *)&x1, _cffi_type(3), arg1) < 0)
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = ipset_type_get(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_pointer((char *)result, _cffi_type(11));
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static int _cffi_const_printf(PyObject *lib)
{
  PyObject *o;
  int res;
  int(* i)(char const *, ...);
  i = (printf);
  o = _cffi_from_c_pointer((char *)i, _cffi_type(13));
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "printf", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_ipset_session_report(lib);
}

static int _cffi_const_AF_INET(PyObject *lib)
{
  PyObject *o;
  int res;
  if ((AF_INET) <= 0 || (unsigned long)(AF_INET) != 2UL) {
    char buf[64];
    if ((AF_INET) <= 0)
        snprintf(buf, 63, "%ld", (long)(AF_INET));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(AF_INET));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "", "AF_INET", buf, "2");
    return -1;
  }
  o = _cffi_from_c_int_const(AF_INET);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AF_INET", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_printf(lib);
}

static int _cffi_const_AF_INET6(PyObject *lib)
{
  PyObject *o;
  int res;
  if ((AF_INET6) <= 0 || (unsigned long)(AF_INET6) != 10UL) {
    char buf[64];
    if ((AF_INET6) <= 0)
        snprintf(buf, 63, "%ld", (long)(AF_INET6));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(AF_INET6));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "", "AF_INET6", buf, "10");
    return -1;
  }
  o = _cffi_from_c_int_const(AF_INET6);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AF_INET6", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AF_INET(lib);
}

static void _cffi_check_struct_in6_addr(struct in6_addr *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  /* cannot generate 'union $2' in field '__in6_u': unknown type name */
}
static PyObject *
_cffi_layout_struct_in6_addr(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct in6_addr y; };
  static Py_ssize_t nums[] = {
    sizeof(struct in6_addr),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct in6_addr, __in6_u),
    sizeof(((struct in6_addr *)0)->__in6_u),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_in6_addr(0);
}

static void _cffi_check_struct_in_addr(struct in_addr *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->s_addr) << 1);
}
static PyObject *
_cffi_layout_struct_in_addr(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct in_addr y; };
  static Py_ssize_t nums[] = {
    sizeof(struct in_addr),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct in_addr, s_addr),
    sizeof(((struct in_addr *)0)->s_addr),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_in_addr(0);
}

static void _cffi_check_struct_ipset_errcode_table(struct ipset_errcode_table *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->errcode) << 1);
  { enum ipset_cmd *tmp = &p->cmd; (void)tmp; }
  { char const * *tmp = &p->message; (void)tmp; }
}
static PyObject *
_cffi_layout_struct_ipset_errcode_table(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct ipset_errcode_table y; };
  static Py_ssize_t nums[] = {
    sizeof(struct ipset_errcode_table),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct ipset_errcode_table, errcode),
    sizeof(((struct ipset_errcode_table *)0)->errcode),
    offsetof(struct ipset_errcode_table, cmd),
    sizeof(((struct ipset_errcode_table *)0)->cmd),
    offsetof(struct ipset_errcode_table, message),
    sizeof(((struct ipset_errcode_table *)0)->message),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_ipset_errcode_table(0);
}

static void _cffi_check_union_nf_inet_addr(union nf_inet_addr *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { uint32_t(*tmp)[4] = &p->all; (void)tmp; }
  (void)((p->ip) << 1);
  { uint32_t(*tmp)[4] = &p->ip6; (void)tmp; }
  { struct in_addr *tmp = &p->in; (void)tmp; }
  { struct in6_addr *tmp = &p->in6; (void)tmp; }
}
static PyObject *
_cffi_layout_union_nf_inet_addr(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; union nf_inet_addr y; };
  static Py_ssize_t nums[] = {
    sizeof(union nf_inet_addr),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(union nf_inet_addr, all),
    sizeof(((union nf_inet_addr *)0)->all),
    offsetof(union nf_inet_addr, ip),
    sizeof(((union nf_inet_addr *)0)->ip),
    offsetof(union nf_inet_addr, ip6),
    sizeof(((union nf_inet_addr *)0)->ip6),
    offsetof(union nf_inet_addr, in),
    sizeof(((union nf_inet_addr *)0)->in),
    offsetof(union nf_inet_addr, in6),
    sizeof(((union nf_inet_addr *)0)->in6),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_union_nf_inet_addr(0);
}

static int _cffi_setup_custom(PyObject *lib)
{
  return _cffi_const_AF_INET6(lib);
}

static PyMethodDef _cffi_methods[] = {
  {"inet_pton", _cffi_f_inet_pton, METH_VARARGS, NULL},
  {"ipset_cmd", _cffi_f_ipset_cmd, METH_VARARGS, NULL},
  {"ipset_commit", _cffi_f_ipset_commit, METH_O, NULL},
  {"ipset_data_get", _cffi_f_ipset_data_get, METH_VARARGS, NULL},
  {"ipset_data_set", _cffi_f_ipset_data_set, METH_VARARGS, NULL},
  {"ipset_envopt_parse", _cffi_f_ipset_envopt_parse, METH_VARARGS, NULL},
  {"ipset_envopt_test", _cffi_f_ipset_envopt_test, METH_VARARGS, NULL},
  {"ipset_errcode", _cffi_f_ipset_errcode, METH_VARARGS, NULL},
  {"ipset_load_types", _cffi_f_ipset_load_types, METH_NOARGS, NULL},
  {"ipset_saved_type", _cffi_f_ipset_saved_type, METH_O, NULL},
  {"ipset_session_data", _cffi_f_ipset_session_data, METH_O, NULL},
  {"ipset_session_error", _cffi_f_ipset_session_error, METH_O, NULL},
  {"ipset_session_fini", _cffi_f_ipset_session_fini, METH_O, NULL},
  {"ipset_session_handle", _cffi_f_ipset_session_handle, METH_O, NULL},
  {"ipset_session_init", _cffi_f_ipset_session_init, METH_O, NULL},
  {"ipset_session_lineno", _cffi_f_ipset_session_lineno, METH_VARARGS, NULL},
  {"ipset_session_outfn", _cffi_f_ipset_session_outfn, METH_VARARGS, NULL},
  {"ipset_session_output", _cffi_f_ipset_session_output, METH_VARARGS, NULL},
  {"ipset_session_report_reset", _cffi_f_ipset_session_report_reset, METH_O, NULL},
  {"ipset_session_warning", _cffi_f_ipset_session_warning, METH_O, NULL},
  {"ipset_type_check", _cffi_f_ipset_type_check, METH_O, NULL},
  {"ipset_type_get", _cffi_f_ipset_type_get, METH_VARARGS, NULL},
  {"_cffi_layout_struct_in6_addr", _cffi_layout_struct_in6_addr, METH_NOARGS, NULL},
  {"_cffi_layout_struct_in_addr", _cffi_layout_struct_in_addr, METH_NOARGS, NULL},
  {"_cffi_layout_struct_ipset_errcode_table", _cffi_layout_struct_ipset_errcode_table, METH_NOARGS, NULL},
  {"_cffi_layout_union_nf_inet_addr", _cffi_layout_union_nf_inet_addr, METH_NOARGS, NULL},
  {"_cffi_setup", _cffi_setup, METH_VARARGS, NULL},
  {NULL, NULL, 0, NULL}    /* Sentinel */
};

#if PY_MAJOR_VERSION >= 3

static struct PyModuleDef _cffi_module_def = {
  PyModuleDef_HEAD_INIT,
  "_cffi__x708f6d1dx9761ee40",
  NULL,
  -1,
  _cffi_methods,
  NULL, NULL, NULL, NULL
};

PyMODINIT_FUNC
PyInit__cffi__x708f6d1dx9761ee40(void)
{
  PyObject *lib;
  lib = PyModule_Create(&_cffi_module_def);
  if (lib == NULL)
    return NULL;
  if (((void)lib,0) < 0 || _cffi_init() < 0) {
    Py_DECREF(lib);
    return NULL;
  }
  return lib;
}

#else

PyMODINIT_FUNC
init_cffi__x708f6d1dx9761ee40(void)
{
  PyObject *lib;
  lib = Py_InitModule("_cffi__x708f6d1dx9761ee40", _cffi_methods);
  if (lib == NULL)
    return;
  if (((void)lib,0) < 0 || _cffi_init() < 0)
    return;
  return;
}

#endif
