# Вибір базового образу
FROM ubuntu:latest

# Встановлення необхідних бібліотек та інструментів
RUN apt-get update && apt-get install -y \
    g++ \
    make \
    cmake \
    libboost-all-dev \
    git \
    curl

# Копіюємо файли в контейнер
COPY . /app

# Встановлюємо робочу директорію
WORKDIR /app

# Складання програми
RUN make

# Запуск сервера
CMD while true; do ./program; sleep 1; done

