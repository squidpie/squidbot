#!/usr/bin/python3

# service-gen.py
# Copyright (C) 2023 Squidpie

import argparse
import os
import uuid

PLUGIN_TEMPLATE = "./scripts/plugin-template/"
TemplateName = "TemplateName"
TEMPLATE_FILES = ['CMakeLists.txt', TemplateName.lower() + '.cpp', TemplateName.lower() + '.h', 'factory.cpp']

def process(replacement, line):
  line = line.replace(TemplateName, replacement)
  line = line.replace(TemplateName.lower(), replacement.lower())
  line = line.replace(TemplateName.upper(), replacement.upper())
  return line


def main(PluginName):
  print("Generating " + PluginName + " from " + TemplateName)
  out_dir = './plugin-gen-output/' + PluginName.lower() + '/'
  os.makedirs(out_dir, exist_ok=True)
  for file in TEMPLATE_FILES:
    with open(PLUGIN_TEMPLATE + file) as template:
      lines = template.readlines()
      out_file = file.replace(TemplateName.lower(), PluginName.lower())
      with open(out_dir + out_file, 'w') as out:
        [out.write(process(PluginName, line)) for line in lines]

if __name__ == "__main__":
  parser = argparse.ArgumentParser(
    prog = 'plugin-generator',
    description = 'generates empty plugin',
    epilog = 'enter your plugin name in camel case, or it won\'t work')
  parser.add_argument('PluginName')
  args = parser.parse_args()
  main(args.PluginName)