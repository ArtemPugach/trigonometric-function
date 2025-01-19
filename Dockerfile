# Вибір базового образу
FROM alpine:3.18 AS builder

# Встановлення необхідних інструментів для збірки
RUN apk add --no-cache \
    g++ \
    make \
    cmake \
    libstdc++ \
    boost-dev \
    git

# Копіювання файлів у контейнер
WORKDIR /app
COPY . /app

# Видалення попередніх зібраних файлів, якщо такі є
RUN find . -name '*.o' -delete && rm -f ./program

# Збирання програми
RUN make

# Створення фінального образу
FROM alpine:3.18

# Копіювання зібраної програми з попереднього етапу
WORKDIR /app
COPY --from=builder /app/program /app/

# Вказівка на основну команду для запуску програми
CMD ["./program"]

