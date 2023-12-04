#!/usr/bin/env bash

set -eu -o pipefail

RETRIES=100
WAIT_DURATION_BETWEEN_RETRIES=1

fail() {
  msg="$1"

  echo "$msg"
  exit 1
}

webserv_ready() {
  curl -fsS http://localhost:8080/ &> /dev/null
}

echo "waiting for webserv startup"

for (( i = 0; i < RETRIES; i++ )); do
    if webserv_ready; then
      exit 0
    fi
    sleep "$WAIT_DURATION_BETWEEN_RETRIES"
done

fail "webserv is not reachable"
