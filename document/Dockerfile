FROM ubuntu:latest

RUN export DEBIAN_FRONTEND=noninteractive && apt update && apt install -y texlive-full
RUN mkdir /host-mounted/

COPY latex/create.sh /bin/create.sh
COPY latex/clean.sh /bin/clean.sh
COPY latex/execute.sh /bin/execute.sh
RUN chmod +x /bin/create.sh /bin/clean.sh /bin/execute.sh

WORKDIR /host-mounted/

ENTRYPOINT /bin/execute.sh