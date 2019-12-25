#include <hawktracer.h>
#include <hawktracer/event_macros_impl.h>

#include <unistd.h>
#include <stdlib.h>

// Declare new event type
HT_DECLARE_EVENT_KLASS(
    ResourceUsageEvent,              // event type name
    HT_Event,                        // base event type
    (INTEGER, size_t, memory_usage), // field
    (INTEGER, size_t, alloc_count))  // field

typedef struct {
    HT_MemoryUsageContext* memory;
    size_t alloc_count;
} Context;

HT_Boolean publish_resource_usage(Context* ctx);

void malloc_counter(void* ptr, size_t size, void* user_data)
{
    Context* context = (Context*)user_data;
    context->alloc_count++;
}

int main(int argc, char** argv)
{
    // initialize library
    ht_init(argc, argv);


    // register ResourceUsageEvent event type
    ht_ResourceUsageEvent_register_event_klass();

    ht_tcp_listener_register(ht_global_timeline_get(), 8765, 0, NULL);

    Context ctx = {
        ht_memory_usage_context_create(NULL),
        0
    };

    ht_alloc_hooks_register_hooks(malloc_counter, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &ctx);

    HT_TaskScheduler* scheduler = ht_task_scheduler_create(NULL);
    ht_task_scheduler_schedule_task(scheduler,
        HT_TASK_SCHEDULING_RESTART_TIMER,
        20 * 1000 * 1000, // 20 ms
        publish_resource_usage,
        &ctx);

    for (int i = 0; i < 100000; i++)
    {
        malloc(1024);
        usleep(100); // 0.1ms
        ht_task_scheduler_tick(scheduler);
    }

    ht_task_scheduler_destroy(scheduler);

    ht_memory_usage_context_destroy(ctx.memory);

    // release resources
    ht_deinit();

    return 0;
}

HT_Boolean publish_resource_usage(Context* ctx)
{
    size_t virtual_memory_bytes;
    ht_memory_usage_get_usage(ctx->memory, &virtual_memory_bytes, NULL, NULL);

    // Push event to global timeline
    HT_TIMELINE_PUSH_EVENT(
        ht_global_timeline_get(), // timeline
        ResourceUsageEvent,       // event type
        virtual_memory_bytes,     // memory_usage value
        ctx->alloc_count);        // alloc_count value

    return HT_TRUE;
}
