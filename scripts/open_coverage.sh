#!/bin/bash

# Script to open the code coverage report in the default browser

COVERAGE_REPORT="build_coverage/coverage/index.html"

if [ ! -f "$COVERAGE_REPORT" ]; then
    echo "Error: Coverage report not found at $COVERAGE_REPORT"
    echo "Please generate the coverage report first by running:"
    echo "  ./scripts/docker-dev.sh coverage"
    exit 1
fi

# Try to open the report in the default browser
if command -v xdg-open &> /dev/null; then
    xdg-open "$COVERAGE_REPORT"
elif command -v open &> /dev/null; then
    open "$COVERAGE_REPORT"
elif command -v start &> /dev/null; then
    start "$COVERAGE_REPORT"
else
    echo "Coverage report generated at: $COVERAGE_REPORT"
    echo "Please open this file in your browser to view the report."
fi
