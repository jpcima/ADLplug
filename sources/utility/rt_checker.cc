//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifdef ADLplug_RT_CHECKER
#include <thread>
#include <stdio.h>
#include <stdlib.h>

static std::thread::id rt_thread_id;

void rt_checker_init()
{
    rt_thread_id = std::this_thread::get_id();
}

static void rt_check(const char *name)
{
    if (rt_thread_id == std::this_thread::get_id())
        fprintf(stderr, "%s was called in the RT thread\n", name);
}

extern "C"
void *__real_malloc(size_t size);
extern "C"
void *__real_realloc(void *ptr, size_t size);
extern "C"
void __real_free(void *ptr);
extern "C"
void *__real_memalign(size_t alignment, size_t size);

extern "C"
void *__wrap_malloc(size_t size)
{
    rt_check("malloc");
    return __real_malloc(size);
}

extern "C"
void *__wrap_realloc(void *ptr, size_t size)
{
    rt_check("realloc");
    return __real_realloc(ptr, size);
}

extern "C"
void __wrap_free(void *ptr)
{
    rt_check("free");
    return __real_free(ptr);
}

extern "C"
void *__wrap_memalign(size_t alignment, size_t size)
{
    rt_check("memalign");
    return __real_memalign(alignment, size);
}
#endif
