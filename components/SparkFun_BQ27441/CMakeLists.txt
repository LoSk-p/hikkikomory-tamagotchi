cmake_minimum_required(VERSION 3.5)
idf_component_register(SRCS "src/SparkFunBQ27441.cpp"
                    INCLUDE_DIRS "src/."
                    REQUIRES "arduino" 
                    )

project(SparkFun_BQ27441)
