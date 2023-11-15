cmd_/home/egor/проекты/module/module.mod := printf '%s\n'   module.o | awk '!x[$$0]++ { print("/home/egor/проекты/module/"$$0) }' > /home/egor/проекты/module/module.mod
