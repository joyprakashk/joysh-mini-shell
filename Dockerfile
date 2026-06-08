# Dockerfile for joysh - Production-grade UNIX shell

# ---- Stage 1: Build ----
# Use a full Ubuntu image with build tools to compile the application
FROM ubuntu:22.04 AS builder

# Metadata
LABEL maintainer="joysh-dev"
LABEL description="Build environment for joysh"

# Install build tools
RUN apt-get update && apt-get install -y \
    gcc \
    make \
    && rm -rf /var/lib/apt/lists/*

# Set working directory and copy source
WORKDIR /joysh
COPY . .

# Build the shell
RUN make clean && make release

# ---- Stage 2: Final Image ----
# Use a minimal base image for the final product
FROM ubuntu:22.04

# Metadata
LABEL maintainer="joysh-dev"
LABEL description="Production-grade UNIX shell demonstrating Linux systems programming"

# Set working directory
WORKDIR /joysh

# Copy only the compiled binary from the 'builder' stage
COPY --from=builder /joysh/bin/joysh /joysh/bin/joysh

# Set environment
ENV PATH="/joysh/bin:${PATH}"

# Default command
CMD ["joysh"]