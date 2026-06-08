# Dockerfile for joysh - Production-grade UNIX shell
FROM ubuntu:22.04

# Metadata
LABEL maintainer="joysh-dev"
LABEL description="Production-grade UNIX shell demonstrating Linux systems programming"

# Install build tools
RUN apt-get update && apt-get install -y \
    gcc \
    make \
    gdb \
    valgrind \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /joysh

# Copy source code
COPY . .

# Build the shell
RUN make clean && make release

# Set environment
ENV PATH="/joysh/bin:${PATH}"

# Default command
CMD ["/joysh/bin/joysh"]
