#include <hawktracer.h>

#include <thread>
#include <unistd.h>


int fibonacci(int n)
{
    std::string message = "fibonacci " + std::to_string(n);
    HT_G_TRACE_OPT_DYNAMIC(message.c_str());

    usleep(1000);

    if(n == 0)
    {
        return 0;
    }
    else if(n == 1) {
        return 1;
    }
    else
    {
        return (fibonacci(n-1) + fibonacci(n-2));
    }
}

void run()
{
    HT_G_TRACE_FUNCTION()

    std::thread([] {
        HT_G_TRACE_OPT_STATIC("new_thread");
        fibonacci(5);
    }).join();
}

int main(int argc, char** argv)
{
    ht_init(argc, argv);

    ht_file_dump_listener_register(ht_global_timeline_get(), "base.htdump", 1024, NULL);
    // ht_tcp_listener_register(ht_global_timeline_get(), 8899, 2048, NULL);

    run();

    ht_deinit();

    return 0;
}