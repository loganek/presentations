add_executable(time-measurement main.cpp)
target_link_libraries(time-measurement HawkTracer::hawktracer)

install(TARGETS time-measurement RUNTIME DESTINATION bin)
install(PROGRAMS time-measurement.py DESTINATION bin)