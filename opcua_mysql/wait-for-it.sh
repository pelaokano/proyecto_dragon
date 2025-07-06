#!/usr/bin/env bash
host="$1"
shift
port="$1"
shift

timeout=15
start_time=$(date +%s)

until nc -z "$host" "$port"; do
  sleep 1
  echo "Waiting for $host:$port..."
  now=$(date +%s)
  if [ $((now - start_time)) -ge $timeout ]; then
    echo "Timeout while waiting for $host:$port"
    exit 1
  fi
done

exec "$@"
