# Dockerfile
# Copyright (C) 2023 Squidpie

# syntax=docer/dockerfile:1
FROM debian:latest
WORKDIR /squidbot
RUN mkdir lib
COPY ./targets/release/bin/squidbot /squidbot/bin/squidbot
COPY ./targets/release/lib/plugins /squidbot/lib/plugins
RUN mkdir /squidbot/lib/services
#COPY ./targets/lib/services /squidbot/lib/services
COPY ./targets/release/lib/plog /squidbot/lib/plog
ENV SQUIDBOT_LIB_DIR /squidbot/lib
CMD ["/squidbot/bin/squidbot"]
EXPOSE 3000
