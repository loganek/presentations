find_package(CURL REQUIRED)

include_directories(${CURL_INCLUDE_DIR})

add_library(downloader
    SHARED
    downloader.c)

target_link_libraries(downloader ${CURL_LIBRARIES} HawkTracer::hawktracer)

add_custom_target(image-rotation ALL
    ${CMAKE_COMMAND} -E env DOWNLOADER_LIB=${CMAKE_CURRENT_BINARY_DIR} cargo build --target-dir ${CMAKE_CURRENT_BINARY_DIR}/target/
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS downloader)

install(TARGETS downloader LIBRARY DESTINATION lib)
install(CODE "
    execute_process(
        COMMAND
        ${CMAKE_COMMAND} -E env DOWNLOADER_LIB=${CMAKE_CURRENT_BINARY_DIR}
        cargo install
            --root=${CMAKE_INSTALL_PREFIX}
            --path=${CMAKE_CURRENT_SOURCE_DIR})")