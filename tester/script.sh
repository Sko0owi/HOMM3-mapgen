#!/bin/bash

mapPath="Maps/test.h3m"
# Liczba instancji do uruchomienia (domyślnie 3 jeśli nie podano argumentu)
instances=${1:-3}

testing=${1:-5}

# Tablica na PIDy procesów
declare -a pids

test_map() {

    for ((j=1; j<=testing;j++)); do

        pids=()

        echo "Computing $j/$testing batch of $instances clients"
        testID=$((j))
        run_clients $testID

        echo "Waiting for clients to end"
        
        # Czekaj na zakończenie wszystkich procesów
        for pid in "${pids[@]}"; do
            wait $pid
            echo "Client with PID $pid finished"
        done

        for ((i=1; i<=instances; i++)); do
            instanceID=$((i))
            local log_file="log_${testID}_${instanceID}"
            local log_location="/home/skowi/INŻYNIERKA/another-HOMM3-mapgen/tester/$log_file"

            tail -1000 "$log_location/VCMI_Client_log.txt" > "$log_location/last_lines.txt"
        done

    done

    echo "All clients finished"

}

# Funkcja do uruchamiania klienta
run_client() {
  local instanceID=$1
  local testID=$2
  local log_file="log_${testID}_${instanceID}"
  local log_location="/home/skowi/INŻYNIERKA/another-HOMM3-mapgen/tester/$log_file"

  mkdir -p "$log_location"

  time ../build/bin/vcmiclient --testmap "$mapPath" --headless --logLocation "$log_location" &>"$log_location/$log_file.txt" &

  local pid=$!
  pids+=($pid)

  echo "Started client instance $instanceID (PID: $pid), logging to $log_file"
}

run_clients() {
    testID=$1
    # Uruchom określoną liczbę instancji
    for ((i=1; i<=instances; i++)); do
        instanceID=$((i))
        run_client $instanceID $testID
    done

    echo "Launched $instances clients"

}

test_map
