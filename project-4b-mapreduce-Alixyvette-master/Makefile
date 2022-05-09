CC:=gcc
CFLAGS:= -Wall -Werror -O
TARGETS:=

all: $(TARGETS)

test:
	@gcc -ggdb -pthread -c mapreduce.c
	@gcc -ggdb -pthread -c wordcount.c
	@gcc -ggdb -pthread -o a.out mapreduce.o wordcount.o

handin: handin-check
	@echo "Handing in with git (this may ask for your GitHub username/password)..."
	@git push origin master


handin-check:
	@if ! test -d .git; then \
		echo No .git directory, is this a git repository?; \
		false; \
	fi
	@if test "$$(git symbolic-ref HEAD)" != refs/heads/master; then \
		git branch; \
		read -p "You are not on the master branch.  Hand-in the current branch? [y/N] " r; \
		test "$$r" = y; \
	fi
	@if ! test -e info.txt; then \
		echo "You haven't created an info.txt file!"; \
		echo "Please create one with your name, email, etc."; \
		echo "then add and commit it to continue."; \
		false; \
	fi
	@if ! git diff-files --quiet || ! git diff-index --quiet --cached HEAD; then \
		git status -s; \
		echo; \
		echo "You have uncomitted changes.  Please commit or stash them."; \
		false; \
	fi
	@if test -n "`git status -s`"; then \
		git status -s; \
		read -p "Untracked files will not be handed in.  Continue? [y/N] " r; \
		test "$$r" = y; \
	fi

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(TARGETS) *.o


.PHONY: clean handin-check 
