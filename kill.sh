#!/usr/bin/env bash

set -eu -o pipefail

frontend_up() {
  curl -fsS http://localhost:8080/ &> /dev/null
}

run_kill() {
  kill "$(lsof -sTCP:LISTEN -ti tcp:8080)" &> /dev/null || true
}

run_kill

until ! frontend_up; do
  run_kill
  sleep 1
done

