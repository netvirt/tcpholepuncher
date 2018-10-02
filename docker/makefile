.PHONY:	build compile run stop punch

SRC=/root/puncher
IMAGE = builder
TAG = 1-0
TESTDIR=/opt/thp/tests/.libs
INSTALLBINDIR=/usr/local/bin

TESTPROGR?=setuptest
INSTALLPROGR?=punchctl

build:	
	sudo docker build -t $(IMAGE):$(TAG) .

compile:
	sudo docker exec -i $(IMAGE) /opt/build.sh

run: stop 
	sudo docker run --rm -d -i --name $(IMAGE) --net=host -v ${PWD}/../:/opt/thp $(IMAGE):$(TAG)

stop:
	-sudo docker stop $(IMAGE) 

punch: 
	sudo docker exec -i -t $(IMAGE) /usr/local/bin/punchctl
	sudo docker exec -i -t $(IMAGE) /opt/thp/tests/setuptest

leaks-test:
	# Can be used by passing the program name by the command line
	# make leaks-test TESTPROGR=setuptest
	sudo docker exec -t -i $(IMAGE) valgrind --leak-check=full --show-reachable=yes $(TESTDIR)/$(TESTPROGR)
leaks-bin:		
	# Can be used by passing the program name by the command line
	# make leaks-bin INSTALLPROGR=puncher
	sudo docker exec -t -i $(IMAGE) valgrind --leak-check=full --show-reachable=yes -v $(INSTALLBINDIR)/$(INSTALLPROGR)

debugger-test:
	# Can be used by passing the program name by the command line
	# make debugger-test INSTALLPROGR=puncher
	sudo docker exec -t -i $(IMAGE) cgdb $(TESTDIR)/$(TESTPROGR)