# Dockerfile
# Copyright (C) 2023 Squidpie

# syntax=docer/dockerfile:1
FROM debian:latest
WORKDIR /squidbot
RUN mkdir lib
COPY ./targets/debug/bin/squidbot /squidbot/bin/squidbot
COPY ./targets/debug/lib/plugins /squidbot/lib/plugins
COPY ./targets/debug/lib/services /squidbot/lib/services
COPY ./targets/debug/lib/plog /squidbot/lib/plog
ENV SQUIDBOT_LIB_DIR /squidbot/lib/
CMD ["/squidbot/bin/squidbot", "--test"]
EXPOSE 3000
