add_executable(resource-usage main.c)

target_link_libraries(resource-usage HawkTracer::hawktracer)
install(TARGETS resource-usage RUNTIME DESTINATION bin)
install(PROGRAMS client.py DESTINATION bin)