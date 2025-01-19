# Temporary stage for building the application
FROM alpine:3.18 AS builder

# Install build dependencies
RUN apk add --no-cache \
    g++ \
    make \
    cmake \
    libstdc++ \
    boost-dev \
    git

# Set working directory
WORKDIR /app

# Clone the repository
RUN git clone https://github.com/ArtemPugach/trigonometric-function.git .

# Check the directory contents
RUN ls -la /app

# Clean up old files
RUN find /app -name '*.o' -delete && rm -f /app/program

# Build the application
RUN make -f /app/Makefile

# Final stage for the runtime image
FROM alpine:3.18

# Set working directory
WORKDIR /app

# Copy the executable from the builder stage
COPY --from=builder /app/program /usr/local/bin/

# Expose the server port (optional)
EXPOSE 8080

# Set the entrypoint to run the program
ENTRYPOINT ["program"]

