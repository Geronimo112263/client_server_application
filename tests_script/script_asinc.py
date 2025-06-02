import subprocess
import os

FIRST_PROGRAM_REL = "./../build/async_server"
SECOND_PROGRAM_REL = "./../build/client"

# Абсолютные пути к программам
SCRIPT_DIR = os.path.dirname(os.path.realpath(__file__))
FIRST_PROGRAM = os.path.realpath(os.path.join(SCRIPT_DIR, FIRST_PROGRAM_REL))
SECOND_PROGRAM = os.path.realpath(os.path.join(SCRIPT_DIR, SECOND_PROGRAM_REL))

COUNT = 5

# Проверка существования и исполняемости
if not (os.path.isfile(FIRST_PROGRAM) and os.access(FIRST_PROGRAM, os.X_OK)):
    print(f"Не найден исполняемый файл первой программы: {FIRST_PROGRAM}")
    exit(1)

if not (os.path.isfile(SECOND_PROGRAM) and os.access(SECOND_PROGRAM, os.X_OK)):
    print(f"Не найден исполняемый файл второй программы: {SECOND_PROGRAM}")
    exit(1)

# AppleScript для запуска iTerm
applescript = f'''
tell application "iTerm"
    activate

    -- Открыть окно с сервером
    set serverWindow to (create window with default profile)
    tell serverWindow
        tell current session
            write text "{FIRST_PROGRAM}"
        end tell
    end tell

    -- Открыть окно с клиентами
    set clientWindow to (create window with default profile)
    tell clientWindow
        tell current session
            write text "{SECOND_PROGRAM}"
        end tell

        repeat with i from 2 to {COUNT}
            create tab with default profile
            tell current session
                write text "{SECOND_PROGRAM}"
            end tell
        end repeat
    end tell
end tell
'''

# Запуск AppleScript через osascript
process = subprocess.run(['osascript', '-e', applescript])
if process.returncode != 0:
    print("Ошибка при запуске AppleScript")
