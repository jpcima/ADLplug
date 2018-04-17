//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifdef ADLplug_RT_CHECKER
#include <thread>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

static std::thread::id rt_thread_id;

void rt_checker_init()
{
    rt_thread_id = std::this_thread::get_id();
}

static void no_malloc_vprintf(int fd, const char *fmt, va_list ap)
{
    char msg[256];
    unsigned n = vsnprintf(msg, sizeof(msg), fmt, ap);
    if ((int)n <= 0)
        return;
    msg[sizeof(msg) - 1] = 0;
    write(fd, msg, n);
}

static void no_malloc_printf(int fd, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    no_malloc_vprintf(fd, fmt, ap);
    va_end(ap);
}

static void rt_check(const char *name)
{
    if (rt_thread_id == std::this_thread::get_id()) {
        no_malloc_printf(STDERR_FILENO, "call to \"%s\" in the RT thread\n", name);
        if (0)
            raise(SIGTRAP);
    }
}

extern "C" {

void *__real_malloc(size_t size);
void *__real_realloc(void *ptr, size_t size);
void __real_free(void *ptr);
void *__real_memalign(size_t alignment, size_t size);

void *__wrap_malloc(size_t size)
{
    rt_check("malloc");
    return __real_malloc(size);
}

void *__wrap_realloc(void *ptr, size_t size)
{
    rt_check("realloc");
    return __real_realloc(ptr, size);
}

void __wrap_free(void *ptr)
{
    if (ptr)
        rt_check("free");
    return __real_free(ptr);
}

void *__wrap_memalign(size_t alignment, size_t size)
{
    rt_check("memalign");
    return __real_memalign(alignment, size);
}

}  // extern "C"

void *operator new(size_t sz)
{
    rt_check("operator new");
    void *p = __real_malloc(sz);
    if (!p)
        throw std::bad_alloc();
    return p;
}

void *operator new(size_t sz, const std::nothrow_t &)
{
    rt_check("operator new");
    void *p = __real_malloc(sz);
    return p;
}

void *operator new[](size_t sz)
{
    rt_check("operator new[]");
    void *p = __real_malloc(sz);
    if (!p)
        throw std::bad_alloc();
    return p;
}

void *operator new[](size_t sz, const std::nothrow_t &)
{
    rt_check("operator new[]");
    void *p = __real_malloc(sz);
    return p;
}

void operator delete[](void *ptr)
{
    rt_check("operator delete[]");
    __real_free(ptr);
}

void operator delete[](void *ptr, unsigned long)
{
    rt_check("operator delete[]");
    __real_free(ptr);
}

void operator delete[](void *ptr, const std::nothrow_t &)
{
    rt_check("operator delete[]");
    __real_free(ptr);
}

void operator delete(void *ptr)
{
    rt_check("operator delete");
    __real_free(ptr);
}

void operator delete(void *ptr, unsigned long)
{
    rt_check("operator delete");
    __real_free(ptr);
}

void operator delete(void *ptr, const std::nothrow_t &)
{
    rt_check("operator delete");
    __real_free(ptr);
}

#endif
