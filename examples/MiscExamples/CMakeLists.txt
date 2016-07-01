project( MiscExamples C )
set( PROGRAM_TARGETS
    drawContours
)

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

foreach( TARGET_NAME ${PROGRAM_TARGETS} )
	add_executable( ${TARGET_NAME} ${TARGET_NAME}.cpp )
    set_target_properties( ${TARGET_NAME} PROPERTIES LINKER_LANGUAGE C )
	target_link_libraries( ${TARGET_NAME} Utils ${OpenCV_LIBS} ${QT_LIBRARIES} )
    set_target_properties( ${TARGET_NAME} PROPERTIES FOLDER MiscExamples )
    install( TARGETS ${TARGET_NAME} DESTINATION bin )
endforeach( TARGET_NAME )
