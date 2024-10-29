cmake_minimum_required(VERSION 3.5)
idf_component_register(SRCS "src/Ed25519.cpp" "src/Curve25519.cpp" "src/Crypto.cpp" "src/RNG.cpp" "src/BigNumberUtil.cpp" "src/NoiseSource.cpp" "src/ChaCha.cpp" "src/Cipher.cpp" "src/SHA512.cpp" "src/Hash.cpp"
                    INCLUDE_DIRS "src/."
                    REQUIRES "arduino" "nvs_flash" # Library requires Arduino
                    )

project(Crypto)
