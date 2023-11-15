cmd_/home/egor/проекты/module/mod-1.mod := printf '%s\n'   mod-1.o | awk '!x[$$0]++ { print("/home/egor/проекты/module/"$$0) }' > /home/egor/проекты/module/mod-1.mod
