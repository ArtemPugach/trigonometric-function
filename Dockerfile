# Temporary stage for building the application
FROM alpine:3.18 AS builder

# Install build dependencies
RUN apk add --no-cache \
    g++ \
    make \
    cmake \
    boost-dev \
    git

# Clone the public GitHub repository
WORKDIR /app
RUN git clone https://github.com/ArtemPugach/trigonometric-function.git .

# Move into the cloned repository directory where the Makefile is located
WORKDIR /app/trigonometric-function

# Ensure Makefile is visible and remove old build artifacts
RUN ls -l && find . -type f -name '*.o' -delete && rm -f program

# Build the application using Makefile
ARG TARGETARCH
RUN make ARCH=${TARGETARCH}

# Final stage for the runtime image
FROM alpine:3.18

# Set the working directory
WORKDIR /app

# Copy the executable from the builder stage
COPY --from=builder /app/trigonometric-function/program /usr/local/bin/

# Expose the server port (optional)
EXPOSE 8080

# Set the entrypoint to run the program
ENTRYPOINT ["program"]
