# Temporary stage for building the application
FROM alpine:3.18 AS builder

# Install build dependencies
RUN apk add --no-cache \
    g++ \
    make \
    cmake \
    boost-dev \
    git \
    aarch64-linux-gnu-g++

# Clone the public GitHub repository
WORKDIR /app
RUN git clone https://github.com/ArtemPugach/trigonometric-function.git .

# Build the application using Makefile
RUN make ARCH=$(uname -m)

# Final stage for the runtime image
FROM alpine:3.18

# Set the working directory
WORKDIR /app

# Copy the executable from the builder stage
COPY --from=builder /app/program /usr/local/bin/

# Expose the server port (optional)
EXPOSE 8080

# Set the entrypoint to run the program
ENTRYPOINT ["program"]

