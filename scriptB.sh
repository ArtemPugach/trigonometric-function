#!/bin/bash

# Configuration
TARGET_URL="http://127.0.0.1/compute"

send_request() {
    echo "[INFO] $(date '+%H:%M:%S') >>> Sending GET request to $TARGET_URL..."
    curl -s -o /dev/null -w "Response: %{http_code}\n" "$TARGET_URL" &
    echo "[INFO] $(date '+%H:%M:%S') >>> Request sent in the background."
}

generate_random_interval() {
    echo $((RANDOM % 6 + 5))
}

main_loop() {
    echo "[START] $(date '+%H:%M:%S') ::: Starting request loop to $TARGET_URL."
    while true; do
        send_request
        sleep_time=$(generate_random_interval)
        echo "[WAIT] $(date '+%H:%M:%S') ::: Sleeping for $sleep_time seconds."
        sleep "$sleep_time"
    done
}

trap "echo '[STOP] $(date '+%H:%M:%S') ::: Script interrupted. Exiting...'; exit" SIGINT SIGTERM
main_loop

