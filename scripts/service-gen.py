#!/usr/bin/python3

# service-gen.py
# Copyright (C) 2023 Squidpie

import argparse
import os
import uuid

SERVICE_TEMPLATE = "./scripts/service-template/"
TemplateName = "TemplateName"
TEMPLATE_FILES = ['CMakeLists.txt', TemplateName.lower() + '.cpp', TemplateName.lower() + '.h', 'events.h', 'version.h', 'factory.cpp']

def process(replacement, line):
  line = line.replace(TemplateName, replacement)
  line = line.replace(TemplateName.lower(), replacement.lower())
  line = line.replace(TemplateName.upper(), replacement.upper())
  line = line.replace('_NAMESPACE = "', '_NAMESPACE = "' + str(uuid.uuid1()))
  return line


def main(ServiceName):
  print("Generating " + ServiceName + " from " + TemplateName)
  out_dir = './service-gen-output/' + ServiceName.lower() + '/'
  os.makedirs(out_dir, exist_ok=True)
  for file in TEMPLATE_FILES:
    with open(SERVICE_TEMPLATE + file) as template:
      lines = template.readlines()
      out_file = file.replace(TemplateName.lower(), ServiceName.lower())
      with open(out_dir + out_file, 'w') as out:
        [out.write(process(ServiceName, line)) for line in lines]

if __name__ == "__main__":
  parser = argparse.ArgumentParser(
    prog = 'service-generator',
    description = 'generates empty service',
    epilog = 'enter your service name in camel case, or it won\'t work')
  parser.add_argument('ServiceName')
  args = parser.parse_args()
  main(args.ServiceName)