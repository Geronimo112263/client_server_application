#!/bin/bash

FIRST_PROGRAM_REL="./../build/sync_server"
SECOND_PROGRAM_REL="./../build/client"

FIRST_PROGRAM="$(cd "$(dirname "${BASH_SOURCE[0]}")" && realpath "$FIRST_PROGRAM_REL")"
SECOND_PROGRAM="$(cd "$(dirname "${BASH_SOURCE[0]}")" && realpath "$SECOND_PROGRAM_REL")"

COUNT=5

if [ ! -x "$FIRST_PROGRAM" ]; then
    echo "Не найден исполняемый файл первой программы: $FIRST_PROGRAM"
    exit 1
fi

if [ ! -x "$SECOND_PROGRAM" ]; then
    echo "Не найден исполняемый файл второй программы: $SECOND_PROGRAM"
    exit 1
fi

osascript <<EOF
tell application "iTerm"
    activate

    -- Запуск первого окна с сервером
    set serverWindow to (create window with default profile)
    tell serverWindow
        tell current session
            write text "$FIRST_PROGRAM"
        end tell
    end tell

    -- Запуск второго окна с клиентами
    set clientWindow to (create window with default profile)
    tell clientWindow
        tell current session
            write text "$SECOND_PROGRAM"
        end tell

        repeat with i from 2 to $COUNT
            create tab with default profile
            tell current session
                write text "$SECOND_PROGRAM"
            end tell
        end repeat
    end tell
end tell
EOF
