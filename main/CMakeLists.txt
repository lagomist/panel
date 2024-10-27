list(APPEND inc_list
    ${COMPONENT_DIR}/comm/include
    ${COMPONENT_DIR}/misc/include
)

list (APPEND src_list
    ${COMPONENT_DIR}/main.cpp
    ${COMPONENT_DIR}/comm/tcp_client.cpp
    ${COMPONENT_DIR}/misc/cmds.cpp
)

idf_component_register(
    SRCS
    ${src_list}
    INCLUDE_DIRS
    ${inc_list}
    )
