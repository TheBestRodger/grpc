# Путь к заголовочным файлам
set(INCLUDE_PATH ${CMAKE_SOURCE_DIR}/include)

# Путь к искодным текстам программ
set(SOURCE_PATH ${CMAKE_SOURCE_DIR}/source)

# Путь к собранным исполняемым модулям
set(BIN_PATH ${CMAKE_SOURCE_DIR}/bin)

# Путь к собранным статическим библиотекам
set(LIB_PATH ${CMAKE_SOURCE_DIR}/3th)

# Путь к библиотеке boost
set(BOOST_PATH D:/local/boost_1_82_0)

# Путь к библиотеке Qt
set(QT_PATH "D:\\QT\\5.14.2\\msvc2017_64")

# Путь к каталогу собранной библиотеки gRPC - https://grpc.io/
set(GRPC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../grpc)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})

