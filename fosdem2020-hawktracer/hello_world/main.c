#include <hawktracer.h>
#include <hawktracer/event_macros_impl.h>

// Custom event's class definition
HT_DECLARE_EVENT_KLASS(
    MyEvent,                      // Event class name
    HT_Event,                     // Base event type
    (INTEGER, uint8_t, field_1),  // first field
    (STRING, char*, field_2))     // second field


int main(int argc, char** argv)
{
    // initialize HawkTracer
    ht_init(argc, argv);

    // Register event in HawkTracer system
    ht_MyEvent_register_event_klass();

    // Register listener for the timeline
    ht_file_dump_listener_register(
        ht_global_timeline_get(), // timeline (we use global timeline here)
        "hello-world.htdump",    // file name
        2048,                     // internal buffer size
        NULL);                    // for error handling

    // Push event to a timeline
    HT_TIMELINE_PUSH_EVENT(
        ht_global_timeline_get(), // timeline
        MyEvent,                  // event's class name
        28,                       // field_1
        "Hello World!");          // field_2

    // Release resources
    ht_deinit();

    return 0;
}