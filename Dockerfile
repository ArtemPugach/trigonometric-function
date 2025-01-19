# Етап зборки
FROM alpine:3.18 AS builder

# Встановлення залежностей для збірки
RUN apk add --no-cache \
    g++ \
    make \
    cmake \
    libstdc++ \
    boost-dev \
    git

# Робоча директорія
WORKDIR /app

# Копіювання локальних файлів
COPY . /app

# Очищення попередніх артефактів
RUN find . -name '*.o' -delete && rm -f ./program

# Білд програми
RUN make

# Фінальний етап
FROM alpine:3.18

# Встановлення бібліотек, потрібних для виконання програми
RUN apk add --no-cache libstdc++ musl musl-dev && \
    wget -q -O /etc/apk/keys/sgerrand.rsa.pub https://alpine-pkgs.sgerrand.com/sgerrand.rsa.pub && \
    wget -q https://github.com/sgerrand/alpine-pkg-glibc/releases/download/2.34-r0/glibc-2.34-r0.apk && \
    apk add --force-overwrite glibc-2.34-r0.apk && \
    rm -f glibc-2.34-r0.apk


# Робоча директорія
WORKDIR /app

# Копіювання виконуваного файлу з етапу зборки
COPY --from=builder /app/program /app/

# Відкриття порту
EXPOSE 8080

# Запуск програми
ENTRYPOINT ["./program"]

