FROM ubuntu:24.04

# Install required packages
RUN apt update && apt install -y \
  clang-18 lld-18                \
  cmake                          \
  parted dosfstools              \
  udev

# Install emulation dependencies
RUN apt install -y \
  qemu-system-x86

# Configure llvm tool aliases
RUN update-alternatives --install /usr/bin/cc cc /usr/bin/clang-18 100 \
  && update-alternatives --install /usr/bin/c++ c++ /usr/bin/clang++-18 100 \
  && update-alternatives --install /usr/bin/ld ld /usr/bin/ld.lld-18 100

# Set up work directory and volumes
VOLUME [ "/app" ]
WORKDIR /app

# WesOS build container controller script
RUN echo '#!/bin/bash' > /opt/build_type.sh &&                                   \
  echo 'BUILD_TYPE="$1"' >> /opt/build_type.sh &&                                \
  echo 'CMAKE_ARGS="${@:2}"' >> /opt/build_type.sh &&                            \
  echo '' >> /opt/build_type.sh &&                                               \
  echo 'if [[ "$BUILD_TYPE" != "rapid" && "$BUILD_TYPE" != "debug" &&            \
  "$BUILD_TYPE" != "release" ]]; then' >> /opt/build_type.sh &&                  \
  echo '  echo "Error: Invalid build type: '\''$BUILD_TYPE'\''"'                 \
  >> /opt/build_type.sh &&                                                       \
  echo '  echo "Must be one of: rapid, debug, release"' >> /opt/build_type.sh && \
  echo '  exit 1' >> /opt/build_type.sh &&                                       \
  echo 'fi' >> /opt/build_type.sh &&                                             \
  echo '' >> /opt/build_type.sh &&                                               \
  echo 'echo "Selected build mode: $BUILD_TYPE"' >> /opt/build_type.sh &&        \
  echo 'echo "CMake arguments: $CMAKE_ARGS"' >> /opt/build_type.sh &&            \
  echo '  mkdir -p /app/.build/$BUILD_TYPE' >> /opt/build_type.sh &&             \
  echo 'if [[ "$BUILD_TYPE" == "rapid" ]]; then' >> /opt/build_type.sh &&        \
  echo '  cmake -S /app -B /app/.build/rapid -DCMAKE_BUILD_TYPE=Rapid            \
  -DCMAKE_INSTALL_PREFIX=/app/install/$BUILD_TYPE $CMAKE_ARGS'                   \
  >> /opt/build_type.sh &&                                                       \
  echo 'elif [[ "$BUILD_TYPE" == "debug" ]]; then' >> /opt/build_type.sh &&      \
  echo '  cmake -S /app -B /app/.build/debug -DCMAKE_BUILD_TYPE=Debug            \
  -DCMAKE_INSTALL_PREFIX=/app/install/$BUILD_TYPE $CMAKE_ARGS'                   \
  >> /opt/build_type.sh &&                                                       \
  echo 'elif [[ "$BUILD_TYPE" == "release" ]]; then' >> /opt/build_type.sh &&    \
  echo '  cmake -S /app -B /app/.build/release -DCMAKE_BUILD_TYPE=Release        \
  -DCMAKE_INSTALL_PREFIX=/app/install/$BUILD_TYPE $CMAKE_ARGS'                   \
  >> /opt/build_type.sh &&                                                       \
  echo 'fi' >> /opt/build_type.sh &&                                             \
  echo 'cmake --build /app/.build/$BUILD_TYPE' >> /opt/build_type.sh &&          \
  echo 'if [[ "$?" -ne 0 ]]; then' >> /opt/build_type.sh &&                      \
  echo '  echo "Build failed for mode: $BUILD_TYPE"' >> /opt/build_type.sh &&    \
  echo '  exit 1' >> /opt/build_type.sh &&                                       \
  echo 'fi' >> /opt/build_type.sh &&                                             \
  echo '' >> /opt/build_type.sh &&                                               \
  echo 'cmake --install /app/.build/$BUILD_TYPE' >> /opt/build_type.sh &&        \
  echo 'if [[ "$?" -ne 0 ]]; then' >> /opt/build_type.sh &&                      \
  echo '  echo "Install failed for mode: $BUILD_TYPE"' >> /opt/build_type.sh &&  \
  echo '  exit 1' >> /opt/build_type.sh &&                                       \
  echo 'fi' >> /opt/build_type.sh &&                                             \
  echo '' >> /opt/build_type.sh &&                                               \
  echo 'echo "Build completed for mode: $BUILD_TYPE"' >> /opt/build_type.sh &&   \
  chmod +x /opt/build_type.sh

ENTRYPOINT [ "/opt/build_type.sh" ]
