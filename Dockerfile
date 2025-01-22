# Етап зборки
FROM alpine AS builder

# Встановлення залежностей для збірки
RUN apk add --no-cache build-base automake autoconf libtool git 

# Робоча директорія
WORKDIR /app

# Копіювання локальних файлів
COPY . /app

# Білд програми
RUN aclocal
RUN autoconf
RUN automake --add-missing
RUN chmod +x configure
RUN ./configure
RUN make

# Фінальний етап
FROM alpine
# Копіювання виконуваного файлу з етапу зборки
COPY --from=builder /app/program /usr/local/bin/program



# Запуск програми
ENTRYPOINT ["/usr/local/bin/program"]


