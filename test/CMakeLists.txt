set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

#add opc-server
add_subdirectory(Client_v2)

add_subdirectory(Server_v2)

#add queue test

add_subdirectory(QueueTest)

add_subdirectory(VerarbeiterTest)


add_subdirectory(ConfigTest)

add_subdirectory(InternalsTest)

ADD_EXECUTABLE(QueueTest queueTest.c ../src/queue.c)
TARGET_LINK_LIBRARIES(QueueTest pthread)
add_subdirectory(QueueTest)


