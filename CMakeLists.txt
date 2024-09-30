cmake_minimum_required(VERSION 3.5)

idf_component_register(
    SRCS "src/OpenTherm.cpp"
    INCLUDE_DIRS "." "src"
    PRIV_REQUIRES arduino
    )

project (OpenTherm)
