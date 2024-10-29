cmake_minimum_required(VERSION 3.5)
idf_component_register(SRCS "src/JSON.cpp" "src/JSONVar.cpp" "src/cjson/cJSON.c"
                    INCLUDE_DIRS "src/."
                    REQUIRES "arduino"
                    )

project(Arduino_JSON)