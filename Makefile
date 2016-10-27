all: all_engine all_demo01

build: build_engine build_demo01

test: test_engine test_demo01

clean: clean_engine clean_demo01

engine: all_engine

all_engine:
	@echo Compiling engine.
	cd engine && $(MAKE) all

build_engine:
	@echo Building engine.
	cd engine && $(MAKE) build

test_engine:
	@echo Testing engine.
	cd engine && $(MAKE) test

clean_engine:
	@echo Cleaning engine.
	cd engine && $(MAKE) clean

demo01: all_demo01

all_demo01: build_engine
	@echo Compiling demo01.
	cd demo01 && $(MAKE) all

build_demo01: build_engine
	@echo Building demo01.
	cd demo01 && $(MAKE) build

test_demo01: build_engine
	@echo Testing demo01.
	cd demo01 && $(MAKE) test

clean_demo01:
	@echo Cleaning demo01.
	cd demo01 && $(MAKE) clean
