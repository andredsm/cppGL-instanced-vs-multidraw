# CMake Shader Stringification Module
# Pure CMake solution to convert shader files to C++ header files

# Function to add shader headers to a target
function(add_shader_headers TARGET_NAME)
    set(SHADER_DIR "${CMAKE_SOURCE_DIR}/src/shaders")
    set(GENERATED_DIR "${CMAKE_BINARY_DIR}/generated/shaders")
    
    # Find all shader files
    file(GLOB_RECURSE SHADER_FILES 
        "${SHADER_DIR}/*.vert"
        "${SHADER_DIR}/*.frag"
        "${SHADER_DIR}/*.geom"
        "${SHADER_DIR}/*.comp"
        "${SHADER_DIR}/*.tesc"
        "${SHADER_DIR}/*.tese"
    )
    
    set(GENERATED_HEADERS "")
    
    # Generate header for each shader file
    foreach(SHADER_FILE ${SHADER_FILES})
        # Get relative path and create output filename
        file(RELATIVE_PATH REL_PATH "${SHADER_DIR}" "${SHADER_FILE}")
        get_filename_component(SHADER_NAME "${REL_PATH}" NAME_WE)
        get_filename_component(SHADER_EXT "${REL_PATH}" EXT)
        
        # Create appropriate header name
        if(SHADER_EXT STREQUAL ".vert")
            set(HEADER_SUFFIX "_vertex")
        elseif(SHADER_EXT STREQUAL ".frag")
            set(HEADER_SUFFIX "_fragment")
        elseif(SHADER_EXT STREQUAL ".geom")
            set(HEADER_SUFFIX "_geometry")
        elseif(SHADER_EXT STREQUAL ".comp")
            set(HEADER_SUFFIX "_compute")
        elseif(SHADER_EXT STREQUAL ".tesc")
            set(HEADER_SUFFIX "_tess_control")
        elseif(SHADER_EXT STREQUAL ".tese")
            set(HEADER_SUFFIX "_tess_evaluation")
        else()
            set(HEADER_SUFFIX "_shader")
        endif()
        
        set(OUTPUT_HEADER "${GENERATED_DIR}/${SHADER_NAME}${HEADER_SUFFIX}.h")
        
        # Add custom command to generate header using CMake script mode
        add_custom_command(
            OUTPUT "${OUTPUT_HEADER}"
            COMMAND ${CMAKE_COMMAND} 
                -DSHADER_FILE="${SHADER_FILE}"
                -DOUTPUT_HEADER="${OUTPUT_HEADER}"
                -P "${CMAKE_SOURCE_DIR}/cmake/stringify_single_shader.cmake"
            DEPENDS "${SHADER_FILE}"
            COMMENT "Generating header for ${SHADER_FILE}"
            VERBATIM
        )
        
        list(APPEND GENERATED_HEADERS "${OUTPUT_HEADER}")
    endforeach()
    
    # Create a custom target for all generated headers
    add_custom_target(${TARGET_NAME}_shaders
        DEPENDS ${GENERATED_HEADERS}
        COMMENT "Generating all shader headers"
    )
    
    # Make the main target depend on shader generation
    add_dependencies(${TARGET_NAME} ${TARGET_NAME}_shaders)
    
    # Set a property so other parts can access the generated headers
    set_target_properties(${TARGET_NAME} PROPERTIES GENERATED_SHADER_HEADERS "${GENERATED_HEADERS}")
endfunction()
