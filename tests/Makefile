# Static analisys
SRC= ../main.c\
	../signals/signals.c\
	../variables/init_env.c\
	../utils.c\
	../readline/readline.c\
	../readline/process_key.c\
	../readline/process_arrows.c\
	../readline/history.c\
	../readline/readline_utils.c\
	../parse/lexing.c\
	../parse/parse_utils.c\
	../parse/ast_utils.c\
	../parse/parsing.c

check:
	@cppcheck --enable=warning,style,performance,portability \
         --language=c \
         --std=c11 \
         --template='{file}:{line},{severity},{id},{message}' \
         --suppress=missingIncludeSystem \
         --error-exitcode=1 \
		 --check-level=exhaustive\
		  $(SRC)

.PHONY: check