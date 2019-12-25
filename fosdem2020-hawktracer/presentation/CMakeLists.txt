find_package(LATEX COMPONENTS PDFLATEX)

add_custom_target(presentation ALL
    ${PDFLATEX_COMPILER} -output-directory="${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/hawktracer.tex"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    SOURCES hawktracer.tex)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/hawktracer.pdf" DESTINATION share/doc)