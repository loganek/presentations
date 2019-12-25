add_executable(hello-world main.c)
target_link_libraries(hello-world HawkTracer::hawktracer)

install(TARGETS hello-world RUNTIME DESTINATION bin)
