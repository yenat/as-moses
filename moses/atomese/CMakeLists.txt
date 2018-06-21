ADD_EXECUTABLE(atomese_interpreter atomese_interpreter.cc)
TARGET_LINK_LIBRARIES (atomese_interpreter
        comboreduct
        ${COGUTIL_LIBRARY}
        ${ATOMSPACE_LIBRARIES}
        )



#install header files
INSTALL(FILES

        atomese_representation.h

        DESTINATION

        "include/${PROJECT_NAME}/comboreduct/atomese_representation"
        )
