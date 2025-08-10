list(APPEND inc_list
    ${COMPONENT_DIR}/comm/include
    ${COMPONENT_DIR}/misc/include
    ${COMPONENT_DIR}/config
    ${COMPONENT_DIR}/driver/include
    ${COMPONENT_DIR}/bsp/include
    ${COMPONENT_DIR}/gui/include
)

list (APPEND src_list
    ${COMPONENT_DIR}/main.cpp
    ${COMPONENT_DIR}/comm/tcp_data_handle.cpp
    ${COMPONENT_DIR}/comm/tcp_client.cpp
    ${COMPONENT_DIR}/comm/comm.cpp
    ${COMPONENT_DIR}/misc/cmds.cpp
    ${COMPONENT_DIR}/driver/gt911.cpp
    ${COMPONENT_DIR}/driver/ch422g.cpp
    ${COMPONENT_DIR}/driver/ioexpand.cpp
    ${COMPONENT_DIR}/driver/sd_card.cpp
    ${COMPONENT_DIR}/bsp/bsp.cpp
    ${COMPONENT_DIR}/gui/lv_port_disp.cpp
)

idf_component_register(
    SRCS
    ${src_list}
    INCLUDE_DIRS
    ${inc_list}
)
