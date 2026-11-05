# Base Linux environment
FROM ubuntu:22.04

# Avoid interactive prompts during install
ENV DEBIAN_FRONTEND=noninteractive

# Install build dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    libsdl2-dev \
    x11-apps \
    libgl1-mesa-dev \
    && rm -rf /var/lib/apt/lists/*

# Create working directory
WORKDIR /app

# Copy source code into container
COPY . .

# Configure project
RUN cmake -S . -B build

# Build project
RUN cmake --build build

# Default command
CMD ["./build/game_main"]