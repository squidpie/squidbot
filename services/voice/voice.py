# Developed by Squidpie_Meowface @ 2022
# References:
# https://towardsdatascience.com/a-simple-chatbot-in-python-with-deep-learning-3e8669997758

import json
import string
import random
import asyncio as aio

import keras.optimizers
import nltk
import numpy as np
from nltk.stem import WordNetLemmatizer

import tensorflow as tf
from tensorflow.keras import Sequential
from tensorflow.keras.layers import Dense, Dropout

import redis

class NLPModule:

    lemmatizer = WordNetLemmatizer()

    def __init__(self, intentsJSON):
        nltk.download("punkt")
        nltk.download("wordnet")
        nltk.download('omw-1.4')


        self.words = []
        self.classes = []
        self.doc_x = []
        self.doc_y = []
        self.training = []

        f = open(intentsJSON)
        self.data = json.load(f)
        f.close()

        self.epochs = 200
        self.model = Sequential()

        self._tokenize()
        self._train()

    def _tokenize(self):
        # Loop through all the intents
        # tokenize each pattern and append tokens to words, the patterns and
        # the associated tag to their associated list
        for intent in self.data["intents"]:
            for pattern in intent["patterns"]:
                tokens = nltk.word_tokenize(pattern)
                self.words.extend(tokens)
                self.doc_x.append(pattern)
                self.doc_y.append(intent["tag"])

            # add the tag to the classes if it's not there already
            if intent["tag"] not in self.classes:
                self.classes.append(intent["tag"])
        # lemmatize all the words in the vocab and convert them to lowercase
        # if the words don't appear in punctuation
        self.words = [self.lemmatizer.lemmatize(word.lower()) for word in self.words if word not in string.punctuation]
        # sorting the vocab and classes in alphabetical order and taking the # set to ensure no duplicates occur
        self.words = sorted(set(self.words))
        self.classes = sorted(set(self.classes))

    def _train(self):
        out_empty = [0] * len(self.classes)

        # creating the bag of words model
        for idx, doc in enumerate(self.doc_x):
            bow = []
            text = self.lemmatizer.lemmatize(doc.lower())
            for word in self.words:
                bow.append(1) if word in text else bow.append(0)
            # mark the index of class that the current pattern is associated
            # to
            output_row = list(out_empty)
            output_row[self.classes.index(self.doc_y[idx])] = 1
            # add the one hot encoded BoW and associated classes to training
            self.training.append([bow, output_row])
        # shuffle the data and convert it to an array
        random.shuffle(self.training)
        self.training = np.array(self.training, dtype=object)
        # split the features and target labels
        train_X = np.array(list(self.training[:, 0]))
        train_y = np.array(list(self.training[:, 1]))

        # defining some parameters
        input_shape = (len(train_X[0]),)
        output_shape = len(train_y[0])
        # the deep learning model
        self.model.add(Dense(128, input_shape=input_shape, activation="relu"))
        self.model.add(Dropout(0.5))
        self.model.add(Dense(64, activation="relu"))
        self.model.add(Dropout(0.3))
        self.model.add(Dense(output_shape, activation = "softmax"))
        lr_schedule = tf.keras.optimizers.schedules.ExponentialDecay(
            initial_learning_rate=0.01,
            decay_steps=10000,
            decay_rate=1e-6
        )
        adam = tf.keras.optimizers.Adam(learning_rate=lr_schedule)
        self.model.compile(loss='categorical_crossentropy',
                      optimizer=adam,
                      metrics=["accuracy"])
        print(self.model.summary())
        self.model.fit(x=train_X, y=train_y, epochs=self.epochs, verbose=1)

    def _clean_text(self, text):
        print("DEBUG: " + text)
        tokens = nltk.word_tokenize(text)
        tokens = [self.lemmatizer.lemmatize(word) for word in tokens]
        return tokens

    def _bag_of_words(self, text):
        tokens = self._clean_text(text)
        bow = [0] * len(self.words)
        for w in tokens:
            for idx, word in enumerate(self.words):
                if str(word) == str(w):
                    bow[idx] = 1
        return np.array(bow)

    def pred_class(self, text):
        bow = self._bag_of_words(text)
        result = self.model.predict(np.array([bow]))[0]
        thresh = 0.15
        y_pred = [[idx, res] for idx, res in enumerate(result) if res > thresh]
        y_pred.sort(key=lambda x: x[1], reverse=True)
        return_list = []
        for r in y_pred:
            return_list.append(self.classes[r[0]])
        return return_list

    def get_response(self, intents_list):
        tag = intents_list[0]
        print(tag)
        list_of_intents = self.data["intents"]
        for i in list_of_intents:
            if i["tag"] == tag:
                result = ''
                for index in range(0,np.random.randint(1,5)):
                    result += ' '
                    result += random.choice(i["responses"])
                break
        return result


# NLP Processing Module
squidbotVoice = NLPModule('intents.json')

strauss_redis = redis.Redis(host="redis", port=6379, db=0)
chat_rx = strauss_redis.pubsub()
chat_rx.subscribe("#strauss-chat-msg-rx")


def is_filter_match(packet):
    if packet['sender'] != "squidbot_roboface" \
    and "squidbot_roboface" in packet['message_text']:
        return True
    else:
        return False

def run():
    while True:
        for raw_msg in chat_rx.listen():
            if raw_msg['type'] != "message":
                continue
            packet = json.loads(raw_msg['data'])
            if (is_filter_match(packet)):
                msg = packet['message_text']
                intents = squidbotVoice.pred_class(msg)
                result = squidbotVoice.get_response(intents)
                strauss_redis.publish("#strauss-chat-msg-tx", result)

if __name__=='__main__':
    run()
    while True:
        message = input("")
        intents = squidbotVoice.pred_class(message)
        result = squidbotVoice.get_response(intents)
        print(result)
