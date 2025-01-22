#!/bin/bash

IMAGE_NAME="pugachartem/devops"
CONTAINERS=("srv1" "srv2" "srv3")
CPU_CORES=(0 1 2)

start_container() {
    local name=$1
    local cpu=$2
    echo "[$(date '+%H:%M:%S')] --> Launching $name using core $cpu..."
    docker run -d --name "$name" --cpuset-cpus="$cpu" $IMAGE_NAME &> /dev/null
    if [ $? -eq 0 ]; then
        echo "[$(date '+%H:%M:%S')] >>> Container $name has been started!"
    else
        echo "[$(date '+%H:%M:%S')] !!! Error: Failed to start $name."
    fi
}

stop_container() {
    local name=$1
    echo "[$(date '+%H:%M:%S')] --> Stopping $name and cleaning up..."
    docker stop "$name" &> /dev/null && docker rm "$name" &> /dev/null
    if [ $? -eq 0 ]; then
        echo "[$(date '+%H:%M:%S')] >>> $name is no longer active."
    else
        echo "[$(date '+%H:%M:%S')] !!! Warning: Failed to stop $name."
    fi
}

cleanup() {
    echo "[$(date '+%H:%M:%S')] --> Cleaning up all containers before exiting..."
    for container in "${CONTAINERS[@]}"; do
        if docker ps --filter "name=^${container}$" --format "{{.Names}}" | grep -q "^${container}$"; then
            stop_container "$container"
        fi
    done
    docker ps -aq | xargs docker rm -f &> /dev/null
    echo "[$(date '+%H:%M:%S')] >>> All containers have been removed."
    exit 0
}

check_container_load() {
    local name=$1
    local cpu_usage
    cpu_usage=$(docker stats --no-stream --format "{{.CPUPerc}}" "$name" | sed 's/%//')
    echo "[$(date '+%H:%M:%S')] -- CPU usage of $name: $cpu_usage%."
    if (( $(echo "$cpu_usage > 20.0" | bc -l) )); then
        echo "[$(date '+%H:%M:%S')] ** High load detected for $name."
        return 0
    else
        echo "[$(date '+%H:%M:%S')] ** $name is within normal load limits."
        return 1
    fi
}

update_image() {
    echo "[$(date '+%H:%M:%S')] --> Checking for updates to $IMAGE_NAME..."
    if docker pull $IMAGE_NAME | grep -q "Downloaded newer image"; then
        echo "[$(date '+%H:%M:%S')] !!! New version detected, updating containers!"
        for i in "${!CONTAINERS[@]}"; do
            local container=${CONTAINERS[$i]}
            if docker ps --filter "name=^${container}$" --format "{{.Names}}" | grep -q "^${container}$"; then
                stop_container "$container"
                start_container "$container" "${CPU_CORES[$i]}"
                sleep 5
            fi
        done
    else
        echo "[$(date '+%H:%M:%S')] --> $IMAGE_NAME is up to date."
    fi
}

main_loop() {
    declare -A busy_count idle_count

    while true; do
        echo "==== Monitoring cycle started at $(date '+%H:%M:%S') ===="
        for i in "${!CONTAINERS[@]}"; do
            local container=${CONTAINERS[$i]}
            local cpu_core=${CPU_CORES[$i]}

            if docker ps --filter "name=^${container}$" --format "{{.Names}}" | grep -q "^${container}$"; then
                if check_container_load "$container"; then
                    busy_count["$container"]=$(( ${busy_count["$container"]:-0} + 1 ))
                    idle_count["$container"]=0
                    if [ "${busy_count["$container"]}" -ge 2 ] && [ $i -lt $((${#CONTAINERS[@]} - 1)) ]; then
                        local next_container=${CONTAINERS[$((i + 1))]}
                        if ! docker ps --filter "name=^${next_container}$" --format "{{.Names}}" | grep -q "^${next_container}$"; then
                            echo "[$(date '+%H:%M:%S')] --> Activating $next_container due to high load on $container."
                            start_container "$next_container" "${CPU_CORES[$((i + 1))]}"
                        fi
                    fi
                else
                    busy_count["$container"]=0
                    idle_count["$container"]=$(( ${idle_count["$container"]:-0} + 1 ))
                    if [ "${idle_count["$container"]}" -ge 2 ] && [ "$container" != "${CONTAINERS[0]}" ]; then
                        echo "[$(date '+%H:%M:%S')] --> $container is idle for too long, stopping it."
                        stop_container "$container"
                    fi
                fi
            else
                if [ "$container" == "${CONTAINERS[0]}" ]; then
                    echo "[$(date '+%H:%M:%S')] --> $container is down, starting it."
                    start_container "$container" "$cpu_core"
                fi
            fi
        done

        update_image
        echo "==== Monitoring cycle completed at $(date '+%H:%M:%S') ===="
        sleep 60
    done
}

trap cleanup SIGINT SIGTERM
main_loop
