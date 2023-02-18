FROM debian:bullseye-slim
ARG SQUIDBOT_BUILD_DIR
WORKDIR /txt_cmds
COPY ${SQUIDBOT_BUILD_DIR}/txt_cmds /txt_cmds/
ENTRYPOINT ["./txt_cmds"]