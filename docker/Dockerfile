# Use Ubuntu 24.04 as the base image
FROM ubuntu:24.04

# Set environment variables to avoid interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Update package list and install required packages
RUN apt update && apt install -y \
    g++ \
    cmake \
    git \
    build-essential \
    ninja-build \
    libboost-all-dev \
    libssl-dev \
    gdb \
    lldb \
    clang-format \
    valgrind \
    libcurl4-openssl-dev \
    libgtest-dev \
    && rm -rf /var/lib/apt/lists/*

# Build and install Google Test
RUN cd /usr/src/gtest && \
    cmake CMakeLists.txt && \
    make && \
    cp lib/*.a /usr/lib/

# Set the working directory
WORKDIR /app

# Copy the project files into the container
COPY . .

# Create a non-root user for security
RUN useradd -m -s /bin/bash developer && chown -R developer:developer /app

# Switch to the non-root user
USER developer

# Set the default command
CMD ["/bin/bash"]
