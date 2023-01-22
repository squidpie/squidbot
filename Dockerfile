# Dockerfile
# Copyright (C) 2023 Squidpie

# syntax=docer/dockerfile:1
FROM debian:bullseye
ARG build_dir
WORKDIR /squidbot
RUN mkdir lib
COPY $build_dir/bin/squidbot /squidbot/bin/squidbot
COPY $build_dir/lib/plugins /squidbot/lib/plugins
COPY $build_dir/lib/services /squidbot/lib/services
COPY $build_dir/lib/plog /squidbot/lib/plog
ENV SQUIDBOT_LIB_DIR /squidbot/lib/
CMD ["/squidbot/bin/squidbot"]
EXPOSE 3000
