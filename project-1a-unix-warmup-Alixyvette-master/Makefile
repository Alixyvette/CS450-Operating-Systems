CC:=gcc
CFLAGS:= -Wall -Werror -O2
TARGETS:=my-cat my-grep my-zip my-unzip

all: $(TARGETS)

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

test-all:
	@tests/bin/test-my-cat.csh
	@tests/bin/test-my-grep.csh
	@tests/bin/test-my-zip.csh
	@tests/bin/test-my-unzip.csh

test-my-cat:
	@tests/bin/test-my-cat.csh

test-my-grep:
	@tests/bin/test-my-grep.csh

test-my-zip:
	@tests/bin/test-my-zip.csh

test-my-unzip:
	@tests/bin/test-my-unzip.csh

clean:
	@rm -f $(TARGETS)

