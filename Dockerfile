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

# Додання прав на виконання для програмного файлу
RUN chmod +x ./program

# Фінальний етап
FROM alpine:3.18

# Встановлення бібліотек, потрібних для виконання програми
RUN apk add --no-cache libstdc++

# Робоча директорія
WORKDIR /app

# Копіювання виконуваного файлу з етапу зборки
COPY --from=builder /app/program /usr/local/bin/

# Відкриття порту (опціонально)
EXPOSE 8080

# Запуск програми
ENTRYPOINT ["program"]

