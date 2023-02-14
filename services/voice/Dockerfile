FROM python:3.10-slim-bullseye
WORKDIR /usr/src/app

COPY services/voice/requirements.txt ./
RUN pip install --no-cache-dir -r requirements.txt
COPY services/voice/voice.py .
COPY services/voice/intents.json .
ENTRYPOINT ["python", "./voice.py"]